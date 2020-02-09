#include "Reinforcement_LearningSARSA.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Reinforcement_LearningSARSA w;
	w.show();
	return a.exec();
}
