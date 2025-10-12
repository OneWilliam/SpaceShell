#pragma once 
#include "parser.hpp"
#include <string>
#include <vector>
using namespace std;

class Shell {
  public:
    void run();
  private:
    void mostrar_prompt();
    string leer_linea();
    void ejecutar(const ComandoInfo &args);
    Parser parser;
};
