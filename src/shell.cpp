#include "shell.hpp"
#include <iostream>
#include <string>
#include <sstream>
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

