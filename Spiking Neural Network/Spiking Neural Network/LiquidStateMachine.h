#include "SpikingHiddenLayer.h"
#include "ANN.h"

#pragma once


class LiquidStateMachine
{
public:
	LiquidStateMachine();
	LiquidStateMachine(int newinputs, int newoutputs, int newhiddenInputs, int newhiddenOutputs, double newLearningRate,
		int newtimeSteps, double newresistance, double newrestingPotential,
		int newconnections, int newsynapseLength, double newminWeights, double newmaxWeights,
		int newdepth, int newwidth, int newheight);

	void setConstants(int newinputs, int newoutputs, int newhiddenInputs, int newhiddenOutputs, double newLearningRate,
		int newtimeSteps, double newresistance, double newrestingPotential,
		int newconnections, int newsynapseLength, double newminWeights, double newmaxWeights,
		int newdepth, int newwidth, int newheight);

	void recreate();
	void clearLayers();
	void connectInputToHidden();
	void connectHiddenToOutput();

	std::vector<double> activateOnce(std::vector<double> input);
	std::vector<double> activate(std::vector<double> input);

	~LiquidStateMachine();

private:

	std::vector<Neuron*> inputLayer = std::vector<Neuron*>(), outputLayer = std::vector<Neuron*>();
	SpikingHiddenLayer hiddenLayer;

	bool initialized = false;
	int inputs, outputs, hiddenInputs, hiddenOutputs;
	double learningRate;
	int timeSteps; // in miliseconds
	double resistance, restingPotential;
	int connections, synapseLength;
	double minWeight, maxWeight;
	int hiddenDepth, hiddenWidth, hiddenHeight;


};

