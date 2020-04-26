#pragma once

#include <iostream>
#include <armadillo>
#include <vector>
#include <math.h>

using namespace std;

class NeuralNetworkBase
{
public:
	NeuralNetworkBase();
	NeuralNetworkBase(NeuralNetworkBase &base);
	NeuralNetworkBase(vector<arma::mat>& weights, double alpha);
	~NeuralNetworkBase();

	virtual arma::mat compute(arma::mat &input_array); // returns output
	virtual void update(arma::mat &input_array, arma::mat &expected_output_array);

protected:
	vector<arma::mat> layers;
	vector<arma::mat> weights;
	double error = 0.0;
	double alpha = 0.01; // learning rate
};
