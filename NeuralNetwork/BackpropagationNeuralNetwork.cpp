#include "pch.h"
#include "BackpropagationNeuralNetwork.h"


BackpropagationNeuralNetwork::BackpropagationNeuralNetwork() : NeuralNetworkBase()
{
}

BackpropagationNeuralNetwork::BackpropagationNeuralNetwork(BackpropagationNeuralNetwork & network)
{
	this->alpha = network.alpha;
	this->weights = network.weights;
	this->layers = network.layers;
	this->activfunc = network.activfunc;
	this->dropout_prob = network.dropout_prob;
	this->batch_size = batch_size;
	this->distribution = binomial_distribution<int>(1, dropout_prob);
}

BackpropagationNeuralNetwork::BackpropagationNeuralNetwork(vector<arma::mat>& weights, double alpha, vector<pair<activFunc, activFunc>> &activFunc, double dropout_prob, int batch_size) :
	NeuralNetworkBase(weights, alpha)
{
	this->activfunc = activFunc;
	this->dropout_prob = dropout_prob;
	this->batch_size = batch_size;
	this->distribution = binomial_distribution<int>(1, dropout_prob);
}


BackpropagationNeuralNetwork::~BackpropagationNeuralNetwork()
{
}

arma::mat BackpropagationNeuralNetwork::compute(arma::mat & input_array)
{
	auto input = input_array;
	for (int i = 0; i < this->weights.size(); ++i) {
		arma::mat output = input * this->weights[i].t();
		if (this->activfunc[i].first != nullptr) {
			output = this->activfunc[i].first(output); 
		}
		if (this->training_state && i != this->weights.size() - 1) {
			auto mask = arma::mat(output.size(), distribution(generator));
			output *= (mask / this->dropout_prob);
			this->dropout_mask.push_back(mask);
		}
		this->layers.push_back(output);
		input = output;
	}
	return input;
}

void BackpropagationNeuralNetwork::update(arma::mat & input_array, arma::mat & expected_output_array)
{
	arma::mat past_delta;
	vector<arma::mat> weight_delta;
	for (int i = this->weights.size() - 1; i >= 0; --i) {
		if (i == this->weights.size() - 1) {
			past_delta = this->layers.at[i] - expected_output_array;
			if (this->activfunc[i].second != nullptr) {
				past_delta = this->activfunc[i].second(past_delta) * (1 / this->batch_size);
			}
			weight_delta.push_back((i == 0 ? past_delta.t() * input_array : past_delta.t() * this->layers[i - 1]) * this->alpha);
			continue;
		}

		arma::mat delta = past_delta * this->weights[i + 1];
		if (this->activfunc[i].second != nullptr) {
			delta = this->activfunc[i].second(delta);
		}
		if(this->training_state) delta *= this->dropout_mask.at(i);
		weight_delta.push_back((i == 0 ? delta.t() * input_array : delta.t() * this->layers[i - 1]) * this->alpha);
		past_delta = delta;
	}

	int i = this->weights.size() - 1;
	for (auto & wd : weight_delta) {
		auto weight = this->weights.at(i);
		weight -= wd;
		this->weights[i] = weight;
		i--;
	}
}
