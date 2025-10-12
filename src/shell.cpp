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

using namespace std;

void Shell::run() {
    string linea;
    ComandoInfo comando;

    while (true) {
        mostrar_prompt();
        linea = leer_linea();
        comando = parser.parsear_linea(linea);

        cout << "[INPUT] " << linea << endl;
        
        if(comando.args.empty()){
            continue;
        }

        if(comando.args[0] == "salir"){
          break;
        }
        try{
          ejecutar(comando);
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

void Shell::ejecutar(const ComandoInfo &comando){
    pid_t pid = fork();

    if(pid == -1) {
      throw system_error(errno, generic_category(), "[SHELL] Fallo en fork");
    }

    if(pid == 0) {
      vector<char*> c_args;
      
      for (const auto& arg : comando.args) {
         c_args.push_back(const_cast<char*>(arg.c_str()));
      }

      c_args.push_back(nullptr);

      string command_path = comando.args[0];
      if (command_path.find('/') == string::npos) {
          command_path = "/bin/" + comando.args[0];
      }
      cout << "[SHELL] " << command_path << endl;
    
      execv(command_path.c_str(), c_args.data());
      
      cerr << "[SHELL] ERROR en execv: " << strerror(errno) << endl;
      if (errno == ENOENT) {
          cerr << "[SHELL] Comando '" << comando.args[0] << "' no encontrado." << endl;
      }
      exit(EXIT_FAILURE);
    } else {
      int status;
      if (waitpid(pid, &status, 0) == -1) {
          throw system_error(errno, generic_category(), "[SHELL] Fallo en waitpid");
      }
    }
}
