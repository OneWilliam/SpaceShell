#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "parser.hpp"

using namespace std;

class BuiltinManager {
 public:
  BuiltinManager();

  bool es_builtin(const string& nombre) const;
  void ejecutar(const ComandoInfo& cmd);

 private:
  void handle_cd(const ComandoInfo& cmd);
  void handle_pwd(const ComandoInfo& cmd);

  map<string, function<void(const ComandoInfo&)>> builtins;
};
