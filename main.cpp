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

vector<string> file_name = {"150_7_4", "225_8_10", "625_12_100", "900_15_20", "1500_15_100", "1500_18_100"};
vector<vector<int>> R_ = {{1,1},{2,2}};
vector<int> K_={1};


int main_before(){
  ofstream csv_results;
  csv_results.open("results.csv");
  csv_results <<"Instance, K, R_comm, R_capt, SensorsLeft, LowerBound" <<endl;
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

              Population p(200, *v, 0.1, 1,1, targets.size());
              p.cross(0.5, 10, K, M_comm, M_capt);
              p.compute_metrics();
              cout << p.avg_fitness;
              writeToTxt(targets,instance, K, R_COMM, R_CAPT);

              system("python visualizer.py");
              csv_results <<instance <<", " <<K <<", " <<R_COMM <<", " <<R_CAPT <<", " <<v->vect->size()-1<<", "<<lower_bound <<endl;
              clear_all(targets);
          }
      }
  }

  return 0;
}

int main(){
    vector<Target> targets;
    string parserPrefix = "./instances/", instancePrefix = "captANOR", extension = ".dat";
    Parser *parser = new Parser(parserPrefix+instancePrefix+file_name[0]+extension, targets);
    int R_COMM = 1, R_CAPT = 1, K=1;

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

    Population p(200, *v, 0.1, 1,1, targets.size());
    p.cross(0.5, 10, K, M_comm, M_capt);
    p.compute_metrics();
    cout << "avant natural_select" << p.size << "," <<  p.individuals->size() << "     fitness moy " << p.avg_fitness << endl;

    p.natural_select(0.2);
    p.compute_metrics();
    cout <<"apres" << p.size << "," <<  p.individuals->size() << "     fitness moy " << p.avg_fitness <<endl;
}
