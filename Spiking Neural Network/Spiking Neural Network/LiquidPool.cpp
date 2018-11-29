#include "stdafx.h"
#include "LiquidPool.h"


LiquidPool::LiquidPool()
{
}

LiquidPool::LiquidPool(int aDepth, int aWidth, int aHeight)
{
	createNetwork(aDepth, aWidth, aHeight);
}

void LiquidPool::resetNetwork(int aDepth, int aWidth, int aHeight)
{
	for (int n = 0; n < networkDepth; n++)
	{
		for (int m = 0; m < networkWidth; m++)
		{
			for (int i = 0; i < networkHeight; i++)
			{
				delete network[n][m][i];
			}
			network[n][m].resize(0);
		}

		network[n].resize(0);
	}

	network.resize(0);

	createNetwork(aDepth, aWidth, aHeight);
}

void LiquidPool::setInputsAndOutputs(std::vector<position> inputPositions, std::vector<position> outputPositions)
{
	inputs.resize(inputPositions.size());
	outputs.resize(outputPositions.size());

	for (int n = 0; n < inputs.size(); n++)
	{
		inputs[n] = network[inputPositions[n].x][inputPositions[n].y][inputPositions[n].z];
	}

	for (int n = 0; n < outputs.size(); n++)
	{
		outputs[n] = network[outputPositions[n].x][outputPositions[n].y][outputPositions[n].z];
	}
}

void LiquidPool::setRandomInputsAndOutputs(int numberOfInputs, int numberOfOutputs)
{
	Neuron* currentNeuron;

	inputs.resize(numberOfInputs);
	outputs.resize(numberOfOutputs);

	for (int n = 0; n < numberOfInputs; n++)
	{
		currentNeuron = network[rand() % networkDepth][rand() % networkWidth][rand() % networkHeight];
		for (int i = 0; i < inputs.size(); i++)
		{
			if (currentNeuron == inputs[i])
			{
				n--;
				continue;
			}
			else if (i == inputs.size() - 1)
			{
				inputs[n] = currentNeuron;
				//currentNeuron->printPosition();
			}
		}
	}

	//std::cout << std::endl;

	for (int n = 0; n < numberOfOutputs; n++)
	{
		currentNeuron = network[rand() % networkDepth][rand() % networkWidth][rand() % networkHeight];
		for (int i = 0; i < outputs.size(); i++)
		{
			if (currentNeuron == outputs[i])
			{
				n--;
				continue;
			}
			else if (i == outputs.size() - 1)
			{
				outputs[n] = currentNeuron;
				//currentNeuron->printPosition();
			}
		}
	}
}

void LiquidPool::setParameters(float aTimeConstant, float aResistance, float aRestingPotential)
{
	timeConstant = aTimeConstant;
	resistance = aResistance;
	restingPotential = aRestingPotential;

	for (int depth = 0; depth < networkDepth; depth++)
	{
		for (int width = 0; width < networkWidth; width++)
		{
			for (int height = 0; height < networkHeight; height++)
			{
				network[depth][width][height]->setconstants(timeConstant, resistance, restingPotential);
			}
		}
	}
}

void LiquidPool::setSynapses(std::string append, int depth, int width, int height, std::vector<synapse> synapses)
{
	int x, y, z;
	for (int n = 0; n < synapses.size(); n++)
	{
		x = synapses[n].deltaX + depth;
		y = synapses[n].deltaY + width;
		z = synapses[n].deltaZ + height;
		if (x < 0 || x >= networkDepth || y < 0 || y >= networkWidth || z < 0 || z >= networkHeight)
			continue;
		network[depth][width][height]->addSynapses(network[x][y][z], synapses[n].weight);
		saveSynapse(append, depth, width, height, x, y, z, synapses[n].weight);
	}
}

