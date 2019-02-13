#include "ProbabilitySimulator.h"
#include <iostream>
#include <random>
#include <time.h>

using std::cout; using std::endl;
using std::default_random_engine;

ProbabilitySimulator::ProbabilitySimulator() {
	guaranteeRoundCount = 0;
	bigGuranteeRoundCount = 0;
	totalRoundCount = 0;
	prizeCount = 0;
	currentPrizePR = init.initPrizePR;
	currentBigPrizePR = init.initBigPrizePR;
	bigInPrizePR = init.bigPrizePR / init.prizePR / 2;
	bigPrize = (init.bigPrizePR > 0);
	window = init.totalRounds / 10;
	randEngine = default_random_engine(time(nullptr));
	cout << "Create a default simulator." << endl;
}

ProbabilitySimulator::ProbabilitySimulator(int TotalRounds, int PrintRounds, int TuneRounds,
	double PrizePR, double BigPrizePR, double InitPrizePR, double InitBigPrizePR, double LearningRate) {
	init.totalRounds = TotalRounds; init.printRounds = PrintRounds;
	init.tuneRounds = TuneRounds; init.prizePR = PrizePR;
	init.initPrizePR = InitPrizePR; init.initBigPrizePR = InitBigPrizePR;
	init.learningRate = LearningRate; init.bigPrizePR = BigPrizePR;
	guaranteeRoundCount = 0;
	bigGuranteeRoundCount = 0;
	bigPrize = (init.bigPrizePR > 0);
	totalRoundCount = 0;
	prizeCount = 0;
	currentPrizePR = init.initPrizePR;
	currentBigPrizePR = init.initBigPrizePR;
	window = init.totalRounds / 10;
	bigInPrizePR = init.bigPrizePR / init.prizePR / 2;
	randEngine = default_random_engine(time(nullptr));
	
	cout << "Create a custom simulator." << endl;
}

ProbabilitySimulator::ProbabilitySimulator(InitParam &initParam) {
	init = initParam;
	guaranteeRoundCount = 0;
	bigGuranteeRoundCount = 0;
	bigPrize = (init.bigPrizePR > 0);
	totalRoundCount = 0;
	prizeCount = 0;
	currentPrizePR = init.initPrizePR;
	currentBigPrizePR = init.initBigPrizePR;
	window = init.totalRounds / 10;
	bigInPrizePR = init.bigPrizePR / init.prizePR / 2;
	randEngine = default_random_engine(time(nullptr));

	cout << "Create a custom simulator." << endl;
}

void ProbabilitySimulator::onWinPrize() {
	
	++totalRoundCount;
	++prizeCount;
	guaranteeRoundCount = 0;

	int tmp = prizeRecord[totalRoundCount - 1];
	prizeRecord.push_back(tmp + 1);
	tmp = bigPrizeRecord[totalRoundCount - 1];
	bigPrizeRecord.push_back(tmp);
}

void ProbabilitySimulator::onMissPrize() {
	++totalRoundCount;
	++guaranteeRoundCount;

	int tmp = prizeRecord[totalRoundCount - 1];
	prizeRecord.push_back(tmp);
	tmp = bigPrizeRecord[totalRoundCount - 1];
	bigPrizeRecord.push_back(tmp);
	
	if (bigPrize) {
		++bigGuranteeRoundCount;
		
	}
		
}

void ProbabilitySimulator::onWinBigPrize() {
	++totalRoundCount;
	++prizeCount;
	++bigPrizeCount;
	bigGuranteeRoundCount = 0;
	guaranteeRoundCount = 0;

	int tmp = prizeRecord[totalRoundCount - 1];
	prizeRecord.push_back(tmp + 1);
	tmp = bigPrizeRecord[totalRoundCount - 1];
	bigPrizeRecord.push_back(tmp + 1);
}

int ProbabilitySimulator::takeOne() {
	double tmp = randEngine();
	tmp /= UINT_MAX;
	if (tmp <= currentBigPrizePR && bigPrize) {
		onWinBigPrize();
		test[0]++;
		return 2;
	}
	if (tmp <= currentPrizePR) {
		onWinPrize();
		return 1;
	}
	else {
		if ((bigGuranteeRoundCount + 1) == bigGuranteeRounds) {
			onWinBigPrize();
			test[2]++;
			return 5;
		}
		if ((guaranteeRoundCount + 1) == guaranteeRounds) {
			if (bigPrize) {
				tmp = randEngine();
				tmp /= UINT_MAX;
				if (tmp <= bigInPrizePR) {
					onWinBigPrize();
					test[1]++;
					return 4;
				}
				else {
					onWinPrize();
					++bigGuranteeRoundCount;
					return 3;
				}
				
			}
			else {
				onWinPrize();
				return 3;
			}
		}
		else {
			onMissPrize();
			return 0;
		}
	}
}

void ProbabilitySimulator::fineTune() {
	int num = (totalRoundCount > window) ? window : totalRoundCount;
	double prizeNum, targetPR, currFreq, diff;
	
	if (bigPrize) {
		prizeNum = bigPrizeRecord[totalRoundCount] - bigPrizeRecord[totalRoundCount - num + 1];
		targetPR = init.bigPrizePR;
		currFreq = prizeNum / num;
		diff = (targetPR - currFreq) * init.learningRate;
		currentBigPrizePR += diff;
	}

	prizeNum = prizeRecord[totalRoundCount] - prizeRecord[totalRoundCount - num + 1];
	targetPR = init.prizePR;
	currFreq = prizeNum / num;
	diff = (targetPR - currFreq) * init.learningRate;
	currentPrizePR += diff;
		
	
	
	//cout << "diff: " << diff * init.learningRate << endl;
	//cout << "currFreq: " << currFreq << endl;
	//cout << "prize count: " << prizeNum << endl;
	//cout << "total count: " << totalRoundCount << endl;
}

void ProbabilitySimulator::printResults() {
	double tmp = bigPrize ? currentBigPrizePR : currentPrizePR;
	cout << "round: " << totalRoundCount <<
		"\tcurrent probability: " << tmp << endl;
}

void ProbabilitySimulator::simulate() {
	
}

void ProbabilitySimulator::learn() {
	//cout << (bigPrize ? "true" : "false") << endl;
	prizeRecord.push_back(0);
	bigPrizeRecord.push_back(0);
	if (!bigPrize) {
		bigGuranteeRounds = INT_MAX;
	}

	for (int i = 0; i < init.totalRounds; ++i) {
		takeOne();
		if (i % init.tuneRounds == 0) {
			fineTune();
		}
		if (i % init.printRounds == 0) {
			printResults();
		}
	}

	cout << "prize count: " << prizeRecord[totalRoundCount] << endl;
	cout << "big prize count: " << bigPrizeRecord[totalRoundCount] << endl;
}

void ProbabilitySimulator::showHelp() {
	cout << "-t\tnumber of rounds to simulate, default = 1000000" << endl
		<< "-r\tto print results, default = 10000" << endl
		<< "-T\tto fine tune, default = 100" << endl
		<< "-p\tprobability of prize, default = 0.125" << endl
		<< "-P\tprobability of big prize, default = 0.015" << endl
		<< "-i\tinitial probability of prize to learn, default = 0.01" << endl
		<< "-I\tinitial probability of big prize to learn, default = 0.005" << endl
		<< "-l\tlearning rate, default = 0.001" << endl;
}