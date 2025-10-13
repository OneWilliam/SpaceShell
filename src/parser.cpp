#include "parser.hpp"

#include <iostream>
#include <sstream>

using namespace std;

Pipeline_cmd Parser::parsear_linea(const string& linea) {
  Pipeline_cmd resultado;
  vector<string> tokens = parsear(linea);

  if (tokens.empty()) {
    return resultado;
  }

  if (tokens.back() == "&") {
    resultado.background = true;
    tokens.pop_back();
  }

  resultado.pipeline.emplace_back();
  bool outputFileExp = false;
  bool inputFileExp = false;

  for (const auto& token : tokens) {
    if (token == ">" || token == ">>") {
      if (outputFileExp || inputFileExp) {
        cerr << "[SHELL] Error de sintaxis: Múltiples redirecciones no "
                "soportadas."
             << endl;
        return {};
      }
      outputFileExp = true;
      if (token == ">>") {
        resultado.pipeline.back().anexar = true;
      }
    } else if (token == "<") {
      if (outputFileExp || inputFileExp) {
        cerr << "[SHELL] Error de sintaxis: Múltiples redirecciones no "
                "soportadas."
             << endl;
      }
      inputFileExp = true;
    } else if (token == "|") {
      if (resultado.pipeline.back().args.empty() || inputFileExp || outputFileExp) {
        cerr << "[SHELL] Error de sintaxis: Comando vacío en la tubería." << endl;
        return {};
      }
      outputFileExp = false;
      resultado.pipeline.emplace_back();

    } else {
      if (outputFileExp) {
        resultado.pipeline.back().outputFile = token;
        outputFileExp = false;
      } else if (inputFileExp) {
        resultado.pipeline.back().inputFile = token;
        inputFileExp = false;
      } else {
        resultado.pipeline.back().args.push_back(token);
      }
    }
  }

  if (inputFileExp || outputFileExp) {
    cerr << "[SHELL] Error de sintaxis: Falta nombre de archivo para "
            "redirección."
         << endl;
    return {};
  }

  if (!resultado.pipeline.empty() && resultado.pipeline.back().args.empty()) {
    cerr << "[SHELL] Error de sintaxis: La línea no puede terminar con '|'." << endl;
    return {};
  }

  return resultado;
}

vector<string> Parser::parsear(const string& linea) {
  vector<string> tokens;
  string token;
  char comilla = 0;

  for (char c : linea) {
    if (comilla) {
      if (c == comilla) {
        comilla = 0;
      } else {
        token += c;
      }
    } else {
      if (c == '\'' || c == '"') {
        comilla = c;
      } else if (isspace(c)) {
        if (!token.empty()) {
          tokens.push_back(token);
          token.clear();
        }

      } else {
        token += c;
      }
    }
  }

  if (!token.empty()) {
    tokens.push_back(token);
  }

  return tokens;
}
