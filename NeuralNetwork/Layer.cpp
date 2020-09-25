#include "pch.h"
#include "Layer.h"

binomial_distribution<int> Layer::distribution(2.0, 0.5);

Layer::Layer()
{
}

Layer::Layer(arma::mat &weights)
{
	this->weights = weights;
}


Layer::~Layer()
{
}

arma::mat Layer::computeNeurons(arma::mat & input)
{
	this->neurons = input * this->weights.t();
	return this->neurons;
}

arma::mat Layer::computeDelta(arma::mat & expected)
{
	return this->neurons - expected;
}

arma::mat Layer::computeHiddenDelta(arma::mat & delta)
{
	return delta * this->weights;
}

arma::mat Layer::outerProduct(arma::mat & delta)
{
	//cout << arma::conv_to<arma::colvec>::from(delta) << endl;
	//cout << arma::conv_to<arma::rowvec>::from(this->neurons) << endl;
	//cout << arma::conv_to<arma::colvec>::from(delta) * arma::conv_to<arma::rowvec>::from(this->neurons) << endl;
	return arma::conv_to<arma::colvec>::from(delta) * arma::conv_to<arma::rowvec>::from(this->neurons);
}

void Layer::updateWeights(arma::mat & weight_delta)
{
	this->weights -= weight_delta;
}

arma::mat Layer::activateNeurons(arma::mat & neurons)
{
	if (this->activFunc.first != nullptr) neurons = this->activFunc.first(neurons);
	return neurons;
}

arma::mat Layer::activateDelta(arma::mat & delta)
{
	if (this->activFunc.second != nullptr) delta = this->activFunc.second(delta);
	return delta;
}

arma::mat Layer::addDropoutMask(arma::mat & neurons, double & dropout_prob)
{
	this->dropout_mask = generateDropoutMask(neurons.n_rows, neurons.n_cols);
	neurons %= (this->dropout_mask / dropout_prob);
	return neurons;
}

arma::mat Layer::computeDropoutMask(arma::mat & delta)
{
	delta %= this->dropout_mask;
	return delta;
}

arma::mat Layer::generateDropoutMask(const int & rows, const int & columns)
{
	auto mask = arma::mat(rows, columns, arma::fill::zeros);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; ++j) {
			if(Layer::distribution(Layer::generator)) {
				mask.row(i).col(j) = 1;
			}
		}
	}
	return mask;
}

bool Layer::operator!=(const Layer & other)
{
	return !arma::approx_equal(this->weights, other.weights, "absdiff", 0.0);
}

void Layer::set_neurons(const arma::mat & neurons)
{
	this->neurons = neurons;
}

arma::mat Layer::get_weights()
{
	return this->weights;
}
