#include "parser.hpp"
#include <sstream>
#include <iostream>

using namespace std;

vector<ComandoInfo> Parser::parsear_linea(const string &linea) {
  stringstream ss(linea);
  string token;
  vector<string> tokens;
  
  while (ss >> token) {
    tokens.push_back(token);
  }

  vector <ComandoInfo> pipeline;
  if (tokens.empty()) {
      return pipeline;
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

  return pipeline;
}
