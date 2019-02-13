#include <iostream>
#include <string>
#include "ProbabilitySimulator.h"
using namespace std;

class InputParser {
public:
	InputParser(int &argc, char** argv) {
		for (int i = 1; i < argc; i++) {
			inputs.push_back(string(argv[i]));
		}
		//cout << inputs.size() << endl;
	}

	ProbabilitySimulator* create() {
		ProbabilitySimulator *simulator;
		if (inputs.size() == 0)
			simulator = new ProbabilitySimulator();
		else {
			vector<string>::iterator p = inputs.begin();
			while (p != inputs.end()) {
				cout << *p << "\t";
				if (*p == "-t"){
					if (++p == inputs.end()) return nullptr;
					init.totalRounds = stoi(string(*p));
				}else if (*p == "-r") {
					if (++p == inputs.end()) return nullptr;
					init.printRounds = stoi(string(*p));
				}else if (*p == "-T") {
					if (++p == inputs.end()) return nullptr;
					init.tuneRounds = stoi(string(*p));
				}else if (*p == "-p") {
					if (++p == inputs.end()) return nullptr;
					init.prizePR = stod(string(*p));
				}
				else if (*p == "-P") {
					if (++p == inputs.end()) return nullptr;
					init.bigPrizePR = stod(string(*p));
				}
				else if (*p == "-i") {
					if (++p == inputs.end()) return nullptr;
					init.initPrizePR = stod(string(*p));
				}
				else if (*p == "-I") {
					if (++p == inputs.end()) return nullptr;
					init.initBigPrizePR = stod(string(*p));
				}
				else if (*p == "-l") {
					if (++p == inputs.end()) return nullptr;
					init.learningRate = stod(string(*p));
				}
				else {
					return nullptr;
				}
				++p;
			}
			simulator = new ProbabilitySimulator(init);
		}
		return simulator;
	}
private:
	vector<string> inputs;
	InitParam init = InitParam();
};


int main(int argc, char** argv) {
	InputParser parser = InputParser(argc, argv);
	ProbabilitySimulator *simulator = parser.create();
	if (simulator) {
		simulator->learn();
	}
	else {
		ProbabilitySimulator::showHelp();
	}
	
	//simulator.simulate();
	system("pause");
	return 0;
}