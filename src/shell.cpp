#include "shell.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <system_error>
#include <cerrno>
#include <cstring>
#include <fcntl.h>

using namespace std;

void Shell::run() {
    string linea;
    vector<ComandoInfo> comandos;

    while (true) {
        mostrar_prompt();
        linea = leer_linea();
        comandos = parser.parsear_linea(linea);

        cout << "[INPUT] " << linea << endl;
        
        if(comandos.empty()){
          continue;
        }

        if(comandos[0].args[0] == "salir"){
          break;
        }
        try{
          ejecutar(comandos);
        } catch (system_error &e){
           cerr << "[SHELL] " << e.what() << endl; 
        }
    }
}

void Shell::mostrar_prompt() {
    cout << ">>> ";
}

string Shell::leer_linea() {
    string linea;
    getline(cin, linea);
    return linea;
}

void ejecutar_comando(const ComandoInfo &comando) {
  if (!comando.outputFile.empty()) {
      int file_fd = open(comando.outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
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



void Shell::ejecutar(const vector<ComandoInfo> &pipeline){
  if (pipeline.empty()) {
      return;
  }

  int pipeline_size = pipeline.size();

  if (pipeline_size == 1) {
      pid_t pid = fork();
      if (pid == -1) { 
          throw system_error(errno, generic_category(), "[SHELL] Fallo en fork");
      }
      if (pid == 0) {
          ejecutar_comando(pipeline[0]);
      } else {
          int status;
          if (waitpid(pid, &status, 0) == -1) {
              throw system_error(errno, generic_category(), "[SHELL] Fallo en waitpid");
          }
      }
      return;
  }

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
        }

        child_pids.push_back(pid);

        if (in_fd != STDIN_FILENO) {
            close(in_fd);
        }

        if (i < pipeline_size - 1) {
            close(pipefd[1]);
            in_fd = pipefd[0];
        }
    }

    for (pid_t child_pid : child_pids) {
        if (waitpid(child_pid, nullptr, 0) == -1) {
            throw system_error(errno, generic_category(), "[ERROR] Fallo en waitpid en el pipeline");
        }
    }
}
