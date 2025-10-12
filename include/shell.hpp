#pragma once 
#include "parser.hpp"
#include "builtins.hpp"
#include <string>
#include <vector>
using namespace std;

class Shell {
  public:
    void run();
  private:
    void mostrar_prompt();
    string leer_linea();
    void ejecutar(const vector<ComandoInfo> &args);
    Parser parser;
    BuiltinManager builtin_manager;
};
