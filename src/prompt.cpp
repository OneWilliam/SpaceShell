#include "prompt.hpp"
#include <iostream>
#include <unistd.h>
#include <climits>

using namespace std;


void Prompt::mostrar() {
    char cwd_buffer[PATH_MAX];
    if (getcwd(cwd_buffer, sizeof(cwd_buffer)) != nullptr) {
        cout << cwd_buffer << "@ " << flush;
    } else {
        perror("[SHELL] Error al obtener el directorio actual para el prompt");
        cout << "[?] >>> " << flush;
    }
}
