#include "Point.h"

Point::Point() {
	this->x = 0;
	this->y = 0;
}

Point::Point(int x, int y)
{
	this->x = x;
	this->y = y;
}

Point::Point(const Point &p)
{
	*this = p;
}

Point::~Point()
{
}

int Point::get_X()
{
	return this->x;
}

int Point::get_Y()
{
	return this->y;
}

/*Point Point::operator!() const
{
	return Point(!this->x, !this->y);
}*/

Point & Point::operator++()
{
	this->x++;
	this->y++;
	return *this;
}

Point & Point::operator--()
{
	this->x--;
	this->y--;
	return *this;
}

Point & Point::operator=(const Point & p)
{
	this->x = p.x;
	this->y = p.y;
	return *this;
}

Point & Point::operator=(const int & i)
{
	this->x = i;
	this->y = i;
	return *this;
}

Point & Point::operator+=(const Point & p)
{
	this->x += p.x;
	this->y += p.y;
	return *this;
}

Point & Point::operator-=(const Point & p)
{
	this->x -= p.x;
	this->y -= p.y;
	return *this;
}

Point & Point::operator*=(const Point & p)
{
	this->x *= p.x;
	this->y *= p.y;
	return *this;
}

Point & Point::operator/=(const Point & p)
{
	if (p.x != 0 && p.y != 0) {
		this->x /= p.x;
		this->y /= p.y;
	}
	return *this;
}

Point & Point::operator+=(const int & i)
{
	this->x += i;
	this->y += i;
	return *this;
}

Point & Point::operator-=(const int & i)
{
	this->x -= i;
	this->y -= i;
	return *this;
}

Point & Point::operator*=(const int & i)
{
	this->x *= i;
	this->y *= i;
	return *this;
}

Point & Point::operator/=(const int & i)
{
	if (i != 0) {
		this->x /= i;
		this->y /= i;
	}
	return *this;
}

Point Point::operator+(const Point &p) const
{
	return Point(this->x + p.x, this->y + p.y);
}

Point Point::operator-(const Point & p) const
{
	return Point(this->x - p.x, this->y - p.y);
}

Point Point::operator*(const Point & p) const
{
	return Point(this->x * p.x, this->y * p.y);
}

Point Point::operator/(const Point & p) const
{
	if(p.x != 0 && p.y != 0)
		return Point(this->x / p.x, this->y / p.y);
	return Point(this->x, this->y);
}

Point Point::operator+(const int & i) const
{
	return Point(this->x + i, this->y + i);
}

Point Point::operator-(const int & i) const
{
	return Point(this->x - i, this->y - i);
}

Point Point::operator*(const int & i) const
{
	return Point(this->x * i, this->y * i);
}

Point Point::operator/(const int & i) const
{
	if(i != 0)
		return Point(this->x / i, this->y / i);

	return Point(this->x, this->y);
}

bool Point::operator==(const Point & p) const
{
	if (this->x == p.x && this->y == p.y) 
		return true;
	return false;
}

bool Point::operator!=(const Point & p) const
{
	return (this->x != p.x && this->y != p.y);
}

bool Point::operator<(const Point & p) const
{
	return this->x < p.x || (this->x == p.x && this->y < p.y);
}

bool Point::operator>(const Point & p) const
{
	if (this->x > p.x && this->y > p.y)
		return true;
	return false;
}

bool Point::operator<=(const Point & p) const
{
	if (this->x <= p.x && this->y <= p.y)
		return true;
	return false;
}

bool Point::operator>=(const Point & p) const
{
	if (this->x >= p.x && this->y >= p.y)
		return true;
	return false;
}

ostream & operator<<(ostream & out, const Point &p)
{
	string x = to_string(p.x);
	string y = to_string(p.y);
	out << "(" << x << ", " << y << ")";
	return out;
}

