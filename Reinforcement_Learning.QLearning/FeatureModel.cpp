#include "FeatureModel.h"

void SimpleFeatureModel::set_environment_elements_to_features(vector<Point> &elements, const env_type &et)
{
	switch (et) {
	case env_type::wall:
		for (auto &f : this->features) {
			if (f->get_feature_name() == feature_names::is_wall_feature) {
				((Is_Wall_Feature *)f)->set_walls(elements);
			}
			else if(f->get_feature_name() == feature_names::min_wall_feature) {
				((Min_Wall_Feature *)f)->set_walls(elements);
			}
		}
		break;
	default:
		break;
	}
}

map<feature_names, double> SimpleFeatureModel::get_feature_values(const pair<Point, Point>& s, const action&a)
{
	map<feature_names, double> values;
	for (auto &f : this->features) {
		values[f->get_feature_name()] = f->get_feature_value(s, a);
	}
	return values;
}
