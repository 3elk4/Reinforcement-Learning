#include "Environment.h"


Environment::Environment()
{
}

Environment::~Environment()
{
}

map<Point, env_type> Environment::get_environment()
{
	return this->environment;
}

void Environment::set_environemnt_element(const Point & s, const env_type & et)
{
	this->environment[s] = et;
}

vector<Point> Environment::get_environment_elements(const env_type & et)
{
	vector<Point> elements;
	for_each(this->environment.begin(), this->environment.end(),
		[&elements, et](const pair<Point, env_type> kv) {if (kv.second == et) elements.push_back(kv.first); });
	return elements;
}

list<pair<Point, Point>> Environment::init_all_states(const vector<Point>& point_list) // only paths
{
	list<pair<Point, Point>> pair_list;

	for (auto &pl1 : point_list) {
		for (auto &pl2 : point_list) {
			pair_list.push_back(make_pair(pl1, pl2));
		}
	}

	return pair_list;
}

Point Environment::get_next_Point(Point start, action a, int x_step_len, int y_step_len)
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

void Environment::init_transition_probs_and_rewards(const list<pair<Point, Point>>& pair_list)
{
	double prob = 1.0;
	double reward = 0.0;

	this->obstacle = make_pair(Point(-win_size.block_width, -win_size.block_height), Point(0, 0));
	vector<Point> obstacles = this->get_environment_elements(env_type::wall);
	action a[] = { action::up, action::down, action::left, action::right };

	for (auto &p1 : pair_list) {
		for (int i = 0; i < 4; ++i) {
			Point p = get_next_Point(p1.first, a[i], win_size.block_width, win_size.block_height);
			pair<Point, Point> pair = make_pair(p, p1.second);
			reward = 0.0;

			if (find(obstacles.begin(), obstacles.end(), p) != obstacles.end()) {
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

step_details<pair<Point, Point>> Environment::step(const action & a, const pair<Point, Point>& state)
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

bool Environment::is_wall(const pair<Point, Point>& p)
{
	return p.first == this->obstacle.first && p.second == this->obstacle.second;
}

bool Environment::is_terminal(const pair<Point, Point>& p)
{
	return p.first == p.second;
}

/*double Environment::get_reward(const pair<Point, Point>& p1, const action & a, const pair<Point, Point>& p2)
{
	if (is_wall(p2)) {
		return -1.0;
	}
	else if (is_terminal(p2)) {
		return 1.0;
	}
	return 0.0;
}*/

pair<Point, Point> Environment::reset()
{
	current_state = initial_state;
	return current_state;
}
