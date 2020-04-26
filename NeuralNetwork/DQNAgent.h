#pragma once
#include "NeuralNetworkBase.h"
#include <optional>

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
	DQNAgent() {
		this->alpha = 0.75;
		this->epsilon = 0.5; 
		this->gamma = 0.99; 
	}
	DQNAgent(double alpha, double epsilon, double gamma, NeuralNetworkBase base, vector<A> possible_actions) {
		this->alpha = alpha;
		this->epsilon = epsilon;
		this->gamma = gamma;
		this->neural_network = base;
		this->possible_actions = possible_actions;
	}
	~DQNAgent(){}

	void remember(const S &s1, const A &a, const double &reward, const S&s2, bool is_done) {
		step_info<S, A> si; si.state = s1; si.action = a; si.reward = reward; si.next_state = s2; si.is_done = is_done;
		memory.push_back(si);
	}

	optional<A> get_action(const S & s) {
		if (possible_actions.empty()) return nullopt;

		if (((rand() % 1001) / 1000.0) <= this->epsilon)
		{
			auto it = possible_actions.begin();
			advance(it, rand() % possible_actions.size());
			return *it;
		}
		auto action_values = this->neural_network.compute(s); //predict
		double max_value = action_values.max();

		list<A> rand_action;
		for (int i = 0; i < possible_actions.size(); ++i) {
			if (action_values.at(i) == max_value) {
				rand_action.push_back(possible_actions.at(i));
			}
		}

		auto it = rand_action.begin();
		advance(it, rand() % rand_action.size());
		return *it;
	}

	void replay(int batch_size) {
		this->memory_batch.clear();
		while (this->memory_batch.size() < batch_size) {
			auto it = this->memory.begin();
			advance(it, rand() % this->memory.size());
			memory_batch.push_back(*it);
		}

		double target = 0.0;
		arma::mat Q_next;
		arma::mat expected;
		for (auto & sasrd : this->memory_batch) {
			target = sasrd.reward;
			if (!sasrd.is_done) {
				Q_next = this->neural_network.compute(sasrd.next_state);
				target = (sasrd.reward + this->gamma * Q_next.max());
			}	
			expected = this->neural_network.compute(sasrd.state);
			auto it = find(this->possible_actions.begin(), this->possible_actions.end(), sasrd.action);
			if (it != this->possible_actions.end()) {
				expected.at(distance(this->possible_actions.begin(), it)) = target;
				this->neural_network.update(sasrd.state, expected);
			}
		}
	}

private:
	vector<step_info<S, A>> memory; 
	vector<step_info<S, A>> memory_batch; 
	NeuralNetworkBase neural_network;

	double alpha;
	double epsilon;
	double gamma;
	vector<A> possible_actions;
};

