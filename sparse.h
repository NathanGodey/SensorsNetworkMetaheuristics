#pragma once
#include <unordered_set>
#include <vector>
#include "target.h"
#define INF (unsigned)!((int)0)

using namespace std;


class sparse_vector
{
public:
    std::unordered_set<int> vect;
    bool isEligible;
    double fitness = INF;
    double lambda_capt;
    double lambda_comm;
    void display();
    sparse_vector(){vect = {};}
    sparse_vector(std::unordered_set<int> &V){vect = V;}
    sparse_vector(const sparse_vector &v_original);
    ~sparse_vector(){vect.clear();}
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
    ~sparse_matrix(){mat.clear();}
    void add_edge(int a, int b);
    void delete_edge(int a, int b);
    void fill_as_communication_graph(sparse_matrix &M_comm, sparse_vector *vect);
    void fill_as_captation_graph(sparse_matrix &M_capt, sparse_vector *vect);
    void display();
};

class modification
{
public:
    unordered_set<int>* deleted_captor;
    unordered_set<int>* added_captor;
    modification();
    modification(sparse_vector *vect, int nb_del, int nb_add, int total_targets);
    modification(unordered_set<int> del_point, unordered_set<int> add_point);
    modification(int del_point);
    ~modification(){deleted_captor->clear(); added_captor->clear();}
    bool check_modif(sparse_vector *vect, int k, sparse_matrix &M_comm, sparse_matrix &M_capt);
    void penalization(sparse_vector *vect, int k, sparse_matrix &M_comm, sparse_matrix &M_capt, int& k_captation, int& connexity);
    sparse_vector* apply_modification(sparse_vector *vect, int k, sparse_matrix &M_comm, sparse_matrix &M_capt);
    sparse_vector* apply_modification(sparse_vector *vect);
};

const unordered_set<int> intersection(unordered_set<int> a, unordered_set<int> b);
bool is_eligible(sparse_vector *vect, int k, sparse_matrix &M_comm, sparse_matrix &M_capt);
void create_solution(sparse_vector *captors, vector<Target> targets, double R_capt, int k);
void setSensorsFromVect(vector<Target> &targets, sparse_vector &v);
void setNeighboorsFromCommunicationGraph(vector<Target> &targets, sparse_matrix &Com_graph);
void create_communication_tree(sparse_matrix &M_result, sparse_matrix M_comm, sparse_vector vect);

const unordered_set<int> intersection(unordered_set<int> a, unordered_set<int> b);
