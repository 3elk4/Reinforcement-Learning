#ifndef MDP_H
#define MDP_H

#include <QtWidgets/QMainWindow>
#include <QkeyEvent>
#include <QTimer>
#include "ui_Reinforcement_LearningMDP.h"
#include <iostream>
#include <vector>
#include <utility>
#include "Pair.h"
#include "Constants.h"
#include "PolicyIteration.h"

using namespace std;

class Reinforcement_LearningMDP : public QMainWindow
{
	Q_OBJECT

public:
	Reinforcement_LearningMDP(QWidget *parent = Q_NULLPTR);
	~Reinforcement_LearningMDP();

	void paintEvent(QPaintEvent *event);

	void initFrame();
	void initEnvironment();
	Point getRandomFoodPos();
	Point getRandomHeadPos();

private:
	Ui::Reinforcement_LearningMDPClass ui;
	QTimer *timer = nullptr;

	MDPmodel mdp;
	map<pair<Point, Point>, action> policy;
	pair<Point, Point> snakeAndFood;
	vector<Point> frame;
	list<Point> environment;
	Point frameSize;
	Point squareSize;

private slots:
	void loop();
};

#endif //MDP_H