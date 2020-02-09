#pragma once
#include "Point.h"
#include <iostream>
using namespace std;

class Pair {
public:
	Pair();
	Pair(const Point &f, const Point &s);
	Pair(const Pair &p);
	~Pair();

	Point getFirst();
	Point getSecond();

	#pragma region operators
	Pair & operator=(const Pair &p);
	bool operator==(const Pair &p) const;
	bool operator!=(const Pair &p) const;
	bool operator<(const Pair &p) const;
	bool operator>(const Pair &p) const;
	bool operator<=(const Pair &p) const;
	bool operator>=(const Pair &p) const;

	#pragma endregion 

private:
	Point first;
	Point second; 
};

