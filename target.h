#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
class Target {
public:
  double x;
	double y;
  bool isWell;
  bool isSensor;
  Target(double x_, double y_, bool isWell_) {
    x = x_;
    y = y_;
    isWell = isWell_;
  }
};
