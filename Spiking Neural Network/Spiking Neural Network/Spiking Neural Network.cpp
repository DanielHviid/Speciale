// Spiking Neural Network.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include "stdafx.h"
#include <stdlib.h>     /* srand, rand */
#include "LiquidPool.h"
#include "ANN.h"

#include <cmath>

double getScore(std::vector<int> listA, std::vector<double>listB)
{
	double score = 0;

	for (int n = 0; n < listA.size(); n++)
	{
		score += std::pow(listA[n] - listB[n],2);
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
	std::vector<std::vector<double>> sequence;

	for (int n = 0; n < 33; n++)
		sequence.push_back({ 1,0,0 });

	for (int n = 0; n < 33; n++)
		sequence.push_back({ 0,1,0 });

	for (int n = 0; n < 33; n++)
		sequence.push_back({ 0,0,1 });

	std::vector<double> result;

	int timeSteps = 100;
	std::string appendName = "temp";
	float timeconstant = 1.0 / timeSteps, resistance = 1, restingPotential = 0.1;
	int inputs = 3, outputs = 10;
	int depth = 5, width = 5, height = 5;
	int connections = 25, synapseLength = 3;
	float minWeight = 25, maxWeight = 50;

	ANN interpreter = ANN({ outputs, 25, inputs }, 0.01);
	//ANN interpreter = ANN({ outputs, (outputs + inputs) / 2, inputs }, 0.1);
	LiquidPool network = LiquidPool(depth, width, height);
	network.setRandomInputsAndOutputs(inputs, outputs);
	network.setParameters(timeconstant, resistance, restingPotential);
	network.resetSaveFiles(appendName);
	network.setRandomSynapses(connections, synapseLength, minWeight, maxWeight);

	std::vector<double> output, outputSum;
	outputSum.resize(outputs);

	std::vector<double> error;
	error.resize(sequence[0].size());

	std::ofstream myfile, myfile2;

	myfile.open("score" + appendName + ".txt", std::ios::trunc);
	myfile2.open("stuff" + appendName + ".txt", std::ios::trunc);

	for (int n = 0; n < 1000; n++)
	{
		for (int i = 0; i < outputSum.size(); i++)
		{
			outputSum[i] = 0;
		}

		for (int t = 0; t < timeSteps; t++)
		{
			network.activateInputs(sequence[n % sequence.size()]);
			network.activateNeurons();
			//network.activateNeurons(appendName);
			//network.saveInputValues(appendName);
			network.checkActivity();
			//network.saveInternalValues(appendName);
			//network.saveActivity(appendName);
			output = network.getOutputVector();

			for (int i = 0; i < outputSum.size(); i++)
			{
				outputSum[i] += output[i];
				/*
				if (i == n % sequence.size())
				{
					outputSum[i] = 1;
				}
				else
				{
					outputSum[i] += output[i];
				}
				*/
			}
		}



		/*
		for (int i = 0; i < outputSum.size(); i++)
		{
			std::cout << outputSum[i] << " ";
		}

		std::cout << std::endl; //*/

		result = interpreter.activate(outputSum);

		for (int i = 0; i < result.size(); i++)
		{
			error[i] = sequence[(n) % sequence.size()][i] - result[i];
		}

		if (n % 10 == 0)
		{ 
			std::cout << n << std::endl;
		}

		interpreter.backPropagate(error);

		myfile << getScore(error) << std::endl;
		myfile2 << error[0] << " " << error[1] << " " << error[2] << std::endl;
		
	}

	myfile.close();
	myfile2.close();

	return 0;

}

