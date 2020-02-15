#pragma once
#include "Constants.h"
#include "Point.h"
#include "Feature.h"
#include <list>
#include <map>

using namespace std;

template<class S, class A>
class FeatureModel
{
public:
	FeatureModel() {}
	FeatureModel(const list<Feature<S, A>>& f) { this->features = f; }
	~FeatureModel() {}

	virtual map<feature_names, double> get_feature_values(const S&s, const A&a) {
		return map<feature_names, double>();
	}
	list<Feature<S, A>> get_features() {
		return this->features;
	}
protected:
	list<Feature<S, A>> features;
};

class SimpleFeatureModel : public FeatureModel<pair<Point, Point>, action> {
public:
	SimpleFeatureModel(const list<Feature<pair<Point, Point>, action>>& f) : FeatureModel(f) {}
	~SimpleFeatureModel() {}
	map<feature_names, double> get_feature_values(const pair<Point, Point>&s, const action&a) override;
};