#pragma once
#include <map>
#include <list>
#include <array>
#include <optional>
#include <algorithm> 
#include <utility>
#include <stdio.h>

using namespace std;

template<class S, class A>
class QLearningAgent
{
public:
	QLearningAgent() {

		//TODO: alpha i epsilon na 0 + klawisz wy³¹czajacy naukê + okienko do ustawiania parametrów
		this->alpha = 0.5; // szybkoœæ z jak¹ siê uczy
		this->epsilon = 0.1; // do tego czy ma wybieraæ losowo czy najlepsz¹ akcjê
		this->gamma = 0.99; //ile ma siê uczyæ z przesz³oœci
	}

	QLearningAgent(double alpha, double epsilon, double gamma) {
		this->alpha = alpha;
		this->epsilon = epsilon;
		this->gamma = gamma;
	}

	~QLearningAgent() {}

	map<S, map<A, double>> get_qtable() {
		return this->qtable;
	}
	
	void init_qvalues(const list<S> &states, const list<A> &actions) {
		for (auto &s : states) {
			for (auto &a : actions) {
				this->qtable[s][a] = 1.0;
			}
		}
	}

	void init_qvalue(const S &state, const list<A> &actions) {
		for (auto &a : actions) {
			this->qtable[s][a] = 0.0;
		}
	}

	double get_qvalue(const S&s, const A& a) {
		return this->qtable.at(s).at(a);
	}
	void set_qvalue(const S&s, const A&a, const double &value) {
		this->qtable[s][a] = value;
	}

	list<A> get_legal_actions(const S &s) {
		list<A> actions;
		if (this->qtable.count(s) > 0) {
			for (auto const& kv : this->qtable.at(s)) {
				actions.push_front(kv.first);
			}
		}
		return actions;
	}

	double get_max_value(const S &s) {
		return max_element(this->qtable.at(s).begin(), this->qtable.at(s).end(),
			[](const pair<A, double>& p1, const pair<A, double>& p2) {return p1.second < p2.second; })->second;
	}

	virtual double get_value(const S &s) {
		/*
		Compute your agent's estimate of V(s) using current q-values
			V(s) = max_over_action Q(state, action) over possible actions.
			Note : please take into account that q - values can be negative.
		*/

		//check if is possible to make any action
		auto possible_actions = get_legal_actions(s);
		if (possible_actions.empty()) return -1.0;

		return get_max_value(s);
	}

	void update(const S &s1, const A &a, const double &reward, const S&s2) {
		/*
		You should do your Q - Value update here :
		Q(s, a) : = (1 - alpha) * Q(s, a) + alpha * (r + gamma * V(s'))
		*/
		double value = (1 - this->alpha) * this->qtable.at(s1).at(a) + this->alpha * (reward + this->gamma * get_value(s2));
		//cout << "VALUE: " << value << endl;
		this->set_qvalue(s1, a, value);
	}

	optional<A> get_best_action(const S &s) {
		/*
		Compute the best action to take in a state(using current q - values).
		*/
		auto possible_actions = get_legal_actions(s);
		if (possible_actions.empty()) return nullopt;

		double max_value = get_max_value(s);
		list<A> rand_action;
		for (auto &a : possible_actions) {
			if (this->qtable.at(s).at(a) == max_value) {
				rand_action.push_back(a);
			}
		}

		auto it = rand_action.begin();
		advance(it, rand() % rand_action.size());
		return *it;
	}

	
	optional<A> get_action(const S & s) {
		/*Compute the action to take in the current state, including exploration.
		With probability self.epsilon, we should take a random action.
		otherwise - the best policy action(self.get_best_action).

		Note : To pick randomly from a list, use random.choice(list).
		To pick True or False with a given probablity, generate uniform number in[0, 1]
		and compare it with your probability*/

		auto possible_actions = get_legal_actions(s);
		if (possible_actions.empty()) return nullopt;

		/*bool flag = true;
		for (auto &a : possible_actions) {
			if (this->qtable.at(s).at(a) != 0.0) {
				flag = false;
				break;
			}
		}*/

		if (((rand() % 1001) / 1000.0) <= this->epsilon) //|| flag == true)
		{
			auto it = possible_actions.begin();
			advance(it, rand() % possible_actions.size());
			return *it;
		}
		
		return this->get_best_action(s);
	}

	
protected:
	double alpha;
	double epsilon;
	double gamma;

	map<S, map<A, double>> qtable;
};

