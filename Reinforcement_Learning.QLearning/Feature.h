#pragma once
#include "Constants.h"
#include "Point.h"
#include <vector>
#include <algorithm>

using namespace std;

enum class feature_names {
	food_feature = 0, //--
	is_food_feature, //--
	is_wall_feature,//-- //todo: przerobiæ to tak, by bra³o scianê jako element 
	min_wall_feature
};

/*template<class S, class A>*/
/*
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
protected:
	feature_names feature_name;
};
*/

class Feature
{
public:
	Feature(const feature_names &fn) {
		this->feature_name = fn;
	}
	Feature(const Feature&f) {
		this->feature_name = f.feature_name;
	}
	~Feature() {}

	virtual double get_feature_value(const pair<Point, Point>&s, const action&a) {
		return 0.0;
	}
	feature_names get_feature_name() { return this->feature_name; }
protected:
	feature_names feature_name;
};


class Food_Feature : public Feature{
public:
	Food_Feature() : Feature(feature_names::food_feature) {}
	~Food_Feature() {}

	double get_feature_value(const pair<Point, Point>&s, const action&a) override;
private:
};

class Is_Wall_Feature : public Feature {
public:
	Is_Wall_Feature() : Feature(feature_names::is_wall_feature) {}
	~Is_Wall_Feature() {}

	double get_feature_value(const pair<Point, Point>&s, const action&a) override;
	void set_walls(vector<Point> &walls);
private:
	vector<Point> walls;
};

class Is_Food_Feature : public Feature {
public:
	Is_Food_Feature() : Feature(feature_names::is_food_feature) {}
	~Is_Food_Feature() {}

	double get_feature_value(const pair<Point, Point>&s, const action&a) override;
private:
};

//todo: musi byc przygotowane podczas ustawiania œrodowiska
class Min_Wall_Feature : public Feature {
public:
	Min_Wall_Feature() : Feature(feature_names::min_wall_feature) {}
	~Min_Wall_Feature() {}

	double get_feature_value(const pair<Point, Point>&s, const action&a) override;
	void set_walls(vector<Point> &walls);
private: 
	vector<Point> walls;
};
