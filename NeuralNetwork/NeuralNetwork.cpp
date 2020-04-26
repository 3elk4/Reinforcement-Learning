// NeuralNetwork.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <armadillo>
#include "NeuralNetworkBase.h"
#include "NeuralNetwork.h"

using namespace std;

int main(int argc, const char **argv) {
	// Initialize the random generator
	arma::arma_rng::set_seed_random();

	arma::mat input = { 8.5, 0.65,  1.2 };
	cout << "A:\n" << input << "\n";
	arma::mat temp = { {0.1, 0.2, -0.1}, {-0.1, 0.1, 0.9 }, {0.1, 0.4, 0.1 } };
	arma::mat temp2 = { {0.3, 1.1, -0.3}, {0.1, 0.2, 0.0 }, {0.0, 1.3, 0.1} };
	
	arma::mat weights = { 0.1, 0.2, -0.1 };
	arma::mat expected = { 1.0 };

	vector<arma::mat> neural_num;
	neural_num.push_back(weights);
	//neural_num.push_back(temp2);
	//vector<int> neural_num = { 3, 3 };
	
	NeuralNetworkBase base(neural_num, 0.01);
	auto output = base.compute(input);
	cout << "OUTPUT:\n" << output << endl;
	base.update(input, expected);
	
	return 0;
	
}
