#include "pch.h"
#include "NeuralNetworkCreator.h"

vector<int> NeuralNetworkCreator::get_neural_numbers()
{
	vector<int> neural_numbers;
	int number = 0;
	cout << "\nEnter number of input neurons: ";
	cin >> number;
	neural_numbers.push_back(number);

	cout << "\nEnter number of hidden neurons: ";
	cin >> number;
	neural_numbers.push_back(number);

	cout << "\nEnter number of output neurons: ";
	cin >> number;
	neural_numbers.push_back(number);
	return neural_numbers;
}

vector<arma::mat> NeuralNetworkCreator::create_weights(vector<int> &neural_numbers, vector<pair<double, double>> &intervals)
{
	vector<arma::mat> weights;
	for (int i = 0; i < neural_numbers.size()-1; ++i) {
		auto weight = arma::randi<arma::mat>(neural_numbers.at(i+1), neural_numbers.at(i), arma::distr_param(intervals[i].first, intervals[i].second)); 
		weights.push_back(weight);
	}

	return weights;
}

vector<pair<activFunc, activFunc>> NeuralNetworkCreator::create_activate_functions(vector<int>& neural_numbers)
{
	vector<pair<activFunc, activFunc>> functions;
	string name;
	ActivFunction af;
	for (int i = 1; i < neural_numbers.size(); ++i) {
		cout << "\nEnter name of function activation (or 'none'): ";
		cin >> name;	
		if (name == "RELU") af = ActivFunction::RELU;
		else if (name == "SIGMOID") af = ActivFunction::SIGMOID;
		else if (name == "TANH") af = ActivFunction::TANH;
		else if (name == "SOFTMAX") af = ActivFunction::SOFTMAX;
		else af = ActivFunction::NONE;
		functions.push_back(ActivateFunctions::get_funcs_by_type(af));
	}
	return functions;
}
