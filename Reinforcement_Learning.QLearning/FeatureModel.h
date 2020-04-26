#pragma once
#include "Constants.h"
#include "Point.h"
#include "Feature.h"
#include <list>
#include <map>

using namespace std;

/*template<class S, class A>*/
/*class FeatureModel
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
};*/

class SimpleFeatureModel{
public:
	SimpleFeatureModel() { this->features = { new Food_Feature(),
												new Is_Wall_Feature(),
												new Is_Food_Feature(),
												new Min_Wall_Feature() }; } 
	SimpleFeatureModel(const list<Feature*> &f) { this->features = f; }
	~SimpleFeatureModel() { }
	void set_environment_elements_to_features(vector<Point> &elements, const env_type &et);
	map<feature_names, double> get_feature_values(const pair<Point, Point>&s, const action&a);

	list<Feature*> get_features() {
		return this->features;
	}

private:
	list<Feature*> features;

	//Food_Feature ff;
	//Min_Wall_Feature mwf;
};