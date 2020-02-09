#pragma once
#include <iostream>
#include <cstdlib>

using namespace std;

class Point
{
public:
	Point();
	Point(int x, int y);
	Point(const Point &p);
	~Point();

	int get_X();
	int get_Y();

	#pragma region operators

	//Point operator!() const;
	Point& operator++();
	Point& operator--();

	Point& operator=(const Point &p);
	Point& operator=(const int &i);

	Point& operator+=(const Point &p);
	Point& operator-=(const Point &p);
	Point& operator*=(const Point &p);
	Point& operator/=(const Point &p);

	Point& operator+=(const int &i);
	Point& operator-=(const int &i);
	Point& operator*=(const int &i);
	Point& operator/=(const int &i);

	Point operator+(const Point &p) const;
	Point operator-(const Point &p) const;
	Point operator*(const Point &p) const;
	Point operator/(const Point &p) const;

	Point operator+(const int &i) const;
	Point operator-(const int &i) const;
	Point operator*(const int &i) const;
	Point operator/(const int &i) const;

	bool operator==(const Point &p) const;
	bool operator!=(const Point &p) const;
	bool operator<(const Point &p) const;
	bool operator>(const Point &p) const;
	bool operator<=(const Point &p) const;
	bool operator>=(const Point &p) const;

	friend ostream& operator<< (ostream& out, const Point& p);

	#pragma endregion

private:
	int x;
	int y;
};
