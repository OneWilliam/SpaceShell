#pragma once 
#include <string>
#include <vector>
using namespace std;

struct ComandoInfo {
  vector<std::string> args;
  string outputFile;
};



class Parser {
  public:
    vector<ComandoInfo> parsear_linea(const string &linea);
  private:
    vector<string> parsear(const string& linea);
};
