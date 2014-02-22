#pragma once

#include <core/types.h>
#include <shape/dimension2d.h>

namespace jenny
{

template < typename T > class vector3d;

template < typename T >
class vector2d
{
public:

	typedef T SValueType;

	vector2d();

	vector2d(T nx, T ny);

	explicit vector2d(T n);

	explicit vector2d(const T* p);

	vector2d(const vector2d<T>& other);

	template <typename T2>
	explicit vector2d(const vector2d<T2>& other);

	template <typename T2>
	explicit vector2d(const vector3d<T2>& other);

	// data access

	//!
	const T* getDataPtr() const;

	//!
	T* getDataPtr();

	T& operator [] (u32 i);
	const T& operator [] (u32 i) const;

	T& x();
	T& y();

	const T& x() const;
	const T& y() const;

	const T& getX() const;
	const T& getY() const;

	vector2d<T>& setX(const T val);
	vector2d<T>& setY(const T val);

	vector2d<T>& incrX(const T val);
	vector2d<T>& incrY(const T val);

	

	vector2d<T> operator - () const;

	vector2d<T>& operator = (const vector2d<T>& other);

	vector2d<T>& operator += (const vector2d<T>& other);
	vector2d<T> operator + (const vector2d<T>& other) const;
	vector2d<T>& operator += (const T v);
	vector2d<T> operator + (const T v) const;

	vector2d<T>& operator -= (const vector2d<T>& other);
	vector2d<T> operator - (const vector2d<T>& other) const;
	vector2d<T>& operator -= (const T v);
	vector2d<T> operator - (const T v) const;

	vector2d<T>& operator *= (const vector2d<T>& other);
	vector2d<T> operator * (const vector2d<T>& other) const;
	vector2d<T>& operator *= (const T v);
	vector2d<T> operator * (const T v) const;

	vector2d<T>& operator /= (const vector2d<T>& other);
	vector2d<T> operator / (const vector2d<T>& other) const;
	vector2d<T>& operator /= (const T v);
	vector2d<T> operator / (const T v) const;

	bool operator <= (const vector2d<T>& other) const;
	bool operator >= (const vector2d<T>& other) const;

	bool operator < (const vector2d<T>& other) const;
	bool operator > (const vector2d<T>& other) const;

	bool operator == (const vector2d<T>& other) const;
	bool operator != (const vector2d<T>& other) const;

	const vector2d<T>& operator += (const dimension2d<T>& other);
	vector2d<T> operator + (const dimension2d<T>& other) const;

	const vector2d<T>& operator -= (const dimension2d<T>& other);
	vector2d<T> operator - (const dimension2d<T>& other) const;



	bool equals(const vector2d<T>& other) const;

	vector2d<T>& set(T val);
	vector2d<T>& set(T nx, T ny);
	vector2d<T>& set(const vector2d<T>& p);
	vector2d<T>& set(const vector3d<T>& p);


	T dotProduct(const vector2d<T>& other) const;


	T getLengthSQ() const;


	T getLength() const;


	T getDistanceFromSQ(const vector2d<T>& other) const;

	T getDistanceFrom(const vector2d<T>& other) const;

	T perpDotProduct(const vector2d<T>& other) const;

	vector2d<T>& rotateBy(f64 degrees, const vector2d<T>& center);

	vector2d<T>& normalize();

	f64 getAngleTrig() const;

	f64 getAngle() const;

	f64 getAngleWith(const vector2d<T>& b) const;

	bool isBetweenPoints(const vector2d<T>& begin, const vector2d<T>& end) const;

	vector2d<T> getInterpolated(const vector2d<T>& other, f32 d) const;

	vector2d<T> getInterpolated_quadratic(const vector2d<T>& v2, const vector2d<T>& v3, const T d) const;

