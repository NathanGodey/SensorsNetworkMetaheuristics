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
	sparse_matrix *M_comm = new sparse_matrix(targets, 0.01);
	M_comm->add_edge(3, 4);
	M_comm->display();
	cout <<endl <<"We delete 3->4" <<endl;
	M_comm->delete_edge(3, 4);
	M_comm->display();
	writeToTxt(targets);
	system("python visualizer.py");
  return 0;
}
