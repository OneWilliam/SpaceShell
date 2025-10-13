#pragma once
#include <string>
#include <vector>

#include "builtins.hpp"
#include "parser.hpp"
#include "prompt.hpp"
using namespace std;

class Shell {
 public:
  void run();

 private:
  string leer_linea();
  void ejecutar(const Pipeline_cmd &args);
  Parser parser;
  BuiltinManager builtin_manager;
  vector<pid_t> background_pids;
  Prompt prompt;
};
