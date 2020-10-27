#include "parser.h"
#include "sparse.h"
#include "greedy.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

void writeToTxt(vector<Target> targets){
	ofstream myfile;
  myfile.open("result.txt");
	for (int i=0; i<targets.size(); i++) {
		string neighbours_str = "";
		for (auto itr = targets[i].neighbours.begin(); itr!=targets[i].neighbours.end(); itr++) {
				neighbours_str = neighbours_str + to_string(*itr) + ',';
		}
		neighbours_str = "["+neighbours_str.substr(0,neighbours_str.length()-1)+']';
		myfile <<targets[i].id <<" " <<targets[i].x <<' ' <<targets[i].y <<' '<< targets[i].isSensor <<' '<<neighbours_str << endl;
	}
	myfile.close();
}

int main(){
	vector<Target> targets;
	string parserPrefix = "./instances/", instancePrefix = "captANOR", extension = ".dat";
	string instance = "150_7_4";
	Parser *parser = new Parser(parserPrefix+instancePrefix+instance+extension, targets);
	MinorantsParser *mino_parser = new MinorantsParser(instancePrefix+instance+extension);
	int K = 3, R_COMM = 2, R_CAPT = 2;
	int lower_bound = mino_parser->getMinorant(K, R_COMM, R_CAPT);
	cout <<"Un minorant pour ce problème est : " <<lower_bound <<endl;

	sparse_matrix M_comm(targets, R_COMM);
	sparse_matrix M_capt(targets, R_CAPT);

	setTargetsWeights(targets, M_capt);
	vector<int> removal_queue = sortedTargetsIds(targets);

	unordered_set<int> a;
	for (int i=0; i<targets.size(); i++) {
			a.insert(i);
	}

	sparse_vector* v = new sparse_vector(a);
	greedyOptimization(v, removal_queue, K, M_comm, M_capt);

	sparse_matrix Com_graph(targets.size());
	Com_graph.fill_as_communication_graph(M_comm, v);
	setSensorsFromVect(targets, *v);
	setNeighboorsFromCommunicationGraph(targets, Com_graph);

	writeToTxt(targets);

	//displayWeights(targets);
	system("python visualizer.py");
  return 0;
}
