#pragma once
#include <string>
#include <vector>
#include "target.h"
using namespace std;

class Parser {
public:
  Parser(string instanceFile, vector<Target>& targets);
};

class MinorantsParser {
public:
  MinorantsParser(string instanceFile);
  vector<vector<int>> content;
  int getMinorant(int K, int R_com, int R_capt);
};

vector<string> split(string line, string delimiter);
