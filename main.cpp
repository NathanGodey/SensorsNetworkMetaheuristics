#include "parser.h"
#include "sparse.h"
#include "greedy.h"
#include "evolutionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <tgmath.h>
#include <chrono>

using namespace std;

vector<string> file_name = {"625_12_100"};
vector<vector<int>> R_ = {{1,1}};
vector<int> K_={1};


int main(){
  ofstream csv_results;
  csv_results.open("results.csv");
  csv_results <<"Instance, K, R_comm, R_capt, SensorsLeft, LowerBound" <<endl;

  std::ofstream ofs;
  ofs.open("result_evol.txt", std::ofstream::out | std::ofstream::trunc);
  ofs.close();

  for (auto instance: file_name) {
      vector<Target> targets;
      string parserPrefix = "./instances/", instancePrefix = "captANOR", extension = ".dat";
      Parser *parser = new Parser(parserPrefix+instancePrefix+instance+extension, targets);
      MinorantsParser *mino_parser = new MinorantsParser(instancePrefix+instance+extension);
      for (auto K: K_) {
          for (auto R : R_) {
              srand (time(NULL));


              int R_COMM = R[0], R_CAPT = R[1];
              int lower_bound = mino_parser->getMinorant(K, R_COMM, R_CAPT);
              cout <<endl <<"#########" <<endl;
              cout <<instance<<" "<<K<<" "<<R_COMM<<" "<<R_CAPT<<endl;
              cout <<"A lower bound for this problem is : " <<lower_bound <<endl;

              sparse_matrix M_comm(targets, R_COMM);
              sparse_matrix M_capt(targets, R_CAPT);

              setTargetsWeights(targets, M_capt);
              sparse_vector* v = new sparse_vector();
              create_solution(v, targets,R_CAPT,K);


              vector<int> removal_queue = sortedTargetsIds(targets);
              v = greedyOptimization(v, removal_queue, K, M_comm, M_capt);

              sparse_matrix Com_graph(targets.size());
              Com_graph.fill_as_communication_graph(M_comm, v);
              setSensorsFromVect(targets, *v);
              setNeighboorsFromCommunicationGraph(targets, Com_graph);

              EvolutionnaryOptimizer opt(*v, targets, 0.5);
              opt.nb_max_generations = 30;
              opt.init_size = 300;
              opt.mutation_rate = 0.01;
              opt.run(K, M_comm, M_capt, "result_evol.txt");

              writeToTxt(targets,instance, K, R_COMM, R_CAPT);

              //system("python visualizer.py");
              csv_results <<instance <<", " <<K <<", " <<R_COMM <<", " <<R_CAPT <<", " <<v->vect.size()-1<<", "<<lower_bound <<endl;
              clear_all(targets);
          }
      }
  }

  return 0;
}
