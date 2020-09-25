#include "pch.h"
#include "BackpropagationNeuralNetwork.h"


BackpropagationNeuralNetwork::BackpropagationNeuralNetwork() : NeuralNetworkBase()
{
}

BackpropagationNeuralNetwork::BackpropagationNeuralNetwork(BackpropagationNeuralNetwork & network) :
	NeuralNetworkBase(network.layers, network.alpha)
{
	this->dropout_prob = network.dropout_prob;
	this->batch_size = batch_size;
}

BackpropagationNeuralNetwork::BackpropagationNeuralNetwork(vector<Layer>& layers, double alpha, double dropout_prob, int batch_size) :
	NeuralNetworkBase(layers, alpha)
{
	this->dropout_prob = dropout_prob;
	this->batch_size = batch_size;
}


BackpropagationNeuralNetwork::~BackpropagationNeuralNetwork()
{
}

arma::mat BackpropagationNeuralNetwork::predict(const arma::mat & input_array)
{
	auto input = input_array;
	for (auto & layer : this->layers) {
		arma::mat output = layer.computeNeurons(input);
		output = layer.activateNeurons(output);

		if (this->training_state && layer != this->layers.back()) {
			output = layer.addDropoutMask(output, this->dropout_prob);
		}
		input = output;
		layer.set_neurons(output);
	}
	return input;
}

void BackpropagationNeuralNetwork::fit(const arma::mat & input_array, arma::mat & expected_output_array)
{
	//cout << this->layers[0].get_weights() << endl;
	this->training_state = true;
	predict(input_array);
	
	arma::mat past_delta;
	vector<arma::mat> weight_delta;
	int maxIndex = (int)this->layers.size() - 1;
	for (int i = maxIndex; i >= 0; --i) {
		if (i == maxIndex) {
			past_delta = this->layers[i].computeDelta(expected_output_array);
			past_delta = this->layers[i].activateDelta(past_delta);
			inplace_trans(past_delta, "lowmem");
			auto result = (i == 0 ? past_delta * input_array : this->layers[i - 1].outerProduct(past_delta));
			//cout << result << endl;
			result *= this->alpha;
			//cout << result << endl;
			weight_delta.push_back(result);
			//cout << weight_delta[0] << endl;
			inplace_trans(past_delta, "lowmem");
			continue;
		}

		arma::mat delta = this->layers[i + 1].computeHiddenDelta(past_delta);
		delta = this->layers[i].activateDelta(delta);

		if(this->training_state) delta = this->layers[i].computeDropoutMask(delta);
		inplace_trans(delta, "lowmem");
		//cout << delta << endl;
		weight_delta.push_back(
			(i == 0 ? delta * input_array : this->layers[i-1].outerProduct(delta))
			* this->alpha);
		inplace_trans(delta, "lowmem");
		past_delta = delta;
	}

	for (auto & wd : weight_delta) {
		this->layers[maxIndex].updateWeights(wd);
		maxIndex--;
	}

	this->training_state = false;
	//cout << this->layers[0].get_weights() << endl;
}
