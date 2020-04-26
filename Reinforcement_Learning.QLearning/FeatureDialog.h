#pragma once

#include <iostream>
#include <QDialog>
#include "ui_FeatureDialog.h"
#include "Feature.h"

using namespace std;

class FeatureDialog : public QDialog
{
	Q_OBJECT

public:
	FeatureDialog(QWidget *parent = Q_NULLPTR);
	~FeatureDialog();
	
	map<feature_names, double> get_feature_weight_values();

private:
	Ui::FeatureDialog ui;
	map<feature_names, double> feature_values;
	map<QString, feature_names> feature_names;
	map<QCheckBox*, QDoubleSpinBox*> checkboxGroup;

	void on_ok_clicked();
	void on_cancel_clicked();
private slots:
	void on_foodDistanceCheckBox_clicked();
	void on_foodCurrentStateCheckBox_clicked();
	void on_wallCurrentStateCheckBox_clicked();
	void on_wallMinDistanceCheckBox_clicked();
};
