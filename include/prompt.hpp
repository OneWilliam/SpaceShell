#pragma once
#include <string>

/**
 * Responsable de generar y mostrar el indicador de la shell.
 *
 * Encapsula la lógica para crear un prompt que puede mostrar
 * información como el directorio de actual.
 */
class Prompt {
 public:
  /**
   * Muestra el prompt formateado en la salida estándar.
   *
   * @tip Se planear hacer uso de algunas funciones builtins para mejorar
   * la funciones del prompt.
   */
  void mostrar();
};
