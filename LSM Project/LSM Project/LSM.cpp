#include "stdafx.h"
#include "LSM.h"


LSM::LSM()
{
}

LSM::LSM(int aDepth, int aWidth, int aHeight, int inputSize, int outputSize)
{
	createNetwork(aDepth, aWidth, aHeight, inputSize, outputSize);
}

void LSM::resetNetwork(int aDepth, int aWidth, int aHeight, int inputSize, int outputSize)
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

	for (int n = 0; n < inputLayer.size(); n++)
	{
		delete inputLayer[n];
	}

	for (int n = 0; n < outputLayer.size(); n++)
	{
		delete outputLayer[n];
	}

	createNetwork(aDepth, aWidth, aHeight, inputSize, outputSize);
}

void LSM::setInputs(std::vector<std::vector<position>> inputPositions, std::vector<std::vector<float>> inputWeights)
{
	for (int n = 0; n < inputLayer.size(); n++)
	{
		for (int m = 0; m < inputPositions.size(); m++)
		{
			inputLayer[n]->addSynapses(network[inputPositions[n][m].x][inputPositions[n][m].y][inputPositions[n][m].z], inputWeights[n][m]);
		}
	}
}

void LSM::setOutputs(std::vector<std::vector<position>> outputPositions, std::vector<std::vector<float>> outputWeights)
{
	for (int n = 0; n < outputLayer.size(); n++)
	{
		for (int m = 0; m < outputPositions.size(); m++)
		{
			outputLayer[n]->addSynapses(network[outputPositions[n][m].x][outputPositions[n][m].y][outputPositions[n][m].z], outputWeights[n][m]);
		}
	};
}

void LSM::setRandomInputs(int numberOfInputs, float minWeight, float maxWeight)
{
	Neuron* currentNeuron;

	for (int i = 0; i < inputLayer.size(); i++)
	{
		for (int n = 0; n < numberOfInputs; n++)
		{			
			currentNeuron = network[rand() % networkDepth][rand() % networkWidth][rand() % networkHeight];
			if (!inputLayer[i]->addSynapses(currentNeuron, randomFloat(minWeight, maxWeight)))
			{
				n--;
			}
		}
	}
}

void LSM::setRandomOutputs(int numberOfOutputs, float minWeight, float maxWeight)
{
	Neuron* currentNeuron;

	for (int i = 0; i < outputLayer.size(); i++)
	{
		for (int n = 0; n < numberOfOutputs; n++)
		{
			currentNeuron = network[rand() % networkDepth][rand() % networkWidth][rand() % networkHeight];
			if (!outputLayer[i]->addSynapses(currentNeuron, randomFloat(minWeight, maxWeight)))
			{
				n--;
			}
		}
	}
}

void LSM::setParameters(float aTimeConstant, float aResistance, float aRestingPotential, float aLearningRate, double suppresivePortion)
{
	timeConstant = aTimeConstant;
	resistance = aResistance;
	restingPotential = aRestingPotential;
	learningRate = aLearningRate;
	suppresiveRate = suppresivePortion;

	for (int depth = 0; depth < networkDepth; depth++)
	{
		for (int width = 0; width < networkWidth; width++)
		{
			for (int height = 0; height < networkHeight; height++)
			{
				network[depth][width][height]->setconstants(timeConstant, resistance, restingPotential, learningRate, suppresiveRate);
			}
		}
	}

	for (int n = 0; n < inputLayer.size(); n++)
	{
		inputLayer[n]->setconstants(timeConstant, resistance, restingPotential, learningRate, 0);
	}

	for (int n = 0; n < outputLayer.size(); n++)
	{
		outputLayer[n]->setconstants(timeConstant, resistance, restingPotential, learningRate, 0);
	}

}

void LSM::setSynapses(std::string append, int depth, int width, int height, std::vector<synapse> synapses)
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
		saveSynapse(depth, width, height, x, y, z, synapses[n].weight);
	}
}

void LSM::setSynapses(int depth, int width, int height, std::vector<synapse> synapses)
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

void LSM::setRandomSynapses(int connections, int synapseLength, float minWeight, float maxWeight)
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

void LSM::setSaveFileName(std::string newName)
{
	saveName = newName;
	resetSaveFiles();
}

std::vector<double> LSM::activate(std::vector<double> input)
{

	activateInputs(input);
	saveInputValues();
	saveInternalValues();
	checkActivity();
	saveActivity();
	activateNeurons();

	return getOutputVector();
}

std::vector<double> LSM::activate(std::vector<double> input, std::vector<std::vector<int>>& activations)
{
	std::vector<double> result = activate(input);

	activations = getPoolOutputs();

	return getOutputVector();
}

std::vector<double> LSM::activate(std::vector<double> input, std::vector<std::vector<double>>& activations)
{
	std::vector<double> result = activate(input);

	activations = getPoolOutputInternals();

	return getOutputVector();
}

void LSM::activateInputs(std::vector<double> input)
{

	for (int n = 0; n < inputNeurons; n++)
	{
		inputLayer[n]->receivePulse(input[n]);
		if (inputLayer[n]->update())
		{
			buffer.addNeuron(inputLayer[n]);
		}
	}
}

