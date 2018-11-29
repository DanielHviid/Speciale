#include "LiquidPool.h"
#include "ANN.h"

#pragma once


class LiquidStateMachine
{
public:
	LiquidStateMachine();
	LiquidStateMachine(int newinputs, int newoutputs, int newpoolInputs, int newpoolOutputs, double newLearningRate,
		int newtimeSteps, double newresistance, double newrestingPotential,
		int newconnections, int newsynapseLength, double newminWeights, double newmaxWeights,
		int newdepth, int newwidth, int newheight);

	void setConstants(int newinputs, int newoutputs, int newpoolInputs, int newpoolOutputs, double newLearningRate,
		int newtimeSteps, double newresistance, double newrestingPotential,
		int newconnections, int newsynapseLength, double newminWeights, double newmaxWeights,
		int newdepth, int newwidth, int newheight);

	void recreate();

	std::vector<double> activateOnce(std::vector<double> input);
	std::vector<double> activate(std::vector<double> input);

	void backProbagate(std::vector<double> error);
	void backProbagate(std::vector<double> result, std::vector<double> expectation);

	~LiquidStateMachine();

private:

	LiquidPool pool;
	ANN inputLayer, outputLayer;

	bool initialized = false;
	int inputs, outputs, poolInputs, poolOutputs;
	double learningRate;
	int timeSteps; // in miliseconds
	double resistance, restingPotential;
	int connections, synapseLength;
	double minWeight, maxWeight;
	int poolDepth, poolWidth, poolHeight;


};

