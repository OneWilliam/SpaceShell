#include "shell.hpp"

#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <system_error>

#include "signals.hpp"

using namespace std;

void Shell::run() {
  Signals::setup_handlers();
  string linea;

  while (true) {
    if (g_signal_recv == SIGINT) {
      cout << "\n" << flush;
      g_signal_recv = 0;
      continue;
    }

    prompt.mostrar();
    linea = leer_linea();

    if (g_signal_recv == SIGINT) {
      cout << "\n" << flush;
      g_signal_recv = 0;
      continue;
    }

    if (cin.eof()) {
      cout << "salir" << endl;
      break;
    }

    if (cin.fail()) {
      cin.clear();
      continue;
    }

    if (linea.empty()) {
      continue;
    }
    cout << "[INPUT] " << linea << endl;

    if (linea.empty()) {
      continue;
    }

    Pipeline_cmd comandos = parser.parsear_linea(linea);

    if (comandos.pipeline.empty()) {
      continue;
    }

    const auto& primer_cmd = comandos.pipeline.front();
    const string& nombre_cmd = primer_cmd.args[0];

    if (nombre_cmd == "salir") {
      break;
    }

    if (comandos.pipeline.size() == 1 && builtin_manager.es_builtin(nombre_cmd)) {
      builtin_manager.ejecutar(primer_cmd);
    } else {
      try {
        ejecutar(comandos);
      } catch (system_error& e) {
        cerr << "[SHELL] " << e.what() << endl;
      }
    }
  }
}

string Shell::leer_linea() {
  string linea;

  if (g_signal_recv == SIGINT) {
    g_signal_recv = 0;
  }

  char ch;
  while (true) {
    ssize_t bytes_r = read(STDIN_FILENO, &ch, 1);

    if (bytes_r == 1) {
      if (ch == '\n') {
        break;
      } else if (ch != '\r') {
        linea += ch;
      }
    } else if (bytes_r == 0) {
      cin.setstate(ios::eofbit);
      break;
    } else if (bytes_r < 0) {
      if (errno == EINTR) {
        if (g_signal_recv == SIGINT) {
          linea.clear();
          break;
        }
        continue;
      } else {
        break;
      }
    }
  }

  return linea;
}

void ejecutar_comando(const ComandoInfo& comando) {
  if (!comando.inputFile.empty()) {
    int file_fd = open(comando.inputFile.c_str(), O_RDONLY);
    if (file_fd == -1) {
      cerr << "[SHELL] Error al abrir el archivo de entrada: " << strerror(errno) << endl;
      exit(EXIT_FAILURE);
    }
    dup2(file_fd, STDIN_FILENO);
    close(file_fd);
  }

  if (!comando.outputFile.empty()) {
    int flags = O_WRONLY | O_CREAT;
    if (comando.anexar) {
      flags |= O_APPEND;
    } else {
      flags |= O_TRUNC;
    }
    int file_fd = open(comando.outputFile.c_str(), flags, 0644);
    if (file_fd == -1) {
      cerr << "[SHELL] Error al abrir el archivo de salida: " << strerror(errno) << endl;
      exit(EXIT_FAILURE);
    }
    dup2(file_fd, STDOUT_FILENO);
    close(file_fd);
  }

  vector<char*> c_args;
  for (const auto& arg : comando.args) {
    c_args.push_back(const_cast<char*>(arg.c_str()));
  }
  c_args.push_back(nullptr);

  string comando_path = comando.args[0];
  if (comando_path.find('/') == string::npos) {
    comando_path = "/bin/" + comando_path;
  }

  execv(comando_path.c_str(), c_args.data());

  cerr << "[SHELL] Error en execv: " << strerror(errno) << endl;
  if (errno == ENOENT) {
    cerr << "[SHELL] Comando '" << comando.args[0] << "' no encontrado." << endl;
  }
  exit(EXIT_FAILURE);
}

void Shell::ejecutar(const Pipeline_cmd& cmds) {
  const auto& pipeline = cmds.pipeline;
  if (pipeline.empty()) {
    return;
  }

  int pipeline_size = pipeline.size();
  int in_fd = STDIN_FILENO;
  std::vector<pid_t> child_pids;

  for (int i = 0; i < pipeline_size; ++i) {
    int pipefd[2];
    if (i < pipeline_size - 1) {
      if (pipe(pipefd) == -1) {
        throw system_error(errno, generic_category(), "[SHELL] Fallo en pipe");
      }
    }

    pid_t pid = fork();
    if (pid == -1) {
      throw system_error(errno, generic_category(), "[SHELL] Fallo en fork del pipeline");
    }

    if (pid == 0) {
      signal(SIGINT, SIG_DFL);
      signal(SIGCHLD, SIG_DFL);

      if (in_fd != STDIN_FILENO) {
        if (dup2(in_fd, STDIN_FILENO) == -1) {
          cerr << "[SHELL] Error en dup2 (stdin): " << strerror(errno) << endl;
          exit(EXIT_FAILURE);
        }
        close(in_fd);
      }

      if (i < pipeline_size - 1) {
        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
          cerr << "[SHELL] Error en dup2 (stdout): " << strerror(errno) << endl;
          exit(EXIT_FAILURE);
        }
        close(pipefd[1]);
      }

      ejecutar_comando(pipeline[i]);
    } else {
      child_pids.push_back(pid);

      if (in_fd != STDIN_FILENO) {
        close(in_fd);
      }

      if (i < pipeline_size - 1) {
        close(pipefd[1]);
        in_fd = pipefd[0];
      }
    }
  }

  if (!cmds.background) {
    for (pid_t child_pid : child_pids) {
      if (waitpid(child_pid, nullptr, 0) == -1) {
        throw system_error(errno, generic_category(), "[ERROR] Fallo en waitpid en el pipeline para un hijo");
      }
    }
  } else {
    cout << "[Proceso en segundo plano iniciado con PID(s): ";
    for (pid_t pid : child_pids) {
      cout << pid << " ";
      background_pids.push_back(pid);
    }
    cout << "]" << endl;
  }
}
