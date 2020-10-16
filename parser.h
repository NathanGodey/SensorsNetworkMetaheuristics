#pragma once
#include <string>
#include <vector>
#include "target.h"
using namespace std;

class Parser {
public:
  Parser(string instanceFile, vector<Target>& targets);
};
