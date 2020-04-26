#pragma once
#include <iostream>
#include <map>
#include <armadillo>
#include <math.h>
#include <string>

using namespace std;

typedef arma::mat(*activFunc)(arma::mat&);

enum class ActivFunction {
	NONE = 0,
	RELU,
	SIGMOID,
	TANH,
	SOFTMAX
};

class ActivateFunctions
{
public:
	ActivateFunctions() = delete;
	~ActivateFunctions() = delete;

	static arma::mat ReLu(arma::mat &values);
	static arma::mat ReLu_deriv(arma::mat &values);

	static arma::mat Sigmoid(arma::mat &values);
	static arma::mat Sigmoid_deriv(arma::mat &values);

	static arma::mat Tanh(arma::mat &values);
	static arma::mat Tanh_deriv(arma::mat &values);

	static arma::mat Softmax(arma::mat &values);
	static arma::mat Softmax_deriv(arma::mat &values);

	static pair<activFunc, activFunc> get_funcs_by_type(ActivFunction &af);
};

