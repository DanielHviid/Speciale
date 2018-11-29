#include "stdafx.h"
#include "Neuron.h"


Neuron::Neuron()
{
}

Neuron::Neuron(int aDepth, int aWidth, int aHeight)
{
	depth = aDepth;
	width = aWidth;
	height = aHeight;
}


Neuron::~Neuron()
{
}

void Neuron::setPosition(int aDepth, int aWidth, int aHeight)
{
	depth = aDepth;
	width = aWidth;
	height = aHeight;
}

void Neuron::setconstants(float aTimeConstant, float aResistance, float aRestingPotential)
{
	timeConstant = aTimeConstant;
	resistance = aResistance;
	restingPotential = aRestingPotential;
}

bool Neuron::addSynapses(Neuron * synapse, float weight)
{
	for (int n = 0; n < synapses.size(); n++)
	{
		if (synapses[n] == synapse)
			return false;
	}
	numberOfSynapses++;
	synapses.push_back(synapse);
	synapseWeights.push_back(weight);
	return true;
}

void Neuron::savePulses(std::string append)
{
	std::ofstream myfile;

	myfile.open("activatedSynapses" + append + ".txt", std::ios::app);

	for (int n = 0; n < synapses.size(); n++)
	{
		myfile << std::to_string(depth) + " " + std::to_string(width) + " " + std::to_string(height) + " " +
			std::to_string(synapses[n]->depth) + " " + std::to_string(synapses[n]->width) + " " + std::to_string(synapses[n]->height) + " " +
			std::to_string(synapseWeights[n]) << std::endl;
	}

	myfile.close();
}

void Neuron::activate()
{
	std::vector<Neuron*> newActivations = std::vector<Neuron*>();

	for (int n = 0; n < numberOfSynapses; n++)
	{
		synapses[n]->receivePulse(synapseWeights[n]);
	}

}

void Neuron::receivePulse(double pulse)
{
	input += pulse;
}

void Neuron::printPosition()
{
	std::cout << depth << " " << width << " " << height << std::endl;
}

bool Neuron::update()
{
	// apply activation function, return true if neuron is active

	//internal += timeConstant * (input - internal / resistance); Make sure this is wrong
	internal += (resistance * input - internal) / timeConstant;
	input = 0;

	if (internal >= 1)
	{
		internal = restingPotential;
		output = 1;
		return true;
	}
	else
	{
		output = 0;
		return false;
	}
}

int Neuron::getOutput()
{
	return output;
}

float Neuron::getInput()
{
	return input;
}

float Neuron::getInternalValue()
{
	return internal;
}

void Neuron::setAsActive()
{
	output = 1;
}
