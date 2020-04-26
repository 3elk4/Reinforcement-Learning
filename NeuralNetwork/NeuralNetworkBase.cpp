#include "pch.h"
#include "NeuralNetworkBase.h"


NeuralNetworkBase::NeuralNetworkBase()
{
	this->alpha = 0.01;
}

NeuralNetworkBase::NeuralNetworkBase(NeuralNetworkBase & base)
{
	this->alpha = base.alpha;
	this->weights = base.weights;
	this->layers = base.layers;
}

NeuralNetworkBase::NeuralNetworkBase(vector<arma::mat>& weights, double alpha)
{
	if (alpha < 0 || alpha > 1) throw invalid_argument("Alpha should be value in <0,1> range.");
	this->alpha = alpha;
	this->weights = weights;
}

NeuralNetworkBase::~NeuralNetworkBase()
{
}

arma::mat NeuralNetworkBase::compute(arma::mat &input_array)
{
	auto input = input_array;
	for (auto & weight : this->weights) {
		arma::mat output = input * weight.t();
		this->layers.push_back(output);
		input = output;
	}
	return input;
}

void NeuralNetworkBase::update(arma::mat &input_array, arma::mat &expected_output_array)
{
	arma::mat delta = this->layers.back() - expected_output_array;
	this->error = 0.0;
	for (auto it = delta.begin(); it != delta.end(); ++it) {
		this->error += pow(*it, 2);
	}
	arma::mat weight_delta = delta * input_array;
	auto weight = this->weights.back();
	weight -= weight_delta * this->alpha;
	this->weights.pop_back(); this->weights.push_back(weight);
	cout << "WEIGHTS\n" << this->weights.back() << endl;
}
