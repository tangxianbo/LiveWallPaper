#pragma once

#include <type_traits>
#include <math/math.h>
#include <math/vector2d.h>

namespace jenny
{

template < typename T > class vector4d;

template < typename T >
class vector3d
{
public:

	typedef T SValueType;

	vector3d();

	vector3d(T nx, T ny, T nz);

	explicit vector3d(T n);

	explicit vector3d(const T* p);

	vector3d(const vector3d<T>& other);

	template <typename T2>
	explicit vector3d(const vector3d<T2>& other);


	template <typename T2>
	explicit vector3d(const vector2d<T2>& xy, T z = T(0));

	
	template <typename T2>
	explicit vector3d(const vector4d<T2>& other);

	// data access

	const T* getDataPtr() const;

	T* getDataPtr();

	T& operator [] (u32 i);

	const T& operator [] (u32 i) const;

	T& x();
	T& y();
	T& z();

	const T& x() const;
	const T& y() const;
	const T& z() const;

	const T& getX() const;
	const T& getY() const;
	const T& getZ() const;

	vector3d<T>& setX(T val);
	vector3d<T>& setY(T val);
	vector3d<T>& setZ(T val);

	vector3d<T>& incrX(const T val);
	vector3d<T>& incrY(const T val);
	vector3d<T>& incrZ(const T val);

	// operators

	vector3d<T> operator - () const;

	vector3d<T>& operator = (const vector3d<T>& other);

	vector3d<T>& operator += (const vector3d<T>& other);
	vector3d<T> operator + (const vector3d<T>& other) const;
	vector3d<T>& operator += (const T val);
	vector3d<T> operator + (const T val) const;

	vector3d<T>& operator -= (const vector3d<T>& other);
	vector3d<T> operator - (const vector3d<T>& other) const;
	vector3d<T>& operator -= (const T val);
	vector3d<T> operator - (const T val) const;

	vector3d<T>& operator *= (const vector3d<T>& other);
	vector3d<T> operator * (const vector3d<T>& other) const;
	vector3d<T>& operator *= (const T v);
	vector3d<T> operator * (const T v) const;

	vector3d<T>& operator /= (const vector3d<T>& other);
	vector3d<T> operator / (const vector3d<T>& other) const;
	vector3d<T>& operator /= (const T v);
	vector3d<T> operator / (const T v) const;

	bool operator <= (const vector3d<T>& other) const;
	bool operator >= (const vector3d<T>& other) const;
	bool operator < (const vector3d<T>& other) const;
	bool operator > (const vector3d<T>& other) const;

	bool operator == (const vector3d<T>& other) const;

	bool operator != (const vector3d<T>& other) const;

	// functions

	//! returns if this vector equals the other one, taking floating point rounding errors into account
	bool equals(const vector3d<T>& other, const T tolerance = T(ROUNDING_ERROR_32)) const;

	vector3d<T>& set(const T val);
	vector3d<T>& set(const T nx, const T ny, const T nz);
	vector3d<T>& set(const vector3d<T>& p);
	vector3d<T>& set(const vector2d<T>& p);
	vector3d<T>& set(const vector2d<T>& p, T z);

	//! Get the dot product with another vector.
	T dotProduct(const vector3d<T>& other) const;

	//! Get squared length of the vector.
	/** This is useful because it is much faster than getLength().
		\return Squared length of the vector. */
	T getLengthSQ() const;

	//! Get length of the vector.
	T getLength() const;

	//! Returns squared distance from another point.
	/** Here, the vector is interpreted as point in 3 dimensional space. */
	T getDistanceFromSQ(const vector3d<T>& other) const;

	//! Get distance from another point.
	/** Here, the vector is interpreted as point in 3 dimensional space. */
	T getDistanceFrom(const vector3d<T>& other) const;

	//! Gets longest negative axis
	u32 getMinAxis() const;

	//! Gets longest positive axis
	u32 getMaxAxis() const;

	//! Gets shortest absolute axis
	u32 getMinorAxis() const;

	//! Gets longest absolute axis
	u32 getMajorAxis() const;

