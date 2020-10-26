#include "parser.h"
#include "target.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <boost/algorithm/string.hpp>
using namespace std;
using namespace boost::algorithm;


Parser::Parser(string instanceFile, vector<Target>& targets){
	ifstream inFile(instanceFile);
	string line;
	while (getline(inFile, line)) {
			vector<string> parts;
			split(parts, line, boost::is_any_of(" "));
			targets.push_back(Target(stod(parts[3]), stod(parts[4]), 0));
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
									split(results, line, [](char c){return c == '=';});
									token = results[results.size()-1];
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
