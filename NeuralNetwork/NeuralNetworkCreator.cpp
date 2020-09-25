#include "pch.h"
#include "NeuralNetworkCreator.h"

Layer NeuralNetworkCreator::createLayer(pair<int, int>& input_output_number, pair<double, double>& intervals, string &funcName)
{
	auto weights = NeuralNetworkCreator::create_weights(input_output_number, intervals);
	auto activFunc = NeuralNetworkCreator::create_activate_functions(funcName);
	Layer layer(weights);
	layer.set_activate_functions(activFunc);
	return layer;
}

arma::mat NeuralNetworkCreator::create_weights(pair<int, int>& input_output_number, pair<double, double>& intervals)
{
	arma::mat temp = arma::randu<arma::mat>(input_output_number.second, input_output_number.first);
	//cout << temp << endl;
	temp = temp * (intervals.second - intervals.first) + intervals.first;
	//cout << temp << endl;
	return temp;
}

pair<activFunc, activFunc> NeuralNetworkCreator::create_activate_functions(string funcName)
{
	ActivFunction af;
	if (funcName == "RELU") af = ActivFunction::RELU;
	else if (funcName == "SIGMOID") af = ActivFunction::SIGMOID;
	else if (funcName == "TANH") af = ActivFunction::TANH;
	else if (funcName == "SOFTMAX") af = ActivFunction::SOFTMAX;
	else af = ActivFunction::NONE;
	return ActivateFunctions::get_funcs_by_type(af);
}