	//! Calculates the cross product with another vector.
	/** \param p Vector to multiply with.
		\return Crossproduct of this vector with p. */
	vector3d<T> crossProduct(const vector3d<T>& p) const;

	//! Returns if this vector interpreted as a point is on a line between two other points.
	/** It is assumed that the point is on the line.
		\param begin Beginning vector to compare between.
		\param end Ending vector to compare between.
		\return True if this vector is between begin and end, false if not. */
	bool isBetweenPoints(const vector3d<T>& begin, const vector3d<T>& end) const;

	//! Normalizes the vector.
	/** In case of the 0 vector the result is still 0, otherwise
		the length of the vector will be 1.
		TODO: 64 Bit template doesnt work.. need specialized template
		\return Reference to this vector after normalization. */
	vector3d<T>& normalize();

	//! Sets the length of the vector to a newly value
	vector3d<T>& setLength(T newlength);

	//! Inverts the vector.
	vector3d<T>& invert();

	//! Rotates the vector by a specified number of degrees around the Y axis and the specified center.
	/** \param degrees Number of degrees to rotate around the Y axis.
		\param center The center of the rotation. */
	vector3d<T>& rotateXZBy(f64 degrees, const vector3d<T>& center = vector3d<T>());

	//! Rotates the vector by a specified number of degrees around the Z axis and the specified center.
	/** \param degrees: Number of degrees to rotate around the Z axis.
		\param center: The center of the rotation. */
	vector3d<T>& rotateXYBy(f64 degrees, const vector3d<T>& center = vector3d<T>());

	//! Rotates the vector by a specified number of degrees around the X axis and the specified center.
	/** \param degrees: Number of degrees to rotate around the X axis.
		\param center: The center of the rotation. */
	vector3d<T>& rotateYZBy(f64 degrees, const vector3d<T>& center = vector3d<T>());

	//! Returns interpolated vector.
	/** \param other Other vector to interpolate between
		\param d Value between 0.0f and 1.0f. */
	vector3d<T> getInterpolated(const vector3d<T>& other, const T d) const;

	//! Returns interpolated vector. ( quadratic )
	/** \param v2 Second vector to interpolate with
		\param v3 Third vector to interpolate with
		\param d Value between 0.0f and 1.0f. */
	vector3d<T> getInterpolated_quadratic(const vector3d<T>& v2, const vector3d<T>& v3, const T d) const;

	//! Gets the Y and Z rotations of a vector.
	/** Thanks to Arras on the Irrlicht forums for this method.
		\return A vector representing the rotation in degrees of
		this vector. The Z component of the vector will always be 0. */
	vector3d<T> getHorizontalAngle() const;

	vector3d<T> rotationToDirection(const vector3d<T>& forwards) const;

	vector3d<T> clip(const vector3d<T>& min, const vector3d<T>& max) const;

	vector3d<T>& abs();

	T* getAs4Values(T* array, T w = T(0)) const;

	T* getAs3Values(T* array) const;

private:

	T Data[3];
};

} // end namespace jenny


