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
    n = M.n;
    mat.resize(n);
    for (int i=0; i<M.n;i++){
        mat.push_back(M.mat[i]);
    }
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

void sparse_matrix::fill_as_communication_graph(sparse_matrix &M_comm, sparse_vector *vect){
    for (auto itr = vect->vect->begin(); itr != vect->vect->end(); ++itr) {
        mat[*itr] = intersection(M_comm.mat[*itr], *vect->vect);
    }
}

void sparse_matrix::fill_as_captation_graph(sparse_matrix &M_capt, sparse_vector *vect){
    for (int i=0; i<M_capt.n; i++) {
        mat[i] = intersection(M_capt.mat[i], *vect->vect);
        if (vect->vect->find(i)!=vect->vect->end()) {
            mat[i].insert(i);
        }
    }
}

modification::modification(sparse_vector *vect, int nb_deletion, int nb_add, int &total_targets){
    int nb_del = min(nb_deletion, int(vect->vect->size()-1)); // there needs to be at least 0
    deleted_captor = new unordered_set<int>;
    added_captor = new unordered_set<int>;
    int r;
    while (added_captor->size()<nb_add){
        r = rand()%total_targets;
        if (vect->vect->find(r)==vect->vect->end() and added_captor->find(r)==added_captor->end()){
            added_captor->insert(r);
        }
    }
    while (deleted_captor->size()<nb_del){
        // taken from https://www.tfzx.net/article/2821385.html
        // randomly selects an item from vect->vect with uniform distribution?
        auto b_count =vect->vect->bucket_count();
        auto b_idx = rand() % b_count;
        size_t b_size = 0;

        for (int i = 0; i < b_count; ++i)
        {
            b_size = vect->vect->bucket_size(b_idx);
            if (b_size > 0)
                break;

            b_idx = (b_idx + 1) % b_count;
        }

        auto idx = rand() % b_size;

        auto it = vect->vect->cbegin(b_idx);

        for (int i = 0; i < idx; ++i)
        {
            it++;
        }
        if (deleted_captor->find(*it) == deleted_captor->end() and *it){
            deleted_captor->insert(*it);
        }
    }
}

modification::modification(unordered_set<int> del_point, unordered_set<int> add_point){
    deleted_captor = new unordered_set<int>;
    added_captor = new unordered_set<int>;
    for (auto i = del_point.begin(); i != del_point.end(); ++i){
        if (*i != 0) deleted_captor->insert(*i);
    }
    for (auto i = add_point.begin(); i != add_point.end(); ++i){
        deleted_captor->insert(*i);
    }
}

