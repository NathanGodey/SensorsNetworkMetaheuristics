#pragma once
#include "sparse.h"

using namespace std;


class Individual: public sparse_vector {
public:
		int id;
		Individual(int id, sparse_vector &elder, double avg_insertions, int nb_targets, double lambda_capt, double lambda_comm);
		Individual(const Individual &ind_original);
		Individual(sparse_vector& v_original);
		Individual();
    Individual cross(Individual& other, int avg_gene_size, int id_child, int K, sparse_matrix M_comm, sparse_matrix M_capt); // Cross-over operation
		void mutate(int mutation_size, int K, sparse_matrix &M_comm, sparse_matrix &M_capt);
};


inline bool operator< (Individual& l, Individual& r){ return r.fitness > l.fitness; };

class Population {
public:
		vector<Individual>* individuals;
		int size;
		double best_fitness;
		double avg_fitness;
		int best_id;
		int nb_targets;
		double lambda_capt;
		double lambda_comm;
		Population(int init_size, sparse_vector &elder, double avg_insertions, double lambda_capt, double lambda_comm, int nb_targets);
		void compute_metrics();
		void cross(double reproduction_rate, int avg_gene_size, int K, sparse_matrix &M_comm, sparse_matrix &M_capt);
		void mutate(double mutation_rate, int mutation_size, int K, sparse_matrix &M_comm, sparse_matrix &M_capt);
		void natural_select(double kill_rate); //First implementation : select (1-kill_rate)% of individuals
		void display(int ranking_size);
		void write_to_file(string file_name, vector<Target>& targets, sparse_matrix &M_comm);
};

class EvolutionnaryOptimizer {
public:
		Population population;
		int nb_max_generations;
		int init_size;
		double mutation_rate;
		int mutation_size;
		double reproduction_rate;
		double kill_rate;
		double lambda_capt;
		double lambda_comm;
		int avg_gene_size;
		EvolutionnaryOptimizer(int nb_max_generations,int init_size,double mutation_rate,double reproduction_rate, double kill_rate, double avg_insertions, int avg_gene_size);
		void run(string file_name);
		vector<Individual> get_top_n(int n);
		Individual get_best();
};
