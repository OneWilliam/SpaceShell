#include "builtins.hpp"

#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

BuiltinManager::BuiltinManager() {
  builtins["cd"] = [this](const ComandoInfo& cmd) { this->handle_cd(cmd); };
  builtins["pwd"] = [this](const ComandoInfo& cmd) { this->handle_pwd(cmd); };
}

bool BuiltinManager::es_builtin(const string& nombre) const {
  return builtins.count(nombre) > 0;
}

void BuiltinManager::ejecutar(const ComandoInfo& cmd) {
  const string& nombre_cmd = cmd.args[0];
  if (es_builtin(nombre_cmd)) {
    builtins.at(nombre_cmd)(cmd);
  }
}

void BuiltinManager::handle_cd(const ComandoInfo& cmd) {
  if (cmd.args.size() < 2) {
    const char* home_dir = getenv("HOME");
    if (home_dir != nullptr) {
      if (chdir(home_dir) != 0) {
        perror("[SHELL] cd");
      }
    }
  } else {
    if (chdir(cmd.args[1].c_str()) != 0) {
      perror("[SHELL] cd");
    }
  }
}

void BuiltinManager::handle_pwd(const ComandoInfo& cmd) {
  char buffer[1024];
  if (getcwd(buffer, sizeof(buffer)) != nullptr) {
    cout << buffer << endl;
  } else {
    perror("[SHELL] pwd");
  }
}
