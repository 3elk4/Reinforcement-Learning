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

	//TODO: sprawdziæ czy to o to chodzi
	auto action = get_action(s);
	return this->qtable.at(s).at(action.value());
}
