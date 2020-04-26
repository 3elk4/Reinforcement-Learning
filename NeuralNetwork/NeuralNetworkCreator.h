#pragma once
#include "NeuralNetworkBase.h"
#include "ActivateFunctions.h"
#include <armadillo>

class NeuralNetworkCreator
{
public:
	NeuralNetworkCreator() = delete;
	~NeuralNetworkCreator() = delete;

	static vector<int> get_neural_numbers();
	static vector<arma::mat> create_weights(vector<int> &neural_numbers, vector<pair<double, double>> &intervals);
	static vector<pair<activFunc, activFunc>> create_activate_functions(vector<int> &neural_numbers);
};

