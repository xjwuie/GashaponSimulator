#pragma once
#include <vector>
#include <random>

struct InitParam {
	int totalRounds = 1000000;
	int printRounds = 20000;
	int tuneRounds = 100;
	double prizePR = 0.125;
	double bigPrizePR = 0.015;
	double initPrizePR = 0.01;
	double initBigPrizePR = 0.005;
	double learningRate = 0.001;
};

class ProbabilitySimulator {
private:
	/*
	int totalRounds = 1000000;
	int printRounds = 20000;
	int tuneRounds = 100;
	double prizePR = 0.125;
	double bigPrizePR = -0.015;
	double initPrizePR = 0.01;
	double initBigPrizePR = 0.005;
	double learningRate = 0.001;
	*/

	int guaranteeRounds = 10;
	int bigGuranteeRounds = 100;
	int window = 50000;
	
	InitParam init = InitParam();

	int guaranteeRoundCount, bigGuranteeRoundCount, totalRoundCount;
	int prizeCount, bigPrizeCount;
	double currentPrizePR, currentBigPrizePR, bigInPrizePR;
	bool bigPrize;
	int test[3] = { 0, 0, 0 };
	std::default_random_engine randEngine;
	std::vector<int> results;
	std::vector<int> prizeRecord, bigPrizeRecord;

	void printResults();
	int takeOne();
	void onWinPrize();
	void onMissPrize();
	void onWinBigPrize();
	void fineTune();

public:
	ProbabilitySimulator();
	ProbabilitySimulator(int TotalRounds, int PrintRounds, int TuneRounds,
		double PrizePR, double BigPrizePR, double InitPrizePR, double InitBigPrizePR, double LearningRate);
	ProbabilitySimulator(InitParam &initParam);
	void simulate();
	void learn();

	static void showHelp();
	
	
};