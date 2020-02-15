#include "Reinforcement_LearningQLearning.h"
#include <iostream>
#include <algorithm>

using namespace std;

void Reinforcement_LearningQLearning::loopSARSA()
{
	auto is_done = do_step_SARSA(-1);
	if (is_done) {
		this->headAndFood = this->environment.reset();
		this->agentSARSA.reset_next_action();
	}
	update();
}

void Reinforcement_LearningQLearning::loopapproxqlearning()
{
	auto is_done = do_step_approxqlearning(-1);
	if (is_done) {
		this->headAndFood = this->environment.reset();
	}
	update();
}

void Reinforcement_LearningQLearning::loopqlearning() {
	auto is_done = do_step_qlearning(-1);
	if (is_done) {
		this->headAndFood = this->environment.reset();
	}
	update();
}

void Reinforcement_LearningQLearning::on_envbutton_clicked()
{
	if (!this->environment.get_environment().empty() && ui.startstopbutton->text().toStdString() == "STOP") return;
	ui.statusBar->showMessage(p_states.at(program_state::init_env));
	string path = fd.getOpenFileName().toStdString();
	if (path.empty()) {
		ui.statusBar->showMessage(p_states.at(program_state::none));
		return;
	}
	//environment
	//walls, paths, food and head

	//clear
	this->environment.clear_environment();
	init_environment(path);

	//mdpmodel set
	auto states = this->environment.init_all_states(environment.get_environment_elements(env_type::path));
	this->environment.init_transition_probs_and_rewards(states);
	this->environment.set_initial_state(this->headAndFood);

	list<action> curr_actions;
	for (auto & a : this->actions) curr_actions.push_back(a.first);
	//QLEARNING
	this->agent.init_qvalues(states, curr_actions);
	//SARSA
	this->agentSARSA.init_qvalues(states, curr_actions);

	repaint();
	ui.statusBar->showMessage(p_states.at(program_state::none));
}

void Reinforcement_LearningQLearning::on_startstopbutton_clicked()
{
	if (ui.startstopbutton->text().toStdString() == "STOP" && this->environment.get_all_states().size() != 0) {
		ui.statusBar->showMessage(p_states.at(program_state::mode_choosing));
		ui.qlearning->setEnabled(true);
		ui.sarsa->setEnabled(true);
		ui.aproxqlearning->setEnabled(true);

		this->timer->stop();
		this->timerSARSA->stop();
		this->timerapproxqlerning->stop();

		ui.startstopbutton->setText("START");
	}
	else {
		ui.statusBar->showMessage(p_states.at(program_state::none));
		ui.qlearning->setEnabled(false);
		ui.sarsa->setEnabled(false);
		ui.aproxqlearning->setEnabled(false);

		ui.statusBar->showMessage(p_states.at(program_state::setting_parameters));
		pardialog.exec();
		ui.episodes_num->setText(QString::number(pardialog.get_episodes()));
		ui.alpha_num->setText(QString::number(pardialog.get_alpha()));
		ui.gamma_num->setText(QString::number(pardialog.get_gamma()));
		ui.epsilon_num->setText(QString::number(pardialog.get_epsilon()));
		set_random_episodes_to_show(pardialog.get_episodes_number(), pardialog.get_episodes());

		ui.statusBar->showMessage(p_states.at(program_state::train_and_play));
		if (mode == rl_mode::qlearning) {
			this->agent.set_parameters(pardialog.get_alpha(), pardialog.get_epsilon(), pardialog.get_gamma());
			double result = play_and_train_qlearning(pardialog.get_episodes());
			cout << "DONE, TOTAL RESULT: " << result << endl;

			this->agent.reset_parameters();
			this->current_agent = this->agent;

			ui.statusBar->showMessage(p_states.at(program_state::qlearning));
			this->timer->start(100);
			this->timerSARSA->stop();
			this->timerapproxqlerning->stop();
		}
		else if (mode == rl_mode::sarsa) {
			this->agentSARSA.set_parameters(pardialog.get_alpha(), pardialog.get_epsilon(), pardialog.get_gamma());
			double result = play_and_train_SARSA(pardialog.get_episodes());
			cout << "DONE, TOTAL RESULT: " << result << endl;
			this->agentSARSA.reset_parameters();
			this->current_agent = this->agentSARSA;

			ui.statusBar->showMessage(p_states.at(program_state::sarsa));
			this->timerSARSA->start(100);
			this->timer->stop();
			this->timerapproxqlerning->stop();
		}
		else if (mode == rl_mode::approx) {
			this->approxAgent.set_parameters(pardialog.get_alpha(), pardialog.get_gamma());
			double result = play_and_train_approxqlearning(pardialog.get_episodes());
			cout << "DONE, TOTAL RESULT: " << result << endl;
			this->approxAgent.reset_parameters();

			ui.statusBar->showMessage(p_states.at(program_state::approx));
			this->timerapproxqlerning->start(100);
			this->timer->stop();
			this->timerSARSA->stop();
		}
		ui.startstopbutton->setText("STOP");
	}
}

void Reinforcement_LearningQLearning::set_random_episodes_to_show(int counter, int episodes) {
	int value = 0;
	for (int i = 0; i < counter; ++i) {
		while (true) {
			value = rand() % episodes;
			if (std::find(this->episodes_to_show.begin(), this->episodes_to_show.end(), value) == this->episodes_to_show.end()){
				this->episodes_to_show.push_back(value);
				break;
			}
		}
	}
}


void Reinforcement_LearningQLearning::on_qlearning_clicked()
{
	mode = rl_mode::qlearning;
}

void Reinforcement_LearningQLearning::on_sarsa_clicked()
{
	mode = rl_mode::sarsa;
}

void Reinforcement_LearningQLearning::on_aproxqlearning_clicked() {
	mode = rl_mode::approx;
}

