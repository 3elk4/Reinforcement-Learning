#ifndef DIALOG_H
#define DIALOG_H

#include <iostream>
#include <QDialog>
#include "ui_ParameterDialog.h"

using namespace std;

class ParameterDialog : public QDialog
{
	Q_OBJECT

public:
	ParameterDialog(QWidget *parent = Q_NULLPTR);
	~ParameterDialog();

	int get_episodes() {
		return this->episodes;
	}

	double get_alpha() {
		return this->alpha;
	}

	double get_epsilon() {
		return this->epsilon;
	}

	double get_gamma() {
		return this->gamma;
	}

	int get_episodes_number() {
		return this->episodes_number;
	}

private:
	Ui::ParameterDialog ui;

	int episodes = 20;
	double alpha = 0.5;
	double epsilon = 0.90;
	double gamma = 0.5;
	int episodes_number = 0;

	void on_ok_clicked();
	void on_cancel_clicked();
};

#endif
