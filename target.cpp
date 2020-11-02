#include "target.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;


void displayWeights(vector<Target> &targets) {
    for (int i =0; i<targets.size(); i++) {
        cout <<targets[i].id <<"  ->  " <<targets[i].weight <<endl;
    }
}

vector<int> sortedTargetsIds(vector<Target> &targets){
    sort(targets.begin(), targets.end());
    vector<int> sorted_targets_ids;
    for (int i=0; i<targets.size(); i++) {
        sorted_targets_ids.push_back(targets[i].id);
    }
    sort(targets.begin(), targets.end(), cmpId);
    return sorted_targets_ids;
}

void writeToTxt(vector<Target> targets, string instance_name, int K, int R_comm, int R_capt){
    ofstream myfile;
    myfile.open("result.txt");
    myfile << instance_name <<endl;
    myfile <<K <<' ' << R_comm <<' ' << R_capt <<endl;
    for (int i=0; i<targets.size(); i++) {
        string neighbours_str = "";
        for (auto itr = targets[i].neighbours.begin(); itr!=targets[i].neighbours.end(); itr++) {
                neighbours_str = neighbours_str + to_string(*itr) + ',';
        }
        neighbours_str = "["+neighbours_str.substr(0,neighbours_str.length()-1)+']';
        myfile <<targets[i].id<<" " <<targets[i].x <<' ' <<targets[i].y <<' '<< targets[i].isSensor <<' '<<neighbours_str <<' '<<targets[i].weight << endl;
    }
    myfile.close();
}


void clear_all(vector<Target> &targets){
    for (int i=0; i<targets.size(); i++) {
        targets[i].clear();
    }
}
