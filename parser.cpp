#include "parser.h"
#include "target.h"
#include <fstream>
#include <iostream>
#include <string>
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
			Target tmpTarget(stod(parts[3]), stod(parts[4]), stoi(parts[1])==0);
			targets.push_back(tmpTarget);
	}
	inFile.close();
}
