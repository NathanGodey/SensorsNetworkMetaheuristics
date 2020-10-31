#include "parser.h"
#include "sparse.h"
#include "greedy.h"
#include "evolutionary.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <tgmath.h>
#include <chrono>

using namespace std;

vector<string> file_name = {"150_7_4", "225_8_10","625_12_100", "900_15_20", "1500_15_100","1500_18_100"};
vector<vector<int>> R = {{1,1},{2,1},{2,2},{3,2}};
vector<int> K_={1,2,3};


int main(){
	vector<Target> targets;
	string parserPrefix = "./instances/", instancePrefix = "captANOR", extension = ".dat";
	string instance = "150_7_4";
	Parser *parser = new Parser(parserPrefix+instancePrefix+instance+extension, targets);
	MinorantsParser *mino_parser = new MinorantsParser(instancePrefix+instance+extension);
	int K = 1, R_COMM = 2, R_CAPT = 1;
	int lower_bound = mino_parser->getMinorant(K, R_COMM, R_CAPT);
	cout <<"Un minorant pour ce problème est : " <<lower_bound <<endl;

	sparse_matrix M_comm(targets, R_COMM);
	sparse_matrix M_capt(targets, R_CAPT);

	setTargetsWeights(targets, M_capt);
	vector<int> removal_queue = sortedTargetsIds(targets);

	//TODO : remove when valentine's heuristic works
	unordered_set<int> a;
	for (int i=0; i<targets.size(); i++) {
			a.insert(i);
	}

	sparse_vector* v = new sparse_vector(a);
	//create_solution(captors, targets,R_CAPT,K);
	v = greedyOptimization(v, removal_queue, K, M_comm, M_capt);
	sparse_matrix Com_graph(targets.size());
	Com_graph.fill_as_communication_graph(M_comm, v);
	setSensorsFromVect(targets, *v);
	setNeighboorsFromCommunicationGraph(targets, Com_graph);

	writeToTxt(targets);

	system("python visualizer.py");
	//displayWeights(targets);
  Population p(10, *v, 0.05, 150);
  //p.write_to_file("result_evol.txt", targets, M_comm);
  v = new Individual(p.individuals->at(2));
  sparse_matrix Com_graph_2(targets.size());
	Com_graph_2.fill_as_communication_graph(M_comm, v);
	setSensorsFromVect(targets, *v);
	setNeighboorsFromCommunicationGraph(targets, Com_graph_2);
  writeToTxt(targets);
  clear_all(targets);

	//displayWeights(targets);
	system("python visualizer.py");

  return 0;
}
