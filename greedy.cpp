#include "greedy.h"

using namespace std;


void setTargetsWeights(vector<Target> &targets, sparse_matrix &M_capt) {
		vector<double> target_interest_rating;
		for (int i=0; i<targets.size();i++) {
				target_interest_rating.push_back(1./(M_capt.mat[i].size()-1)*(M_capt.mat[i].size()-1));
		}
		double summed_interest = 0;
		for (int i=0; i<targets.size();i++) {
				summed_interest = 0;
				for (auto itr = M_capt.mat[i].begin(); itr != M_capt.mat[i].end(); itr++) {
						summed_interest = summed_interest + target_interest_rating[*itr];
				}
				targets[i].weight = summed_interest;
		}
}


void greedyOptimization(sparse_vector *v, vector<int> &removal_queue, int K, sparse_matrix &M_comm, sparse_matrix &M_capt) {
		cout <<"Running Greedy Optimization..." <<endl;
		int current;
		for (int i=0; i<removal_queue.size(); i++) {
				current = removal_queue[i];
				v->vect->erase(current);
				if (!is_eligible(v,K,M_comm,M_capt) || current==0){
						v->vect->insert(current);
						cout <<"Sensors left = " <<v->vect->size() <<endl;
				}
		}
		cout <<"We have " <<v->vect->size() <<" sensors left" <<endl;
}
