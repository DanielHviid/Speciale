#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>


class Neuron
{
public:
	Neuron();
	Neuron(int aDepth, int aWidth, int aHeight);
	~Neuron();

	void setPosition(int aDepth, int aWidth, int aHeight);

	void setconstants(float aTimeConstant, float aResistance, float aRestingPotential);

	bool addSynapses(Neuron* synapse, float weight);

	void savePulses(std::string append);

	void activate();

	void receivePulse(double pulse);

	void printPosition();

	bool update();

	int getOutput();
	float getInput();
	float getInternalValue();

	void setAsActive();

	int depth, width, height;

protected:

	double input = 0;
	double internal = 0;
	int output = 0;
	int numberOfSynapses = 0;
	double timeConstant = 1, resistance = 1, restingPotential = 0;
	std::vector<Neuron*> synapses;
	std::vector<float> synapseWeights;
	bool activation;
};

