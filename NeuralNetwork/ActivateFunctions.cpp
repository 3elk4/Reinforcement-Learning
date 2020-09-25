#include "pch.h"
#include "ActivateFunctions.h"

pair<activFunc, activFunc> ActivateFunctions::get_funcs_by_type(ActivFunction &af)
{
	switch (af) {
		case ActivFunction::NONE:
			return make_pair(nullptr, nullptr);
		case ActivFunction::RELU:
			return make_pair(ReLu, ReLu_deriv);
		case ActivFunction::SIGMOID:
			return make_pair(Sigmoid, Sigmoid_deriv);
		case ActivFunction::TANH:
			return make_pair(Tanh, Tanh_deriv);
		case ActivFunction::SOFTMAX:
			return make_pair(Softmax, Softmax_deriv);
		default:
			throw invalid_argument("None function this name");
	}
}

arma::mat ActivateFunctions::ReLu(arma::mat &values)
{
	values.for_each([](auto& value) { value = value > 0 ? value : 0.0; });
	return values;
}

arma::mat ActivateFunctions::ReLu_deriv(arma::mat &values)
{
	values.for_each([](auto& value) { value = value > 0 ? 1.0 : 0.0; });
	return values;
}

arma::mat ActivateFunctions::Sigmoid(arma::mat &values)
{
	values.for_each([](auto& value) { value = 1.0 / (1.0 + exp(-value)); });
	return values;
}

arma::mat ActivateFunctions::Sigmoid_deriv(arma::mat &values)
{
	values.for_each([](auto& value) { value = value * (1.0 - value); });
	return values;
}

arma::mat ActivateFunctions::Tanh(arma::mat &values)
{
	values.for_each([](auto& value) { value = tanh(value); });
	return values;
}

arma::mat ActivateFunctions::Tanh_deriv(arma::mat &values)
{
	values.for_each([](auto& value) { value = 1.0 - pow(value, 2); });
	return values;
}

arma::mat ActivateFunctions::Softmax(arma::mat & values)
{
	arma::mat temp = arma::exp(values);
	temp.each_row([](arma::drowvec &row) { row /= arma::sum(row); });
	return temp;
}

arma::mat ActivateFunctions::Softmax_deriv(arma::mat & values)
{
	return values / (int)values.n_cols;
}

