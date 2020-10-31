#include "sparse.h"
#include "target.h"
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
    int R_squared = R * R;
    mat.resize(targets.size());
    for (int i=0; i<targets.size(); i++) {
        for (int j=i+1; j<targets.size(); j++){
            if ((targets[i].x - targets[j].x)*(targets[i].x - targets[j].x)+(targets[i].y - targets[j].y)*(targets[i].y - targets[j].y) <= R_squared) {
                mat[i].insert(j);
                mat[j].insert(i);
            }
        }
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


sparse_vector::sparse_vector(sparse_vector &v_original) {
    vect = new unordered_set<int>(*v_original.vect);
    isEligible = v_original.isEligible;
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
        added_captor->insert(*i);
    }
}

modification::modification(int del_point) {
    added_captor = new unordered_set<int>;
    deleted_captor = new unordered_set<int>;
    deleted_captor->insert(del_point);
}

modification::modification() {
    added_captor = new unordered_set<int>;
    deleted_captor = new unordered_set<int>;
}

bool modification::check_modif(sparse_vector *vect, int k, sparse_matrix &M_comm, sparse_matrix &M_capt){
    // we verify that the modification is applicable to the vecteur (and we apply it)
    sparse_vector* new_vect = new sparse_vector(*vect);
    for (auto i = deleted_captor->begin(); i != deleted_captor->end(); ++i){
        if (vect->vect->find(*i) == vect->vect->end()){
            return false;
        }
        else {
            (*new_vect).delete_point(*i);
        }
    }
    for (auto i = added_captor->begin(); i != added_captor->end(); ++i){
        if (vect->vect->find(*i) != vect->vect->end()){
            return false;
        }
        else {
            (*new_vect).add_point(*i);
        }
    }

    sparse_matrix M_comm_activated(M_comm.n);
    M_comm_activated.fill_as_communication_graph(M_comm, new_vect);

    unordered_set<int> targ_captors,capt; // targ_captors will hold the list of captors for a target
    if (vect->isEligible){
        // we verify that each target that was linked to deleted captors are capted by at least k captors
        for (auto i = deleted_captor->begin(); i != deleted_captor->end(); ++i){
            targ_captors = M_capt.mat[*i];
            // the deleted captor needs to have k captors around it as well
            for (auto j=targ_captors.begin(); j!=targ_captors.end();++j){
                capt = intersection(M_capt.mat[*j],*new_vect->vect);
                if (capt.size()<(k-(new_vect->vect->find(*j) != new_vect->vect->end()))){
                    return false;
                }
            }
        }
        // we verify that each captor that was linked to deleted captors can communitate to the well
        // we first quickly check that no captor is isolated (otherwise it is going to be done with the code below)
        unordered_set<int> neighboring_captors, checked_captors;
        for (auto i = deleted_captor->begin(); i != deleted_captor->end(); ++i){
            neighboring_captors = intersection(*(*new_vect).vect,M_comm_activated.mat[*i]);
            for (auto j = neighboring_captors.begin(); j != neighboring_captors.end(); ++j){
                if (intersection(*(*new_vect).vect,M_comm_activated.mat[*j]).size() == 0){
                    return false;
                }
            }
        }

    }
    else { // we check that every target can be capted by k different captors
        for (int i=1; i<M_capt.n; i++) {
            capt = intersection(M_capt.mat[i],*new_vect->vect);
            if (capt.size()<(k-(vect->vect->find(i) != vect->vect->end()))) {
                return false;
            }
        }

    }
    // we look at the connexity of the graph (if vect is not eligible or if the communication graph is not trivially non-connex)
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
        return false;
    }
    return true;
}

sparse_vector* modification::apply_modification(sparse_vector *vect, int k, sparse_matrix &M_comm, sparse_matrix &M_capt){
    sparse_vector* new_vect = new sparse_vector(*vect);
    new_vect->isEligible = check_modif(vect,k,M_comm,M_capt);
    for (auto i = deleted_captor->begin(); i != deleted_captor->end(); ++i){
        (new_vect)->delete_point(*i);
    }
    for (auto i = added_captor->begin(); i != added_captor->end(); ++i){
        (new_vect)->add_point(*i);
    }
    return new_vect;
}

sparse_vector* modification::apply_modification(sparse_vector *vect){
    sparse_vector* new_vect = new sparse_vector(*vect);
    for (auto i = deleted_captor->begin(); i != deleted_captor->end(); ++i){
        (new_vect)->delete_point(*i);
    }
    for (auto i = added_captor->begin(); i != added_captor->end(); ++i){
        (new_vect)->add_point(*i);
    }
    return new_vect;
}

void create_solution(sparse_vector *captors, vector<Target> targets, double R_capt, int k){
    sparse_matrix M = sparse_matrix(targets,R_capt);
    // we will create k tree (with BFS), those trees have minimum depth
    // each time a tree is created, nodes become captors and the edges of the tree are deleted
    // if such a thing is possible, the list of captors will be admissible
    for (int it=0; it<k; it++){
        unordered_set<int> visited = {0}, current;
        vector<int> queue, edges_b, edges_e;
        queue.push_back(0);
        while (queue.size()>0) {
            current = M.mat[*queue.begin()];
            for (auto itr = current.begin(); itr != current.end(); ++itr){
                if (visited.find(*itr) == visited.end()) {
                    queue.push_back(*itr);
                    visited.insert(*itr);
                    captors->add_point(*queue.begin());
                    edges_b.push_back(*queue.begin());
                    edges_e.push_back(*itr);
                }
            }
            queue.erase(queue.begin());
        }

        if (it == 0 and k!=1){
            for (int e=0; e<edges_e.size();e++){
                if (edges_b[e] !=0){
                    M.delete_edge(edges_b[e],edges_e[e]);
                }
            }
        }

        if (it == 1 and k!=2){
            for (int e=0; e<edges_e.size();e++){
                if (edges_b[e] !=0 and captors->vect->find(edges_e[e]) == captors->vect->end()){
                    M.delete_edge(edges_b[e],edges_e[e]);
                }
            }
        }

        if (visited.size()<M.n){
            for (int f=0; f<M.n;f++){
                if (visited.find(f) == visited.end()){
                    captors->add_point(f);
                }
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
        if (M_capt_activated.mat[i].size() < (k-(vect->vect->find(i) != vect->vect->end()))) {
            vect->isEligible = false;
            return false;
        }
    }
    vect->isEligible = true;
    return true;
};


void setSensorsFromVect(vector<Target> &targets, sparse_vector &v) {
		for (auto itr = v.vect->begin(); itr != v.vect->end(); itr++){
				targets[*itr].isSensor = true;
		}
};

void setNeighboorsFromCommunicationGraph(vector<Target> &targets, sparse_matrix &Com_graph){
    for (int i=0; i<targets.size(); i++)  {
        targets[i].neighbours = Com_graph.mat[i];
    }
}
