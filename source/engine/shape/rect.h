#pragma once

#include <core/types.h>
#include <shape/dimension2d.h>
#include <math/vector2d.h>

namespace jenny
{

template <class T>
class rect
{
public:

	rect();
	rect(T x, T y, T x2, T y2);
	rect(const vector2d<T>& upperLeft, const vector2d<T>& lowerRight);
	rect(const vector2d<T>& pos, const dimension2d<T>& size);

	rect<T>& set(T x, T y, T x2, T y2);
	rect<T>& set(const vector2d<T>& upperLeft, const vector2d<T>& lowerRight);
	rect<T>& set(const vector2d<T>& pos, const dimension2d<T>& size);

	rect<T> operator + (const vector2d<T>& pos) const;
	rect<T>& operator += (const vector2d<T>& pos);
	rect<T> operator - (const vector2d<T>& pos) const;
	rect<T>& operator -= (const vector2d<T>& pos);

	bool operator == (const rect<T>& other) const;
	bool operator != (const rect<T>& other) const;

	rect<T>& operator += (const rect<T>& other);

	bool operator < (const rect<T>& other) const;

	T getArea() const;

	bool isPointInside(const vector2d<T>& pos) const;

	bool isPointInside(T x, T y) const;

	bool isRectCollided(const rect<T>& other) const;

	void clipAgainst(const rect<T>& other);

	bool constrainTo(const rect<T>& other);

	T getWidth() const;

	T getHeight() const;

	void repair();

	bool isValid() const;

	vector2d<T> getCenter() const;

	dimension2d<T> getSize() const;

	void addInternalPoint(const vector2d<T>& p);

	void addInternalPoint(T x, T y);

    void scale(T w, T h);

