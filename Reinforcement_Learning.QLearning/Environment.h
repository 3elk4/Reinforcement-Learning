#pragma once
#include <iostream>
#include <utility>
#include <map>
#include <vector>
#include <algorithm>
#include "Constants.h"
#include "AbstractMDP.h"
#include "Point.h"
#include <armadillo>

using namespace std;

class Environment : public AbstractMDP<pair<Point, Point>, action>
{
public:
	Environment();
	~Environment();

	//environment - drawing
	map<Point, env_type> get_environment();
	void set_environemnt_element(const Point & s, const env_type & et);
	vector<Point> get_environment_elements(const env_type & et);

	//MDP
	list<pair<Point, Point>> init_all_states(const vector<Point> &point_list);
	Point get_next_Point(Point start, action a, int x_step_len, int y_step_len);
	void init_transition_probs_and_rewards(const list<pair<Point, Point>> &pair_list) override;

	step_details<pair<Point, Point>> step(const action &a, const pair<Point, Point> &state) override;

	bool is_wall(const pair<Point, Point> &p);
	bool is_terminal(const pair<Point, Point> &p);
	double get_reward(const pair<Point, Point> &p1, const action &a, const pair<Point, Point> &p2) override;
	pair<Point, Point> reset();
	void change_reward_position();

	void clear_environment();

	void set_current_snapshot(Point &head, Point &food);
	arma::mat get_current_snapshot();
	double get_distance(const pair<Point, Point> &p);

private:
	pair<Point, Point> obstacle;
	map<Point, env_type> environment;
	map<Point, env_type> current_snapshot;
	vector<double> features;
};

