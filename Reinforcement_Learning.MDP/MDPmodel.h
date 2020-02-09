#pragma once

#include "Point.h"
//#include "Pair.h"
#include <utility>
#include "Constants.h"
#include <map>
#include <list>
#include <vector>
#include <iostream>
#include "AbstractMDP.h"

using namespace std;

class MDPmodel : public AbstractMDP<pair<Point, Point>, action> {
public:
	MDPmodel();
	MDPmodel(pair<Point, Point> &init_st, vector<Point> obst, list<Point> environment);
	MDPmodel(MDPmodel &mdp, vector<Point> obst);
	~MDPmodel() {};

	void set_obstacles(vector<Point> obst);
	Point get_next_Point(Point start, action a, int x_step_len, int y_step_len);
	list<pair<Point, Point>> init_all_states(const list<Point> &point_list);
	
	bool is_wall(const pair<Point, Point> &p);
	bool is_terminal(const pair<Point, Point> &p);

	//MDP
	void init_transition_probs_and_rewards(const list<pair<Point, Point>> &pair_list) override;
	//list<pair<Point, Point>> get_all_states() override;
	//list<action> get_possible_actions(const pair<Point, Point> &p) override;
	//map<pair<Point, Point>, double> get_next_states(const pair<Point, Point> &p, const action &a) override;
	double get_reward(const pair<Point, Point> &p1,const action &a, const pair<Point, Point> &p2) override;
	//double get_transition_prob(const pair<Point, Point> &p1, const action &a, const pair<Point, Point> &p2) override;
	step_details<pair<Point, Point>> step(const action &a, const pair<Point, Point> &state) override;

private:
	vector<Point> obstacles;
	pair<Point, Point> obstacle;
};
