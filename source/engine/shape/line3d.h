#pragma once

#include <core/types.h>
#include <math/vector3d.h>

namespace jenny
{

template < typename T >
class line3d
{
public:

	line3d();

	line3d(T xa, T ya, T za, T xb, T yb, T zb);

	line3d(const vector3d<T>& start, const vector3d<T>& end);

	// operators
	line3d<T> operator + (const vector3d<T>& point) const;
	line3d<T>& operator += (const vector3d<T>& point);

	line3d<T> operator - (const vector3d<T>& point) const;
	line3d<T>& operator -= (const vector3d<T>& point);

	bool operator == (const line3d<T>& other) const;
	bool operator != (const line3d<T>& other) const;

	// functions
	void setLine(T xa, T ya, T za, T xb, T yb, T zb);

	void setLine(const vector3d<T>& nstart, const vector3d<T>& nend);

	void setLine(const line3d<T>& line);

	T getLength() const;

	T getLengthSQ() const;

	vector3d<T> getMiddle() const;

	vector3d<T> getVector() const;

	bool isPointBetweenStartAndEnd(const vector3d<T>& point) const;

	vector3d<T> getClosestPoint(const vector3d<T>& point) const;

    void getClosestPoints(const line3d<T>& line, vector3d<T>& pt0, vector3d<T>& pt1) const;

	bool getIntersectionWithSphere(vector3d<T> sorigin, T sradius, f64& outdistance) const;

	// member variables
	vector3d<T> Start;
	vector3d<T> End;
};

typedef line3d<f32> line3df;
typedef line3d<s32> line3di;

template < typename T >
inline
line3d<T>::line3d()
	: Start(T(0))
	, End(T(1))
{
}

template < typename T >
inline
line3d<T>::line3d(T xa, T ya, T za, T xb, T yb, T zb)
	: Start(xa, ya, za)
	, End(xb, yb, zb)
{
}

template < typename T >
inline
line3d<T>::line3d(const vector3d<T>& start, const vector3d<T>& end)
	: Start(start)
	, End(end)
{
}

template < typename T >
inline
line3d<T>
line3d<T>::operator + (const vector3d<T>& point) const
{
	return line3d<T>(Start + point, End + point);
}

template < typename T >
inline
line3d<T>&
line3d<T>::operator += (const vector3d<T>& point)
{
	Start += point;
	End += point;
	return *this;
}

template < typename T >
inline
line3d<T>
line3d<T>::operator - (const vector3d<T>& point) const
{
	return line3d<T>(Start - point, End - point);
}

template < typename T >
inline
line3d<T>&
line3d<T>::operator -= (const vector3d<T>& point)
{
	Start -= point;
	End -= point;
	return *this;
}

template < typename T >
inline
bool
line3d<T>::operator == (const line3d<T>& other) const
{
	return (Start == other.Start && End == other.End) || (End == other.Start && Start == other.End);
}

template < typename T >
inline
bool
line3d<T>::operator != (const line3d<T>& other) const
{
	return !(*this == other);
}

template < typename T >
inline
void
line3d<T>::setLine(T xa, T ya, T za, T xb, T yb, T zb)
{
	Start.set(xa, ya, za);
	End.set(xb, yb, zb);
}

template < typename T >
inline
void
line3d<T>::setLine(const vector3d<T>& nstart, const vector3d<T>& nend)
{
	Start.set(nstart);
	End.set(nend);
}

template < typename T >
inline
void
line3d<T>::setLine(const line3d<T>& line)
{
	Start.set(line.Start);
	End.set(line.End);
}

template < typename T >
inline
T
line3d<T>::getLength() const
{
	return Start.getDistanceFrom(End);
}

template < typename T >
inline
T
line3d<T>::getLengthSQ() const
{
	return Start.getDistanceFromSQ(End);
}

template < typename T >
inline
vector3d<T>
line3d<T>::getMiddle() const
{
	return (Start + End) * T(0.5);
}

template < typename T >
inline
vector3d<T>
line3d<T>::getVector() const
{
	return End - Start;
}

template < typename T >
inline
bool
line3d<T>::isPointBetweenStartAndEnd(const vector3d<T>& point) const
{
	return point.isBetweenPoints(Start, End);
}

template < typename T >
inline
vector3d<T>
line3d<T>::getClosestPoint(const vector3d<T>& point) const
{
	vector3d<T> c = point - Start;
	vector3d<T> v = getVector();
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

	return Start + t * v;
}

template < typename T >
inline
bool
line3d<T>::getIntersectionWithSphere(vector3d<T> sorigin, T sradius, f64& outdistance) const
{
	const vector3d<T> q = sorigin - Start;
	T c = q.getLength();
	T v = q.dotProduct(getVector().normalize());
	T d = sradius * sradius - (c * c - v * v);

	if(d < T(0))
	{
		return false;
	}
	outdistance = f64(v) - glf::Sqrt(f64(d));
	return true;
}


template < typename T >
inline
void
line3d<T>::getClosestPoints(const line3d<T>& line, vector3d<T>& pt0, vector3d<T>& pt1) const
{
    core::vector3d<T> u = End - Start;
    core::vector3d<T> v = line.End - line.Start;
    core::vector3d<T> w = Start - line.Start;

    const T a = u.dotProduct(u);
    const T b = u.dotProduct(v);
    const T c = v.dotProduct(v);

    const T denom = (a * c) - (b * b);
    if(denom == T(0)) // lines are parallel
    {
        pt0 = Start;
        pt1 = line.Start;
        return;
    }

    const T d = u.dotProduct(w);
    const T e = v.dotProduct(w);
    
    T s = (b * e - c * d) / denom;
    T t = (a * e - b * d) / denom;

    pt0 = Start + s * u;
    pt1 = line.Start + t * v;
}

} // end namespace jenny
