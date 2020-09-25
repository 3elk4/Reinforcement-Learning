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

	this->obstacle = make_pair(Point(-window_size::get_instance().get_block_width(), -window_size::get_instance().get_block_height()), Point(0, 0));
	vector<Point> obstacles = this->get_environment_elements(env_type::wall);
	action a[] = { action::up, action::down, action::left, action::right };

	for (auto &p1 : pair_list) {
		for (int i = 0; i < 4; ++i) {
			Point p = get_next_Point(p1.first, a[i], window_size::get_instance().get_block_width(), window_size::get_instance().get_block_height());
			pair<Point, Point> pair = make_pair(p, p1.second);
			//reward = -1.0;

			if (find(obstacles.begin(), obstacles.end(), p) != obstacles.end()) {
				reward = env_reward.wall;
				this->transition_probs[p1][a[i]][obstacle] = prob;
				this->rewards[p1][a[i]][obstacle] = reward;
			}
			else if (find(pair_list.begin(), pair_list.end(), pair) != pair_list.end()) {
				reward = env_reward.path;
				if (is_terminal(pair)) {
					reward = env_reward.food;
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

double Environment::get_reward(const pair<Point, Point>& p1, const action & a, const pair<Point, Point>& p2)
{
	if (is_wall(p2)) {
		return -1.0;
	}
	if (is_terminal(p2)) {
		return 1.0;
	}
	double old_distance = get_distance(p1) / (this->environment.size() * 100);
	double new_distance = get_distance(p2) / (this->environment.size() * 100);
	return old_distance - new_distance;
}

pair<Point, Point> Environment::reset()
{
	if (is_wall(current_state)) {
		current_state = initial_state;
	}
	else if (is_terminal(current_state)) {
		//change_reward_position();
		//initial_state = current_state;
		current_state = initial_state;
	}
	return current_state;
}

void Environment::change_reward_position()
{
	/*auto empty_states = this->get_environment_elements(env_type::path);
	auto del_it = std::find(empty_states.begin(), empty_states.end(), this->current_state.second);
	if (del_it != empty_states.end()) empty_states.erase(del_it);
	auto it = empty_states.begin();
	advance(it, rand() % empty_states.size());
	this->current_state.second = *it;
	*/
}

void Environment::clear_environment()
{
	this->environment.clear();
	this->transition_probs.clear();
	this->rewards.clear();
}

void Environment::set_current_snapshot(Point & head, Point & food)
{
	this->features.clear();
	//cout << this->environment.size() << endl;
	//cout << get_distance(make_pair(head, food)) << endl;

	double food_distance = get_distance(make_pair(head, food)) / (this->environment.size() * 100);
	/*double is_head_wall = head == this->obstacle.first ? 1.0 : 0.0;
	double is_head_food = head == food ? 1.0 : 0.0;*/
	double up_wall = head.get_Y() / 50.0;
	double right_wall = (60 - head.get_X()) / 50.0;
	double down_wall = (60 - head.get_Y()) / 50.0;
	double left_wall = head.get_X() / 50.0;

	this->features.push_back(food_distance);
	this->features.push_back(up_wall);
	this->features.push_back(right_wall);
	this->features.push_back(down_wall);
	this->features.push_back(left_wall);
	/*this->features.push_back(is_head_wall);
	this->features.push_back(is_head_food);
*/
	/*if (this->current_snapshot.empty()) {
		//this->current_snapshot[this->obstacle.first] = env_type::wall;

		auto path_elements = this->get_environment_elements(env_type::path);

		for_each(path_elements.begin(), path_elements.end(),
			[this](const Point &p) { this->current_snapshot[p] = env_type::path; });
	}
	
	for (auto & kv : this->current_snapshot) {
 		if (kv.second == env_type::head || kv.second == env_type::food) {
			kv.second = env_type::path;
		}
	}
	if (head != this->obstacle.first) {
		this->current_snapshot[head] = env_type::head;
	}
	this->current_snapshot[food] = env_type::food;*/
}

arma::mat Environment::get_current_snapshot()
{
	/*vector<double> simplified;
	const double part = 1.0 / 3.0;
	for (const auto& kv : this->current_snapshot) {
		simplified.push_back((int)kv.second * part);
	}
	return arma::mat(simplified).t();
	*/
	
	return arma::mat(this->features).t();
}

double Environment::get_distance(const pair<Point, Point> &p)
{
	return sqrt(pow(const_cast<Point&>(p.second).get_X() - const_cast<Point&>(p.first).get_X(), 2) 
		+ pow(const_cast<Point&>(p.second).get_Y() - const_cast<Point&>(p.first).get_Y(), 2));
}
