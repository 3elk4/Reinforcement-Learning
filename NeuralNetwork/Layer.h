#pragma once

#include <iostream>
#include <armadillo>
#include <vector>
#include <math.h>
#include "ActivateFunctions.h"

using namespace std;

class Layer
{
public:
	Layer();
	Layer(arma::mat &weights);
	~Layer();

	arma::mat computeNeurons(arma::mat & input);
	arma::mat computeDelta(arma::mat &expected);
	arma::mat computeHiddenDelta(arma::mat &delta);
	arma::mat outerProduct(arma::mat &delta);

	void updateWeights(arma::mat &weight_delta);

	arma::mat activateNeurons(arma::mat & neurons);
	arma::mat activateDelta(arma::mat & delta);

	arma::mat addDropoutMask(arma::mat & neurons, double & dropout_prob);
	arma::mat computeDropoutMask(arma::mat & delta);

	arma::mat generateDropoutMask(const int &rows, const int &columns);

	void set_activate_functions(pair<activFunc, activFunc> &activFunc) {
		this->activFunc = activFunc;
	}

	bool operator!=(const Layer&other);

	void set_neurons(const arma::mat &neurons);
	arma::mat get_weights();

private:
	default_random_engine generator;
	static binomial_distribution<int> distribution;
	
	arma::mat dropout_mask;

	arma::mat weights;
	arma::mat neurons;
	pair<activFunc, activFunc> activFunc;
};