void LiquidPool::setSynapses(int depth, int width, int height, std::vector<synapse> synapses)
{
	int x, y, z;
	for (int n = 0; n < synapses.size(); n++)
	{
		x = synapses[n].deltaX + depth;
		y = synapses[n].deltaY + width;
		z = synapses[n].deltaZ + height;
		if (x < 0 || x >= networkDepth || y < 0 || y >= networkWidth || z < 0 || z >= networkHeight)
			continue;
		network[depth][width][height]->addSynapses(network[x][y][z], synapses[n].weight);
	}
}

void LiquidPool::setRandomSynapses(int connections, int synapseLength, float minWeight, float maxWeight)
{
	int x, y, z;

	for (int depth = 0; depth < networkDepth; depth++)
		for (int width = 0; width < networkWidth; width++)
			for (int height = 0; height < networkHeight; height++)
			{
				for (int n = 0; n < connections; n++)
				{
					for (int attemps = 0; attemps < 5; attemps++)
					{
						x = depth + rand() % (1 + 2 * synapseLength) - synapseLength;
						y = width + rand() % (1 + 2 * synapseLength) - synapseLength;
						z = height + rand() % (1 + 2 * synapseLength) - synapseLength;
						if (x < 0 || x >= networkDepth || y < 0 || y >= networkWidth || z < 0 || z >= networkHeight)
						{
							continue;
						}
						if (network[depth][width][height]->addSynapses(network[x][y][z], randomFloat(minWeight, maxWeight)))
						{
							break;
						}
					}
				}
			}

}

std::vector<double> LiquidPool::activate(std::vector<double> input)
{
	//std::cout << inputs[0]->getInternalValue() << std::endl;
	activateInputs(input);
	activateNeurons();
	checkActivity();

	return getOutputVector();
}

void LiquidPool::activateInputs(std::vector<double> input)
{

	for (int n = 0; n < input.size(); n++)
	{
		inputs[n]->receivePulse(input[n]);
	}

}

void LiquidPool::activateNeurons(std::string append)
{
	Neuron* currentNeuron = NULL;

	while (1)
	{
		currentNeuron = buffer.removeNeuron();

		if (currentNeuron == NULL)
			break;

		currentNeuron->activate();
		currentNeuron->savePulses(append);

	}

	std::ofstream myfile;

	myfile.open("activatedSynapses" + append + ".txt", std::ios::app);

	myfile << "-1 -1 -1 -1 -1 -1 -1" << std::endl;

	myfile.close();
}

void LiquidPool::activateNeurons()
{
	Neuron* currentNeuron = NULL;

	while (1)
	{
		currentNeuron = buffer.removeNeuron();
		if (currentNeuron == NULL)
			break;

		currentNeuron->activate();

	}
}

void LiquidPool::checkActivity()
{
	std::vector<std::vector<Neuron*>> currentLayer;
	std::vector<Neuron*> currentLine;

	for (int depth = 0; depth < networkDepth; depth++)
	{
		currentLayer = network[depth];
		for (int width = 0; width < networkWidth; width++)
		{
			currentLine = currentLayer[width];
			for (int height = 0; height < networkHeight; height++)
			{
				if (currentLine[height]->update())
				{
					buffer.addNeuron(currentLine[height]);
				}
			}
		}
	}

}

std::vector<double> LiquidPool::getOutputVector()
{

	std::vector<double> result = std::vector<double>(outputs.size());

	for (int n = 0; n < outputs.size(); n++)
	{
		result[n] = outputs[n]->getOutput();
	}

	return result;
}

void LiquidPool::resetSaveFiles(std::string append)
{
	std::ofstream myfile;

	saveVariables(append);

	myfile.open("input" + append + ".txt", std::ios::trunc);
	myfile.close();

	myfile.open("internal" + append + ".txt", std::ios::trunc);
	myfile.close();

	myfile.open("activity" + append + ".txt", std::ios::trunc);
	myfile.close();

	myfile.open("activatedSynapses" + append + ".txt", std::ios::trunc);
	myfile.close();

	myfile.open("allSynapses" + append + ".txt", std::ios::trunc);
	myfile.close();
}

