#include "FeatureDialog.h"

FeatureDialog::FeatureDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	feature_names = { { ui.foodDistanceCheckBox->text(), feature_names::food_feature },
						{ ui.foodCurrentStateCheckBox->text(), feature_names::is_food_feature },
						{ ui.wallCurrentStateCheckBox->text(), feature_names::is_wall_feature },
						{ ui.wallMinDistanceCheckBox->text(), feature_names::min_wall_feature } };

	checkboxGroup = { {ui.foodDistanceCheckBox, ui.foodDistanceSpinBox },
						{ui.foodCurrentStateCheckBox, ui.foodCurrentStateSpinBox },
						{ui.wallCurrentStateCheckBox, ui.wallCurrentStateSpinBox },
						{ui.wallMinDistanceCheckBox, ui.wallMinDistanceSpinBox } };

	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &FeatureDialog::on_ok_clicked);
	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &FeatureDialog::on_cancel_clicked);
}

FeatureDialog::~FeatureDialog()
{
}

map<feature_names, double> FeatureDialog::get_feature_weight_values()
{
	return this->feature_values;
}

void FeatureDialog::on_ok_clicked()
{
	feature_values.clear();
	for (auto & cb : checkboxGroup) {
		if (cb.first->isChecked()) {
			auto fn = feature_names.at(cb.first->text());
			feature_values[fn] = cb.second->value();
		}
	}
	this->close();
}

void FeatureDialog::on_cancel_clicked()
{
	if (feature_values.empty()) {
		feature_values[feature_names::food_feature] = 0.0;
		feature_values[feature_names::is_wall_feature] = 0.0;
	}
	this->close();
}

void FeatureDialog::on_foodDistanceCheckBox_clicked() 
{
	ui.foodDistanceCheckBox->isChecked() ? ui.foodDistanceSpinBox->setEnabled(true) : ui.foodDistanceSpinBox->setEnabled(false);
}

void FeatureDialog::on_foodCurrentStateCheckBox_clicked()
{
	ui.foodCurrentStateCheckBox->isChecked() ? ui.foodCurrentStateSpinBox->setEnabled(true) : ui.foodCurrentStateSpinBox->setEnabled(false);
}

void FeatureDialog::on_wallCurrentStateCheckBox_clicked()
{
	ui.wallCurrentStateCheckBox->isChecked() ? ui.wallCurrentStateSpinBox->setEnabled(true) : ui.wallCurrentStateSpinBox->setEnabled(false);
}

void FeatureDialog::on_wallMinDistanceCheckBox_clicked()
{
	ui.wallMinDistanceCheckBox->isChecked() ? ui.wallMinDistanceSpinBox->setEnabled(true) : ui.wallMinDistanceSpinBox->setEnabled(false);
}
