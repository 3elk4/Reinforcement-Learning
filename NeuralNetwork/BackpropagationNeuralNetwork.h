#pragma once
#include <iostream>
#include "NeuralNetworkBase.h"
#include "ActivateFunctions.h"

using namespace std;

class BackpropagationNeuralNetwork : NeuralNetworkBase
{
public:
	BackpropagationNeuralNetwork();
	BackpropagationNeuralNetwork(BackpropagationNeuralNetwork &network);
	BackpropagationNeuralNetwork(vector<arma::mat>& weights, double alpha, vector<pair<activFunc, activFunc>> &activFunc, double dropout_prob, int batch_size);
	~BackpropagationNeuralNetwork();

	void set_training_state(bool value) {
		this->training_state = value;
	}

	arma::mat compute(arma::mat &input_array) override; // returns output
	void update(arma::mat &input_array, arma::mat &expected_output_array) override;

private:
	default_random_engine generator;
	binomial_distribution<int> distribution;

	vector<pair<activFunc, activFunc>> activfunc;
	vector<arma::mat> dropout_mask;
	double dropout_prob = 1.0;
	bool training_state = true;
	int batch_size = 1;
	//TODO: posprawdzaæ
};

