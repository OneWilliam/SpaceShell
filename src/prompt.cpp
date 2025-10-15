#include "prompt.hpp"

#include <unistd.h>

#include <climits>
#include <iostream>

using namespace std;

void Prompt::mostrar() {
  char cwd_buffer[PATH_MAX];
  if (getcwd(cwd_buffer, sizeof(cwd_buffer)) != nullptr) {
    cout <<  "\033[36m" << cwd_buffer << "\033[33m" << "@ " << "\033[0m"  << flush;
  } else {
    perror("[SHELL] Error al obtener el directorio actual para el prompt");
    cout << "[?] >>> " << flush;
  }
}
