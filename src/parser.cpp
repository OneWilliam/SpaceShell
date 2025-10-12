#include "parser.hpp"
#include <sstream>
#include <iostream>

using namespace std;

ComandoInfo Parser::parsear_linea(const string& linea) {
  stringstream ss(linea);
  string token;
  vector<string> tokens;
  
  while (ss >> token) {
    tokens.push_back(token);
  }

  ComandoInfo resultado;
    
  for (size_t i = 0; i < tokens.size(); ++i) {
    if (tokens[i] == ">") {
      
      if (i + 1 < tokens.size()) {
        resultado.outputFile = tokens[i + 1];
        i++;
      } else {
        cerr << "[SHELL] Error de sintaxis: Falta el nombre del archivo después de '>'." << endl;
        resultado.args.clear();
        return resultado;
      }

    } else {
      resultado.args.push_back(tokens[i]);
    }
  }
  
  return resultado;
}
