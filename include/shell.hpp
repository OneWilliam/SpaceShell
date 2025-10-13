#pragma once
#include <string>
#include <vector>

#include "builtins.hpp"
#include "parser.hpp"
#include "prompt.hpp"
using namespace std;

/**
 * Clase principal para el intérprete de comandos.
 *
 * Encapsula el bucle principal de la shell (leer, analizar, ejecutar),
 * y gestiona los diferentes módulos como el Parser, el BuiltinManager, ...
 */
class Shell {
 public:
  /**
   * Inicia el bucle principal de la shell.
   *
   * Esta función no retorna hasta que el usuario ejecuta el comando 'salir'
   * o envía una señal de fin de archivo (Ctrl+D).
   */
  void run();

 private:
  /**
   * Lee una línea de entrada de forma segura.
   *
   * Utiliza 'read' a bajo nivel para tener un control preciso sobre
   * la interrupción por señales.
   * @return La línea de texto ingresada por el usuario.
   */
  string leer_linea();

  /**
   * Organiza la ejecución de comandos externos y tuberías.
   * @param args La línea de comando analizada, que puede contener una tubería.
   */
  void ejecutar(const Pipeline_cmd &args);

  Parser parser;  ///<  Módulo responsable de analizar la entrada del usuario.

  BuiltinManager builtin_manager;  ///< Módulo responsable de gestionar los comandos internos.

  vector<pid_t> background_pids;  ///< Vector para rastrear los PIDs de los procesos en segundo plano.

  Prompt prompt;  ///< Módulo responsable de mostrar el prompt.
};