void LSM::activateNeurons()
{
	Neuron* currentNeuron = NULL;

	while (1)
	{
		currentNeuron = buffer.removeNeuron();

		if (currentNeuron == NULL)
			break;

		currentNeuron->activate();
		if (saveName != "")
			currentNeuron->savePulses(saveName);

	}

	std::ofstream myfile;

	if (saveName != "")
	{
		myfile.open("activatedSynapses" + saveName + ".txt", std::ios::app);

		myfile << "-1 -1 -1 -1 -1 -1 -1" << std::endl;

		myfile.close();
	}
}

void LSM::checkActivity()
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

std::vector<double> LSM::getOutputVector()
{
	std::vector<double> result = std::vector<double>(outputLayer.size());

	for (int n = 0; n < outputLayer.size(); n++)
	{
		outputLayer[n]->activate();
		outputLayer[n]->update();
		result[n] = outputLayer[n]->getOutput();
	}

	return result;
}

std::vector<std::vector<int>> LSM::getPoolOutputs()
{
	std::vector<std::vector<int>> result = std::vector<std::vector<int>>(outputNeurons);

	for (int n = 0; n < outputNeurons; n++)
	{
		result[n] = outputLayer[n]->getInputActivations();
	}

	return result;
}

std::vector<std::vector<double>> LSM::getPoolOutputInternals()
{
	std::vector<std::vector<double>> result = std::vector<std::vector<double>>(outputNeurons);

	for (int n = 0; n < outputNeurons; n++)
	{
		result[n] = outputLayer[n]->getInputInternals();
	}

	return result;
}

void LSM::train(std::vector<std::vector<int>> activations, std::vector<double> learningFactors)
{
	for (int n = 0; n < outputNeurons; n++)
	{
		outputLayer[n]->train(activations[n], learningFactors[n]);
	}
}

int LSM::getNumberOfActiveNeurons()
{
	return buffer.getNumberOfActiveNeurons();
}

void LSM::reset()
{
	for (int n = 0; n < inputNeurons; n++)
	{
		inputLayer[n]->reset();
	}

	for (int x = 0; x < networkDepth; x++)
		for (int y = 0; y < networkWidth; y++)
			for (int z = 0; z < networkHeight; z++)
			{
				network[x][y][z]->reset();
			}

	for (int n = 0; n < outputNeurons; n++)
	{
		outputLayer[n]->reset();
	}
}

void LSM::resetSaveFiles()
{
	std::ofstream myfile;

	saveVariables();

	myfile.open("input" + saveName + ".txt", std::ios::trunc);
	myfile.close();

	myfile.open("internal" + saveName + ".txt", std::ios::trunc);
	myfile.close();

	myfile.open("activity" + saveName + ".txt", std::ios::trunc);
	myfile.close();

	myfile.open("activatedSynapses" + saveName + ".txt", std::ios::trunc);
	myfile.close();

	myfile.open("allSynapses" + saveName + ".txt", std::ios::trunc);
	myfile.close();
}

void LSM::saveInputValues()
{
	if (saveName == "")
	{
		return;
	}

	std::ofstream myfile;

	myfile.open("input" + saveName + ".txt", std::ios::app);

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

void LSM::saveInternalValues()
{
	if (saveName == "")
	{
		return;
	}

	std::ofstream myfile;

	myfile.open("internal" + saveName + ".txt", std::ios::app);

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

void LSM::saveActivity()
{
	if (saveName == "")
	{
		return;
	}

	std::ofstream myfile;

	myfile.open("activity" + saveName + ".txt", std::ios::app);

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

void LSM::saveSynapse(int startDepth, int startWidth, int startHeight, int endDepth, int endWidth, int endHeight, float weight)
{
	if (saveName == "")
	{
		return;
	}

	std::ofstream myfile;

	myfile.open("allSynapses" + saveName + ".txt", std::ios::app);

	myfile << std::to_string(startDepth) + " " + std::to_string(startWidth) + " " + std::to_string(startHeight) + " " +
		std::to_string(endDepth) + " " + std::to_string(endWidth) + " " + std::to_string(endHeight) + " " +
		std::to_string(weight) << std::endl;

	myfile.close();


}

LSM::~LSM()
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

float LSM::randomFloat(float a, float b)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void LSM::saveVariables()
{
	std::ofstream myfile;

	myfile.open("variables" + saveName + ".txt", std::ios::trunc);

	myfile << networkDepth << " " << networkWidth << " " << networkHeight << " " << timeConstant << " " << resistance << " " << restingPotential;

	myfile.close();

}

void LSM::createNetwork(int aDepth, int aWidth, int aHeight, int inputSize, int outputSize)
{
	network.resize(aDepth);
	inputLayer.resize(inputSize);
	outputLayer.resize(outputSize);

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

	for (int n = 0; n < inputSize; n++)
	{
		inputLayer[n] = new Neuron(-1, n, 0);
	}

	for (int n = 0; n < outputSize; n++)
	{
		outputLayer[n] = new OutputNeuron(aDepth, n, 0);
	}

	networkDepth = aDepth;
	networkWidth = aWidth;
	networkHeight = aHeight;
	inputNeurons = inputSize;
	outputNeurons = outputSize;

	updateBufferSize();
}

void LSM::updateBufferSize()
{
	int n = 0;

	for (int x = 0; x < network.size(); x++)
	{
		for (int y = 0; y < network[0].size(); y++)
		{
			n += network[0][0].size();
		}
	}

	n += inputNeurons;

	buffer.setSize(n);
}

