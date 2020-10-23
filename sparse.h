#pragma once
#include<unordered_set>
#include <vector>
#include "target.h"
using namespace std;


class sparse_vector
{
public:
    std::unordered_set<int>* vect;
    bool isEligible;
    sparse_vector(){vect = new std::unordered_set<int>;}
    sparse_vector(std::unordered_set<int> &V){vect = new std::unordered_set<int>(V);}
    ~sparse_vector(){delete[] vect;}
    void add_point(int point);
    void delete_point(int point);
};

class sparse_matrix
{
public:
    int n;
    std::vector<std::unordered_set<int>> mat;
    sparse_matrix(int size);
    sparse_matrix(vector<Target> targets, double R);
    sparse_matrix(const sparse_matrix &M);
    ~sparse_matrix(){mat.clear();}
    void add_edge(int a, int b);
    void delete_edge(int a, int b);
    void fill_as_communication_graph(sparse_matrix &M_comm, sparse_vector *vect);
    void fill_as_captation_graph(sparse_matrix &M_capt, sparse_vector *vect);
    void display();
};

bool is_eligible(sparse_vector *vect, int k, sparse_matrix &M_comm, sparse_matrix &M_capt);
void create_communication_tree(sparse_matrix &M_result, sparse_matrix M_comm, sparse_vector vect);
bool check_modification(sparse_matrix M_capt, sparse_matrix M_comm, sparse_vector vect_init, sparse_vector vect_fin);
