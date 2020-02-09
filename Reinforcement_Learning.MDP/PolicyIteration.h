#pragma once
#include <map>
#include <vector>
#include <algorithm> 
#include <stdlib.h>
#include <utility>
#include "Pair.h"
#include "Point.h"
#include "Constants.h"
#include "MDPmodel.h"
#include "ValueIteration.h"
#include <iostream>
using namespace std;


template<class S, class A>
class PolicyIteration
{
public:
	PolicyIteration() {
		this->gamma = 0.9;
		this->mdpModel = MDPmodel();
		for (auto const& p : mdpModel.get_all_states()) {
			this->policy_state_values[p] = 0;
		}
	}

	PolicyIteration(const MDPmodel &model) {
		this->gamma = 0.9;
		this->mdpModel = model;
		for (auto const& p : mdpModel.get_all_states()) {
			this->policy_state_values[p] = 0;
		}
	}

	~PolicyIteration(){}

	map<S, double> get_policy_state_values() {
		if (this->policy_state_values.empty()) {
			return map<S, double>();
		}
		return this->policy_state_values;
	}

	map<S, A> get_policy() {
		if (this->policy.empty()) {
			return map<S, A>();
		}
		return this->policy;
	}

	A get_optimal_action(const S &state, const map<S, double> &policy_state_values) {
		map<A, double> temp;
		for (auto const & a : this->mdpModel.get_possible_actions(state)) {
			temp[a] = get_action_value(state, a, policy_state_values);
		}
		return max_element(temp.begin(), temp.end(), [](const pair<A, double>& p1, const pair<A, double>& p2) {return p1.second < p2.second; })->first;
	}

	template <class S, class A>
	double get_action_value(const S &state, A a, const map<S, double> &policy_state_values) {
		double value = 0.0;

		for (auto const &st : this->mdpModel.get_next_states(state, a)) {
			double p = st.second;
			double r = this->mdpModel.get_reward(state, a, st.first);
			double v = 0.0;
			try {
				v = policy_state_values.at(st.first);
			}
			catch (out_of_range & oor) {
				v = 0.0;
			}
			value += p * (r + this->gamma * v);
		}

		return value;
	}

	map<S, double> compute_policy_state_values(const map<S, A> &policy) {
		map<S, double> temp;
		for (auto const & st : this->mdpModel.get_all_states()) {
			temp[st] = get_action_value(st, policy.at(st), this->policy_state_values);
		}
		return temp;
	}

	map<S, A> compute_new_policy(const map<S, double> &policy_state_values) {
		map<S, A> temp;
		for (auto const & p : this->mdpModel.get_all_states()) {
			temp[p] = get_optimal_action(p, policy_state_values);
		}
		return temp;
	}

	void main_policy_iteration(const map<S, A> &policy, int num_iteration, double minimum_diffrence) {
		if (policy.empty()) {
			for (auto const & state : mdpModel.get_all_states()) {
				//clear
				this->policy_state_values[state] = 0;

				//try new policy
				list<A> temp = mdpModel.get_possible_actions(state);
				auto it = temp.begin();
				advance(it, rand() % temp.size());
				this->policy[state] = *it;
			}
		}
		else {
			this->policy = policy;
		}

		vector<double> diffrences;
		map<S, double> new_policy_state_values;
		for (int i = 0; i < num_iteration; ++i) {
			diffrences.clear();

			new_policy_state_values = compute_policy_state_values(this->policy);
			for (const auto & st : this->mdpModel.get_all_states()) {
				diffrences.push_back(abs(new_policy_state_values.at(st) - this->policy_state_values.at(st)));
				this->policy_state_values[st] = new_policy_state_values[st];
			}

			this->policy = compute_new_policy(new_policy_state_values);
			double diffrence = *max_element(diffrences.begin(), diffrences.end());
			if (diffrence < minimum_diffrence) break;
		}
	}


private:
	double gamma;
	map<S, A> policy;
	map<S, double> policy_state_values;
	MDPmodel mdpModel;
};

