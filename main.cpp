#include "parser.h"
#include "target.h"
#include "sparse.h"
#include <vector>
#include <iostream>
#include <string>

using namespace std;

void writeToTxt(vector<Target> targets){
	ofstream myfile;
  myfile.open("result.txt");
	for (int i=0; i<targets.size(); i++) {
		myfile <<targets[i].x <<' ' <<targets[i].y <<' '<< targets[i].isWell << endl;
	}
	myfile.close();
}

int main(){
	vector<Target> targets;
	Parser *parser = new Parser("./instances/captANOR1500_18_100.dat", targets);
	int K = 1, R_COMM = 1, R_CAPT = 1;

	sparse_matrix M_comm(targets, R_COMM);
	sparse_matrix M_capt(targets, R_CAPT);

	unordered_set<int> a = {};
	for (int i=0; i<targets.size(); i++){
			a.insert(i);
	}
	sparse_vector* v = new sparse_vector(a);
	sparse_matrix Com_graph(targets.size());
	Com_graph.fill_as_communication_graph(M_comm, v);
	Com_graph.display();

	sparse_matrix Capt_graph(targets.size());
	Capt_graph.fill_as_captation_graph(M_capt, v);
	Capt_graph.display();

	is_eligible(v,K,Com_graph,Capt_graph);

	if (v->isEligible) {
			cout <<endl <<"ce vecteur est éligible" <<endl;
	} else {
			cout <<endl <<"ce vecteur n'est pas éligible" <<endl;
	}

	writeToTxt(targets);
	system("python visualizer.py");
  return 0;
}