bool modification::check_modif(sparse_vector *vect, int k, sparse_matrix &M_comm, sparse_matrix &M_capt){
    // we verify that the modification is applicable to the vecteur
    sparse_vector* new_vect = new sparse_vector(*vect);
    for (auto i = deleted_captor->begin(); i != deleted_captor->end(); ++i){
        if (vect->vect->find(*i) == vect->vect->end()){
            delete new_vect;
            return false;
        }
        else {
            (*new_vect).delete_point(*i);
        }
    }
    for (auto i = added_captor->begin(); i != added_captor->end(); ++i){
        if (vect->vect->find(*i) != vect->vect->end()){
            delete new_vect;
            return false;
        }
        else {
            (*new_vect).add_point(*i);
        }
    }
    sparse_matrix M_comm_activated(M_comm.n);
    M_comm_activated.fill_as_communication_graph(M_comm, new_vect);

    unordered_set<int> targ,targ_captors; // targ_captors will hold the list of captors for a target
    targ.insert(0); // targ is the list of checked target, we don't need to check for 0
    int is_capt=0;
    if (vect->isEligible){
        // we verify that each target that was linked to deleted captors are capted by at least k captors
        for (auto i = deleted_captor->begin(); i != deleted_captor->end(); ++i){
            // the deleted captor needs to have k captors around it as well
            if (targ.find(*i)==targ.end()){
                targ_captors = intersection(*(*new_vect).vect,M_capt.mat[*i]);
                if (targ_captors.size()<k){
                    delete new_vect;
                    return false;
                }
                targ.insert(*i);
            }
            for (auto j = M_capt.mat[*i].begin(); j != M_capt.mat[*i].end(); ++j){
                if (targ.find(*j)==targ.end()){
                    targ_captors = intersection(*(*new_vect).vect,M_capt.mat[*j]);
                    if ((*new_vect).vect->find(*j) != (*new_vect).vect->end()) is_capt = 1; // if the target is a captor, we have to count it
                    else is_capt = 0;
                    if (targ_captors.size()+is_capt<k){
                        delete new_vect;
                        return false;
                    }
                    targ.insert(*j);
                }
            }
        }
        // we verify that each captor that was linked to deleted captors can communitate to the well
        // we first quickly check that no captor is isolated
        unordered_set<int> neighboring_captors, checked_captors;
        for (auto i = deleted_captor->begin(); i != deleted_captor->end(); ++i){
            neighboring_captors = intersection(*(*new_vect).vect,M_comm_activated.mat[*i]);
            for (auto j = neighboring_captors.begin(); j != neighboring_captors.end(); ++j){
                if (intersection(*(*new_vect).vect,M_comm_activated.mat[*j]).size() == 0){
                    delete new_vect;
                    return false;
                }
            }
        }

    }
    else { // we check for target
        for (int i=1; i<M_capt.n;i++){
            for (auto j = M_capt.mat[i].begin(); j != M_capt.mat[i].end(); ++j){
                targ_captors = intersection(*(*new_vect).vect,M_capt.mat[*j]);
                if ((*new_vect).vect->find(*j) != (*new_vect).vect->end()) is_capt = 1; // if the target is a captor, we have to count it
                else is_capt = 0;
                if (targ_captors.size()<k+is_capt){
                    delete new_vect;
                    return false;
                }
            }
        }
    }
    // we look at the connexity of the graph (if vect is not eligible or if the communication graph is not trivially non-connex
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
    if (visited.size()<new_vect->vect->size()){
        delete new_vect;
        return false;
    }
    delete new_vect;
    return true;
}

sparse_vector modification::apply_modification(sparse_vector *vect, int k, sparse_matrix &M_comm, sparse_matrix &M_capt){
    sparse_vector new_vect = sparse_vector(*vect);
    new_vect.isEligible = check_modif(vect,k,M_comm,M_capt);
    for (auto i = deleted_captor->begin(); i != deleted_captor->end(); ++i){
        if (vect->vect->find(*i) != vect->vect->end()){
            (new_vect).delete_point(*i);
        }
    }
    for (auto i = added_captor->begin(); i != added_captor->end(); ++i){
        if (vect->vect->find(*i) == vect->vect->end()){
            (new_vect).add_point(*i);
        }
    }
    return new_vect;
}

sparse_vector create_solution(sparse_matrix M_capt, int k){
    sparse_matrix M = sparse_matrix(M_capt);
    sparse_vector captors = sparse_vector();
    // we will create k tree (with BFS), those trees have minimum depth
    // each time a tree is created, nodes become captors and the edges of the tree are deleted
    // if such a thing is possible, the list of captors will be admissible
    for (int i=0; i<k; i++){
        unordered_set<int> visited = {0}, current;
        vector<int> queue;
        queue.push_back(0);
        while (queue.size()>0) {
            current = M_capt.mat[*queue.begin()];
            for (auto itr = current.begin(); itr != current.end(); ++itr) {
                if (visited.find(*itr) == visited.end()) {
                    queue.push_back(*itr);
                    visited.insert(*itr);
                    M.delete_edge(*itr,*queue.begin()); // in further loops, this edge wont be able to form a captation connection
                    captors.add_point(*queue.begin()); // every node will be a captor (leaves wont be)
                }
            }
            queue.erase(queue.begin());
        }
        if (visited.size()<M_capt.n){
            for (int i=0; i< M_capt.n; i++){
                captors.vect->insert(i);
            }
        }
    }
}


bool is_eligible(sparse_vector *vect, int k, sparse_matrix &M_comm, sparse_matrix &M_capt) {
    sparse_matrix M_comm_activated(M_comm.n);

    M_comm_activated.fill_as_communication_graph(M_comm, vect);
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
    if (visited.size()<vect->vect->size()){
        vect->isEligible = false;
        return false;
    }
    sparse_matrix M_capt_activated(M_capt.n);
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
