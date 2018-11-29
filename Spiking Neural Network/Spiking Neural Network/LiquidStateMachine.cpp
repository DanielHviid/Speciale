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


	connections = newconnections;
	synapseLength = newsynapseLength;
	minWeight = newminWeight;
	maxWeight = newmaxWeight;

	timeSteps = newtimeSteps;
	resistance = newresistance;
	restingPotential = newrestingPotential;

	recreate();
}

void LiquidStateMachine::recreate()
{
	pool = LiquidPool(poolDepth, poolWidth, poolHeight);
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


LiquidStateMachine::~LiquidStateMachine()
{
}
