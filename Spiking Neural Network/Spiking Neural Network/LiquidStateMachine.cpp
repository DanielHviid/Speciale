#include "stdafx.h"
#include "LiquidStateMachine.h"


LiquidStateMachine::LiquidStateMachine()
{
}

LiquidStateMachine::LiquidStateMachine(int newinputs, int newoutputs, int newpoolInputs, int newpoolOutputs, double newLearningRate,
	int newtimeSteps, double newresistance, double newrestingPotential, 
	int newconnections, int newsynapseLength, double newminWeights, double newmaxWeights, 
	int newdepth, int newwidth, int newheight)
{
	setConstants(newinputs, newoutputs, newpoolInputs, newpoolOutputs, newLearningRate,
		newtimeSteps, newresistance, newrestingPotential,
		newconnections, newsynapseLength, newminWeights, newmaxWeights,
		newdepth, newwidth, newheight);

}

void LiquidStateMachine::setConstants(int newinputs, int newoutputs, int newpoolInputs, int newpoolOutputs, double newLearningRate,
	int newtimeSteps, double newresistance, double newrestingPotential, 
	int newconnections, int newsynapseLength, double newminWeight, double newmaxWeight, 
	int newdepth, int newwidth, int newheight)
{
	inputs = newinputs;
	outputs = newoutputs;
	poolInputs = newpoolInputs;
	poolOutputs = newpoolOutputs;
	learningRate = newLearningRate;

	timeSteps = newtimeSteps;
	resistance = newresistance;
	restingPotential = newrestingPotential;

	connections = newconnections;
	synapseLength = newsynapseLength;
	minWeight = newminWeight;
	maxWeight = newmaxWeight;

	poolDepth = newdepth;
	poolWidth = newwidth;
	poolHeight = newheight;

	recreate();
}

void LiquidStateMachine::recreate()
{
	pool.resetNetwork(poolDepth, poolWidth, poolHeight);
	pool.setRandomInputsAndOutputs(poolInputs, poolOutputs);
	pool.setParameters(timeSteps, resistance, restingPotential);
	pool.setRandomSynapses(connections, synapseLength, minWeight, maxWeight);


	inputLayer = ANN({inputs, poolInputs}, learningRate);
	outputLayer = ANN({poolOutputs, outputs}, learningRate);
}

std::vector<double> LiquidStateMachine::activateOnce(std::vector<double> input)
{
	std::vector<double> poolInput, poolOutput;

	poolInput = inputLayer.activate(input);
	
	poolOutput = pool.activate(poolInput);

	return outputLayer.activate(poolOutput);
}

std::vector<double> LiquidStateMachine::activate(std::vector<double> input)
{
	std::vector<double> poolInput, poolOutput;

	for (int n = 0; n < timeSteps; n++)
	{
		poolInput = inputLayer.activate(input);

		poolOutput = pool.activate(poolInput);
	}

	return outputLayer.activate(poolOutput);
}

void LiquidStateMachine::backProbagate(std::vector<double> error)
{
	outputLayer.backPropagate(error);
}

void LiquidStateMachine::backProbagate(std::vector<double> result, std::vector<double> truth)
{
	if (result.size() != truth.size())
	{
		std::cout << "Bad size! Result vs truth" << std::endl;
		return;
	}

	std::vector<double> error = std::vector<double>(result.size());

	for (int n = 0; n < result.size(); n++)
	{
		error[n] = truth[n] - result[n];
	}

	outputLayer.backPropagate(error);
}


LiquidStateMachine::~LiquidStateMachine()
{
}
