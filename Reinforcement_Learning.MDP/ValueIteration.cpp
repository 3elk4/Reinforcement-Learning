#include "ValueIteration.h"
#include <iostream>

using namespace std;

/*
ValueIteration::ValueIteration()
{
	this->gamma = 0.9;
	this->mdpModel = MDPmodel();
	for (auto const& p : mdpModel.get_all_states()) {
		this->state_values[p] = 0;
	}
}

ValueIteration::ValueIteration(const MDPmodel &mdpmodel)
{
	this->gamma = 0.9;
	this->mdpModel = mdpModel;
	for (auto const& p : mdpModel.get_all_states()) {
		this->state_values[p] = 0;
	}
}


ValueIteration::~ValueIteration()
{
}

map<pair<Point,Point>, double> ValueIteration::get_state_values()
{
	return this->state_values;
}

double ValueIteration::get_action_value(pair<Point,Point> state, action a, map<pair<Point, Point>, double> state_values)
{
	double value = 0.0;
	
	for (auto const &st : this->mdpModel.get_next_states(state, a)) {
		double p = st.second;
		double r = this->mdpModel.get_reward(state, a, st.first);
		double v = state_values[st.first];
		value += p * (r + this->gamma * v);
	}

	return value;
}

double ValueIteration::get_new_state_value(pair<Point,Point> state, map<pair<Point, Point>, double> state_values)
{
	if (this->mdpModel.is_terminal(state)) return 1.0;

	vector<double> temp_values;
	for (auto const& a_temp : this->mdpModel.get_possible_actions(state)) {
		temp_values.push_back(get_action_value(state, a_temp, state_values));
	}
	return *max_element(temp_values.begin(), temp_values.end());
}

void ValueIteration::main_value_iteration(int numIteration, double minimumDiffrence)
{
	for (int i = 0; i < numIteration; ++i) {
		map<pair<Point,Point>, double> new_state_values;
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
}*/
