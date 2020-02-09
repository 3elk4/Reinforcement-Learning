#include "Reinforcement_LearningQLearning.h"
#include <QPainter>

Reinforcement_LearningQLearning::Reinforcement_LearningQLearning(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowTitle("Q-LEARNING");

	//environment
	//walls, paths, food and head
	init_environment("./env/maze3.txt");

	//mdpmodel set
	auto states = this->environment.init_all_states(environment.get_environment_elements(env_type::path));
	this->environment.init_transition_probs_and_rewards(states);
	this->environment.set_initial_state(this->headAndFood);

	//QAgent 
	list<action> actions = { action::up, action::down, action::left, action::right };
	this->agent.init_qvalues(states, actions);
	//SARSA
	this->agentSARSA.init_qvalues(states, actions);
	
	//timer = new QTimer(this);
	//connect(timer, SIGNAL(timeout()), this, SLOT(loop()));
	//timer->start(250);
}

Reinforcement_LearningQLearning::~Reinforcement_LearningQLearning()
{
	//if (this->timer != nullptr)
		//delete timer;
}

void Reinforcement_LearningQLearning::init_environment(const string &path) {
	ifstream inFile;
	inFile.open(path);
	if (!inFile) {
		cerr << "Unable to open this file!";
		exit(1);
	}

	string line;
	int x = 0, y = 0, h = 0, w = 0;
	if (inFile >> line) {
		size_t pos = 0;
		string del = "x";
		if ((pos = line.find(del)) != string::npos) {
			w = stoi(line.substr(0, pos));
			line.erase(0, pos + del.length());
			h = stoi(line);
		}

		win_size.set_height(h * win_size.block_height);
		win_size.set_width(w * win_size.block_width);
	}

	Point temp;
	while (inFile >> line) {
		//w = line.size();
		//h++;
		x = 0;
		for (int i = 0; i < line.size(); ++i) {
			char c = line[i];
			temp = Point(x, y);
			if (c == e_sign.wall) {
				this->environment.set_environemnt_element(temp, env_type::wall);
			}
			else if (c == e_sign.path) {
				this->environment.set_environemnt_element(temp, env_type::path);
				//tutaj next states
				/*this->environment.set_next_state(temp, action::up, temp + Point(x, y - win_size.block_height));
				this->environment.set_next_state(temp, action::down, temp + Point(x, y + win_size.block_height));
				this->environment.set_next_state(temp, action::left, temp + Point(x - win_size.block_width, y));
				this->environment.set_next_state(temp, action::right, temp + Point(x + win_size.block_width, y));*/
			}
			else if (c == e_sign.head) {
				this->environment.set_environemnt_element(temp, env_type::path);
				//this->environment.set_initial_state(temp);
				this->headAndFood.first = temp;
			}
			else if (c == e_sign.food) {
				this->environment.set_environemnt_element(temp, env_type::path);
				//this->environment.set_end_state(temp);
				this->headAndFood.second = temp;
			}
			x += win_size.block_width;
		}
		y += win_size.block_height;
	}
	inFile.close();
}

void Reinforcement_LearningQLearning::keyPressEvent(QKeyEvent* event) {
	switch (event->key()) {
	case Qt::Key_Q: {
		double result = play_and_train(10, this->agent);
		cout << "Your result: " << result << endl;
		//print_table();
		break;
	}
	case Qt::Key_S: {
		double result = play_and_train(10, this->agentSARSA);
		cout << "Your result: " << result << endl;
		break;
	}
	default: {
		return;
	}
	}
}

void Reinforcement_LearningQLearning::paintEvent(QPaintEvent * event)
{
	QPainter paint(this);

	paint.setBrush(Qt::blue);
	auto walls = this->environment.get_environment_elements(env_type::wall);
	for_each(walls.begin(), walls.end(),
		[&paint](Point w) {paint.drawRect(w.get_X(), w.get_Y(), win_size.block_width, win_size.block_height); });

	paint.setBrush(Qt::green);
	auto food = this->environment.get_environment_elements(env_type::food);
	paint.drawRect(this->headAndFood.second.get_X(), this->headAndFood.second.get_Y(), win_size.block_width, win_size.block_height);

	paint.setBrush(Qt::red);
	paint.drawRect(this->headAndFood.first.get_X(), this->headAndFood.first.get_Y(), win_size.block_width, win_size.block_height);
}


void Reinforcement_LearningQLearning::print_table(QLearningAgent<pair<Point, Point>, action> &agent) {

	//TODO: odfiltrowaæ do obecnej planszy
	map<pair<Point, Point>, map<action, double>> qtable = agent.get_qtable();
	map<pair<Point, Point>, map<action, double>> filtred;
	for (auto &kv : qtable) {
		if(kv.first.second == this->headAndFood.second) filtred[kv.first] = kv.second;
	}

	printf("|%18s|%8s|%8s|%8s|%8s|\n", "STATE", "up", "right", "down", "left");
	for (auto &kv : filtred) {
		cout << "|" << kv.first.first << ", " << kv.first.second << "|" <<
			kv.second.at(action::up) << "|" <<
			kv.second.at(action::right) << "|" <<
			kv.second.at(action::down) << "|" <<
			kv.second.at(action::left) << "|" << endl;

		/*printf("|%8s, %8s|", kv.first.first, kv.first.second);
		printf("|%10f|", kv.second.at(action::up));
		printf("|%10f|", kv.second.at(action::right));
		printf("|%10f|", kv.second.at(action::down));
		printf("|%10f|\n", kv.second.at(action::left));*/
	}
}

double Reinforcement_LearningQLearning::play_and_train(const int & episodes, QLearningAgent<pair<Point, Point>, action> &agent)
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
		while (true) {
			//get agent to pick action given state s.
			a = agent.get_action(this->headAndFood);
			if (!a.has_value()) {
				this->headAndFood = this->environment.reset();
				this_thread::sleep_for(0.5s);
				repaint();
				continue;
			}
			sd = this->environment.step(a.value(), this->headAndFood);

			//train(update) agent for state s
			agent.update(this->headAndFood, a.value(), sd.reward, sd.next_state);
			this->headAndFood = sd.next_state;
			total_reward += sd.reward;
			this_thread::sleep_for(0.5s);
			repaint();
			if (sd.is_done) break;
		}
		cout << "EPISODE: " << i+1 << endl;
		cout << "SCORE: " << total_reward << endl;
		print_table(agent);
	}
	return total_reward;
}



void Reinforcement_LearningQLearning::loop() {

}