#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "parser.hpp"

using namespace std;

/**
 * Gestiona el registro y la ejecución de comandos internos (built-ins).
 *
 * Esta clase proporciona una manera de extender comandos que se
 * ejecutan directamente en el proceso de la shell, como 'cd' o 'pwd'.
 */
class BuiltinManager {
 public:
  /**
   * Constructor que registra todos los comandos (built-ins) disponibles.
   */
  BuiltinManager();
  /**
   * Comprueba si un nombre de comando corresponde a un built-in registrado.
   *
   * @param nombre El nombre del comando a comprobar.
   * @return Verdadero si es un built-in, falso en caso contrario.
   */
  bool es_builtin(const string& nombre) const;

  /**
   * Ejecuta el comando interno correspondiente.
   *
   * @param cmd El comando analizado que se va a ejecutar.
   */
  void ejecutar(const ComandoInfo& cmd);

 private:
  /**
   * Implementación del comando 'cd'. Cambia el directorio del proceso de la shell.
   * @param cmd El comando 'cd' con sus argumentos.
   */
  void handle_cd(const ComandoInfo& cmd);

  /**
   * Implementación del comando 'pwd'. Imprime el directorio actual.
   *
   * @param cmd El comando 'pwd' (los argumentos se ignoran).
   */
  void handle_pwd(const ComandoInfo& cmd);

  /**
   * Implementación del comando 'parallel'. Ejecuta múltiples comandos en hilos concurrentes.
   *
   * @param cmd El comando 'parallel' seguido de la lista de comandos a ejecutar.
   */
  void handle_parallel(const ComandoInfo& cmd);

  /**
   * Mapa que asocia nombres de comandos con las funciones que los implementan.
   */
  map<string, function<void(const ComandoInfo&)>> builtins;
};
