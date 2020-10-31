#pragma once
#include "sparse.h"

using namespace std;


class Individual: public sparse_vector {
public:
		int id;
		Individual(int id, sparse_vector &elder, double avg_insertions, int nb_targets);
		Individual();
};

Individual operator*(Individual const& parent_a, Individual const& parent_b); // Cross-over operation
inline bool operator< (Individual& l, Individual& r){ return r.fitness > l.fitness; };

class Population {
public:
		vector<Individual>* individuals;
		int size;
		double best_fitness;
		double avg_fitness;
		int best_id;
		Population(int init_size, sparse_vector &elder, double avg_insertions, int nb_targets);
		void compute_metrics();
		void cross(double cross_over_rate);
		void mutate(double mutation_rate);
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
		double cross_over_rate;
		double kill_rate;
		EvolutionnaryOptimizer(int nb_max_generations,int init_size,double mutation_rate,double cross_over_rate, double kill_rate, double avg_insertions);
		void run(string file_name);
		vector<Individual> get_top_n(int n);
		Individual get_best();
};
