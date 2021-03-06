#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

class Target {
public:
  int id;
  double x;
	double y;
  bool isSensor;
  unordered_set<int> neighbours;
  double weight = 0;
  Target(int id_, double x_, double y_, bool isSensor_) {
    id = id_;
    x = x_;
    y = y_;
    isSensor = isSensor_;
  }
  void clear() {neighbours.clear(); isSensor=0;}
};

void displayWeights(vector<Target> &targets);
vector<int> sortedTargetsIds(vector<Target> &targets);
inline bool operator< (Target& l, Target& r){ return r.weight > l.weight; };
inline bool cmpId (Target& l, Target& r){ return r.id > l.id; };
void writeToTxt(vector<Target> targets, string instance_name, int K, int R_comm, int R_capt);
void clear_all(vector<Target> &targets);
