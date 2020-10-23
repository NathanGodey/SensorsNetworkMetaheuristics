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
	Parser *parser = new Parser("./instances/captANOR150_7_4.dat", targets);
	sparse_matrix M_comm(targets, 1);
	M_comm.add_edge(3, 4);
	M_comm.display();
	cout <<endl <<"We delete 3->4" <<endl;
	M_comm.delete_edge(3, 4);
	M_comm.display();
	unordered_set<int> a = {20,88,26,29,23};
	sparse_vector* v = new sparse_vector(a);
	cout <<endl <<"Intersect with 20 88 26 29 23 as comm" <<endl;
	sparse_matrix M(targets.size());
	M.fill_as_communication_graph(M_comm, v);
	M.display();
	cout <<endl <<"Intersect with 20 26 29 23 as capt" <<endl;
	unordered_set<int> b = {20,26,29,23};
	sparse_vector* v_b = new sparse_vector(b);
	sparse_matrix N(targets.size());
	N.fill_as_captation_graph(M_comm, v_b);
	N.display();
	is_eligible(v,1,M,N);
	cout <<v->isEligible;
	fflush(stdout);
	writeToTxt(targets);
	system("python visualizer.py");
  return 0;
}
