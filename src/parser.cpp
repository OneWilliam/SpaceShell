#include "parser.hpp"
#include <sstream>
#include <iostream>

using namespace std;

Pipeline_cmd Parser::parsear_linea(const string &linea) {
  Pipeline_cmd resultado;
  vector<string> tokens = parsear(linea);

  vector <ComandoInfo> pipeline;
  if (tokens.empty()) {
      return resultado;
  }
  
  if (tokens.back() == "&") {
      resultado.background = true;
      tokens.pop_back();
  }

  pipeline.emplace_back();
  bool outputFileExp = false;


  for (size_t i = 0; i < tokens.size(); ++i) {

    if (tokens[i] == ">") {
        if (outputFileExp) {
            cerr << "[SHELL] Error de sintaxis: Múltiples redirecciones no soportadas." << endl;
            return {};
        }
        outputFileExp = true;
    } else if (tokens[i] == "|"){
        if (pipeline.back().args.empty()) {
                cerr << "[SHELL] Error de sintaxis: Comando vacío en la tubería." << endl;
                return {};
        }
        outputFileExp = false;
        pipeline.emplace_back();

    } else {
        if(outputFileExp) {
          pipeline.back().outputFile = tokens[i];
          outputFileExp = false;
        } else {
          pipeline.back().args.push_back(tokens[i]);
        }
    }

  }
  
  if (outputFileExp) {
      cerr << "[SHELL] Error de sintaxis: Falta el nombre del archivo después de '>'." << endl;
      return {};
  }

  if (!pipeline.empty() && pipeline.back().args.empty()) {
      cerr << "[SHELL] Error de sintaxis: La línea no puede terminar con '|'." << endl;
      return {};
  }

  resultado.pipeline = pipeline;
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
