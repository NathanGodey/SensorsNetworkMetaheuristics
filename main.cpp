#include "parser.h"
#include "target.h"
#include "sparse.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <tgmath.h>

using namespace std;

vector<string> file_name = {"150_7_4", "225_8_10","625_12_100", "900_15_20", "1500_15_100","1500_18_100"};
vector<vector<int>> R = {{1,1},{2,1},{2,2},{3,2}};
vector<int> K_={1,2,3};

void writeToTxt(vector<Target> targets){
    ofstream myfile;
    myfile.open("result.txt");
    for (int i=0; i<targets.size(); i++) {
        string neighbours_str = "";
        for (auto itr = targets[i].neighbours.begin(); itr!=targets[i].neighbours.end(); itr++) {
                neighbours_str = neighbours_str + to_string(*itr) + ',';
        }
        neighbours_str = "["+neighbours_str.substr(0,neighbours_str.length()-1)+']';
        myfile <<targets[i].x <<' ' <<targets[i].y <<' '<< targets[i].isSensor <<' '<<neighbours_str << endl;
    }
    myfile.close();
}

int main(){
	vector<Target> targets;
	Parser *parser = new Parser("./instances/captANOR1500_18_100.dat", targets);
	MinorantsParser *mino_parser = new MinorantsParser("captANOR1500_18_100.dat");
	int K = 1, R_COMM = 3, R_CAPT = 1;

	cout <<"Un minorant pour ce problème est : " <<mino_parser->getMinorant(K, R_COMM, R_CAPT);

	sparse_matrix M_comm(targets, R_COMM);
	sparse_matrix M_capt(targets, R_CAPT);

	unordered_set<int> a = {24,52,36,58,2,14,25,62,10,0,76};
	sparse_vector* v = new sparse_vector(a);
	sparse_matrix Com_graph(targets.size());
	Com_graph.fill_as_communication_graph(M_comm, v);
	Com_graph.display();

	sparse_matrix Capt_graph(targets.size());
	Capt_graph.fill_as_captation_graph(M_capt, v);
	//Capt_graph.display();

	is_eligible(v,K,Com_graph,Capt_graph);

	if (v->isEligible) {
			cout <<endl <<"ce vecteur est éligible" <<endl;
	} else {
			cout <<endl <<"ce vecteur n'est pas éligible" <<endl;
	}

	setSensorsFromVect(targets, *v);
	setNeighboorsFromCommunicationGraph(targets, Com_graph);

	writeToTxt(targets);
	system("python visualizer.py");
  return 0;
}
