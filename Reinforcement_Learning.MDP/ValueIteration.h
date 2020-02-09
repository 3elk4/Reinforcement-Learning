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
#include <iostream>
using namespace std;


template<class S, class A>
class ValueIteration
{
public:
	ValueIteration() {
		this->gamma = 0.9;
		this->mdpModel = MDPmodel();
		for (auto const& p : mdpModel.get_all_states()) {
			this->state_values[p] = 0;
		}
	}

	ValueIteration(const MDPmodel &mdpmodel) {
		this->gamma = 0.9;
		this->mdpModel = mdpModel;
		for (auto const& p : mdpModel.get_all_states()) {
			this->state_values[p] = 0;
		}
	}

	~ValueIteration() {}

	map<S, double> get_state_values() {
		return this->state_values;
	}

	double get_action_value(S state, A a, map<S, double> state_values) {
		double value = 0.0;

		for (auto const &st : this->mdpModel.get_next_states(state, a)) {
			double p = st.second;
			double r = this->mdpModel.get_reward(state, a, st.first);
			double v = state_values[st.first];
			value += p * (r + this->gamma * v);
		}

		return value;
	}

	double get_new_state_value(S state, map<S, double> state_values) {
		if (this->mdpModel.is_terminal(state)) return 1.0;

		vector<double> temp_values;
		for (auto const& a_temp : this->mdpModel.get_possible_actions(state)) {
			temp_values.push_back(get_action_value(state, a_temp, state_values));
		}
		return *max_element(temp_values.begin(), temp_values.end());
	}

	void main_value_iteration(int numIteration, double minimumDiffrence) {
		for (int i = 0; i < numIteration; ++i) {
			map<S, double> new_state_values;
			vector<double> diffrences;
			for (auto const & st : this->mdpModel.get_all_states()) {
				new_state_values[st] = get_new_state_value(st, this->state_values);
			}

			for (auto const & st : this->mdpModel.get_all_states()) {
				diffrences.push_back(abs(new_state_values[st] - this->state_values[st]));
				this->state_values[st] = new_state_values[st];
			}

			double diffrence = *max_element(diffrences.begin(), diffrences.end());
			if (diffrence < minimumDiffrence) {
				break;
			}

		}
	}

private:
	map<S, double> state_values;
	MDPmodel mdpModel;
	double gamma;
};

