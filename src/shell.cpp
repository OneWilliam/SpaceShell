#include "shell.hpp"
#include <iostream>
#include <string>

using namespace std;

void Shell::run() {
    string linea;

    while (true) {
        mostrar_prompt();
        linea = leer_linea();
        cout << "[INPUT] " << linea << endl;
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
