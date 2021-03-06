// Spiking Neural Network.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include "stdafx.h"
#include <stdlib.h>     /* srand, rand */
#include "LSM.h"
#include "LiquidStateMachine.h"

#include <cmath>

void calculateCarryover(std::vector<std::vector<double>> &list, int index, int compareIndex, double carryOver)
{
	for (int n = 0; n < list[0].size(); n++)
	{
		if (list[index][n] == 0)
		{
			list[index][n] = list[compareIndex][n] * carryOver;
		}
	}
}

void createData(std::vector<std::vector<double>> truths, std::vector<std::vector<double>> &inputs,
	std::vector<std::vector<double>> &trainingWeights, int sequenceOverlap, double carryOver, double inputWeight)
{
	inputs.resize(truths.size());
	trainingWeights.resize(truths.size());

	for (int n = 0; n < truths.size(); n++)
	{
		inputs[n].resize(truths[0].size());
		trainingWeights[n].resize(truths[0].size());

		for (int m = 0; m < truths[0].size(); m++)
		{
			inputs[n][m] = truths[n][m] * inputWeight;
			trainingWeights[n][m] = truths[n][m] * 2 - 1;
		}
	}

	for (int i = 0; i < sequenceOverlap; i++)
	{

		calculateCarryover(trainingWeights, 0, trainingWeights.size() - 1, carryOver);
		for (int n = 1; n < truths.size(); n++)
		{
			calculateCarryover(trainingWeights, n, n - 1, carryOver);
			//calculateCarryover(trainingWeights, trainingWeights.size() - n, trainingWeights.size() - n + 1, carryOver);
		}
		//calculateCarryover(trainingWeights, trainingWeights.size() - 1, 0, carryOver);
	}

}

double getScore(std::vector<double> listA, std::vector<double>listB)
{
	double score = 1;

	for (int n = 0; n < listA.size(); n++)
	{
		score -= std::pow(listA[n] - listB[n], 2) / (double)listA.size();
	}

	return score;
}

int main()
{
	int numberOfRuns = 100, deltaT = 0, sequenceOverlap = 5;

	double preTrainingScore = 0, postTrainingScore = 0;
	int preActiveNeurons = 0, postActiveNeurons = 0;

	int depth = 5, width = 5, height = 5, inputs = 3, outputs = 3, hiddenLayerInputs = 10, hiddenLayerOutputs = 10;
	int connections = 10, synapseLength = 2;
	float timeConstant = 10, resistance = 3.3, restingPotential = 0.1, learningRate = 0.001, minWeight = 0.5, maxWeight = 1;
	double inputWeight = 50, carryOver = 0.9, suppresivePortion = 0.15;
	std::vector<double> result;
	std::vector<std::vector<int>> activations;
	std::vector<std::vector<std::vector<int>>> activationsList;
	activationsList.resize(numberOfRuns);
	std::vector<std::vector<double>> internals;

	std::vector<std::vector<double>> data;
	std::vector<std::vector<double>> truths;
	std::vector<std::vector<double>> learningFactors;
	std::vector<int> trainingSequence = std::vector<int>(numberOfRuns);

	for (int n = 0; n < numberOfRuns; n++)
	{
		trainingSequence[n] = n;
	}
	std::random_shuffle(trainingSequence.begin(), trainingSequence.end());

	for (int n = 0; n < 100; n++)
	{
		truths.push_back({ 1,0,0 });
	}
	for (int n = 0; n < 100; n++)
	{
		truths.push_back({ 0,1,0 });
	}
	for (int n = 0; n < 100; n++)
	{
		truths.push_back({ 0,0,1 });
	}

	createData(truths, data, learningFactors, sequenceOverlap, carryOver, inputWeight);

	LSM network = LSM(depth, width, height, inputs, outputs);
	network.setParameters(timeConstant, resistance, restingPotential, learningRate, suppresivePortion);
	network.setRandomSynapses(connections, synapseLength, minWeight, maxWeight);
	network.setRandomInputs(hiddenLayerInputs, minWeight, maxWeight);
	network.setRandomOutputs(hiddenLayerOutputs, minWeight, maxWeight);

	network.setSaveFileName("test");

	std::cout << "collecting data" << std::endl;
	// data collection
	for (int n = 0; n < numberOfRuns; n++)
	{
		if ((n * 100) % numberOfRuns == 0)
			std::cout << "progress : " << 100.0 * (double)n / (double)numberOfRuns << " / " << 300 << std::endl;

		result = network.activate(data[n % data.size()], activations);

		activationsList[n] = activations;

		preTrainingScore += getScore(truths[(n + deltaT) % data.size()], result);
		preActiveNeurons += network.getNumberOfActiveNeurons();

		/*
		for (int m = 0; m < data[0].size(); m++)
		{
			std::cout << data[n % data.size()][m] << " ";
		}

		std::cout << " vs ";

		for (int m = 0; m < result.size(); m++)
		{
			std::cout << result[m] << " ";
		}
		*/

	}
	/*
	std::cout << "training" << std::endl;
	// training
	for (int n = 0; n < numberOfRuns; n++)
	{
		if ((n * 100) % numberOfRuns == 0)
			std::cout << "progress : " << 100.0 + 100.0 * (double)n / (double)numberOfRuns << " / " << 300 << std::endl;

		network.train(activationsList[trainingSequence[n]], learningFactors[trainingSequence[n + deltaT] % data.size()]);
	}

	std::cout << "testing" << std::endl;
	// test
	for (int n = 0; n < numberOfRuns; n++)
	{
		if ((n * 100) % numberOfRuns == 0)
			std::cout << "progress : " << 200.0 + 100.0 * (double)n / (double)numberOfRuns << " / " << 300 << std::endl;

		result = network.activate(data[n % data.size()]);

		postTrainingScore += getScore(truths[(n + deltaT) % data.size()], result);
		postActiveNeurons += network.getNumberOfActiveNeurons();
	}

	std::cout << "Average score pre-training: " << preTrainingScore / (double)numberOfRuns << std::endl;
	std::cout << "Average score post-training: " << postTrainingScore / (double)numberOfRuns << std::endl;
	std::cout << "Average neuron activity pre-training: " << preActiveNeurons / (double)numberOfRuns << std::endl;
	std::cout << "Average neuron activity post-training: " << postActiveNeurons / (double)numberOfRuns << std::endl;
	*/
	return 0;

}

