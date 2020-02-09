#pragma once
#include <iostream>
#include "Constants.h"
#include <map>
#include <list>

using namespace std;

template<class S>
struct step_details {
	S next_state;
	double reward;
	bool is_done;
};

template<class S, class A>
class AbstractMDP
{
public:
	AbstractMDP() {
		n_states = 0;
	}
	virtual void init_transition_probs_and_rewards(const list<S> &) = 0;
	virtual step_details<S> step(const A &, const S &) = 0;

	list<S> get_all_states() {
		list<S> temp;
		for (auto const& imap : this->transition_probs) {
			temp.push_back(imap.first);
		}
		return temp;
	}

	list<A> get_possible_actions(const S &p) {
		list<A> temp;
		auto state = transition_probs[p];
		for (auto const& mapi : state) {
			temp.push_back(mapi.first);
		}
		return temp;
	}

	map<S, double> get_next_states(const S &s, const A &a) {
		if (this->transition_probs.count(s) > 0 && this->transition_probs.at(s).count(a) > 0) {
			return this->transition_probs.at(s).at(a);
		}
		return map<S, double>(); // empty
	}

	double get_transition_prob(const S &s1, const A &a, const S &s2) {
		if (this->transition_probs.count(s1) > 0 && this->transition_probs.at(s1).count(a) > 0 && this->transition_probs.at(s1).at(a).count(s2) > 0) {
			return this->transition_probs.at(s1).at(a).at(s2);
		}
		return INT_MIN;
	}

	virtual double get_reward(const S &s1, const A &a, const S &s2) {
		if (this->rewards.count(s1) > 0 && this->rewards.at(s1).count(a) > 0 && this->rewards.at(s1).at(a).count(s2) > 0) {
			return this->rewards.at(s1).at(a).at(s2);
		}
		return INT_MIN;
	}

	void set_initial_state(const S &s) {
		this->initial_state = s;
		this->current_state = this->initial_state;
		this->n_states = this->transition_probs.size();
	}

protected:
	map<S, map<A, map<S, double>>> transition_probs;
	map<S, map<A, map<S, double>>> rewards;
	S current_state;
	S initial_state;
	int n_states;
};

