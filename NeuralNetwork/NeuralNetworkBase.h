#pragma once

#include <iostream>
#include <armadillo>
#include <vector>
#include <math.h>
#include "Layer.h"

using namespace std;

class NeuralNetworkBase
{
public:
	NeuralNetworkBase();
	~NeuralNetworkBase();
	NeuralNetworkBase(NeuralNetworkBase &base);
	NeuralNetworkBase(vector<Layer>& layers, double alpha);

	virtual arma::mat predict(const arma::mat &input_array);
	virtual void fit(const arma::mat &input_array, arma::mat &expected_output_array);
	virtual void computeError(const arma::mat &delta);

protected:
	vector<Layer> layers;
	double error = 0.0;
	double alpha = 0.01; // learning rate
};
