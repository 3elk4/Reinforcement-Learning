#include "Feature.h"
pair<Point, Point> get_next_state(const pair<Point, Point>& s, const action & a)
{
	pair<Point, Point> temp;
	Point next_state;
	auto current_state = s.first;
	switch (a) {
	case action::up:
		next_state = Point(current_state.get_X(), current_state.get_Y() - window_size::get_instance().get_block_height());
		break;
	case action::right:
		next_state = Point(current_state.get_X() + window_size::get_instance().get_block_width(), current_state.get_Y());
		break;
	case action::down:
		next_state = Point(current_state.get_X(), current_state.get_Y() + window_size::get_instance().get_block_height());
		break;
	case action::left:
		next_state = Point(current_state.get_X() - window_size::get_instance().get_block_width(), current_state.get_Y());
		break;
	}
	temp = make_pair(next_state, s.second);
	return temp;
}
double distance(Point & p1, Point & p2)
{
	return ((double)abs(p1.get_X() - p2.get_X()) + abs(p1.get_Y() - p2.get_Y()));
}

//------------
//FOOD FEATURE
//------------
double Food_Feature::get_feature_value(const pair<Point, Point>& s, const action&a)
{
	auto temp = get_next_state(s, a);
	return distance(temp.first, temp.second) / (window_size::get_instance().get_height() * window_size::get_instance().get_width());
}

//------------
//IS_WALL_FEATURE
//------------
double Is_Wall_Feature::get_feature_value(const pair<Point, Point>& s, const action & a)
{
	auto next_state = get_next_state(s, a);
	for (auto &w : this->walls) {
		if (next_state.first == w) {
			return 1.0;
		}
	}
	return 0.0;
}
void Is_Wall_Feature::set_walls(vector<Point>& walls)
{
	this->walls = walls;
}

//------------
//IS FOOD FEATURE
//------------
double Is_Food_Feature::get_feature_value(const pair<Point, Point>& s, const action & a)
{
	auto next_state = get_next_state(s, a);
	return (next_state.first == next_state.second) ? 1.0 : 0.0;
}

//----------------
//MIN WALL FEATURE
//----------------
double Min_Wall_Feature::get_feature_value(const pair<Point, Point>& s, const action&a)
{
	double min_value = (window_size::get_instance().get_height() * window_size::get_instance().get_width());
	double temp = 0.0;
	auto p = get_next_state(s, a).first;
	for (auto &rw : this->walls) {
		temp = distance(p, rw);
		if (temp < min_value) min_value = temp;
	}
	return min_value / (window_size::get_instance().get_height() * window_size::get_instance().get_width());
}
void Min_Wall_Feature::set_walls(vector<Point>& walls)
{
	this->walls = walls;
}




