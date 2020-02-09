#include "Reinforcement_LearningMDP.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	qsrand(time(NULL));
	QApplication a(argc, argv);
	Reinforcement_LearningMDP w;
	w.show();
	return a.exec();
}
