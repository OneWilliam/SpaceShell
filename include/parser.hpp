#pragma once 
#include <string>
#include <vector>
using namespace std;

struct ComandoInfo {
  vector<string> args;
  string outputFile;
  string inputFile;
  bool anexar = false;
};

struct Pipeline_cmd {
  vector<ComandoInfo> pipeline;
  bool background = false;
};



class Parser {
  public:
    Pipeline_cmd parsear_linea(const string &linea);
  private:
    vector<string> parsear(const string& linea);
};
