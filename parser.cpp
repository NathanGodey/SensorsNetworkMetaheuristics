#include "parser.h"
#include "target.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <tgmath.h>
using namespace std;

vector<string> split(string line, string delimiter){
    vector<string> list;
    size_t pos = 0;
    while ((pos = line.find(delimiter)) != string::npos) {
        list.push_back(line.substr(0, pos));
        line.erase(0, pos + delimiter.length());
    }
    if ((pos = line.find(";")) != string::npos){
        line.erase(pos,pos+1);
    }
    list.push_back(line);
    return list;
}

Parser::Parser(string instanceFile, vector<Target>& targets){
    ifstream inFile(instanceFile);
    string line;
    string delimiter = " ";
    while (getline(inFile, line)) {
        vector<string> parts=split(line,delimiter);
        targets.push_back(Target(stod(parts[3]), stod(parts[4]), stoi(parts[1])==0));
    }
    inFile.close();
}

MinorantsParser::MinorantsParser(string instanceFile){
    ifstream inFile("./instances/Minorants.txt");
    string line, token;
    vector<string> results;
    while (getline(inFile, line)) {
            if (line==instanceFile) {
                    getline(inFile, line);
                    for (int k=1; k<4; k++) {
                            vector<int> current;
                            getline(inFile, line);
                            for (int i=0; i<4; i++) {
                                    getline(inFile, line);
                                    results.clear();
                                    results = split(line, "=");
                                    token = results[results.size()-1];
                                    if (*token.begin() == ' '){
                                        token.erase(token.begin());
                                    }
                                    current.push_back(ceil(stod(token)));
                            }
                            content.push_back(current);
                    }
            }
    }
    inFile.close();
}

int MinorantsParser::getMinorant(int K, int R_com, int R_capt) {
        return content[K-1][R_com+R_capt-2];
}