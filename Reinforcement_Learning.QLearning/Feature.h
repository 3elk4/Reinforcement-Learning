#pragma once
#include "Constants.h"
#include "Point.h"
#include <vector>
#include <algorithm>

using namespace std;

enum class feature_names {
	min_wall_feature = 0,
	left_wall_feature,
	right_wall_feature,
	up_wall_feature,
	down_wall_feature,
	food_feature
};

template<class S, class A>
class Feature
{
public:
	Feature(const feature_names &fn) {
		this->feature_name = fn;
	}
	~Feature() {}

	virtual double get_feature_value(const S& s, const A& a) {
		return 0.0;
	}
	feature_names get_feature_name() { return this->feature_name; }
	virtual S get_next_state(const S& s, const A& a) {
		return s;
	}
protected:
	feature_names feature_name;
};

class Food_Feature : public Feature<pair<Point, Point>, action>{
public:
	Food_Feature() : Feature(feature_names::food_feature) {}
	~Food_Feature() {}

	double get_feature_value(const pair<Point, Point>&s, const action&a) override;
	pair<Point, Point> get_next_state(const pair<Point, Point>& s, const action& a) override;
private:
};

class Min_Wall_Feature : public Feature<pair<Point, Point>, action> {
public:
	Min_Wall_Feature() : Feature(feature_names::min_wall_feature) {}
	~Min_Wall_Feature() {}

	double get_feature_value(const pair<Point, Point>&s, const action&a) override;
	pair<Point, Point> get_next_state(const pair<Point, Point>& s, const action& a) override;
	vector<Point> get_relatable_walls(Point&p);
	double distance(Point &p1, Point &p2);

private: 
};

class One_Wall_Feature : public Feature<pair<Point, Point>, action> {
public:
	One_Wall_Feature(const feature_names&fn) : Feature(fn) {}
	~One_Wall_Feature() {}

	double get_feature_value(const pair<Point, Point>&s, const action&a) override;
	pair<Point, Point> get_next_state(const pair<Point, Point>& s, const action& a) override;
	double distance(Point &p1, Point &p2);

private:
};
