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
    for (int i=0;i<file_name.size();i++){
        for (int j=0;j<R.size();j++){
            for (int k=0;k<K_.size();k++){
                vector<Target> targets;
                string instanceFile = "captANOR" + file_name[i] + ".dat";
                MinorantsParser *mino_parser = new MinorantsParser(instanceFile);

                Parser *parser = new Parser("./instances/" + instanceFile, targets);

                int K =K_[k] ,  R_COMM = R[j][0], R_CAPT = R[j][1];
                cout <<"Un minorant pour ce problème est : " << mino_parser->getMinorant(K, R_COMM, R_CAPT) << endl;
                sparse_matrix M_comm(targets, R_COMM);
                sparse_matrix M_capt(targets, R_CAPT);

                sparse_matrix M(targets, R_CAPT);
                sparse_vector* captors = new sparse_vector();
                create_solution(captors, targets,R_CAPT,K);

                cout << "instance " << file_name[i] << ", R = (" << R[j][0] << "," << R[j][1] << "), K = " << K_[k] << " is_eligible: ";
                cout << is_eligible(captors,K,M_comm,M_capt) << " - ";
                cout << "percentage of captors = " << double(captors->vect->size())/double(M_capt.n) << endl;
            }
        }
    }
    return 0;
}
