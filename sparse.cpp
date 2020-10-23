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

bool is_eligible(sparse_vector *vect, int k, sparse_matrix &M_comm, sparse_matrix &M_capt) {
    sparse_matrix M_comm_activated(M_comm.n);

    M_comm_activated.fill_as_communication_graph(M_comm, vect);
    M_comm_activated.display();
    unordered_set<int> visited = {0}, current;
    vector<int> queue;
    queue.push_back(0);
    while (queue.size()>0) {
        current = M_comm_activated.mat[*queue.begin()];
        for (auto itr = current.begin(); itr != current.end(); ++itr) {
            if (visited.find(*itr) == visited.end()) {
                queue.push_back(*itr);
                visited.insert(*itr);
            }
        }
        queue.erase(queue.begin());
    }
    if (visited.size()<M_comm.n){
        vect->isEligible = false;
        return false;
    }
    sparse_matrix M_capt_activated(M_comm.n);
    M_capt_activated.fill_as_captation_graph(M_capt, vect);
    for (int i=1; i<M_capt.n; i++) {
        if (M_capt.mat[i].size()<k) {
            vect->isEligible = false;
            return false;
        }
    }
    vect->isEligible = true;
    return true;
};
