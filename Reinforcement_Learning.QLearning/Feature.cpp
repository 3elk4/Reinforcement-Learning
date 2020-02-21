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
	auto relatables = get_relatable_walls(next_state.first);
	for (auto &r : relatables) {
		if (next_state.first == r) {
			return 1.0;
		}
	}
	return 0.0;
}
vector<Point> Is_Wall_Feature::get_relatable_walls(Point & p)
{
	vector<Point> relatables = {
		Point(p.get_X(), 0), //up
		Point(window_size::get_instance().get_width() - window_size::get_instance().get_block_width(), p.get_Y()), //right
		Point(0, p.get_Y()), //left
		Point(p.get_X(), window_size::get_instance().get_height() - window_size::get_instance().get_block_height()) //down
	};

	return relatables;
}

//------------
//IS FOOD FEATURE
//------------
double Is_Food_Feature::get_feature_value(const pair<Point, Point>& s, const action & a)
{
	auto next_state = get_next_state(s, a);
	return (next_state.first == next_state.second) ? 1.0 : 0.0;
}
double Is_Food_Up_Feature::get_feature_value(const pair<Point, Point>& s, const action & a)
{
	auto next_state = get_next_state(s, a);
	return next_state.first.get_Y() >= next_state.second.get_Y() ? 1.0 : 0.0;
}
double Is_Food_Down_Feature::get_feature_value(const pair<Point, Point>& s, const action & a)
{
	auto next_state = get_next_state(s, a);
	return next_state.first.get_Y() <= next_state.second.get_Y() ? 1.0 : 0.0;
}
double Is_Food_Left_Feature::get_feature_value(const pair<Point, Point>& s, const action & a)
{
	auto next_state = get_next_state(s, a);
	return next_state.first.get_X() >= next_state.second.get_X() ? 1.0 : 0.0;
}
double Is_Food_Right_Feature::get_feature_value(const pair<Point, Point>& s, const action & a)
{
	auto next_state = get_next_state(s, a);
	return next_state.first.get_X() <= next_state.second.get_X() ? 1.0 : 0.0;
}

//----------------
//MIN WALL FEATURE
//----------------
double Min_Wall_Feature::get_feature_value(const pair<Point, Point>& s, const action&a)
{
	double min_value = (window_size::get_instance().get_height() * window_size::get_instance().get_width());
	double temp = 0.0;
	auto p = get_next_state(s, a).first;
	vector<Point> relatable_walls = get_relatable_walls(p);
	for (auto &rw : relatable_walls) {
		temp = distance(p, rw);
		if (temp < min_value) min_value = temp;
	}
	return min_value / (window_size::get_instance().get_height() * window_size::get_instance().get_width());
}
vector<Point> Min_Wall_Feature::get_relatable_walls(Point & p)
{
	vector<Point> relatables = {
		Point(p.get_X(), 0), //up
		Point(window_size::get_instance().get_width() - window_size::get_instance().get_block_width(), p.get_Y()), //right
		Point(0, p.get_Y()), //left
		Point(p.get_X(), window_size::get_instance().get_height() - window_size::get_instance().get_block_height()) //down
	};

	return relatables;
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
		temp = Point(p.get_X(), window_size::get_instance().get_height() - window_size::get_instance().get_block_height());
		break;
	case feature_names::up_wall_feature:
		temp = Point(p.get_X(), 0);
		break;
	case feature_names::left_wall_feature:
		temp = Point(0, p.get_Y());
		break;
	case feature_names::right_wall_feature:
		temp = Point(window_size::get_instance().get_width() - window_size::get_instance().get_block_width(), p.get_Y());
		break;
	}
	return distance(p, temp);
}


