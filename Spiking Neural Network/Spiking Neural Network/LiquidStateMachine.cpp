#include "stdafx.h"
#include "LiquidStateMachine.h"


LiquidStateMachine::LiquidStateMachine()
{
}

LiquidStateMachine::LiquidStateMachine(int newinputs, int newoutputs, int newhiddenInputs, int newhiddenOutputs, double newLearningRate,
	int newtimeSteps, double newresistance, double newrestingPotential, 
	int newconnections, int newsynapseLength, double newminWeights, double newmaxWeights, 
	int newdepth, int newwidth, int newheight)
{
	setConstants(newinputs, newoutputs, newhiddenInputs, newhiddenOutputs, newLearningRate,
		newtimeSteps, newresistance, newrestingPotential,
		newconnections, newsynapseLength, newminWeights, newmaxWeights,
		newdepth, newwidth, newheight);

}

void LiquidStateMachine::setConstants(int newinputs, int newoutputs, int newhiddenInputs, int newhiddenOutputs, double newLearningRate,
	int newtimeSteps, double newresistance, double newrestingPotential, 
	int newconnections, int newsynapseLength, double newminWeight, double newmaxWeight, 
	int newdepth, int newwidth, int newheight)
{
	inputs = newinputs;
	outputs = newoutputs;
	hiddenInputs = newhiddenInputs;
	hiddenOutputs = newhiddenOutputs;
	learningRate = newLearningRate;

	timeSteps = newtimeSteps;
	resistance = newresistance;
	restingPotential = newrestingPotential;

	connections = newconnections;
	synapseLength = newsynapseLength;
	minWeight = newminWeight;
	maxWeight = newmaxWeight;

	hiddenDepth = newdepth;
	hiddenWidth = newwidth;
	hiddenHeight = newheight;

	recreate();
}

void LiquidStateMachine::recreate()
{
	hiddenLayer.resetNetwork(hiddenDepth, hiddenWidth, hiddenHeight);
	hiddenLayer.setRandomInputsAndOutputs(hiddenInputs, hiddenOutputs);
	hiddenLayer.setParameters(timeSteps, resistance, restingPotential);
	hiddenLayer.setRandomSynapses(connections, synapseLength, minWeight, maxWeight);

	clearLayers();

	for (int n = 0; n < inputs; n++)
	{
		inputLayer.push_back(new Neuron);
	}

	for (int n = 0; n < outputs; n++)
	{
		outputLayer.push_back(new Neuron);
	}

	connectInputToHidden();
	connectHiddenToOutput();

}

void LiquidStateMachine::clearLayers()
{
	for (int n = 0; n < inputLayer.size(); n++)
		delete(inputLayer[n]);

	for (int n = 0; n < inputLayer.size(); n++)
		delete(inputLayer[n]);
}

void LiquidStateMachine::connectInputToHidden()
{
	for (int n = 0; n < inputs; n++)
	{
		for (int m = 0; m < hiddenInputs; m++)
		{
			inputLayer[n]->addSynapses()
		}
	}
}

std::vector<double> LiquidStateMachine::activateOnce(std::vector<double> input)
{
	std::vector<double> hiddenInput, hiddenOutput;

	hiddenInput = inputLayer.activate(input);
	
	hiddenOutput = hiddenLayer.activate(hiddenInput);

	return outputLayer.activate(hiddenOutput);
}

std::vector<double> LiquidStateMachine::activate(std::vector<double> input)
{
	std::vector<double> hiddenInput, hiddenOutput;

	for (int n = 0; n < timeSteps; n++)
	{
		hiddenInput = inputLayer.activate(input);

		hiddenOutput = hiddenLayer.activate(hiddenInput);
	}

	return outputLayer.activate(hiddenOutput);
}


LiquidStateMachine::~LiquidStateMachine()
{
}
