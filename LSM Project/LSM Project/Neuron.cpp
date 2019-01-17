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

void Neuron::setconstants(float aTimeConstant, float aResistance, float aRestingPotential, float aLearningRate)
{
	timeConstant = aTimeConstant;
	resistance = aResistance;
	restingPotential = aRestingPotential;
	learningRate = aLearningRate;
}

bool Neuron::addSynapses(Neuron* synapse, float weight)
{
	for (int n = 0; n < synapses.size(); n++)
	{
		if (synapses.size() != 0 && synapses[n] == synapse)
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
	internal += (resistance * input - internal) / timeConstant;
	input = 0;

	if (internal >= 1)
	{
		internal = restingPotential;
		output = 1;
		return true;
	}
	else if (internal < 0)
	{
		internal = 0;
	}
	else
	{
		output = 0;
		return false;
	}
}

void Neuron::reset()
{
	input = 0;
	internal = 0;
	output = 0;
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

OutputNeuron::OutputNeuron()
{
}

OutputNeuron::OutputNeuron(int aDepth, int aWidth, int aHeight)
{
	depth = aDepth;
	width = aWidth;
	height = aHeight;
}

OutputNeuron::~OutputNeuron()
{
}

void OutputNeuron::activate()
{
	for (int n = 0; n < numberOfSynapses; n++)
	{
		if (synapses[n]->getOutput() == 1)
		{
			input += synapseWeights[n];
		}
	}
}

std::vector<int> OutputNeuron::getInputActivations()
{
	std::vector<int> result = std::vector<int>(synapses.size());

	for (int n = 0; n < result.size(); n++)
	{
		result[n] = synapses[n]->getOutput();
	}

	return result;
}

std::vector<double> OutputNeuron::getInputInternals()
{
	std::vector<double> result = std::vector<double>(synapses.size());

	for (int n = 0; n < result.size(); n++)
	{
		result[n] = synapses[n]->getInternalValue();
	}

	return result;
}

void OutputNeuron::train(std::vector<int> activations, double learningFactor)
{
	for (int n = 0; n < activations.size(); n++)
	{
		if (activations[n])
		{
			synapseWeights[n] += learningFactor * learningRate;
			if (synapseWeights[n] < 0)
			{
				synapseWeights[n] = 0;
			}
			else if (synapseWeights[n] > 1)
			{
				synapseWeights[n] = 1;
			}
		}
	}
}
