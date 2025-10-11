#pragma once 
#include <string>
#include <vector>
using namespace std;

class Shell {
  public:
    void run();
  private:
    void mostrar_prompt();
    string leer_linea();
    vector<string> parsear_linea(const string &linea);
    void ejecutar(const vector<string> &args);
};