	vector2d<T> UpperLeftCorner;
	vector2d<T> LowerRightCorner;
};

typedef rect<s32> recti;
typedef rect<float> rectf;

template < typename T >
inline
rect<T>::rect()
{
}

template < typename T >
inline
rect<T>::rect(T x, T y, T x2, T y2)
	: UpperLeftCorner(x, y)
	, LowerRightCorner(x2, y2)
{
}

template < typename T >
inline
rect<T>::rect(const vector2d<T>& upperLeft, const vector2d<T>& lowerRight)
	: UpperLeftCorner(upperLeft)
	, LowerRightCorner(lowerRight)
{
}

template < typename T >
inline
rect<T>::rect(const vector2d<T>& pos, const dimension2d<T>& size)
	: UpperLeftCorner(pos)
	, LowerRightCorner(pos + size)
{
}

template < typename T >
inline
rect<T>&
rect<T>::set(T x, T y, T x2, T y2)
{
	UpperLeftCorner.set(x, y);
	LowerRightCorner.set(x2, y2);
	return *this;
}

template < typename T >
inline
rect<T>&
rect<T>::set(const vector2d<T>& upperLeft, const vector2d<T>& lowerRight)
{
	UpperLeftCorner = upperLeft;
	LowerRightCorner = lowerRight;
	return *this;
}

template < typename T >
inline
rect<T>&
rect<T>::set(const vector2d<T>& pos, const dimension2d<T>& size)
{
	UpperLeftCorner = pos;
	LowerRightCorner = pos + size;
	return *this;
}

template < typename T >
inline
rect<T>
rect<T>::operator + (const vector2d<T>& pos) const
{
	rect<T> ret(*this);
	return ret += pos;
}

template < typename T >
inline
rect<T>&
rect<T>::operator += (const vector2d<T>& pos)
{
	UpperLeftCorner += pos;
	LowerRightCorner += pos;
	return *this;
}

template < typename T >
inline
rect<T>
rect<T>::operator - (const vector2d<T>& pos) const
{
	rect<T> ret(*this);
	return ret -= pos;
}

template < typename T >
inline
rect<T>&
rect<T>::operator-=(const vector2d<T>& pos)
{
	UpperLeftCorner -= pos;
	LowerRightCorner -= pos;
	return *this;
}

template < typename T >
inline
bool
rect<T>::operator == (const rect<T>& other) const
{
	return (UpperLeftCorner == other.UpperLeftCorner
			&& LowerRightCorner == other.LowerRightCorner);
}

template < typename T >
inline
bool
rect<T>::operator != (const rect<T>& other) const
{
	return (UpperLeftCorner != other.UpperLeftCorner
			|| LowerRightCorner != other.LowerRightCorner);
}

template < typename T >
inline
rect<T>&
rect<T>::operator += (const rect<T>& other)
{
	addInternalPoint(other.UpperLeftCorner);
	addInternalPoint(other.LowerRightCorner);
	return *this;
}

template < typename T >
inline
bool
rect<T>::operator < (const rect<T>& other) const
{
	return getArea() < other.getArea();
}

template < typename T >
inline
T
rect<T>::getArea() const
{
	return getWidth() * getHeight();
}

template < typename T >
inline
bool
rect<T>::isPointInside(const vector2d<T>& pos) const
{
	return UpperLeftCorner <= pos && LowerRightCorner >= pos;
}

template < typename T >
inline
bool
rect<T>::isPointInside(T x, T y) const
{
	return isPointInside(vector2d<T>(x, y));
}

template < typename T >
inline
bool
rect<T>::isRectCollided(const rect<T>& other) const
{
	return (LowerRightCorner.getY() > other.UpperLeftCorner.getY()
			&& UpperLeftCorner.getY() < other.LowerRightCorner.getY()
			&& LowerRightCorner.getX() > other.UpperLeftCorner.getX()
			&& UpperLeftCorner.getX() < other.LowerRightCorner.getX());
}

template < typename T >
inline
void
rect<T>::clipAgainst(const rect<T>& other)
{
	if(other.LowerRightCorner.getX() < LowerRightCorner.getX())
	{
		LowerRightCorner.setX(other.LowerRightCorner.getX());
	}
	if(other.LowerRightCorner.getY() < LowerRightCorner.getY())
	{
		LowerRightCorner.setY(other.LowerRightCorner.getY());
	}

	if(other.UpperLeftCorner.getX() > UpperLeftCorner.getX())
	{
		UpperLeftCorner.setX(other.UpperLeftCorner.getX());
	}
	if(other.UpperLeftCorner.getY() > UpperLeftCorner.getY())
	{
		UpperLeftCorner.setY(other.UpperLeftCorner.getY());
	}

	// correct possible invalid rect resulting from clipping
	if(UpperLeftCorner.getY() > LowerRightCorner.getY())
	{
		UpperLeftCorner.setY(LowerRightCorner.getY());
	}
	if(UpperLeftCorner.getX() > LowerRightCorner.getX())
	{
		UpperLeftCorner.setX(LowerRightCorner.getX());
	}
}

template < typename T >
inline
bool
rect<T>::constrainTo(const rect<T>& other)
{
	if(other.getWidth() < getWidth() || other.getHeight() < getHeight())
	{
		return false;
	}

	T diff = other.LowerRightCorner.getX() - LowerRightCorner.getX();
	if(diff < 0)
	{
		LowerRightCorner.setX(LowerRightCorner.getX() + diff);
		UpperLeftCorner.setX(UpperLeftCorner.getX()  + diff);
	}

	diff = other.LowerRightCorner.getY() - LowerRightCorner.getY();
	if(diff < 0)
	{
		LowerRightCorner.setY(LowerRightCorner.getY() + diff);
		UpperLeftCorner.setY(UpperLeftCorner.getY() + diff);
	}

	diff = UpperLeftCorner.getX() - other.UpperLeftCorner.getX();
	if(diff < 0)
	{
		UpperLeftCorner.setX(UpperLeftCorner.getX() - diff);
		LowerRightCorner.setX(LowerRightCorner.getX() - diff);
	}

	diff = UpperLeftCorner.getY() - other.UpperLeftCorner.getY();
	if(diff < 0)
	{
		UpperLeftCorner.setY(UpperLeftCorner.getY() - diff);
		LowerRightCorner.setY(LowerRightCorner.getY() - diff);
	}

	return true;
}

template < typename T >
inline
T
rect<T>::getWidth() const
{
	return LowerRightCorner.getX() - UpperLeftCorner.getX();
}

template < typename T >
inline
T
rect<T>::getHeight() const
{
	return LowerRightCorner.getY() - UpperLeftCorner.getY();
}

template < typename T >
inline
void
rect<T>::repair()
{
	if(LowerRightCorner.getX() < UpperLeftCorner.getX())
	{
		T t = LowerRightCorner.getX();
		LowerRightCorner.setX(UpperLeftCorner.getX());
		UpperLeftCorner.setX(t);
	}

	if(LowerRightCorner.getY() < UpperLeftCorner.getY())
	{
		T t = LowerRightCorner.getY();
		LowerRightCorner.setY(UpperLeftCorner.getY());
		UpperLeftCorner.setY(t);
	}
}

template < typename T >
inline
bool
rect<T>::isValid() const
{
	return (LowerRightCorner.getX() > UpperLeftCorner.getX()
			&& LowerRightCorner.getY() > UpperLeftCorner.getY());
}

template < typename T >
inline
vector2d<T>
rect<T>::getCenter() const
{
	return (UpperLeftCorner + LowerRightCorner) / T(2);
}

template < typename T >
inline
dimension2d<T>
rect<T>::getSize() const
{
	return dimension2d<T>(getWidth(), getHeight());
}

template < typename T >
inline
void
rect<T>::addInternalPoint(const vector2d<T>& p)
{
	addInternalPoint(p.getX(), p.getY());
}

template < typename T >
inline
void
rect<T>::addInternalPoint(T x, T y)
{
	if(x > LowerRightCorner.getX())
	{
		LowerRightCorner.setX(x);
	}
	if(y > LowerRightCorner.getY())
	{
		LowerRightCorner.setY(y);
	}

	if(x < UpperLeftCorner.getX())
	{
		UpperLeftCorner.setX(x);
	}
	if(y < UpperLeftCorner.getY())
	{
		UpperLeftCorner.setY(y);
	}
}

template < typename T >
inline
void
rect<T>::scale(T w, T h)
{
	UpperLeftCorner *= vector2d<T>(w, h);
	LowerRightCorner *= vector2d<T>(w, h);
}

} // end namespace jenny
