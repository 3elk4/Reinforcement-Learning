#pragma once
#include <iostream>
#include <vector>

using namespace std;

class Matrix
{
public:
	Matrix();
	Matrix(int rows, int columns);
	~Matrix();

	vector<vector<double>> get_matrix();
	int get_rows();
	int get_columns();
	void init_matrix(const vector<double> values);
	void set_value(int row, int column, double value);
	double get_value(int row, int column);
	vector<double> get_row(int row);
private:
	vector<vector<double>> matrix;
	int rows;
	int columns;
};

