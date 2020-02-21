#include "Reinforcement_LearningMDP.h"
#include <QPainter>

Reinforcement_LearningMDP::Reinforcement_LearningMDP(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowTitle("MDP TEST");

	this->frameSize = Point(win_size.block_width + win_size.width, win_size.block_height + win_size.height);
	this->squareSize = Point(10, 10);
	initFrame();
	initEnvironment();
	this->snakeAndFood = make_pair(Point(10, 10), Point(20, 20));

	//tutaj niech siê uczy dla mdp
	this->mdp = MDPmodel(this->snakeAndFood, this->frame, this->environment);
	//ValueIteration valueIter = ValueIteration(mdp);
	//valueIter.main_value_iteration(10000, 0.001);
	//auto a = valueIter.get_state_values();

	auto policyIteration = PolicyIteration<pair<Point, Point>, action>(mdp);
	policyIteration.main_policy_iteration(map<pair<Point, Point>, action>(), 10000, 0.0001);
	map<pair<Point, Point>, double> pSV = policyIteration.get_policy_state_values();
	this->policy = policyIteration.get_policy();
	//this->policy = policyIteration.compute_new_policy(a);

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(loop()));
	timer->start(250);
}

Reinforcement_LearningMDP::~Reinforcement_LearningMDP()
{
	if (this->timer != nullptr)
		delete timer;
}

void Reinforcement_LearningMDP::paintEvent(QPaintEvent * event)
{
	QPainter paint(this);
	paint.setBrush(Qt::blue);
	for (Point ptk : frame) {
		paint.drawRect(ptk.get_X(), ptk.get_Y(), squareSize.get_X(), squareSize.get_Y());
	}

	for (Point ptk : frame) {
		paint.drawRect(ptk.get_X() + win_size.width + win_size.block_width, ptk.get_Y(), squareSize.get_X(), squareSize.get_Y());
	}

	auto states = this->mdp.get_all_states();
	for (auto &p : states) {
		if (p.second == this->snakeAndFood.second) {
			auto a = this->policy.at(p);
			paint.drawText(p.first.get_X() + win_size.width + win_size.block_width, p.first.get_Y(),
				win_size.block_width, win_size.block_height, Qt::AlignCenter, this->actions.at(a).c_str());
		}
	}
		
	//food
	paint.setBrush(Qt::green);
	paint.drawRect(snakeAndFood.second.get_X(), snakeAndFood.second.get_Y(), squareSize.get_X(), squareSize.get_Y());

	//snake
	paint.setBrush(Qt::red);
	paint.drawRect(snakeAndFood.first.get_X(), snakeAndFood.first.get_Y(), squareSize.get_X(), squareSize.get_Y());
}


void Reinforcement_LearningMDP::initFrame()
{
	frame.clear();
	for (int i = 0; i <= frameSize.get_Y(); i += squareSize.get_Y()) {
		frame.push_back(Point(0, i));
		frame.push_back(Point(frameSize.get_X(), i));
	}

	for (int i = 0; i < frameSize.get_X(); i += squareSize.get_X()) {
		frame.push_back(Point(i, 0));
		frame.push_back(Point(i, frameSize.get_Y()));
	}
}

void Reinforcement_LearningMDP::initEnvironment()
{
	for (int i = win_size.block_width; i < win_size.width + win_size.block_width; i += win_size.block_width) {
		for (int j = win_size.block_height; j < win_size.height + win_size.block_height; j += win_size.block_height) {
			this->environment.push_back(Point(i, j));
		}
	}
}

Point Reinforcement_LearningMDP::getRandomFoodPos()
{
	auto temp_list = this->environment;
	temp_list.remove(this->snakeAndFood.first);
	temp_list.remove(this->snakeAndFood.first);
	auto it = temp_list.begin();
	advance(it, rand() % temp_list.size());
	return *it;
}



void Reinforcement_LearningMDP::loop() {
	if (mdp.is_terminal(this->snakeAndFood)) {
		this->snakeAndFood = make_pair(this->snakeAndFood.first, this->getRandomFoodPos());
		update();
		return;
	}
	auto sd = mdp.step(this->policy[this->snakeAndFood], this->snakeAndFood);
	this->snakeAndFood = sd.next_state;
	update();
}