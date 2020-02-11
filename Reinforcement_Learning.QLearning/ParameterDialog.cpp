#include "ParameterDialog.h"

ParameterDialog::ParameterDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setModal(true);

	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &ParameterDialog::on_ok_clicked);
	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &ParameterDialog::on_cancel_clicked);
}

ParameterDialog::~ParameterDialog()
{
}

void ParameterDialog::on_cancel_clicked()
{
	this->close();
}

void ParameterDialog::on_ok_clicked() {
	this->episodes = ui.episodesBox->value();
	this->alpha = ui.alphaBox->value();
	this->gamma = ui.gammaBox->value();
	this->epsilon = ui.gammaBox->value();
	this->close();
}