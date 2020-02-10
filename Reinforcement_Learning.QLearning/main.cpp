#include "Reinforcement_LearningQLearning.h"
#include <QtWidgets/QApplication>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	srand(time(NULL));
	QApplication a(argc, argv);
	Reinforcement_LearningQLearning w;
	w.show();
	return a.exec();
}
