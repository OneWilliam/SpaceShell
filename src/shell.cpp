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
    vector<string> args;

    while (true) {
        mostrar_prompt();
        linea = leer_linea();
        args = parsear_linea(linea);

        cout << "[INPUT] " << linea << endl;
        
        if(args.empty()){
            continue;
        }

        if(args[0] == "salir"){
          break;
        }
          ejecutar(args);
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

vector<string> Shell::parsear_linea(const string &linea) {
    stringstream ss(linea);
    string token;
    vector<string> tokens;
    while(ss >> token){
        tokens.push_back(token);
    }
    return tokens;
}

void Shell::ejecutar(const vector<string> &args){
    pid_t pid = fork();

    if(pid == -1) {
      throw system_error(errno, generic_category(), "Fallo en fork");
    }

    if(pid == 0) {
      vector<char*> c_args;
      
      for (const auto& arg : args) {
         c_args.push_back(const_cast<char*>(arg.c_str()));
      }

      c_args.push_back(nullptr);

      string command_path = args[0];
      if (command_path.find('/') == string::npos) {
          command_path = "/bin/" + args[0];
      }
      cout << "[SHELL] " << command_path << endl;
    }
}
