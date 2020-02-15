#include "Feature.h"

//------------
//FOOD FEATURE
//------------
double Food_Feature::get_feature_value(const pair<Point, Point>& s, const action&a)
{
	auto temp = get_next_state(s, a);
	return abs(temp.first.get_X() - temp.second.get_X()) + abs(temp.first.get_Y() - temp.second.get_Y());
}
pair<Point, Point> Food_Feature::get_next_state(const pair<Point, Point>& s, const action & a)
{
	pair<Point, Point> temp;
	Point next_state;
	auto current_state = s.first;
	switch (a) {
	case action::up:
		next_state = Point(current_state.get_X(), current_state.get_Y() - win_size.block_height);
		break;
	case action::right:
		next_state = Point(current_state.get_X() + win_size.block_width, current_state.get_Y());
		break;
	case action::down:
		next_state = Point(current_state.get_X(), current_state.get_Y() + win_size.block_height);
		break;
	case action::left:
		next_state = Point(current_state.get_X() - win_size.block_width, current_state.get_Y());
		break;
	}
	temp = make_pair(next_state, s.second);
	return temp;
}

//----------------
//MIN WALL FEATURE
//----------------
double Min_Wall_Feature::get_feature_value(const pair<Point, Point>& s, const action&a)
{
	double min_value = (win_size.height + win_size.width) * 2;
	double temp = 0.0;
	auto p = get_next_state(s, a).first;
	vector<Point> relatable_walls = get_relatable_walls(p);
	for (auto &rw : relatable_walls) {
		temp = distance(p, rw);
		if (temp < min_value) min_value = temp;
	}
	return min_value;
}
pair<Point, Point> Min_Wall_Feature::get_next_state(const pair<Point, Point>& s, const action & a)
{
	pair<Point, Point> temp;
	Point next_state;
	auto current_state = s.first;
	switch (a) {
	case action::up:
		next_state = Point(current_state.get_X(), current_state.get_Y() - win_size.block_height);
		break;
	case action::right:
		next_state = Point(current_state.get_X() + win_size.block_width, current_state.get_Y());
		break;
	case action::down:
		next_state = Point(current_state.get_X(), current_state.get_Y() + win_size.block_height);
		break;
	case action::left:
		next_state = Point(current_state.get_X() - win_size.block_width, current_state.get_Y());
		break;
	}
	temp = make_pair(next_state, s.second);
	return temp;
}
vector<Point> Min_Wall_Feature::get_relatable_walls(Point & p)
{
	vector<Point> relatables = {
		Point(p.get_X(), 0), //up
		Point(win_size.width - win_size.block_width, p.get_Y()), //right
		Point(0, p.get_Y()), //left
		Point(p.get_X(), win_size.height - win_size.block_height) //down
	};

	return relatables;
}
double Min_Wall_Feature::distance(Point & p1, Point & p2)
{
	return abs(p1.get_X() - p2.get_X()) + abs(p1.get_Y() - p2.get_Y());
}


//----------------
//ONE WALL FEATURE
//----------------
double One_Wall_Feature::get_feature_value(const pair<Point, Point>& s, const action & a)
{
	auto p = get_next_state(s, a).first;
	Point temp;
	switch (this->feature_name) {
	case feature_names::down_wall_feature:
		temp = Point(p.get_X(), win_size.height - win_size.block_height);
		break;
	case feature_names::up_wall_feature:
		temp = Point(p.get_X(), 0);
		break;
	case feature_names::left_wall_feature:
		temp = Point(0, p.get_Y());
		break;
	case feature_names::right_wall_feature:
		temp = Point(win_size.width - win_size.block_width, p.get_Y());
		break;
	}
	return distance(p, temp);
}
pair<Point, Point> One_Wall_Feature::get_next_state(const pair<Point, Point>& s, const action & a)
{
	pair<Point, Point> temp;
	Point next_state;
	auto current_state = s.first;
	switch (a) {
	case action::up:
		next_state = Point(current_state.get_X(), current_state.get_Y() - win_size.block_height);
		break;
	case action::right:
		next_state = Point(current_state.get_X() + win_size.block_width, current_state.get_Y());
		break;
	case action::down:
		next_state = Point(current_state.get_X(), current_state.get_Y() + win_size.block_height);
		break;
	case action::left:
		next_state = Point(current_state.get_X() - win_size.block_width, current_state.get_Y());
		break;
	}
	temp = make_pair(next_state, s.second);
	return temp;
}
double One_Wall_Feature::distance(Point & p1, Point & p2)
{
	return abs(p1.get_X() - p2.get_X()) + abs(p1.get_Y() - p2.get_Y());
}
