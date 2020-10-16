#pragma once

class Target {
public:
  double x;
	double y;
  bool isWell;
  bool isSensor;
  Target(double x, double y, bool isWell) {
    x = x;
    y = y;
    isWell = isWell;
  }
};
