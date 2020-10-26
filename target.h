#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

class Target {
public:
  double x;
	double y;
  bool isSensor;
  unordered_set<int> neighbours;
  Target(double x_, double y_, bool isSensor_) {
    x = x_;
    y = y_;
    isSensor = isSensor_;
  }
};