namespace jenny
{

typedef vector3d<f32> vector3df;
typedef vector3d<s32> vector3di;

template < typename T >
inline
vector3d<T>&
vector3d<T>::set(const T val)
{
	Data[0] = val;
	Data[1] = val;
	Data[2] = val;
	return *this;
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::set(const T nx, const T ny, const T nz)
{
	Data[0] = nx;
	Data[1] = ny;
	Data[2] = nz;
	return *this;
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::set(const vector3d<T>& p)
{
	Data[0] = p.Data[0];
	Data[1] = p.Data[1];
	Data[2] = p.Data[2];
	return *this;
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::set(const vector2d<T>& p)
{
	Data[0] = p.getX();
	Data[1] = p.getY();
	return *this;
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::set(const vector2d<T>& p, T z)
{
	Data[0] = p.getX();
	Data[1] = p.getY();
	Data[2] = z;
	return *this;
}

template < typename T >
inline
T&
vector3d<T>::x()
{
	return Data[0];
}

template < typename T >
inline
T&
vector3d<T>::y()
{
	return Data[1];
}

template < typename T >
inline
T&
vector3d<T>::z()
{
	return Data[2];
}

template < typename T >
inline
const T&
vector3d<T>::x() const
{
	return Data[0];
}

template < typename T >
inline
const T&
vector3d<T>::y() const
{
	return Data[1];
}

template < typename T >
inline
const T&
vector3d<T>::z() const
{
	return Data[2];
}

template < typename T >
inline
const T&
vector3d<T>::getX() const
{
	return Data[0];
}

template < typename T >
inline
const T&
vector3d<T>::getY() const
{
	return Data[1];
}

template < typename T >
inline
const T&
vector3d<T>::getZ() const
{
	return Data[2];
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::setX(T val)
{
	Data[0] = val;
	return *this;
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::setY(T val)
{
	Data[1] = val;
	return *this;
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::setZ(T val)
{
	Data[2] = val;
	return *this;
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::incrX(const T val)
{
	Data[0] += val;
	return *this;
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::incrY(const T val)
{
	Data[1] += val;
	return *this;
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::incrZ(const T val)
{
	Data[2] += val;
	return *this;
}


template < typename T >
inline
vector3d<T>::vector3d()
{
	set(T(0));
}


template < typename T >
inline
vector3d<T>::vector3d(T nx, T ny, T nz)
{
	set(nx, ny, nz);
}

template < typename T >
inline
vector3d<T>::vector3d(T n)
{
	set(n);
}

template < typename T >
inline
vector3d<T>::vector3d(const T* p)
{
	set(p[0], p[1], p[2]);
}

template < typename T >
inline
vector3d<T>::vector3d(const vector3d<T>& other)
{
	set(other);
}

template < typename T >
template <typename T2>
inline
vector3d<T>::vector3d(const vector3d<T2>& other)
{
	set(T(other.getX()), T(other.getY()), T(other.getZ()));
}

template < typename T >
template <typename T2>
inline
vector3d<T>::vector3d(const vector2d<T2>& xy, T z)
{
	set(T(xy.getX()), T(xy.getY()), z);
}

template < typename T >
inline
const T*
vector3d<T>::getDataPtr() const
{
	return Data;
}

template < typename T >
inline
T*
vector3d<T>::getDataPtr()
{
	return Data;
}

template < typename T >
inline
T&
vector3d<T>::operator [] (u32 i)
{
	GLITCH_DEBUG_BREAK_IF(i >= 3);
	return getDataPtr()[i];
}

template < typename T >
inline
const T&
vector3d<T>::operator [] (u32 i) const
{
	GLITCH_DEBUG_BREAK_IF(i >= 3);
	return getDataPtr()[i];
}

template < typename T >
inline
vector3d<T>
vector3d<T>::operator - () const
{
	return vector3d<T>(-getX(), -getY(), -getZ());
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::operator = (const vector3d<T>& other)
{
	return set(other);
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::operator += (const vector3d<T>& other)
{
	Data[0] += other.Data[0];
	Data[1] += other.Data[1];
	Data[2] += other.Data[2];
	return *this;
}

template < typename T >
inline
vector3d<T>
vector3d<T>::operator + (const vector3d<T>& other) const
{
	vector3d<T> result(*this);
	result += other;
	return result;
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::operator += (const T val)
{
	Data[0] += val;
	Data[1] += val;
	Data[2] += val;
	return *this;
}

template < typename T >
inline
vector3d<T>
vector3d<T>::operator + (const T val) const
{
	vector3d<T> result(*this);
	result += val;
	return result;
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::operator -= (const vector3d<T>& other)
{
	Data[0] -= other.Data[0];
	Data[1] -= other.Data[1];
	Data[2] -= other.Data[2];
	return *this;
}

template < typename T >
inline
vector3d<T>
vector3d<T>::operator - (const vector3d<T>& other) const
{
	vector3d<T> result(*this);
	result -= other;
	return result;
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::operator -= (const T val)
{
	Data[0] -= val;
	Data[1] -= val;
	Data[2] -= val;
	return *this;
}

template < typename T >
inline
vector3d<T>
vector3d<T>::operator - (const T val) const
{
	vector3d<T> result(*this);
	result -= val;
	return result;
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::operator *= (const vector3d<T>& other)
{
	Data[0] *= other.Data[0];
	Data[1] *= other.Data[1];
	Data[2] *= other.Data[2];
	return *this;
}

template < typename T >
inline
vector3d<T>
vector3d<T>::operator * (const vector3d<T>& other) const
{
	vector3d<T> result(*this);
	result *= other;
	return result;
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::operator *= (const T v)
{
	Data[0] *= v;
	Data[1] *= v;
	Data[2] *= v;
	return *this;
}

template < typename T >
inline
vector3d<T>
vector3d<T>::operator * (const T v) const
{
	vector3d<T> result(*this);
	result *= v;
	return result;
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::operator /= (const vector3d<T>& other)
{
	Data[0] /= other.Data[0];
	Data[1] /= other.Data[1];
	Data[2] /= other.Data[2];
	return *this;
}

template < typename T >
inline
vector3d<T>
vector3d<T>::operator / (const vector3d<T>& other) const
{
	vector3d<T> result(*this);
	result /= other;
	return result;
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::operator /= (const T v)
{
	T i = T(1) / v;
	Data[0] *= i;
	Data[1] *= i;
	Data[2] *= i;
	return *this;
}

template < typename T >
inline
vector3d<T>
vector3d<T>::operator / (const T v) const
{
	vector3d<T> result(*this);
	result /= v;
	return result;
}

template < typename T >
inline
bool
vector3d<T>::operator <= (const vector3d<T>& other) const
{
	return Data[0] <= other.Data[0] && Data[1] <= other.Data[1] && Data[2] <= other.Data[2];
}

template < typename T >
inline
bool
vector3d<T>::operator >= (const vector3d<T>& other) const
{
	return Data[0] >= other.Data[0] && Data[1] >= other.Data[1] && Data[2] >= other.Data[2];
}

template < typename T >
inline
bool
vector3d<T>::operator < (const vector3d<T>& other) const
{
	return Data[0] < other.Data[0] && Data[1] < other.Data[1] && Data[2] < other.Data[2];
}

template < typename T >
inline
bool
vector3d<T>::operator > (const vector3d<T>& other) const
{
	return Data[0] > other.Data[0] && Data[1] > other.Data[1] && Data[2] > other.Data[2];
}

template < typename T >
inline
bool
vector3d<T>::operator == (const vector3d<T>& other) const
{
	return Data[0] == other.Data[0] && Data[1] == other.Data[1] && Data[2] == other.Data[2];
}

template < typename T >
inline
bool
vector3d<T>::operator!=(const vector3d<T>& other) const
{
	return Data[0] != other.Data[0] || Data[1] != other.Data[1] || Data[2] != other.Data[2];
}

template < typename T >
inline
bool
vector3d<T>::equals(const vector3d<T>& other, const T tolerance) const
{
	return (core::equals(Data[0], other.Data[0], tolerance)
			&& core::equals(Data[1], other.Data[1], tolerance)
			&& core::equals(Data[2], other.Data[2], tolerance));
}

template < typename T >
inline
T
vector3d<T>::dotProduct(const vector3d<T>& other) const
{
	return Data[0] * other.Data[0] + Data[1] * other.Data[1] + Data[2] * other.Data[2];
}

template < typename T >
inline
T
vector3d<T>::getLengthSQ() const
{
	return dotProduct(*this);
}

template < typename T >
inline
T
vector3d<T>::getLength() const
{
	typedef typename boost::mpl::if_<boost::mpl::not_<boost::is_floating_point<T> >, float, T>::type TCast;
	return T(glf::Sqrt(TCast(getLengthSQ())));
}

template < typename T >
inline
T
vector3d<T>::getDistanceFromSQ(const vector3d<T>& other) const
{
	return ((*this) - other).getLengthSQ();
}

template < typename T >
inline
T
vector3d<T>::getDistanceFrom(const vector3d<T>& other) const
{
	return ((*this) - other).getLength();
}

template < typename T >
inline
u32
vector3d<T>::getMinAxis() const
{
	return (Data[0] < Data[1]
			? (Data[0] < Data[2]
			   ? 0
			   : 2)
			: (Data[1] < Data[2]
			   ? 1
			   : 2));
}

template < typename T >
inline
u32
vector3d<T>::getMaxAxis() const
{
	return (Data[0] > Data[1]
			? (Data[0] > Data[2]
			   ? 0
			   : 2)
			: (Data[1] > Data[2]
			   ? 1
			   : 2));
}

template < typename T >
inline
u32
vector3d<T>::getMinorAxis() const
{
	const T absData[3] = {glf::Abs(Data[0]), glf::Abs(Data[1]), glf::Abs(Data[2])};
	return (absData[0] < absData[1]
			? (absData[0] < absData[2]
			   ? 0
			   : 2)
			: (absData[1] < absData[2]
			   ? 1
			   : 2));
}

template < typename T >
inline
u32
vector3d<T>::getMajorAxis() const
{
	const T absData[3] = {glf::Abs(Data[0]), glf::Abs(Data[1]), glf::Abs(Data[2])};
	return (absData[0] > absData[1]
			? (absData[0] > absData[2]
			   ? 0
			   : 2)
			: (absData[1] > absData[2]
			   ? 1
			   : 2));
}

template < typename T >
inline
vector3d<T>
vector3d<T>::crossProduct(const vector3d<T>& p) const
{
	return vector3d<T>(Data[1] * p.Data[2] - Data[2] * p.Data[1],
					   Data[2] * p.Data[0] - Data[0] * p.Data[2],
					   Data[0] * p.Data[1] - Data[1] * p.Data[0]);
}

template < typename T >
inline
bool
vector3d<T>::isBetweenPoints(const vector3d<T>& begin, const vector3d<T>& end) const
{
	const T f = (end - begin).getLengthSQ();
	return getDistanceFromSQ(begin) <= f && getDistanceFromSQ(end) <= f;
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::normalize()
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
vector3d<T>&
vector3d<T>::setLength(T newlength)
{
	normalize();
	return (*this *= newlength);
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::invert()
{
	return (*this) *= T(-1);
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::rotateXZBy(f64 degrees, const vector3d<T>& center)
{
	degrees *= DEGTORAD64;
	T cs = T(glf::Cos(degrees));
	T sn = T(glf::Sin(degrees));
	Data[0] -= center[0];
	Data[2] -= center[2];
	set(Data[0] * cs - Data[2] * sn, Data[1], Data[0] * sn + Data[2] * cs);
	Data[0] += center[0];
	Data[2] += center[2];
	return *this;
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::rotateXYBy(f64 degrees, const vector3d<T>& center)
{
	degrees *= DEGTORAD64;
	T cs = T(glf::Cos(degrees));
	T sn = T(glf::Sin(degrees));
	Data[0] -= center[0];
	Data[1] -= center[1];
	set(Data[0] * cs - Data[1] * sn, Data[0] * sn + Data[1] * cs, Data[2]);
	Data[0] += center[0];
	Data[1] += center[1];
	return *this;
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::rotateYZBy(f64 degrees, const vector3d<T>& center)
{
	degrees *= DEGTORAD64;
	T cs = T(glf::Cos(degrees));
	T sn = T(glf::Sin(degrees));
	Data[2] -= center[2];
	Data[1] -= center[1];
	set(Data[0], Data[1] * cs - Data[2] * sn, Data[1] * sn + Data[2] * cs);
	Data[2] += center[2];
	Data[1] += center[1];
	return *this;
}

template < typename T >
inline
vector3d<T>
vector3d<T>::getInterpolated(const vector3d<T>& other, const T d) const
{
	return *this + ((other - *this) * d);
}

template < typename T >
inline
vector3d<T>
vector3d<T>::getInterpolated_quadratic(const vector3d<T>& v2, const vector3d<T>& v3, const T d) const
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
vector3d<T>
vector3d<T>::getHorizontalAngle() const
{
	vector3d<T> angle;

	angle[1] = T(glf::Atan2(f64(Data[0]), f64(Data[2])) * RADTODEG64);

	if(angle[1] < T(0))
	{
		angle[1] += T(360);
	}
	if(angle[1] >= T(360))
	{
		angle[1] -= T(360);
	}

	const f64 z1 = glf::Sqrt(f64(Data[0]) * f64(Data[0]) + f64(Data[2]) * f64(Data[2]));

	angle[0] = T(glf::Atan2(z1, f64(Data[1])) * RADTODEG64 - 90.0);

	if(angle[0] < T(0))
	{
		angle[0] += T(360);
	}
	if(angle[0] >= T(360))
	{
		angle[0] -= T(360);
	}

	return angle;
}

template < typename T >
inline
vector3d<T>
vector3d<T>::rotationToDirection(const vector3d<T> & forwards) const
{
	const f64 cr = glf::Cos(DEGTORAD64 * f64(Data[0]));
	const f64 sr = glf::Sin(DEGTORAD64 * f64(Data[0]));
	const f64 cp = glf::Cos(DEGTORAD64 * f64(Data[1]));
	const f64 sp = glf::Sin(DEGTORAD64 * f64(Data[1]));
	const f64 cy = glf::Cos(DEGTORAD64 * f64(Data[2]));
	const f64 sy = glf::Sin(DEGTORAD64 * f64(Data[2]));

	const f64 srsp = sr * sp;
	const f64 crsp = cr * sp;

	const f64 pseudoMatrix[] =
		{ cp * cy, cp * sy, -sp,
		  srsp * cy - cr * sy, srsp * sy + cr * cy, sr * cp,
		  crsp * cy + sr * sy, crsp * sy - sr * cy, cr * cp };

	return vector3d<T>(T(f64(forwards[0]) * pseudoMatrix[0]
						 + f64(forwards[1]) * pseudoMatrix[3]
						 + f64(forwards[2]) * pseudoMatrix[6]),
					   T(f64(forwards[0]) * pseudoMatrix[1]
						 + f64(forwards[1]) * pseudoMatrix[4]
						 + f64(forwards[2]) * pseudoMatrix[7]),
					   T(f64(forwards[0]) * pseudoMatrix[2]
						 + f64(forwards[1]) * pseudoMatrix[5]
						 + f64(forwards[2]) * pseudoMatrix[8]));
}

template < typename T>
inline
vector3d<T>
vector3d<T>::clip(const vector3d& min, const vector3d& max) const
{
	return vector3d<T>(
		glf::Clamp(getX(), min.getX(), max.getX()),
		glf::Clamp(getY(), min.getY(), max.getY()),
		glf::Clamp(getZ(), min.getZ(), max.getZ()));
}

template < typename T >
inline
vector3d<T>&
vector3d<T>::abs()
{
	Data[0] = glf::Abs(Data[0]);
	Data[1] = glf::Abs(Data[1]);
	Data[2] = glf::Abs(Data[2]);
	return *this;
}

template < typename T >
inline
T*
vector3d<T>::getAs4Values(T* array, T w) const
{
	array[0] = Data[0];
	array[1] = Data[1];
	array[2] = Data[2];
	array[3] = w;
	return array;
}

template < typename T >
inline
T*
vector3d<T>::getAs3Values(T* array) const
{
	array[0] = Data[0];
	array[1] = Data[1];
	array[2] = Data[2];
	return array;
}

//------------------------------------------------------------------------------

//! Function multiplying a scalar and a vector component-wise.
template < typename T >
inline
vector3d<T>
operator * (const T scalar, const vector3d<T>& vector)
{
	return vector * scalar;
}

template < typename T >
inline
vector3d<T>
normalize(const vector3d<T>& vector)
{
	vector3d<T> result(vector);
	result.normalize();
	return result;
}

template < typename T >
inline
vector3d<T>
abs(const vector3d<T>& vector)
{
	vector3d<T> result(vector);
	result.abs();
	return result;
}

//------------------------------------------------------------------------------
// vector2d definitions depending on vector3d defs
//------------------------------------------------------------------------------

template < typename T >
inline
vector2d<T>&
vector2d<T>::set(const vector3d<T>& other)
{
	Data[0] = other.getX();
	Data[1] = other.getY();
	return *this;
}

template < typename T >
template <typename T2>
inline
vector2d<T>::vector2d(const vector3d<T2>& other)
{
	set(T(other.getX()), T(other.getY()));
}

} // end namespace glitch

