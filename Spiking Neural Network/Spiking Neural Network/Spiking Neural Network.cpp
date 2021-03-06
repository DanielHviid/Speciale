// Spiking Neural Network.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include "stdafx.h"
#include <stdlib.h>     /* srand, rand */
#include "SpikingHiddenLayer.h"
#include "LiquidStateMachine.h"
#include "ANN.h"

#include <cmath>

std::vector<double> difference(std::vector<double> A, std::vector<double> B)
{
	std::vector<double> result = std::vector<double>(A.size());
	
	for (int n = 0; n < result.size(); n++)
	{
		result[n] = A[n] - B[n];
	}

	return result;
}

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
	std::vector<std::vector<double>> sequence, inputData, groundTruths;

	bool andTest, orTest, xOrTest1, xOrTest2;

	for (int n = 0; n < 50000; n++)
	{

		andTest = true;
		orTest = false;
		xOrTest1 = true;
		xOrTest2 = false;

		inputData.push_back({ (double)(rand() % 2),(double)(rand() % 2)});
		groundTruths.push_back({0,0,0});

		for (int i = 0; i < inputData[n].size(); i++)
		{
			if (inputData[n][i] != 1)
			{
				andTest = false;
			}
			if (inputData[n][i] == 1)
			{
				orTest = true;
			}
			if (inputData[n][i] == 1)
			{
				if (xOrTest1)
				{
					xOrTest1 = false;
					xOrTest2 = true;
				}
				else
				{
					xOrTest2 = false;
				}
			}
			
		}

		if (andTest)
			groundTruths[n][0] = 1;
		if (andTest)
			groundTruths[n][1] = 1;
		if (andTest)
			groundTruths[n][2] = 1;
	}

	ANN testNetwork = ANN({2, 3}, 0.001);
	std::vector<double> testResult, testError;

	std::ofstream testfile, testfile2;

	testfile.open("scoretest.txt", std::ios::trunc);
	testfile2.open("stufftest.txt", std::ios::trunc);

	for (int n = 0; n < inputData.size(); n++)
	{
		testResult = testNetwork.activate(inputData[n]);
		testError = difference(groundTruths[n], testResult);
		testNetwork.backPropagate(testError);
		
		testfile << getScore(testError) << std::endl;
		testfile2 << testError[0] << " " << testError[1] << " " << testError[2] << std::endl;
	}


	return 0;






	for (int n = 0; n < 1; n++)
		sequence.push_back({ 1,0,0 });

	for (int n = 0; n < 1; n++)
		sequence.push_back({ 0,1,0 });

	for (int n = 0; n < 1; n++)
		sequence.push_back({ 0,0,1 });

	std::vector<double> result;

	int timeSteps = 100;
	std::string appendName = "temp";
	double resistance = 100, restingPotential = 0.1;
	double learningRate = 0.01;
	int inputs = 3, outputs = 3, hiddenInputs = 25, hiddenOutputs = 25;
	int depth = 7, width = 7, height = 7;
	int connections = 25, synapseLength = 3;
	float minWeight = 0.25, maxWeight = 0.5;

	std::ofstream myfile, myfile2;
	
	myfile.open("score" + appendName + ".txt", std::ios::trunc);
	myfile2.open("stuff" + appendName + ".txt", std::ios::trunc);

	LiquidStateMachine network = LiquidStateMachine(
		inputs, outputs, hiddenInputs, hiddenOutputs, 
		learningRate, timeSteps, resistance, restingPotential, 
		connections, synapseLength, minWeight, 
		maxWeight, depth, width, height);

	std::vector<double> output, error;

	error.resize(outputs);

	int nMax = 1000;

	for (int n = 0; n < nMax; n++)
	{

		if (n % (nMax/100) == 0)
		{
			std::cout << "Current progress: " << (double)n / (double)nMax * 100.0 << " / 100" << "\r";
		}

		output = network.activate(sequence[n % sequence.size()]);
		//output = network.activateOnce(sequence[0]);

		for (int i = 0; i < output.size(); i++)
		{
			error[i] = sequence[n % sequence.size()][i] - output[i];
			//error[i] = sequence[0][i] - output[i];
		}

		network.backProbagate(error);

		myfile << getScore(error) << std::endl;
		myfile2 << output[0] << " " << output[1] << " " << output[2] << std::endl;

	}

	myfile.close();
	myfile2.close();

	return 0;

}

