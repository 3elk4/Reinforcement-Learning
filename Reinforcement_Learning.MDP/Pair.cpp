#include "Pair.h"

Pair::Pair() {
	this->first = Point();
	this->second = Point();
}

Pair::Pair(const Point & f, const Point & s)
{
	this->first = f;
	this->second = s;
}

Pair::Pair(const Pair & p)
{
	this->first = p.first;
	this->second = p.second;
}


Pair::~Pair()
{
}

Point Pair::getFirst()
{
	return this->first;
}

Point Pair::getSecond()
{
	return this->second;
}

Pair & Pair::operator=(const Pair & p)
{
	this->first = p.first;
	this->second = p.second;
	return *this;
}

bool Pair::operator==(const Pair & p) const
{
	return (this->first == p.first && this->second == p.second);
}

bool Pair::operator!=(const Pair & p) const
{
	return (this->first != p.first && this->second != p.second);
}

bool Pair::operator<(const Pair & p) const
{
	//this->x < p.x || (this->x == p.x && this->y < p.y);
	return (this->first < p.first) || (this->first == p.first && this->second < p.second);
}

bool Pair::operator>(const Pair & p) const
{
	return (this->first > p.first && this->second > p.second);
}

bool Pair::operator<=(const Pair & p) const
{
	return (this->first <= p.first && this->second <= p.second);
}

bool Pair::operator>=(const Pair & p) const
{
	return (this->first >= p.first && this->second >= p.second);
}
