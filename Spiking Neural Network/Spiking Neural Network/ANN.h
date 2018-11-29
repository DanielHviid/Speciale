#pragma once
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Core>
#include <vector>


class ANN
{
public:
	ANN();
	ANN(std::vector<int> newLayers, double newLearningRate);
	~ANN();

	void setWeights(std::vector<std::vector<std::vector<double>>> weights);

	std::vector<double> activate(std::vector<double> input);
	void backPropagate(std::vector<double> error);

private:

	int depth, inputSize, outputSize;

	std::vector<int> layerSizes;


	std::vector<Eigen::Matrix<double, 1, Eigen::Dynamic>> inputs, activations, errors;
	std::vector<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>> weights;
	
};

