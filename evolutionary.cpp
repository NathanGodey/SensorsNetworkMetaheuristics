#include "evolutionary.h"
#include "sparse.h"
#include <cmath>
#include <fstream>
#include <algorithm>

using namespace std;

Individual::Individual(int id, sparse_vector &elder, double avg_insertions, int nb_targets, double _lambda_capt, double _lambda_comm) : sparse_vector(elder) {
		int nb_insertions = floor(0.5*avg_insertions*nb_targets) + rand() % max(1, int(avg_insertions*nb_targets));
		nb_insertions = min(nb_insertions, nb_targets - int(vect->size()));
		modification* modif = new modification(&elder, 0, nb_insertions, nb_targets);
		vect = modif->apply_modification(&elder)->vect;
		lambda_capt = _lambda_capt;
		lambda_comm = _lambda_comm;
		fitness = vect->size() - 1;
}

Individual::Individual() : sparse_vector() {
		fitness = vect->size() - 1;
}

Individual::Individual(const Individual &ind_original) : sparse_vector(ind_original) {
		id = ind_original.id;
		fitness = ind_original.vect->size() - 1;
}

Individual::Individual(sparse_vector& v_original) : sparse_vector(v_original){
		fitness = v_original.vect->size() - 1;
}

Individual Individual::cross(Individual& other, int avg_gene_size, int id_child, int K, sparse_matrix M_comm, sparse_matrix M_capt){
		int nb_targets = M_comm.n;
		int min_cut = 1+rand()%(nb_targets-1);
		int max_cut = min_cut + int(avg_gene_size/2) + rand()%(int(avg_gene_size/2));
		unordered_set<int> range, geneA, geneB;
		for (int i=min_cut; i<=max_cut; i++){
				range.insert(i);
		}
		geneA = intersection(range, *vect);
		geneB = intersection(range, *(other.vect));
		modification cross_modif(geneA, geneB);
    Individual* child = new Individual(*cross_modif.apply_modification(&other,K,M_comm,M_capt));
		child->id = id_child;
		return *child;
}

Population::Population(int init_size, sparse_vector &elder, double avg_insertions, double _lambda_capt, double _lambda_comm, int _nb_targets){
		size = init_size;
		nb_targets = _nb_targets;
		lambda_capt = _lambda_capt;
		lambda_comm = _lambda_comm;
		individuals = new vector<Individual>;
		Individual* current;
		for (int i=0; i<init_size; i++) {
				current = new Individual(i, elder, avg_insertions, nb_targets, _lambda_capt, _lambda_comm);
				individuals->push_back(*current);
		}
}

void Population::compute_metrics() {
		avg_fitness = 0;
		best_fitness = (*individuals)[0].fitness;
		best_id = 0;
		for (int i=0; i<size; i++) {
				if ((*individuals)[i].fitness < best_fitness) {
						best_fitness = (*individuals)[i].fitness;
						best_id = i;
				}
				avg_fitness = avg_fitness + (*individuals)[i].fitness;
		}
		avg_fitness = avg_fitness / size;
}


void Population::write_to_file(string file_name, vector<Target>& targets, sparse_matrix &M_comm){
		ofstream file;
		file.open(file_name, ios::app);
		file <<"###" <<endl;
		compute_metrics();
		sparse_vector best_individual = (*individuals)[best_id];

		file <<size <<" " <<best_fitness <<' ' <<avg_fitness <<" " <<best_individual.vect->size()<<endl;

		sparse_matrix Com_graph(targets.size());
		Com_graph.fill_as_communication_graph(M_comm, &best_individual);
		setSensorsFromVect(targets, best_individual);
		setNeighboorsFromCommunicationGraph(targets, Com_graph);

		for (int i=0; i<targets.size(); i++) {
				string neighbours_str = "";
				for (auto itr = targets[i].neighbours.begin(); itr!=targets[i].neighbours.end(); itr++) {
								neighbours_str = neighbours_str + to_string(*itr) + ',';
				}
				neighbours_str = "["+neighbours_str.substr(0,neighbours_str.length()-1)+']';
				file <<targets[i].id<<" " <<targets[i].x <<' ' <<targets[i].y <<' '<< targets[i].isSensor <<' '<<neighbours_str <<' '<<targets[i].weight << endl;
		}
		file.close();
}



void Population::cross(double reproduction_rate, int avg_gene_size, int K, sparse_matrix &M_comm, sparse_matrix &M_capt) {
		int nb_cross = int(reproduction_rate*size);
		int parent_A, parent_B;
		for (int i=0; i<nb_cross; i++) {
				parent_A = rand() % size;
				parent_B = parent_A;
				while (parent_B == parent_A) {
						parent_B = rand() % size;
				}
				individuals->push_back(individuals->at(parent_A).cross(individuals->at(parent_B), avg_gene_size, size+i, K, M_comm, M_capt));
		}
		size += nb_cross;
}

void Population::mutate(double mutation_rate, int mutation_size, int K, sparse_matrix &M_comm, sparse_matrix &M_capt) {
		for (int i = 0; i<size; i++) {
				double r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				if (r<mutation_rate) {
						individuals->at(i).mutate(mutation_size, K, M_comm, M_capt);
				}
		}
}

void Individual::mutate(int mutation_size, int K, sparse_matrix &M_comm, sparse_matrix &M_capt) {
		modification mutation_modif(this, mutation_size, mutation_size, M_comm.n);
		sparse_vector* mutated = mutation_modif.apply_modification(this, K, M_comm, M_capt);
		fitness = mutated->fitness;
		vect = mutated->vect;
}
