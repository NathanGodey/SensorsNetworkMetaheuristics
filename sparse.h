#pragma once
#include<unordered_set>
#include <vector>
#include "target.h"
using namespace std;

class sparse_matrix
{
public:
    int n;
    std::unordered_set<int>* mat;
    sparse_matrix(int size){n = size ; mat = new std::unordered_set<int> [n];}
    sparse_matrix(vector<Target> targets, double R);
    ~sparse_matrix(){delete[] mat;}
    sparse_matrix sparse_matrix::copy();
    void add_edge(int a, int b);
    void delete_edge(int a, int b);
};

class sparse_vector
{
public:
    std::unordered_set<int>* vect;
    sparse_vector(){vect = new std::unordered_set<int>;}
    ~sparse_vector(){delete[] vect;}
    void add_point(int point);
    void delete_point(int point);
};

sparse_matrix create_tree(sparse_matrix M, sparse_vector vect);
bool check_modification(sparse_matrix M_capt, sparse_matrix M_comm, sparse_vector vect_init, sparse_vector vect_fin);
