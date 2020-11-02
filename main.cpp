#include "parser.h"
#include "sparse.h"
#include "greedy.h"
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

void writeToTxt(vector<Target> targets){
    ofstream myfile;
    myfile.open("result.txt");
    for (int i=0; i<targets.size(); i++) {
        string neighbours_str = "";
        for (auto itr = targets[i].neighbours.begin(); itr!=targets[i].neighbours.end(); itr++) {
                neighbours_str = neighbours_str + to_string(*itr) + ',';
        }
        neighbours_str = "["+neighbours_str.substr(0,neighbours_str.length()-1)+']';
        myfile <<targets[i].id<<" " <<targets[i].x <<' ' <<targets[i].y <<' '<< targets[i].isSensor <<' '<<neighbours_str << endl;
    }
    myfile.close();
}

int main(){
    std::ofstream myfile;
    myfile.open ("heuristic.csv");
    for (int i=0; i<file_name.size();i++){
        myfile << "Instance :"+file_name[i]+"; ;minorant;without acceleration;with acceleration;time without acceleration;time with acceleration\n";
        for (int j=0; j<R.size();j++){
            for (int k=0; k< K_.size();k++){
                vector<Target> targets;
                string parserPrefix = "./instances/", instancePrefix = "captANOR", extension = ".dat";
                string instance = file_name[i];
                Parser *parser = new Parser(parserPrefix+instancePrefix+instance+extension, targets);
                MinorantsParser *mino_parser = new MinorantsParser(instancePrefix+instance+extension);
                int K = K_[k], R_COMM = R[j][0], R_CAPT = R[j][1];
                int lower_bound = mino_parser->getMinorant(K, R_COMM, R_CAPT);
                cout << file_name[i]+" "+to_string(R[j][0])+","+to_string(R[j][1])+";"+to_string(K_[k]);
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
                auto start = std::chrono::high_resolution_clock::now();

                sparse_vector* v = new sparse_vector(a);
                v = greedyOptimization(v, removal_queue, K, M_comm, M_capt);
                int size1 = v->vect->size();
                auto stop = std::chrono::high_resolution_clock::now();

                std::chrono::duration<double, std::milli> duration1 = stop - start;

                start = std::chrono::high_resolution_clock::now();
                v = new sparse_vector();
                create_solution(v, targets,R_CAPT,K);
                v = greedyOptimization(v, removal_queue, K, M_comm, M_capt);
                int size2 = v->vect->size();
                stop = std::chrono::high_resolution_clock::now();

                std::chrono::duration<double, std::milli> duration2 = stop - start;

                myfile << to_string(R[j][0])+","+to_string(R[j][1])+";"+to_string(K_[k])+";"+to_string(lower_bound)+";"+to_string(size1)+";"+to_string(size2)+";"+to_string(duration1.count())+";"+to_string(duration2.count())+"\n";
            }
        }
    }
    myfile.close();
    //writeToTxt(targets);
return 0;
}

