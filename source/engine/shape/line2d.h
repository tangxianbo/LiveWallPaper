#pragma once

#include <core/types.h>
#include <math/vector2d.h>

namespace jenny
{

template < typename T >
class line2d
{
public:
	line2d();
	
	line2d(T xa, T ya, T xb, T yb);
	
	line2d(const vector2d<T>& start, const vector2d<T>& end);
	
	line2d(const line2d<T>& other);

	// operators

	line2d<T> operator + (const vector2d<T>& point) const;
	line2d<T>& operator += (const vector2d<T>& point);

	line2d<T> operator - (const vector2d<T>& point) const;
	line2d<T>& operator -= (const vector2d<T>& point);

	bool operator == (const line2d<T>& other) const;
	bool operator != (const line2d<T>& other) const;

	// functions
	void setLine(T xa, T ya, T xb, T yb);

	void setLine(const vector2d<T>& nstart, const vector2d<T>& nend);

	void setLine(const line2d<T>& line);

	f64 getLength() const;

	T getLengthSQ() const;

	vector2d<T> getMiddle() const;

	vector2d<T> getVector() const;

	bool intersectWith(const line2d<T>& l, vector2d<T>& out) const;

	vector2d<T> getUnitVector() const;

	f64 getAngleWith(const line2d<T>& l) const;

	T getPointOrientation(const vector2d<T>& point) const;

	bool isPointOnLine(const vector2d<T>& point) const;

	bool isPointBetweenStartAndEnd(const vector2d<T>& point) const;

	vector2d<T> getClosestPoint(const vector2d<T>& point) const;

	T getDistance(const vector2d<T>& point) const;
	
	T getDistanceSQ(const vector2d<T>& point) const;


	vector2d<T> Start;
	vector2d<T> End;
};


typedef line2d<f32> line2df;
typedef line2d<s32> line2di;

template < typename T >
inline
line2d<T>::line2d()
	: Start(T(0))
	, End(T(1))
{
}

template < typename T >
inline
line2d<T>::line2d(T xa, T ya, T xb, T yb)
	: Start(xa, ya)
	, End(xb, yb)
{
}

template < typename T >
inline
line2d<T>::line2d(const vector2d<T>& start, const vector2d<T>& end)
	: Start(start)
	, End(end)
{
}

template < typename T >
inline
line2d<T>::line2d(const line2d<T>& other)
	: Start(other.Start)
	, End(other.End)
{
}

template < typename T >
inline
line2d<T>
line2d<T>::operator + (const vector2d<T>& point) const
{
	return line2d<T>(Start + point, End + point);
}

template < typename T >
inline
line2d<T>&
line2d<T>::operator += (const vector2d<T>& point)
{
	Start += point;
	End += point;
	return *this;
}

template < typename T >
inline
line2d<T>
line2d<T>::operator - (const vector2d<T>& point) const
{
	return line2d<T>(Start - point, End - point);
}

template < typename T >
inline
line2d<T>&
line2d<T>::operator -= (const vector2d<T>& point)
{
	Start -= point;
	End -= point;
	return *this;
}

template < typename T >
inline
bool
line2d<T>::operator == (const line2d<T>& other) const
{
	return (Start == other.Start && End == other.End) || (End == other.Start && Start == other.End);
}

template < typename T >
inline
bool
line2d<T>::operator != (const line2d<T>& other) const
{
	return !(*this == other);
}

template < typename T >
inline
void
line2d<T>::setLine(T xa, T ya, T xb, T yb)
{
	Start.set(xa, ya);
	End.set(xb, yb);
}

template < typename T >
inline
void
line2d<T>::setLine(const vector2d<T>& nstart, const vector2d<T>& nend)
{
	Start.set(nstart);
	End.set(nend);
}

template < typename T >
inline
void
line2d<T>::setLine(const line2d<T>& line)
{
	Start.set(line.Start);
	End.set(line.End);
}

template < typename T >
inline
f64
line2d<T>::getLength() const
{
	return Start.getDistanceFrom(End);
}

template < typename T >
inline
T
line2d<T>::getLengthSQ() const
{
	return Start.getDistanceFromSQ(End);
}

template < typename T >
inline
vector2d<T>
line2d<T>::getMiddle() const
{
	return (Start + End) * T(0.5);
}

template < typename T >
inline
vector2d<T>
line2d<T>::getVector() const
{
	return End - Start;
}

template < typename T >
inline
bool
line2d<T>::intersectWith(const line2d<T>& l, vector2d<T>& out) const
{
	vector2d<T> d0 = End - Start;
	vector2d<T> d1 = l.Start - l.End;

	// check "parallel" lines
	float det = float(d0.getX() * d1.getY() - d0.getY() * d1.getX());
	if(det > -ROUNDING_ERROR_32 && det < ROUNDING_ERROR_32)
	{
		return false;
	}
	det = 1.0f / det;

	vector2d<T> d01 = l.Start - Start;

	// Check intersection with this line
	float t = float(d01.getX() * d1.getY() - d01.getY() * d1.getX()) * det;
	if(t < 0.0f || t > 1.0f)
	{
		return false;
	}

	// check intersection with other line
	t = float(d0.getX() * d01.getY() - d0.getY() * d01.getX()) * det;
	if(t < 0.0f || t > 1.0f)
	{
		return false;
	}

	out = l.Start - d1 * T(t);
	return true;
}

template < typename T >
inline
vector2d<T>
line2d<T>::getUnitVector() const
{
	vector2d<T> v = End - Start;
	v.normalize();
	return v;
}

template < typename T >
inline
f64
line2d<T>::getAngleWith(const line2d<T>& l) const
{
	vector2d<T> vect = getVector();
	vector2d<T> vect2 = l.getVector();
	return vect.getAngleWith(vect2);
}

template < typename T >
inline
T
line2d<T>::getPointOrientation(const vector2d<T>& point) const
{
	return (End.getX() - Start.getX()) * (point.getY() - Start.getY())
			- (point.getX() - Start.getX()) * (End.getY() - Start.getY());
}

template < typename T >
inline
bool
line2d<T>::isPointOnLine(const vector2d<T>& point) const
{
	T d = getPointOrientation(point);
	return (d == T(0) && point.isBetweenPoints(Start, End));
}

template < typename T >
inline
bool
line2d<T>::isPointBetweenStartAndEnd(const vector2d<T>& point) const
{
	return point.isBetweenPoints(Start, End);
}

template < typename T >
inline
vector2d<T>
line2d<T>::getClosestPoint(const vector2d<T>& point) const
{
	vector2d<T> c = point - Start;
	vector2d<T> v = End - Start;
	T d = v.getLength();
	if(d == (T)0)
	{
		return End;
	}
	v /= d;
	T t = v.dotProduct(c);

	if(t < T(0))
	{
		return Start;
	}
	if(t > d)
	{
		return End;
	}

	return Start + v * t;
}

template < typename T >
inline
T line2d<T>::getDistance(const vector2d<T>& point) const
{
	vector2d<T> closest = getClosestPoint(point);
	return point.getDistanceFrom(closest);
}
	
template < typename T >
inline
T line2d<T>::getDistanceSQ(const vector2d<T>& point) const
{
	vector2d<T> closest = getClosestPoint(point);
	return point.getDistanceFromSQ(closest);
}

} // end namespace jenny
