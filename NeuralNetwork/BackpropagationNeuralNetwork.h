#pragma once
#include <iostream>
#include "NeuralNetworkBase.h"

using namespace std;

class BackpropagationNeuralNetwork : NeuralNetworkBase
{
public:
	BackpropagationNeuralNetwork();
	BackpropagationNeuralNetwork(BackpropagationNeuralNetwork &network);
	BackpropagationNeuralNetwork(vector<Layer>& layers, double alpha, double dropout_prob, int batch_size);
	~BackpropagationNeuralNetwork();

	void set_training_state(bool value) {
		this->training_state = value;
	}

	arma::mat predict(const arma::mat &input_array) override;
	void fit(const arma::mat &input_array, arma::mat &expected_output_array) override;

private:
	double dropout_prob = 1.0;
	bool training_state = false;
	int batch_size = 1;
};

