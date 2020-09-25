#include "pch.h"
#include "NeuralNetworkBase.h"


NeuralNetworkBase::NeuralNetworkBase()
{
	this->alpha = 0.01;
}

NeuralNetworkBase::NeuralNetworkBase(NeuralNetworkBase & base)
{
	this->alpha = base.alpha;
	this->layers = base.layers;
}

NeuralNetworkBase::NeuralNetworkBase(vector<Layer>& layers, double alpha)
{
	if (alpha < 0 || alpha > 1) throw invalid_argument("Alpha should be value in <0,1> range.");
	this->alpha = alpha;
	this->layers = layers;
}

NeuralNetworkBase::~NeuralNetworkBase()
{
}

arma::mat NeuralNetworkBase::predict(const arma::mat &input_array)
{
	auto input = input_array;
	for (auto & layer : this->layers) {
		arma::mat output = layer.computeNeurons(input);
		input = output;
	}
	return input;
}

void NeuralNetworkBase::fit(const arma::mat &input_array, arma::mat &expected_output_array)
{
	auto & layer = this->layers.back();
	arma::mat delta = layer.computeDelta(expected_output_array);
	this->computeError(delta);
	arma::mat weight_delta = delta.t() * input_array * this->alpha;
	layer.updateWeights(weight_delta);
}

void NeuralNetworkBase::computeError(const arma::mat & delta)
{
	this->error = 0.0;
	for (auto it = delta.begin(); it != delta.end(); ++it) {
		this->error += pow(*it, 2);
	}
}
