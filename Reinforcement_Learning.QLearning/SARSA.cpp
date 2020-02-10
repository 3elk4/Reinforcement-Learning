#include "SARSA.h"

double SARSA::get_value(const pair<Point, Point>& s)
{
	/*
	Returns Vpi for current state under epsilon - greedy policy :
	V_{ pi }(s) = sum _{ over a_i } {pi(a_i | s) * Q(s, a_i)}

	Hint: all other methods from QLearningAgent are still accessible.
	*/
	auto possible_actions = get_legal_actions(s);
	if (possible_actions.empty()) return 0.0;

	//save action for next state
	auto action = get_action(s);
	this->set_next_action(action.value());
	return this->qtable.at(s).at(action.value());
}

optional<action> SARSA::get_next_action(const pair<Point, Point>& s)
{
	auto possible_actions = get_legal_actions(s);
	if (possible_actions.empty()) return nullopt;

	if(this->next_action.has_value()) return this->next_action;
	return this->get_action(s);
}

void SARSA::set_next_action(action & a)
{
	this->next_action = a;
}

void SARSA::reset_next_action()
{
	this->next_action = nullopt;
}
