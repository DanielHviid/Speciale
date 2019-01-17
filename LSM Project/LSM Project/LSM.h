#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Neuron.h"
#include "stdafx.h"
#include <stdlib.h>     /* srand, rand */

struct synapse
{
	int deltaX;
	int deltaY;
	int deltaZ;
	float weight;
};

struct Buffer
{
	void setSize(int newSize)
	{
		size = newSize;
		storage.resize(newSize);
	}

	void addNeuron(Neuron* newNeuron) // if needed, improve to include increasing in size
	{
		number++;

		storage[number] = newNeuron;
	}

	Neuron* removeNeuron()
	{
		if (number < 0)
			return NULL;

		Neuron* out = storage[number];
		number--;
		return out;
	}

private:
	std::vector<Neuron*> storage;

	int number = -1, size = 0;

};

struct position
{
	position(int X, int Y, int Z)
	{
		x = X;
		y = Y;
		z = Z;
	};
	int x = 0, y = 0, z = 0;
};

class LSM
{
public:
	LSM();

	LSM(int aDepth, int aWidth, int aHeight, int inputSize, int outputSize);

	void resetNetwork(int aDepth, int aWidth, int aHeight, int inputSize, int outputSize);

	void setInputsAndOutputs(std::vector<std::vector<position>> inputPositions, std::vector<std::vector<position>> outputPositions,
		std::vector<std::vector<float>> inputWeights, std::vector<std::vector<float>> outputWeights);

	void setRandomInputsAndOutputs(int numberOfInputs, int numberOfOutputs, float minWeight, float maxWeight);

	void setParameters(float aTimeConstant, float aResistance, float aRestingPotential, float aLearningRate);

	void setSynapses(std::string append, int depth, int width, int height, std::vector<synapse> synapses);
	void setSynapses(int depth, int width, int height, std::vector<synapse> synapses);

	void setRandomSynapses(int connections, int synapseLength, float minWeight, float maxWeight);

	void setSaveFileName(std::string newName);

	std::vector<double> activate(std::vector<double> input);
	std::vector<double> activate(std::vector<double> input, std::vector<std::vector<int>> &activations);
	std::vector<double> activate(std::vector<double> input, std::vector<std::vector<double>> &internals);

	// to be activated in order

	// first
	void activateInputs(std::vector<double> input);

	// second
	void activateNeurons();

	// third
	void checkActivity();

	// fourth
	std::vector<double> getOutputVector();

	// fifth, to save inputs to output layer
	std::vector<std::vector<int>> getPoolOutputs();
	std::vector<std::vector<double>> getPoolOutputInternals();

	void train(std::vector<std::vector<int>> activations, std::vector<double> learningFactors);

	void reset();

	// Save functions
	void resetSaveFiles();

	void saveInputValues();

	void saveInternalValues();

	void saveActivity();

	void saveSynapse(int startDepth, int startWidth, int startHeight, int endDepth, int endWidth, int endHeight, float weight);

	~LSM();

private:

	std::string saveName = "";

	float randomFloat(float a, float b);

	void saveVariables();

	void createNetwork(int aDepth, int aWidth, int aHeight, int inputSize, int outputSize);

	std::vector<Neuron*> inputLayer, inputConnections, outputConnections;
	std::vector<OutputNeuron*> outputLayer;

	int networkDepth = 0, networkWidth = 0, networkHeight = 0, inputNeurons = 0, outputNeurons = 0;

	float timeConstant, resistance, restingPotential, learningRate;
	std::vector<std::vector<std::vector<Neuron*>>> network;
	Buffer buffer;

	void updateBufferSize();
};

