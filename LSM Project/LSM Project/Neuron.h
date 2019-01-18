#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>     /* srand, rand */


class Neuron
{
public:
	Neuron();
	Neuron(int aDepth, int aWidth, int aHeight);
	~Neuron();

	void setPosition(int aDepth, int aWidth, int aHeight);

	void setconstants(float aTimeConstant, float aResistance, float aRestingPotential, float aLearningRate, double suppressive);

	bool addSynapses(Neuron* synapse, float weight);

	void savePulses(std::string append);

	virtual void activate();

	void receivePulse(double pulse);

	void printPosition();

	bool update();

	void reset();

	int getOutput();
	float getInput();
	float getInternalValue();

	void setAsActive();

	int depth, width, height;

protected:

	float randomFloat(float a, float b);

	int suppresiveSign = 1;
	double input = 0;
	double internal = 0;
	int output = 0;
	int numberOfSynapses = 0, numberOfOutputSynapses = 0;
	double timeConstant = 1, resistance = 1, restingPotential = 0, learningRate = 0.01;
	std::vector<Neuron*> synapses;
	std::vector<float> synapseWeights, outputSynapseWeights;
	bool activation;
};

class OutputNeuron : public Neuron
{
public:

	OutputNeuron();
	OutputNeuron(int aDepth, int aWidth, int aHeight);
	~OutputNeuron();

	void activate();

	std::vector<int> getInputActivations();
	std::vector<double> getInputInternals();
	
	void train(std::vector<int> activations, double learningFactor);
};

