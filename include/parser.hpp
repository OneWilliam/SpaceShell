#pragma once
#include <string>
#include <vector>
using namespace std;
/**
 * @struct ComandoInfo
 * @brief Almacena la información de un único comando simple dentro de una
 * tubería (o pipeline)
 *
 */
struct ComandoInfo {
  vector<string> args;  ///< Lista de argumentos del comando
  string outputFile;    ///< Nombre del archivo para redirección de salida ('>' o '>>')
  string inputFile;     ///< Nombre del archivo para redirección de entrada ('<')
  bool anexar = false;  ///< Verdadero si se usa '>>' para anexar a un archivo de salida
};

/**
 * @struct Pipeline_cmd
 * @brief Representa una línea de comando completa analizada
 *
 * Contiene una secuencia de comandos simples (separados por '|') y si debe
 * ejecutarse en segundo plano.
 */
struct Pipeline_cmd {
  vector<ComandoInfo> pipeline;  ///< Secuencia de comandos a ejecutar en la tubería
  bool background = false;       ///< Verdadero si el comando termina con '&' para
                                 ///< tareas en segundo plano
};

/**
 * @class Parser
 * @brief Responsable de analizar la entrada de texto
 *
 * Convierte una cadena de texto sin procesar en una estructura `Pipeline_cmd`
 * que la shell puede entender y ejecutar. Maneja tuberías, redirecciones,
 * tareas en segundo plano y argumentos que usan comillas ('', "").
 */
class Parser {
 public:
  /**
   * @brief Analiza una línea de comando completa (incluyendo tuberías)
   *
   * @param linea La cadena de texto ingresada
   * @return Una estructura `Pipeline_cmd` que representa el comando analizado.
   *
   */
  Pipeline_cmd parsear_linea(const string& linea);

 private:
  /**
   * @brief Convierte una línea de texto en una lista de tokens
   *
   * Esta función es la primera etapa del análisis.
   * Divide la entrada por espacios, pero respeta los argumentos agrupados por comillas
   *
   * @param linea La cadena de texto sin procesar
   * @return Un vector de cadenas, donde cada cadena es un token
   */
  vector<string> parsear(const string& linea);
};
