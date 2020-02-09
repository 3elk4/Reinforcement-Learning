#include "MDPmodel.h"

MDPmodel::MDPmodel()
{
	//empty model
	list<pair<Point, Point>> states = init_all_states(list<Point>());
	init_transition_probs_and_rewards(states);

	this->initial_state = make_pair(Point(0,0), Point(0,0));
	this->current_state = this->initial_state;
	this->n_states = this->transition_probs.size();
}

MDPmodel::MDPmodel(pair<Point, Point> &init_st, vector<Point> obst, list<Point> environment)
{
	list<pair<Point, Point>> states = init_all_states(environment);
	set_obstacles(obst);
	init_transition_probs_and_rewards(states);


	this->initial_state = init_st;
	this->current_state = this->initial_state;
	this->n_states = this->transition_probs.size();	
}

MDPmodel::MDPmodel(MDPmodel & mdp, vector<Point> obst)
{
	set_obstacles(obst);
	init_transition_probs_and_rewards(mdp.get_all_states());
	this->current_state = mdp.current_state;
	this->initial_state = mdp.initial_state;
	this->n_states = mdp.n_states;
}





#pragma region define environment
void MDPmodel::set_obstacles(vector<Point> obst)
{
	this->obstacles = obst;
}

Point MDPmodel::get_next_Point(Point start, action a, int x_step_len, int y_step_len)
{
	Point next_point;
	switch (a)
	{
	case action::up:
		next_point = Point(start.get_X(), start.get_Y() - y_step_len);
		break;
	case action::down:
		next_point = Point(start.get_X(), start.get_Y() + y_step_len);
		break;
	case action::left:
		next_point = Point(start.get_X() - x_step_len, start.get_Y());
		break;
	case action::right:
		next_point = Point(start.get_X() + x_step_len, start.get_Y());
		break;
	default:
		next_point = Point();
	}
	return next_point;
}

list<pair<Point, Point>> MDPmodel::init_all_states(const list<Point> &point_list)
{
	list<pair<Point, Point>> pair_list;

	for (auto &pl1 : point_list) {
		for (auto &pl2 : point_list) {
			pair_list.push_back(make_pair(pl1, pl2));
		}
	}

	return pair_list;
}

#pragma endregion


bool MDPmodel::is_wall(const pair<Point, Point>& p)
{
	return p.first == this->obstacle.first && p.second == this->obstacle.second;
}

bool MDPmodel::is_terminal(const pair<Point, Point> & p)
{
	if (p.first == p.second)
		return true;
	return false;
}


#pragma region mdp
void MDPmodel::init_transition_probs_and_rewards(const list<pair<Point, Point>> &pair_list)
{
	double prob = 1.0;
	double reward = 0.0;

	this->obstacle = make_pair(Point(-win_size.block_width, -win_size.block_height), Point(0, 0));
	action a[] = { action::up, action::down, action::left, action::right };

	for (auto &p1 : pair_list) {
		for (int i = 0; i < 4; ++i) {
			Point p = get_next_Point(p1.first, a[i], win_size.block_width, win_size.block_height);
			pair<Point, Point> pair = make_pair(p, p1.second);
			reward = 0.0;

			if (find(this->obstacles.begin(), this->obstacles.end(), p) != this->obstacles.end()) {
				reward = -1.0;
				this->transition_probs[p1][a[i]][obstacle] = prob;
				this->rewards[p1][a[i]][obstacle] = reward;
			}
			else if (find(pair_list.begin(), pair_list.end(), pair) != pair_list.end()) {
				if (is_terminal(pair)) {
					reward = 1.0;
				}
				this->transition_probs[p1][a[i]][pair] = prob;
				this->rewards[p1][a[i]][pair] = reward;
			}
		}
	}
}
/*
list<pair<Point, Point>> MDPmodel::get_all_states()
{
	list<pair<Point, Point>> temp;
	for (auto const& imap : this->transition_probs) {
		temp.push_back(imap.first);
	}
	return temp;
}

list<action> MDPmodel::get_possible_actions(const pair<Point, Point> & p)
{
	list<action> temp;
	auto state = transition_probs[p];
	for (auto const& mapi : state) {
		temp.push_back(mapi.first);
	}
	return temp;
}

map<pair<Point, Point>, double> MDPmodel::get_next_states(const pair<Point, Point> & p, const action &a)
{
	try {
		this->transition_probs.at(p).at(a);
	}
	catch (const out_of_range &oor) {
		//TODO: tutaj coœ informuj¹cego, ¿e nastepny krok to wejœcie na scianê
		return map<pair<Point, Point>, double>(); // empty
	}
	return this->transition_probs.at(p).at(a);
}*/

double MDPmodel::get_reward(const pair<Point, Point> & p1, const action &a, const pair<Point, Point> & p2)
{
	if (is_terminal(p1)) {
		return 1.0;
	}
	else if(is_wall(p1)){
		return -1.0;
	}
	else {
		return 0.0;
	}
}
/*
double MDPmodel::get_transition_prob(const pair<Point, Point> & p1, const action &a, const pair<Point, Point> & p2)
{
	try {
		this->transition_probs.at(p1).at(a).at(p2);
	}
	catch (const out_of_range &oor) {
		return INT_MIN;
	}
	return this->transition_probs[p1][a][p2];
}
*/
step_details<pair<Point, Point>> MDPmodel::step(const action &a, const pair<Point, Point> &state)
{
	step_details<pair<Point, Point>> sd;
	this->current_state = state;
	auto next_states = get_next_states(this->current_state, a);
	auto it = next_states.begin();
	advance(it, rand() % next_states.size());

	sd.next_state = it->first;
	sd.reward = get_reward(this->current_state, a, sd.next_state);
	sd.is_done = is_terminal(sd.next_state);
	this->current_state = sd.next_state;
	return sd;
}
#pragma endregion