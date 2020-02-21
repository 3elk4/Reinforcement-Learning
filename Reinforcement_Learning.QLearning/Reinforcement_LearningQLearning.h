#ifndef QLEARNING_H
#define QLEARNING_H

#include <QtWidgets/QMainWindow>
#include <qfiledialog.h>
#include "ui_Reinforcement_LearningQLearning.h"
#include "Constants.h"
#include "Environment.h"
#include "QLearningAgent.h"
#include "SARSA.h"
#include <QTimer>
#include <QkeyEvent>
#include "Point.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <thread>
#include <utility>
#include <string>
#include "ParameterDialog.h"
#include "AproxQLearning.h"

using namespace std;

class Reinforcement_LearningQLearning : public QMainWindow
{
	Q_OBJECT

public:
	Reinforcement_LearningQLearning(QWidget *parent = Q_NULLPTR);
	~Reinforcement_LearningQLearning();

	void paintEvent(QPaintEvent* event);
	void keyPressEvent(QKeyEvent* event);


	//double play_and_train_qlearning(const int &episodes);
	//double play_and_train_SARSA(const int &episodes);

	double play_and_train_qlearning(const int &episodes);
	double play_and_train_SARSA(const int &episodes);
	double play_and_train_approxqlearning(const int &episodes);

	bool do_step_qlearning(int current_episode);
	bool do_step_SARSA(int current_episode);
	bool do_step_approxqlearning(int current_episode);


	void init_environment(const string &path);
	void print_table(QLearningAgent<pair<Point, Point>, action> &agent);
	void print_weight_table();
	void show_episode(int current_episode, list<int> episodes);
	void draw_best_actions(QLearningAgent<pair<Point, Point>, action> &agent, QPainter &painter);

private:
	Ui::Reinforcement_LearningQLearningClass ui;
	QTimer *timer = nullptr;
	QTimer *timerSARSA = nullptr;
	QTimer *timerapproxqlerning = nullptr;
	pair<Point, Point> headAndFood;

	QLearningAgent<pair<Point, Point>, action> current_agent;


	Environment environment;
	
	QLearningAgent<pair<Point, Point>, action> agent;
	SARSA agentSARSA;
	AproxQLearning approxAgent;

	
	map<action, string> actions = { {action::up, "u"}, 
									{action::down, "d" },
									{action::left, "l" },
									{action::right, "r" } };

	map<feature_names, string> features = { {feature_names::food_feature, "Food feature"},
											{feature_names::min_wall_feature, "Min wall feature"},
											{feature_names::is_food_feature, "Is food feature"},
											{feature_names::is_wall_feature, "Is wall feature"}
	};

	map<program_state, QString> p_states = {
		{program_state::none, "Waiting for action..."},
		{program_state::init_env, "Loading environment..."},
		{program_state::mode_choosing, "Q-learning or SARSA???"},
		{program_state::qlearning, "Now Q-learning agent in motion."},
		{program_state::sarsa, "Now SARSA agent in motion."},
		{program_state::train_and_play, "Training agent..."},
		{program_state::setting_parameters, "Set parameters for training agent."},
		{program_state::episode, "Now training episode: "},
		{program_state::approx, "Now Approximate Qlearning in motion."}
	};

	rl_mode mode;
	ParameterDialog pardialog = ParameterDialog(this);
	list<int> episodes_to_show;
	void set_random_episodes_to_show(int count, int episodes);
	QFileDialog fd;
private slots:
	void loopqlearning();
	void loopSARSA();
	void loopapproxqlearning();

	void on_envbutton_clicked();
	void on_startstopbutton_clicked();
	void on_qlearning_clicked();
	void on_sarsa_clicked();
	void on_aproxqlearning_clicked();
};

#endif // QLEARNING_H