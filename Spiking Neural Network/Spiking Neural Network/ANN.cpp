#include "stdafx.h"
#include "ANN.h"

double setToPositive(double x)
{
	if (x < 0)
	{
		return -x;
	}
	else
		return x;
}

// use global functions with .unaryExpr(&activationFunction)
double activationFunction(double x) // sigmoid
{
	return x /(1.0 + abs(x));
}

double inverseActivationFunction(double x) // sigmoid
{
	return x * (1.0 - x);
};

double learningRate;
double applyLearningRate(double x)
{
	return x * learningRate;
};

ANN::ANN()
{
}

ANN::ANN(std::vector<int> newLayers, double newLearningRate)
{
	// Set variables
	depth = newLayers.size();
	inputSize = newLayers[0];
	outputSize = newLayers.back();
	layerSizes = newLayers;
	learningRate = newLearningRate;

	// initialize vectors
	inputs.resize(depth);
	activations.resize(depth);
	errors.resize(depth);
	weights.resize(depth);


	// initialize matrices
	for (int n = 0; n < depth; n++)
	{
		inputs[n] = Eigen::Matrix<double, 1, Eigen::Dynamic>();
		activations[n] = Eigen::Matrix<double, 1, Eigen::Dynamic>();
		weights[n] = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>();
		errors[n] = Eigen::Matrix<double, 1, Eigen::Dynamic>();
		errors[n].resize(1, newLayers[n]);
		errors[n].setZero();
	}

	// first layer
	inputs[0].resize(1, newLayers[0]);
	inputs[0].setZero();
	activations[0].resize(1, newLayers[0] + 1);
	activations[0].setOnes();

	// hidden layers
	for (int n = 1; n < depth - 1; n++)
	{
		inputs[n].resize(1, newLayers[n]);
		inputs[n].setZero();
		activations[n].resize(1, newLayers[n] + 1);
		activations[n].setOnes();
		weights[n].resize(newLayers[n - 1] + 1, newLayers[n]);
		weights[n].setRandom();
		weights[n] = weights[n].unaryExpr(&setToPositive);
	}

	// output layer
	inputs[depth - 1].resize(1, newLayers[depth - 1]);
	inputs[depth - 1].setZero();
	activations[depth - 1].resize(1, newLayers[depth - 1]);
	activations[depth - 1].setOnes();
	weights[depth - 1].resize(newLayers[depth - 2] + 1, newLayers[depth - 1]);
	weights[depth - 1].setRandom();
	weights[depth - 1] = weights[depth - 1].unaryExpr(&setToPositive);

}

ANN::~ANN()
{
}

void ANN::setWeights(std::vector<std::vector<std::vector<double>>> weights)
{
}

std::vector<double> ANN::activate(std::vector<double> input)
{

	std::vector<double> result = std::vector<double>(outputSize);

	for (int n = 0; n < input.size() && n < inputSize; n++)
	{
		activations[0](0, n) = input[n];
	}

	inputs[1] = activations[0] * weights[1];

	for (int n = 1; n < depth; n++)
	{
		inputs[n] = activations[n - 1] * weights[n];
		activations[n].block(0,0,1,layerSizes[n]) = inputs[n].unaryExpr(&activationFunction);
	}

	for (int n = 0; n < outputSize; n++)
	{
		result[n] = activations[depth - 1](0, n);
	}

	return result;
}
void ANN::backPropagate(std::vector<double> error)
{
	for (int n = 0; n < error.size() && n < outputSize; n++)
	{
		errors[depth - 1](0, n) = error[n] * inverseActivationFunction(activations[depth - 1](0, n));
	}
	
	weights[depth - 1] += activations[depth - 2].transpose() * errors[depth - 1].unaryExpr(&applyLearningRate);

	for (int n = depth - 2; n > 1; n--)
	{
		errors[n] = weights[n + 1].block(0, 0, layerSizes[n], layerSizes[n + 1]) * errors[n + 1].transpose();
		errors[n] = errors[n].unaryExpr(&inverseActivationFunction);
		weights[n] += activations[n].transpose() * errors[n].unaryExpr(&applyLearningRate);
	}
}

