#pragma once 
#include "parser.hpp"
#include "builtins.hpp"
#include "prompt.hpp"
#include <string>
#include <vector>
using namespace std;


class Shell {
  public:
    void run();
  private:
    void mostrar_prompt();
    string leer_linea();
    void ejecutar(const Pipeline_cmd &args);
    Parser parser;
    BuiltinManager builtin_manager;
    vector<pid_t> background_pids;
    Prompt prompt;
};
