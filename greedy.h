#pragma once
#include <vector>
#include "sparse.h"

using namespace std;


void setTargetsWeights(vector<Target> &targets, sparse_matrix &M_capt);
sparse_vector* greedyOptimization(sparse_vector *v, vector<int> &removal_queue, int K, sparse_matrix &M_comm, sparse_matrix &M_capt);
