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

struct FIFOBuffer
{
	void setSize(int newSize)
	{
		start = 0;
		end = 0;
		size = newSize;
		storage.resize(newSize);
	}

	void addNeuron(Neuron* newNeuron) // if needed, improve to include increasing in size
	{
		storage[end] = newNeuron;

		end++;
		if (end >= size)
			end = 0;
	}

	Neuron* removeNeuron()
	{
		if (start == end)
			return NULL;

		Neuron* out = storage[start];
		start++;
		if (start >= size)
			start = 0;
		return out;
	}

private:
	std::vector<Neuron*> storage;

	int start = 0, end = 0, size = 0;

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

class LiquidPool
{
public:
	LiquidPool();

	LiquidPool(int aDepth, int aWidth, int aHeight);

	void resetNetwork(int aDepth, int aWidth, int aHeight);

	void setInputsAndOutputs(std::vector<position> inputPositions, std::vector<position> outputPositions);

	void setRandomInputsAndOutputs(int numberOfInputs, int numberOfOutputs);

	void setParameters(float aTimeConstant, float aResistance, float aRestingPotential);

	void setSynapses(std::string append, int depth, int width, int height, std::vector<synapse> synapses);
	void setSynapses(int depth, int width, int height, std::vector<synapse> synapses);

	void setRandomSynapses(int connections, int synapseLength, float minWeight, float maxWeight);

	std::vector<double> activate(std::vector<double> input);

	// to be activated in order

	// first
	void activateInputs(std::vector<double> input);

	// second
	void activateNeurons(std::string append);
	void activateNeurons();

	// third
	void checkActivity();

	// fourth
	std::vector<double> getOutputVector();

	void resetSaveFiles(std::string append);

	void saveInputValues(std::string append);

	void saveInternalValues(std::string append);

	void saveActivity(std::string append);

	void saveSynapse(std::string append, int startDepth, int startWidth, int startHeight, int endDepth, int endWidth, int endHeight, float weight);

	~LiquidPool();

private:

	float randomFloat(float a, float b);

	void saveVariables(std::string append);

	void createNetwork(int aDepth, int aWidth, int aHeight);

	std::vector<Neuron*> inputs, outputs;

	int networkDepth = 0, networkWidth = 0, networkHeight = 0;

	float timeConstant, resistance, restingPotential;
	std::vector<std::vector<std::vector<Neuron*>>> network;
	FIFOBuffer buffer;

	void updateBufferSize();
};

