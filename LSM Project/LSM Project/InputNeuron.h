#include "Neuron.h"
#include <vector>
#pragma once
class InputNeuron : public Neuron
{
public:
	InputNeuron();

	void ReceivePulse(float pulse);

	~InputNeuron();

};