void LiquidPool::saveInputValues(std::string append)
{
	std::ofstream myfile;

	myfile.open("input" + append + ".txt", std::ios::app);

	for (int height = 0; height < networkHeight; height++)
	{

		for (int width = 0; width < networkWidth; width++)
		{
			myfile << std::endl;
			for (int depth = 0; depth < networkDepth; depth++)
			{
				myfile << network[depth][width][height]->getInput() << " ";
			}
		}
	}

	myfile << std::endl;

	myfile.close();
}

void LiquidPool::saveInternalValues(std::string append)
{
	std::ofstream myfile;

	myfile.open("internal" + append + ".txt", std::ios::app);

	for (int height = 0; height < networkHeight; height++)
	{

		for (int width = 0; width < networkWidth; width++)
		{
			myfile << std::endl;

			for (int depth = 0; depth < networkDepth; depth++)
			{
				myfile << network[depth][width][height]->getInternalValue() << " ";
			}
		}
	}

	myfile << std::endl;

	myfile.close();
}

void LiquidPool::saveActivity(std::string append)
{
	std::ofstream myfile;

	myfile.open("activity" + append + ".txt", std::ios::app);

	for (int height = 0; height < networkHeight; height++)
	{

		for (int width = 0; width < networkWidth; width++)
		{
			myfile << std::endl;

			for (int depth = 0; depth < networkDepth; depth++)
			{
				myfile << network[depth][width][height]->getOutput() << " ";
			}
		}
	}

	myfile << std::endl;

	myfile.close();
}

void LiquidPool::saveSynapse(std::string append, int startDepth, int startWidth, int startHeight, int endDepth, int endWidth, int endHeight, float weight)
{
	std::ofstream myfile;

	myfile.open("allSynapses" + append + ".txt", std::ios::app);

	myfile << std::to_string(startDepth) + " " + std::to_string(startWidth) + " " + std::to_string(startHeight) + " " +
		std::to_string(endDepth) + " " + std::to_string(endWidth) + " " + std::to_string(endHeight) + " " +
		std::to_string(weight) << std::endl;

	myfile.close();


}

LiquidPool::~LiquidPool()
{
	for (int depth = 0; depth < networkDepth; depth++)
	{
		for (int width = 0; width < networkWidth; width++)
		{
			for (int height = 0; height < networkHeight; height++)
			{
				delete network[depth][width][height];
			}

			network[depth][width].resize(0);

		}

		network[depth].resize(0);
	}

	network.resize(0);
}

float LiquidPool::randomFloat(float a, float b)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void LiquidPool::saveVariables(std::string append)
{
	std::ofstream myfile;

	myfile.open("variables" + append + ".txt", std::ios::trunc);

	myfile << networkDepth << " " << networkWidth << " " << networkHeight << " " << timeConstant << " " << resistance << " " << restingPotential;

	myfile.close();

}

void LiquidPool::createNetwork(int aDepth, int aWidth, int aHeight)
{
	network.resize(aDepth);

	for (int depth = 0; depth < aDepth; depth++)
	{
		network[depth] = std::vector<std::vector<Neuron*>>();
		network[depth].resize(aWidth);

		for (int width = 0; width < aWidth; width++)
		{
			network[depth][width] = std::vector<Neuron*>();
			network[depth][width].resize(aHeight);

			for (int height = 0; height < aHeight; height++)
			{
				network[depth][width][height] = new Neuron(depth, width, height);
			}
		}
	}

	networkDepth = aDepth;
	networkWidth = aWidth;
	networkHeight = aHeight;

	updateBufferSize();
}

void LiquidPool::updateBufferSize()
{
	int n = 0;

	for (int x = 0; x < network.size(); x++)
	{
		n += network[0].size();
	}

	buffer.setSize(n);
}

