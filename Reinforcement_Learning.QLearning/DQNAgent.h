#pragma once
#include <iostream>
#include <list>
#include <armadillo>
#include <optional>
#include "Environment.h"
#include "NeuralNetworkBase.h"
#include "BackpropagationNeuralNetwork.h"
#include <iomanip>

using namespace std;

template<class S, class A>
struct step_info {
	S state;
	A action;
	S next_state;
	double reward;
	bool is_done;
};

template<class S, class A>
class DQNAgent
{
public:
	DQNAgent(BackpropagationNeuralNetwork &q_network, list<A> &possible_actions) : q_network(q_network) {
		this->possible_actions = possible_actions;
	}

	DQNAgent(double &alpha, double &epsilon, double &gamma, BackpropagationNeuralNetwork &q_network, list<A> &possible_actions) 
		: q_network(q_network) {
		this->alpha = alpha; // 0.75
		this->epsilon = epsilon; // 0.5
		this->gamma = gamma; // 0.99
		this->possible_actions = possible_actions;
	}

	void set_parameters(double alpha, double epsilon, double gamma) {
		this->alpha = alpha;
		this->gamma = gamma;
		this->epsilon = epsilon;
	}

	void reset_parameters() {
		this->alpha = 0.0;
		this->gamma = 0.0;
		this->epsilon = 0.0;
	}

	~DQNAgent() {}

	void remember(const S &s1, const A &a, const double &reward, const S&s2, bool is_done) {
		step_info<S, A> si; si.state = s1; si.action = a; si.reward = reward; si.next_state = s2; si.is_done = is_done;
		this->memory.push_back(si);
	}

	optional<A> get_action(const S & s) {
		if (this->possible_actions.empty()) return nullopt;

		//random action
		if (((rand() % 1001) / 1000.0) <= this->epsilon)
		{
			auto it = this->possible_actions.begin();
			advance(it, rand() % possible_actions.size());
			return *it;
		}

		//best action
		auto action_values = this->q_network.predict(s); //predict
		auto max_actions = this->translate_to_best_actions(action_values, this->possible_actions);

		auto it = max_actions.begin();
		advance(it, rand() % max_actions.size());
		return *it;
	}

	void replay(int batch_size) {
		//if (this->memory.size() < batch_size) return;
		init_batch(batch_size);

		double target = 0.0;
		arma::mat Q_next;
		arma::mat expected;
		for (auto & sasrd : this->memory_batch) {
			target = sasrd.reward;
			if (!sasrd.is_done) {
				Q_next = this->q_network.predict(sasrd.next_state); // returns actions
				target += this->gamma * Q_next.max();
			}
			expected = this->q_network.predict(sasrd.state); // returns actions
			auto it = find(this->possible_actions.begin(), this->possible_actions.end(), sasrd.action);
			if (it != this->possible_actions.end()) {
				expected.at(distance(this->possible_actions.begin(), it)) = target;
				this->q_network.fit(sasrd.state, expected);
				expected = this->q_network.predict(sasrd.state);
			}
		}
		Q_next = this->q_network.predict(this->memory[0].state);

		cout << setprecision(10) << this->memory[0].state << endl;
		cout << setprecision(10) << Q_next << endl;
	}

private:
	vector<step_info<S, A>> memory;
	vector<step_info<S, A>> memory_batch;

	double alpha;
	double epsilon;
	double gamma;
	list<A> possible_actions;

	BackpropagationNeuralNetwork &q_network;

	void init_batch(const int & batch_size) {
		this->memory_batch.clear();
		while (this->memory_batch.size() < batch_size) {
			auto it = this->memory.begin();
			advance(it, rand() % this->memory.size());
			memory_batch.push_back(*it);
		}
	}

	list<A> translate_to_best_actions(arma::mat &action_values, list<A> &possible_actions) {
		double max_value = action_values.max();

		list<A> actions;
		for (int i = 0; i < possible_actions.size(); ++i) {
			if (action_values.at(i) == max_value) {
				auto it = possible_actions.begin();
				advance(it, i);
				actions.push_back(*it);
			}
		}
		return actions;
	}
};

