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

void Reinforcement_LearningQLearning::loopdeepqlearning()
{
	auto is_done = do_step_deepqlearning(-1);
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

	//approx - wall features
	vector<Point> walls = this->environment.get_environment_elements(env_type::wall);
	this->approxAgent.get_feature_model().set_environment_elements_to_features(walls, env_type::wall);

	vector<Layer> layers;
	this->environment.set_current_snapshot(this->headAndFood.first, this->headAndFood.second);
	cout << this->environment.get_current_snapshot() << endl;
	int input_number = this->environment.get_current_snapshot().n_elem;
	int output_number = (int)this->listOfActions.size();
	int hidden_number = sqrt(input_number) + output_number;
	layers.push_back(NeuralNetworkCreator::createLayer(make_pair(input_number, hidden_number), make_pair(-0.01, 0.01), string("RELU")));
	layers.push_back(NeuralNetworkCreator::createLayer(make_pair(hidden_number, output_number), make_pair(-0.01, 0.01), string("SOFTMAX")));
	this->neural_network = BackpropagationNeuralNetwork(layers, 0.5, 0.5, 1);
	this->dqnAgent = new DQNAgent<arma::mat, action>(this->neural_network, this->listOfActions);

	repaint();
	ui.statusBar->showMessage(p_states.at(program_state::none));
}

void Reinforcement_LearningQLearning::change_radiobuttons_status(bool status) {
	ui.qlearning->setEnabled(status);
	ui.sarsa->setEnabled(status);
	ui.aproxqlearning->setEnabled(status);
	ui.deepqlearning->setEnabled(status);
}

void Reinforcement_LearningQLearning::on_startstopbutton_clicked()
{
	if (ui.startstopbutton->text().toStdString() == "STOP" && this->environment.get_all_states().size() != 0) {
		ui.statusBar->showMessage(p_states.at(program_state::mode_choosing));
		change_radiobuttons_status(true);

		this->timer->stop();
		this->timerSARSA->stop();
		this->timerapproxqlerning->stop();

		ui.startstopbutton->setText("START");
	}
	else {
		change_radiobuttons_status(false);

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
			this->timerdeepqlearning->stop();
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
			this->timerdeepqlearning->stop();
		}
		else if (mode == rl_mode::approx) {
			this->approxAgent.set_parameters(pardialog.get_alpha(), pardialog.get_gamma(), pardialog.get_epsilon());
			featdialog.exec();
			auto wvalues = featdialog.get_feature_weight_values();

			//cechy, które ma braæ wynikaj¹ z wybranych wartoœci
			this->approxAgent.set_weight_table(wvalues);

			double result = play_and_train_approxqlearning(pardialog.get_episodes());
			cout << "DONE, TOTAL RESULT: " << result << endl;
			this->approxAgent.reset_parameters();

			ui.statusBar->showMessage(p_states.at(program_state::approx));
			this->timerapproxqlerning->start(100);
			this->timer->stop();
			this->timerSARSA->stop();
			this->timerdeepqlearning->stop();
		}
		else if (mode == rl_mode::deepqlearning) {
			this->dqnAgent->set_parameters(pardialog.get_alpha(), pardialog.get_epsilon(), pardialog.get_gamma());
			double result = play_and_train_deepqlearning(pardialog.get_episodes());
			cout << "DONE, TOTAL RESULT: " << result << endl;

			this->dqnAgent->reset_parameters();
			result = play_and_train_deepqlearning(pardialog.get_episodes());

			ui.statusBar->showMessage(p_states.at(program_state::deepqlearning));
			this->timerdeepqlearning->start(100);
			this->timerSARSA->stop();
			this->timerapproxqlerning->stop();
			this->timer->stop();
		}
		ui.startstopbutton->setText("STOP");
	}
}

void Reinforcement_LearningQLearning::set_random_episodes_to_show(int counter, int episodes) {
	this->episodes_to_show.clear();
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

void Reinforcement_LearningQLearning::on_deepqlearning_clicked()
{
	mode = rl_mode::deepqlearning;
}

