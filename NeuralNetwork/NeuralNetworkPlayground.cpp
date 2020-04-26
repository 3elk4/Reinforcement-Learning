#include "pch.h"
#include "NeuralNetworkPlayground.h"


NeuralNetworkPlayground::NeuralNetworkPlayground(NeuralNetworkBase &base, vector<string> &result_names, int batch_size)
{
	this->neural_network = base;
	this->result_names = result_names;
	this->batch_size = batch_size;
}


NeuralNetworkPlayground::NeuralNetworkPlayground(NeuralNetworkPlayground & playground)
{
	this->accuracy = playground.accuracy;
	this->batch_size = playground.batch_size;
	this->neural_network = playground.neural_network;
	this->result_names = playground.result_names;
}

NeuralNetworkPlayground::~NeuralNetworkPlayground()
{
}

double NeuralNetworkPlayground::train_neural_network(vector<pair<arma::mat, int>>& input_array, vector<arma::mat>& expected_output_array, int iteration)
{
	for(int it = 0; it < iteration; ++it){
		this->accuracy = 0.0;
		for (int i = 0; i < input_array.size(); ++i) {
			this->input_array_batch.insert_rows(i % this->batch_size, input_array.at(i).first);
			this->expected_output_array_batch.insert_cols(i % this->batch_size, expected_output_array.at(input_array.at(i).second));
			this->expected_value_batch.push_back(input_array.at(i).second);
			if ((it + 1) % this->batch_size == 0) { 
				auto output = this->neural_network.compute(this->input_array_batch);
				this->neural_network.update(this->input_array_batch, this->expected_output_array_batch);
				for (int j = 0; j < this->expected_value_batch.size(); ++j) {
					if (output.row(j).index_max == this->expected_value_batch.at(j)) this->accuracy++;
				}
				this->input_array_batch.clear();
				this->expected_output_array_batch.clear();
				this->expected_value_batch.clear();
			}
		}
		this->accuracy = (this->accuracy / input_array.size()) * 100;
		if (it % 10 == 0) {
			cout << "ITERATION: " << it << ", accuracy: " << this->accuracy << "%." << endl;
		}
	}
	
	return (this->accuracy / input_array.size()) * 100;
}

double NeuralNetworkPlayground::test_neural_network(vector<pair<arma::mat, int>>& input_array)
{
	this->accuracy = 0.0;
	for (auto & input : input_array) {
		auto output = this->neural_network.compute(input.first);
		if (output.index_max == input.second) this->accuracy++;
	}
	return (this->accuracy / input_array.size()) * 100;
}

