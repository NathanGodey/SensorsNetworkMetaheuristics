#include "greedy.h"

using namespace std;


void setTargetsWeights(vector<Target> &targets, sparse_matrix &M_capt) {
		vector<double> target_interest_rating;
		for (int i=0; i<targets.size();i++) {
				target_interest_rating.push_back(1./(M_capt.mat[i].size()));
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


sparse_vector* greedyOptimization(sparse_vector *v, vector<int> &removal_queue, int K, sparse_matrix &M_comm, sparse_matrix &M_capt) {
		cout <<"Running Greedy Optimization..." <<endl;
		int current;
        modification* removing_point;
		for (int i=0; i<removal_queue.size(); i++) {
				current = removal_queue[i];
                removing_point = new modification(current);
				if (removing_point->check_modif(v, K, M_comm, M_capt) && current!=0){
						v = removing_point->apply_modification(v);
				}
				cout <<'\r' <<"Sensors left : " <<v->vect->size()-1 <<"  ";
				fflush(stdout);
		}
		cout <<endl <<"Greedy Optimization completed." <<endl;
		return v;
}
