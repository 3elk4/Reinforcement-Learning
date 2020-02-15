#include "Reinforcement_LearningQLearning.h"
#include <qstring.h>

//
//double Reinforcement_LearningQLearning::play_and_train_qlearning(const int & episodes)
//{
//	/*
//		This function should
//		- run a full game, actions given by agent's e-greedy policy
//		- train agent using agent.update(...) whenever it is possible
//		- return total reward
//	*/
//
//	double total_reward = 0.0;
//	optional<action> a;
//	step_details<pair<Point, Point>> sd;
//
//	for (int i = 0; i < episodes; ++i) {
//		this->headAndFood = this->environment.reset();
//		show_episode(i, this->episodes_to_show);
//		while (true) {
//			//get agent to pick action given state s.
//			a = this->agent.get_action(this->headAndFood);
//			if (!a.has_value()) break;
//			sd = this->environment.step(a.value(), this->headAndFood);
//
//			//train(update) agent for state s
//			this->agent.update(this->headAndFood, a.value(), sd.reward, sd.next_state);
//			this->headAndFood = sd.next_state;
//			show_episode(i, this->episodes_to_show);
//			total_reward += sd.reward;
//			if (sd.is_done) break;
//		}
//		cout << "EPISODE: " << i + 1 << ", SCORE: " << total_reward << endl;
//
//	}
//	this->headAndFood = this->environment.reset();
//	print_table(this->agent);
//	return total_reward;
//}
//
//double Reinforcement_LearningQLearning::play_and_train_SARSA(const int & episodes)
//{
//	/*
//		This function should
//		- run a full game, actions given by agent's e-greedy policy
//		- train agent using agent.update(...) whenever it is possible
//		- return total reward
//	*/
//
//	double total_reward = 0.0;
//	optional<action> a;
//	step_details<pair<Point, Point>> sd;
//
//	for (int i = 0; i < episodes; ++i) {
//		this->headAndFood = this->environment.reset();
//		show_episode(i, this->episodes_to_show);
//		this->agentSARSA.reset_next_action();
//		while (true) {
//			//get agent to pick action given state s.
//			a = this->agentSARSA.get_next_action(this->headAndFood);
//			if (!a.has_value()) break;
//			sd = this->environment.step(a.value(), this->headAndFood);
//
//			//train(update) agent for state s
//			this->agentSARSA.update(this->headAndFood, a.value(), sd.reward, sd.next_state);
//			this->headAndFood = sd.next_state;
//			show_episode(i, this->episodes_to_show);
//			total_reward += sd.reward;
//			if (sd.is_done) break;
//		}
//		cout << "EPISODE: " << i + 1 << ", SCORE: " << total_reward << endl;
//	}
//	this->headAndFood = this->environment.reset();
//	print_table(this->agentSARSA);
//	return total_reward;
//}

double Reinforcement_LearningQLearning::play_and_train_qlearning(const int & episodes)
{
	/*
		This function should
		- run a full game, actions given by agent's e-greedy policy
		- train agent using agent.update(...) whenever it is possible
		- return total reward
	*/

	double total_reward = 0.0;
	//optional<action> a;
	//step_details<pair<Point, Point>> sd;

	for (int i = 0; i < episodes; ++i) {
		this->headAndFood = this->environment.reset();
		show_episode(i, this->episodes_to_show);
		while (true) {
			auto is_done = do_step_qlearning(i);
			if (is_done) break;
		}
		//cout << "EPISODE: " << i + 1 << ", SCORE: " << total_reward << endl;

	}
	this->headAndFood = this->environment.reset();
	print_table(this->agent);
	return total_reward;
}

double Reinforcement_LearningQLearning::play_and_train_SARSA(const int & episodes)
{
	/*
		This function should
		- run a full game, actions given by agent's e-greedy policy
		- train agent using agent.update(...) whenever it is possible
		- return total reward
	*/

	double total_reward = 0.0;
	optional<action> a;
	step_details<pair<Point, Point>> sd;

	for (int i = 0; i < episodes; ++i) {
		this->headAndFood = this->environment.reset();
		show_episode(i, this->episodes_to_show);
		this->agentSARSA.reset_next_action();
		while (true) {
			bool is_done = do_step_SARSA(i);
			if (is_done) break;
		}
		//cout << "EPISODE: " << i + 1 << ", SCORE: " << total_reward << endl;
	}
	this->headAndFood = this->environment.reset();
	print_table(this->agentSARSA);
	return total_reward;
}

double Reinforcement_LearningQLearning::play_and_train_approxqlearning(const int & episodes)
{
	/*
	This function should
	- run a full game, actions given by agent's e-greedy policy
	- train agent using agent.update(...) whenever it is possible
	- return total reward
*/

	double total_reward = 0.0;
	//optional<action> a;
	//step_details<pair<Point, Point>> sd;

	for (int i = 0; i < episodes; ++i) {
		this->headAndFood = this->environment.reset();
		show_episode(i, this->episodes_to_show);
		while (true) {
			auto is_done = do_step_approxqlearning(i);
			if (is_done) break;
		}
		//cout << "EPISODE: " << i + 1 << ", SCORE: " << total_reward << endl;

	}
	this->headAndFood = this->environment.reset();
	//print_table(this->agent);
	return total_reward;
}

bool Reinforcement_LearningQLearning::do_step_qlearning(int current_episode)
{
	optional<action> a;
	step_details<pair<Point, Point>> sd;

	a = this->agent.get_action(this->headAndFood);
	if (!a.has_value() || this->environment.is_terminal(this->headAndFood)) {
		return true;
	}

	sd = this->environment.step(a.value(), this->headAndFood);
	//train(update) agent for state s
	this->agent.update(this->headAndFood, a.value(), sd.reward, sd.next_state);
	this->headAndFood = sd.next_state;
	show_episode(current_episode, this->episodes_to_show);
	return false;
}

bool Reinforcement_LearningQLearning::do_step_SARSA(int current_episode)
{
	optional<action> a;
	step_details<pair<Point, Point>> sd;

	//get agent to pick action given state s.
	a = this->agentSARSA.get_next_action(this->headAndFood);
	if (!a.has_value() || this->environment.is_terminal(this->headAndFood)) return true;
	sd = this->environment.step(a.value(), this->headAndFood);

	//train(update) agent for state s
	this->agentSARSA.update(this->headAndFood, a.value(), sd.reward, sd.next_state);
	this->headAndFood = sd.next_state;
	show_episode(current_episode, this->episodes_to_show);
	return false;
}

bool Reinforcement_LearningQLearning::do_step_approxqlearning(int current_episode)
{
	optional<action> a;
	step_details<pair<Point, Point>> sd;

	a = this->approxAgent.get_best_action(this->headAndFood);
	if (this->environment.is_wall(this->headAndFood) || this->environment.is_terminal(this->headAndFood)) return true;
	sd = this->environment.step(a.value(), this->headAndFood);

	//train(update) agent for state s
	this->approxAgent.update(this->headAndFood, a.value(), sd.reward, sd.next_state);
	this->headAndFood = sd.next_state;
	show_episode(current_episode, this->episodes_to_show);
	return false;
}

void Reinforcement_LearningQLearning::show_episode(int current_episode, list<int> episodes) {
	for (auto &e : episodes) {
		if (current_episode == e) {
			ui.statusBar->showMessage(p_states.at(program_state::episode) + QString(to_string(current_episode).c_str()));
			repaint();
			this_thread::sleep_for(0.2s);
		}
	}
}
