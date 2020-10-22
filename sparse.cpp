#include "sparse.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

sparse_matrix::sparse_matrix(int size) {
    n = size;
    mat.resize(size);
    unordered_set<int> empty;
    for (int i=0; i<size; i++) {
        mat.push_back(empty);
    }
}


sparse_matrix::sparse_matrix(vector<Target> targets, double R)
{
    n = targets.size();
    mat.resize(targets.size());
    for (int i=0; i<targets.size(); i++) {
        for (int j=i+1; j<targets.size(); j++){
            if ((targets[i].x - targets[j].x)*(targets[i].x - targets[j].x)+(targets[i].y - targets[j].y)*(targets[i].y - targets[j].y) <= R) {
                mat[i].insert(j);
                mat[j].insert(i);
            }
        }
    }
}

sparse_matrix::sparse_matrix(const sparse_matrix &M){
    // copy the matrix
}

void sparse_matrix::add_edge(int a, int b){
    if (a<n and b<n){
        mat[a].insert(b);
        mat[b].insert(a);
    }
}

void sparse_matrix::delete_edge(int a, int b){
    if (a<n and b<n){
        mat[a].erase(b);
        mat[b].erase(a);
    }
}

void sparse_matrix::display(){
    for (int i = 0; i<n; i++){
        if (mat[i].size()>0) {
            cout <<i <<"   ( ";
            for (const auto& elem: mat[i]) {
                cout <<elem <<" ";
            }
            cout <<" )" <<endl;
        }
    }
}



void sparse_vector::add_point(int point){
    vect->insert(point);
}

void sparse_vector::delete_point(int point){
    vect->erase(point);
}

const unordered_set<int> intersection(unordered_set<int> a, unordered_set<int> b) {
    unordered_set<int> c;
    for (auto i = a.begin(); i != a.end(); i++) {
        if (b.find(*i)!=b.end()) {
            c.insert(*i);
        }
    }
    return c;
}

void sparse_matrix::fill_as_communication_graph(sparse_matrix &M_comm, sparse_vector &vect){
    for (auto itr = vect.vect->begin(); itr != vect.vect->end(); ++itr) {
        mat[*itr] = intersection(M_comm.mat[*itr], *vect.vect);
    }
}

void sparse_matrix::fill_as_captation_graph(sparse_matrix &M_capt, sparse_vector &vect){
    for (int i=0; i<M_capt.n; i++) {
        mat[i] = intersection(M_capt.mat[i], *vect.vect);
        if (vect.vect->find(i)!=vect.vect->end()) {
            mat[i].insert(i);
        }
    }
}



void create_communication_tree(sparse_matrix &M_result, sparse_matrix M_comm, sparse_vector vect){
    // create a tree that covers the vertex of vect
};

bool check_modification(sparse_matrix M_capt, sparse_matrix M_comm, sparse_vector vect_init, sparse_vector vect_fin){
    // we suppose that vect_init is an admissible solution

    // first we check that every target can still be received

    // then we check that there exists way for each captor to transmit to the well
}

bool is_eligible(sparse_vector &vect, int k, sparse_matrix &M_comm, sparse_matrix &M_capt) {
    sparse_matrix M_comm_activated(M_comm.n);
    M_comm_activated.fill_as_communication_graph(M_comm, vect);
    unordered_set<int> visited = {0}, current;
    vector<int> queue;
    queue.push_back(0);
    while (!queue.empty()) {
        current = M_comm_activated.mat[*queue.begin()];
        for (auto itr = current.begin(); itr != current.end(); ++itr) {
            if (visited.find(*itr) == visited.end()) {
                queue.push_back(*itr);
            }
        }
        visited.insert(*queue.begin());
        queue.erase(queue.begin());
    }
    if (visited.size()<M_comm.n){
        vect.isEligible = false;
        return false;
    }

    sparse_matrix M_capt_activated(M_comm.n);
    M_capt_activated.fill_as_captation_graph(M_capt, vect);
    for (int i=1; i<M_capt.n; i++) {
        if (M_capt.mat[i].size()<k) {
            vect.isEligible = false;
            return false;
        }
    }
    vect.isEligible = true;
    return true;
};
