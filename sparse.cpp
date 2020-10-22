#include "sparse.h"

sparse_matrix::sparse_matrix(vector<Target> targets, double R)
{
    n = targets.size();
    mat = new std::unordered_set<int>[n];
    for (int i=0; i<targets.size(); i++) {
        for (int j=i+1; j<targets.size(); j++){
            if ((targets[i].x - targets[j].x)*(targets[i].x - targets[j].x)+(targets[i].y - targets[j].y)*(targets[i].y - targets[j].y) <= R) {
                mat[i].insert(j);
                mat[j].insert(i);
            }
        }
    }
}

sparse_matrix sparse_matrix::copy(){
    // copy the matrix
}

void sparse_matrix::add_edge(int a, int b){
    if (a<n and b<n){
        if (mat[a].count(b)==0){
            mat[a].insert(b);
        }
    }
}

void sparse_matrix::delete_edge(int a, int b){
    if (a<n and b<n){
        if (mat[a].count(b)==1){
            mat[a].insert(b);
        }
    }
}

void sparse_vector::add_point(int point){
    if (vect->count(point)==0) {
        vect->insert(point);
    }
}

void sparse_vector::delete_point(int point){
    if (vect->count(point)==1) {
        vect->erase(point);
    }
}

sparse_matrix create_tree(sparse_matrix M, sparse_vector vect){
    // create a tree that covers the vertex of vect
}

bool check_modification(sparse_matrix M_capt, sparse_matrix M_comm, sparse_vector vect_init, sparse_vector vect_fin){
    // we suppose that vect_init is an admissible solution

    // first we check that every target can still be received

    // then we check that there exists way for each captor to transmit to the well
}