	vector2d<T>& interpolate(const vector2d<T>& a, const vector2d<T>& b, const f32 t);

private:
	T Data[2];
};


typedef vector2d<f32> vector2df;
typedef vector2d<s32> vector2di;

template < typename T >
inline
vector2d<T>&
vector2d<T>::set(T v)
{
	(*this)[0] = v;
	(*this)[1] = v;
	return *this;
}

template < typename T >
inline
vector2d<T>&
vector2d<T>::set(T nx, T ny)
{
	(*this)[0] = nx;
	(*this)[1] = ny;
	return *this;
}

template < typename T >
inline
vector2d<T>&
vector2d<T>::set(const vector2d<T>& p)
{
	(*this)[0] = p[0];
	(*this)[1] = p[1];
	return *this;
}

template < typename T >
inline
T&
vector2d<T>::x()
{
	return Data[0];
}

template < typename T >
inline
T&
vector2d<T>::y()
{
	return Data[1];
}

template < typename T >
inline
const T&
	vector2d<T>::x() const
{
	return Data[0];
}

template < typename T >
inline
const T&
	vector2d<T>::y() const
{
	return Data[1];
}

template < typename T >
inline
const T&
vector2d<T>::getX() const
{
	return (*this)[0];
}

template < typename T >
inline
const T&
vector2d<T>::getY() const
{
	return (*this)[1];
}

template < typename T >
inline
vector2d<T>&
vector2d<T>::setX(const T val)
{
	(*this)[0] = val;
	return *this;
}

template < typename T >
inline
vector2d<T>&
vector2d<T>::setY(const T val)
{
	(*this)[1] = val;
	return *this;
}

template < typename T >
inline
vector2d<T>&
vector2d<T>::incrX(const T val)
{
	(*this)[0] += val;
	return *this;
}

template < typename T >
inline
vector2d<T>&
vector2d<T>::incrY(const T val)
{
	(*this)[1] += val;
	return *this;
}

template < typename T >
inline
vector2d<T>::vector2d()
{
	set(T(0));
}


template < typename T >
inline
vector2d<T>::vector2d(T nx, T ny)
{
	set(nx, ny);
}

template < typename T >
inline
vector2d<T>::vector2d(T n)
{
	set(n);
}

template < typename T >
inline
vector2d<T>::vector2d(const T* p)
{
	set(p[0], p[1]);
}

template < typename T >
inline
vector2d<T>::vector2d(const vector2d<T>& other)
{
	set(other);
}

template < typename T >
template <typename T2>
inline
vector2d<T>::vector2d(const vector2d<T2>& other)
{
	set(T(other[0]), T(other[1]));
}

template < typename T >
inline
const T*
vector2d<T>::getDataPtr() const
{
	return Data;
}

template < typename T >
inline
T*
vector2d<T>::getDataPtr()
{
	return Data;
}

template < typename T >
inline
T&
vector2d<T>::operator [] (u32 i)
{
	JENNY_ASSERT(i < 2);
	return getDataPtr()[i];
}

template < typename T >
inline
const T&
vector2d<T>::operator [] (u32 i) const
{
	JENNY_ASSERT(i < 2);
	return getDataPtr()[i];
}

template < typename T >
inline
vector2d<T>
vector2d<T>::operator - () const
{
	return vector2d<T>(-getX(), -getY());
}

template < typename T >
inline
vector2d<T>&
vector2d<T>::operator = (const vector2d<T>& other)
{
	return set(other);
}

template < typename T >
inline
vector2d<T>&
vector2d<T>::operator += (const vector2d<T>& other)
{
	(*this)[0] += other[0];
	(*this)[1] += other[1];
	return *this;
}

template < typename T >
inline
vector2d<T>
vector2d<T>::operator + (const vector2d<T>& other) const
{
	vector2d<T> result(*this);
	result += other;
	return result;
}

template < typename T >
inline
vector2d<T>&
vector2d<T>::operator += (const T v)
{
	(*this)[0] += v;
	(*this)[1] += v;
	return *this;
}

template < typename T >
inline
vector2d<T>
vector2d<T>::operator + (const T v) const
{
	vector2d<T> result(*this);
	result += v;
	return result;
}

template < typename T >
inline
vector2d<T>&
vector2d<T>::operator -= (const vector2d<T>& other)
{
	(*this)[0] -= other[0];
	(*this)[1] -= other[1];
	return *this;
}

template < typename T >
inline
vector2d<T>
vector2d<T>::operator - (const vector2d<T>& other) const
{
	vector2d<T> result(*this);
	result -= other;
	return result;
}

template < typename T >
inline
vector2d<T>&
vector2d<T>::operator -= (const T v)
{
	(*this)[0] -= v;
	(*this)[1] -= v;
	return *this;
}

template < typename T >
inline
vector2d<T>
vector2d<T>::operator - (const T v) const
{
	vector2d<T> result(*this);
	result -= v;
	return result;
}

template < typename T >
inline
vector2d<T>&
vector2d<T>::operator *= (const vector2d<T>& other)
{
	(*this)[0] *= other[0];
	(*this)[1] *= other[1];
	return *this;
}

template < typename T >
inline
vector2d<T>
vector2d<T>::operator * (const vector2d<T>& other) const
{
	vector2d<T> result(*this);
	result *= other;
	return result;
}

template < typename T >
inline
vector2d<T>&
vector2d<T>::operator *= (const T v)
{
	(*this)[0] *= v;
	(*this)[1] *= v;
	return *this;
}

template < typename T >
inline
vector2d<T>
vector2d<T>::operator * (const T v) const
{
	vector2d<T> result(*this);
	result *= v;
	return result;
}

template < typename T >
inline
vector2d<T>&
vector2d<T>::operator /= (const vector2d<T>& other)
{
	(*this)[0] /= other[0];
	(*this)[1] /= other[1];
	return *this;
}

template < typename T >
inline
vector2d<T>
vector2d<T>::operator / (const vector2d<T>& other) const
{
	vector2d<T> result(*this);
	result /= other;
	return result;
}

template < typename T >
inline
vector2d<T>&
vector2d<T>::operator /= (const T v)
{
	(*this)[0] /= v;
	(*this)[1] /= v;
	return *this;
}

template < typename T >
inline
vector2d<T>
vector2d<T>::operator / (const T v) const
{
	vector2d<T> result(*this);
	result /= v;
	return result;
}

template < typename T >
inline
bool
vector2d<T>::operator <= (const vector2d<T>& other) const
{
	return (*this)[0] <= other[0] && (*this)[1] <= other[1];
}

template < typename T >
inline
bool
vector2d<T>::operator >= (const vector2d<T>& other) const
{
	return (*this)[0] >= other[0] && (*this)[1] >= other[1];
}

template < typename T >
inline
bool
vector2d<T>::operator < (const vector2d<T>& other) const
{
	return (*this)[0] < other[0] && (*this)[1] < other[1];
}

template < typename T >
inline
bool
vector2d<T>::operator > (const vector2d<T>& other) const
{
	return (*this)[0] > other[0] && (*this)[1] > other[1];
}

template < typename T >
inline
bool
vector2d<T>::operator == (const vector2d<T>& other) const
{
	return (*this)[0] == other[0] && (*this)[1] == other[1];
}

template < typename T >
inline
bool
vector2d<T>::operator != (const vector2d<T>& other) const
{
	return (*this)[0] != other[0] || (*this)[1] != other[1];

}
template < typename T >
inline
const vector2d<T>&
vector2d<T>::operator += (const dimension2d<T>& other)
{
	(*this)[0] += other.Width;
	(*this)[1] += other.Height;
	return (*this);
}

template < typename T >
inline
vector2d<T>
vector2d<T>::operator + (const dimension2d<T>& other) const
{
	vector2d<T> result(*this);
	result += other;
	return result;
}

template < typename T >
inline
const vector2d<T>&
vector2d<T>::operator -= (const dimension2d<T>& other)
{
	(*this)[0] -= other.Width;
	(*this)[1] -= other.Height;
	return (*this);
}

template < typename T >
inline
vector2d<T>
vector2d<T>::operator - (const dimension2d<T>& other) const
{
	vector2d<T> result(*this);
	result -= other;
	return result;
}

template < typename T >
inline
bool
vector2d<T>::equals(const vector2d<T>& other) const
{
	return core::equals((*this)[0], other[0]) && core::equals((*this)[1], other[1]);
}

template < typename T >
inline
T
vector2d<T>::dotProduct(const vector2d<T>& other) const
{
	return (*this)[0] * other[0] + (*this)[1] * other[1];
}

template < typename T >
inline
T
vector2d<T>::getLengthSQ() const
{
	return dotProduct(*this);
}

template < typename T >
inline
T
vector2d<T>::getLength() const
{
	typedef typename boost::mpl::if_<boost::mpl::not_<boost::is_floating_point<T> >, float, T>::type TCast;
	return T(glf::Sqrt(TCast(getLengthSQ())));
}

template < typename T >
inline
T
vector2d<T>::getDistanceFromSQ(const vector2d<T>& other) const
{
	return (*this - other).getLengthSQ();
}

template < typename T >
inline
T
vector2d<T>::getDistanceFrom(const vector2d<T>& other) const
{
	return (*this - other).getLength();
}

template < typename T >
inline
T
vector2d<T>::perpDotProduct(const vector2d<T>& other) const
{	
	return (*this)[0] * other[1] - (*this)[1] * other[0];
}

template < typename T >
inline
vector2d<T>&
vector2d<T>::rotateBy(f64 degrees, const vector2d<T>& center)
{
	degrees *= DEGTORAD64;
	const T cs = T(glf::Cos(degrees));
	const T sn = T(glf::Sin(degrees));

	(*this) -= center;
	set((*this)[0] * cs - (*this)[1] * sn, (*this)[0] * sn + (*this)[1] * cs);
	return (*this) += center;
}

template < typename T >
inline
vector2d<T>&
vector2d<T>::normalize()
{
	T l = getLengthSQ();
	if(l == T(0))
	{
		return *this;
	}
	l = T(reciprocal_squareroot(f32(l)));
	return (*this) *= l;
}

template < typename T >
inline
f64
vector2d<T>::getAngleTrig() const
{
	if((*this)[0] == T(0))
	{
		return (*this)[1] < T(0) ? 270.0 : 90.0;
	}
	else if((*this)[1] == T(0))
	{
		return (*this)[0] < T(0) ? 180.0 : 0.0;
	}

	if((*this)[1] > T(0))
	{
		if((*this)[0] > T(0))
		{
			return glf::Atan(f64((*this)[1]) / f64((*this)[0])) * RADTODEG64;
		}
		return 180.0 - glf::Atan(f64((*this)[1]) / -f64((*this)[0])) * RADTODEG64;
	}
	if((*this)[0] > T(0))
	{
		return 360.0 - glf::Atan(-f64((*this)[1]) / f64((*this)[0])) * RADTODEG64;
	}
	return 180.0 + glf::Atan(-f64((*this)[1]) / -f64((*this)[0])) * RADTODEG64;
}

template < typename T >
inline
f64
vector2d<T>::getAngle() const
{
	if((*this)[1] == T(0)) // corrected thanks to a suggestion by Jox
	{
		return (*this)[0] < T(0) ? 180.0 : 0.0;
	}
	else if((*this)[0] == T(0))
	{
		return (*this)[1] < T(0) ? 90.0 : 270.0;
	}

	f64 tmp = f64((*this)[1]) / f64(getLength());
	tmp = glf::Atan(glf::Sqrt(1.0 - tmp * tmp) / tmp) * RADTODEG64;

	if((*this)[0] > T(0) && (*this)[1] > T(0))
	{
		return tmp + 270;
	}
	else if((*this)[0] > T(0) && (*this)[1] < T(0))
	{
		return tmp + 90.0;
	}
	else if((*this)[0] < T(0) && (*this)[1] < T(0))
	{
		return 90.0 - tmp;
	}
	else if((*this)[0] < T(0) && (*this)[1] > T(0))
	{
		return 270.0 - tmp;
	}
	return tmp;
}

template < typename T >
inline
f64
vector2d<T>::getAngleWith(const vector2d<T>& b) const
{
	f64 tmp = f64(this->dotProduct(b));

	if(tmp == 0.0)
	{
		return 90.0;
	}

	return glf::SafeAcos(tmp / (f64(getLength()) * f64(b.getLength()))) * RADTODEG64;
}

template < typename T >
inline
bool
vector2d<T>::isBetweenPoints(const vector2d<T>& begin, const vector2d<T>& end) const
{
	if(begin[0] != end[0])
	{
		return ((begin[0] <= (*this)[0] && (*this)[0] <= end[0])
				|| (begin[0] >= (*this)[0] && (*this)[0] >= end[0]));
	}
	return ((begin[1] <= (*this)[1] && (*this)[1] <= end[1])
			|| (begin[1] >= (*this)[1] && (*this)[1] >= end[1]));
}

template < typename T >
inline
vector2d<T>
vector2d<T>::getInterpolated(const vector2d<T>& other, f32 d) const
{
	return (*this) + T(d) * (other - *this);
}

template < typename T >
inline
vector2d<T>
vector2d<T>::getInterpolated_quadratic(const vector2d<T>& v2, const vector2d<T>& v3, const T d) const
{
	// this*(1-d)*(1-d) + 2 * v2 * (1-d) + v3 * d * d;
	const T inv = T(1) - d;
	const T mul0 = inv * inv;
	const T mul1 = T(2) * d * inv;
	const T mul2 = d * d;

	return (*this) * mul0 + v2 * mul1 + v3 * mul2;
}

template < typename T >
inline
vector2d<T>&
vector2d<T>::interpolate(const vector2d<T>& a, const vector2d<T>& b, const f32 t)
{
	set(b.getInterpolated(a, t));
	return *this;
}

//------------------------------------------------------------------------------

template < typename T>
inline
vector2d<T>
operator * (const T scalar, const vector2d<T>& vector)
{
	return vector * scalar;
}

template < typename T >
inline
vector2d<T>
normalize(const vector2d<T>& vector)
{
	vector2d<T> result(vector);
	result.normalize();
	return result;
}



} // end namespace jenny
