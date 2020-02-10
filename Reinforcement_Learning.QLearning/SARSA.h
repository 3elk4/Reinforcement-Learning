#pragma once
#include "Point.h"
#include <map>
#include <list>
#include "Constants.h"
#include "QLearningAgent.h"

using namespace std;

class SARSA : public QLearningAgent<pair<Point, Point>, action>
{
public:
	SARSA() : QLearningAgent() {}
	SARSA(double alpha, double epsilon, double gamma) : QLearningAgent(alpha, epsilon, gamma) {}
	SARSA(const QLearningAgent<pair<Point, Point>, action> & qla) : QLearningAgent(qla) {}
	~SARSA() {};

	double get_value(const pair<Point, Point>& s) override;
		
	optional<action> get_next_action(const pair<Point, Point>& s);
	void set_next_action(action &a);
	void reset_next_action();

private:
	optional<action> next_action = nullopt;
};

