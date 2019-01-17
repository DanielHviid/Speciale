// Spiking Neural Network.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include "stdafx.h"
#include <stdlib.h>     /* srand, rand */
#include "LSM.h"
#include "LiquidStateMachine.h"

#include <cmath>

std::vector<double> calculateExpectation(std::vector<double> data)
{
	std::vector<double> result = std::vector<double>(data.size());

	for (int n = 0; n < data.size(); n++)
	{
		result[n] = data[n] * 2 - 1;
	}

	return result;
}

std::vector<double> difference(std::vector<double> A, std::vector<double> B)
{
	std::vector<double> result = std::vector<double>(A.size());
	
	for (int n = 0; n < result.size(); n++)
	{
		result[n] = A[n] - B[n];
	}

	return result;
}

double getScore(std::vector<double> listA, std::vector<double>listB)
{
	double score = 1;

	for (int n = 0; n < listA.size(); n++)
	{
		score -= std::pow(listA[n] - listB[n],2) / (double)listA.size();
	}

	return score;
}

template <class T>
double getScore(std::vector<T> listA)
{
	double score = 0;

	for (int n = 0; n < listA.size(); n++)
	{
		score += std::pow(listA[n], 2);
	}

	return score;
}

template <class T>
void printVector(std::vector<T> list)
{
	for (int n = 0; n < list.size(); n++)
		std::cout << list[n] << " ";
	std::cout << std::endl;
}

int main()
{
	int numberOfRuns = 30000, deltaT = 1;

	double preTrainingScore = 0, postTrainingScore = 0;

	int depth = 10, width = 10, height = 10, inputs = 3, outputs = 3, hiddenLayerInputs = 50, hiddenLayerOutputs = 250;
	int connections = 2, synapseLength = 2;
	float timeConstant = 2.5, resistance = 2.5, restingPotential = 0.1, learningRate = 0.0001, minWeight = -1, maxWeight = 1;
	std::vector<double> result;
	std::vector<std::vector<int>> activations;
	std::vector<std::vector<std::vector<int>>> activationsList;
	activationsList.resize(numberOfRuns);
	std::vector<std::vector<double>> internals;

	std::vector<std::vector<double>> data;
	std::vector<std::vector<double>> learningFactors;
	std::vector<int> trainingSequence = std::vector<int>(numberOfRuns);

	for (int n = 0; n < numberOfRuns; n++)
	{
		trainingSequence[n] = n;
	}
	std::random_shuffle(trainingSequence.begin(), trainingSequence.end());

	for (int n = 0; n < 100; n++)
	{
		data.push_back({1,0,0});
		learningFactors.push_back({1,-1,-1});
	}
	for (int n = 0; n < 100; n++)
	{
		data.push_back({ 0,1,0 });
		learningFactors.push_back({-1,1,-1});
	}
	for (int n = 0; n < 100; n++)
	{
		data.push_back({ 0,0,1 });
		learningFactors.push_back({-1,-1,1});
	}

	LSM network = LSM(depth, width, height, inputs, outputs);
	network.setParameters(timeConstant, resistance, restingPotential, learningRate);
	network.setRandomSynapses(connections, synapseLength, minWeight, maxWeight);
	network.setRandomInputsAndOutputs(hiddenLayerInputs, hiddenLayerOutputs, minWeight, maxWeight);

	//network.setSaveFileName("test");

	std::cout << "collecting data" << std::endl;
	// data collection
	for(int n = 0; n < numberOfRuns; n++)
	{
		if ((n * 100) % numberOfRuns == 0)
			std::cout << "progress : " << 100.0 * (double)n / (double)numberOfRuns << " / " << 300 << std::endl;

		result = network.activate(data[n % data.size()], activations);

		activationsList[n] = activations;
		
		preTrainingScore += getScore(data[(n + deltaT) % data.size()], result);

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

		postTrainingScore += getScore(data[(n + deltaT) % data.size()], result);
	}

	std::cout << "Average score pre-training: " << preTrainingScore / (double)numberOfRuns << std::endl;
	std::cout << "Average score post-training: " << postTrainingScore / (double)numberOfRuns << std::endl;
	return 0;

}

