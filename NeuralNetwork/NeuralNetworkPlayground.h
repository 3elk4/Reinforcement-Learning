#pragma once
#include "NeuralNetworkBase.h"

using namespace std;

class NeuralNetworkPlayground
{
public:
	NeuralNetworkPlayground() = delete;
	NeuralNetworkPlayground(NeuralNetworkPlayground &playground);
	NeuralNetworkPlayground(NeuralNetworkBase &base, vector<string> &result_names, int batch_size);
	~NeuralNetworkPlayground();

	double train_neural_network(vector<pair<arma::mat, int>> &input_array, vector<arma::mat> &expected_output_array, int iteration);
	double test_neural_network(vector<pair<arma::mat, int>> &input_array);

private:
	double accuracy = 0.0;
	int batch_size = 1;
	vector<string> result_names;
	NeuralNetworkBase neural_network;
	arma::mat input_array_batch;
	arma::mat expected_output_array_batch;
	vector<int> expected_value_batch;

};

