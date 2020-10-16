#include "parser.h"
#include "target.h"
#include <vector>
#include <iostream>
#include <string>

using namespace std;

int main(){
	vector<Target> targets;
	Parser *parser = new Parser("./instances/captANOR150_7_4.dat", targets);
	cout <<"The well is at : " <<targets[0].x <<" " <<targets[0].y <<endl;
	cout <<"There are " <<(targets.size()-1) <<" targets" <<endl;
  return 0;
}
