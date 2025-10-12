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

  if (pipeline.size() == 1) {
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

    int pipefd[2];
    if (pipe(pipefd) == -1) {
      throw system_error(errno, generic_category(), "[SHELL] Fallo en pipe");
    }

    pid_t pid1 = fork();
    if (pid1 == -1) { 
        throw system_error(errno, generic_category(), "[SHELL] Fallo en el primer fork del pipeline");
    }

    if (pid1 == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        ejecutar_comando(pipeline[0]);
    }

    pid_t pid2 = fork();
    if (pid2 == -1) { 
        throw system_error(errno, generic_category(), "[SHELL] Fallo en el segundo fork del pipeline");
    }

    if (pid2 == 0) {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        ejecutar_comando(pipeline[1]);
    }
    
    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);

    if (waitpid(pid1, nullptr, 0) == -1) {
        throw system_error(errno, generic_category(), "[SHELL] Fallo en waitpid para el primer comando del pipeline");
    }
    if (waitpid(pid2, nullptr, 0) == -1) {
        throw system_error(errno, generic_category(), "[SHELL] Fallo en waitpid para el segundo comando del pipeline");
    }

}
