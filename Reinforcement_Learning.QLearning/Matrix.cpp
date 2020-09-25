#include "Matrix.h"



Matrix::Matrix()
{
	this->rows = 3;
	this->columns = 3;
}

Matrix::Matrix(int rows, int columns)
{
	this->rows = rows;
	this->columns = columns;
}


Matrix::~Matrix()
{
}

vector<vector<double>> Matrix::get_matrix()
{
	return this->matrix;
}

int Matrix::get_rows()
{
	return this->rows;
}

int Matrix::get_columns()
{
	return this->columns;
}

void Matrix::init_matrix(const vector<double> values)
{
	for (int i = 0; i < rows; ++i) {
		vector<double> column;
		for (int j = 0; j < columns; ++j) {
			column.push_back(values.at(i * columns + j));
		}
		this->matrix.push_back(column);
	}
}

void Matrix::set_value(int row, int column, double value)
{
	this->matrix.at(row).at(column) = value;
}

double Matrix::get_value(int row, int column)
{
	return this->matrix.at(row).at(column);
}

vector<double> Matrix::get_row(int row)
{
	return this->matrix.at(row);
}
