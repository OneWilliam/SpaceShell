#include "builtins.hpp"

#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

void ejecutar_comando(const ComandoInfo& comando);

BuiltinManager::BuiltinManager() {
  builtins["cd"] = [this](const ComandoInfo& cmd) { this->handle_cd(cmd); };
  builtins["pwd"] = [this](const ComandoInfo& cmd) { this->handle_pwd(cmd); };
  builtins["parallel"] = [this](const ComandoInfo& cmd) { this->handle_parallel(cmd); };
}

bool BuiltinManager::es_builtin(const string& nombre) const { return builtins.count(nombre) > 0; }

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

void* thread_cmd(void* cmd_nombre_ptr) {
  const char* cmd_nombre = static_cast<const char*>(cmd_nombre_ptr);

  pid_t pid = fork();
  if (pid == 0) {
    ComandoInfo cmd_temp;
    cmd_temp.args.push_back(cmd_nombre);

    ejecutar_comando(cmd_temp);
  } else if (pid > 0) {
    waitpid(pid, nullptr, 0);
  }
  return nullptr;
}

void BuiltinManager::handle_parallel(const ComandoInfo& cmd) {
  if (cmd.args.size() < 2) {
    cerr << "[SHELL] Uso: parallel <comando1> <comando2> ..." << endl;
    return;
  }

  int num_procesos = cmd.args.size() - 1;
  vector<pthread_t> threads(num_procesos);

  cout << "[SHELL] Ejecutando " << num_procesos << " procesos en paralelo..." << endl;

  for (int i = 0; i < num_procesos; ++i) {
    const char* cmd_nombre_ptr = cmd.args[i + 1].c_str();

    if (pthread_create(&threads[i], nullptr, thread_cmd, (void*)cmd_nombre_ptr) != 0) {
      perror("[SHELL] Error al crear el hilo");
    }
  }

  for (int i = 0; i < num_procesos; ++i) {
    if (pthread_join(threads[i], nullptr) != 0) {
      perror("[SHELL] Error al esperar al hilo");
    }
  }

  cout << "Todos los comandos en paralelo han terminado." << endl;
}
