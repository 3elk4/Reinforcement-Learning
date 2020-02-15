#include "FeatureModel.h"

map<feature_names, double> SimpleFeatureModel::get_feature_values(const pair<Point, Point>& s, const action&a)
{
	map<feature_names, double> values;
	for (auto &f : this->features) {
		values[f.get_feature_name()] = f.get_feature_value(s, a);
	}
	return values;
}
