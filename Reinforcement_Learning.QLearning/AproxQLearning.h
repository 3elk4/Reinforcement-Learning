#pragma once
#include <map>
#include <list>
#include <array>
#include <optional>
#include <algorithm> 
#include <utility>
#include <stdio.h>
#include "FeatureModel.h"

using namespace std;

template<class S, class A>
class AproxQLearning
{
public:
	AproxQLearning() {}

	AproxQLearning(const FeatureModel<S, A> &fm, const list<A> &actions) {
		this->alpha = 0.5; 
		this->gamma = 0.99; 
		this->feature_model = fm;
		this->possible_actions = actions;
	}

	AproxQLearning(const double alpha, const double gamma, const FeatureModel<S, A> &fm, const list<A> &actions) {
		this->alpha = alpha;
		this->gamma = gamma;
		this->feature_model = fm;
		this->possible_actions = actions;
	}

	~AproxQLearning() {}

	void reset_parameters() {
		this->alpha = 0.0;
		this->gamma = 0.0;
	}

	void set_parameters(double alpha, double gamma) {
		this->alpha = alpha;
		this->gamma = gamma;
	}

	void set_possible_actions(const list<A> &actions) {
		this->possible_actions = actions;
	}

	//weight_table
	map<A, map<feature_names, double>> get_weight_table() {
		return this->weight_table;
	}

	void init_weight_table() {
		for (auto &a : this->possible_actions) {
			for (auto &f : this->feature_model.get_features()) {
				this->weight_table[a][f.get_feature_name()] = 1.0;
			}
		}
	}

	map<feature_names, double> get_weight_table_values(const A& a) { //extract features 
		return this->weight_table.at(a);
	}
	void set_weight_table_values(const A& a, const map<feature_names, double>&f) {
		this->weight_table[a] = f;
	}

	double compute_new_qvalue(const S &s, const A &a) {
		auto feature_values = this->feature_model.get_feature_values(s, a);
		double value = 0.0;
		for (auto &f : feature_values) {
			value += this->weight_table.at(a).at(f.first) * f.second;
		}
		return value;
	}

	double get_max_qvalue(const S &s) {
		double max_value = 0.0, temp = 0.0;
		for (auto & pa : this->possible_actions) {
			temp = compute_new_qvalue(s, pa);
			if (temp > max_value) max_value = temp;
		}
		return max_value;
	}
	
	void update(const S &s1, const A &a, const double &reward, const S&s2) {
		double value = 0.0;
		auto feature_values = this->feature_model.get_feature_values(s1, a);
		auto diffrence = (reward + this->gamma * get_max_qvalue(s2)) - compute_new_qvalue(s1, a);
		map<feature_names, double> new_values;
		for (auto &f : feature_values) {
			new_values[f.first] = this->weight_table.at(a).at(f.first) + this->alpha * diffrence * f.second;
		}
		set_weight_table_values(a, new_values);
	}

	optional<A> get_best_action(const S &s) { 
		//oblicz wartosci dla s¹siadów
		double max_value = get_max_qvalue(s);
		list<A> rand_action;
		for (auto &a : this->possible_actions) {
			if (compute_new_qvalue(s, a) == max_value) {
				rand_action.push_back(a);
			}
		}

		auto it = rand_action.begin();
		advance(it, rand() % rand_action.size());
		return *it;
	}
	
private:
	double alpha;
	double gamma;

	list<A> possible_actions;
	FeatureModel<S, A> feature_model;
	map<A, map<feature_names, double>> weight_table;
};