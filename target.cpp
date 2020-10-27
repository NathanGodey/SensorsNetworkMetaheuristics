#include "target.h"
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
