#pragma once

#include "NeuralNetworkBase.h"
#include "ActivateFunctions.h"
#include <armadillo>

using namespace std;

class NeuralNetworkCreator
{
public:
	NeuralNetworkCreator() = delete;
	~NeuralNetworkCreator() = delete;

	static Layer createLayer(pair<int, int> &input_output_number, pair<double, double> &intervals, string &funcName);
private:
	static arma::mat create_weights(pair<int, int> &input_output_number, pair<double, double> &intervals);
	static pair<activFunc, activFunc> create_activate_functions(string funcName);
};

