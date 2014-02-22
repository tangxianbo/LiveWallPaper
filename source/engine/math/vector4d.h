#pragma once
#include <type_traits>
#include <math/vector3d.h>

namespace jenny
{

template<typename T>
class vector4d
{

public:

	vector4d()
	{
		Data[0] = 0;
		Data[1] = 0;
		Data[2] = 0;
		Data[3] = 0;
	}

	vector4d(T x, T y, T z, T w = 0)
	{
		Data[0] = x;
		Data[1] = y;
		Data[2] = z;
		Data[3] = w;
	}

	explicit vector4d(T n)
	{
		Data[0] = n;
		Data[1] = n;
		Data[2] = n;
		Data[3] = n;
	}

	vector4d(const vector4d<T>& other)
	{
		(*this)[0] = other[0];
		(*this)[1] = other[1];
		(*this)[2] = other[2];
		(*this)[3] = other[3];
	}

	//! Copy conversion constructor
	template < typename T2 >
	explicit vector4d(const vector4d<T2>& other)
	{
			(*this)[0] = other.getX();
			(*this)[1] = other.getY();
			(*this)[2] = other.getZ();
			(*this)[3] = other.getW();
	}

	//!
	template < typename T2 >
	explicit vector4d(const vector3d<T2>& xyz, T w = T(0))
	{
			(*this)[0] = other.getX();
			(*this)[1] = other.getY();
			(*this)[2] = other.getZ();
			(*this)[3] = w;
	}

	template < typename T2 >
	explicit vector4d(const vector2d<T2>& xy, T z = T(0), T w = T(0))
	{
			(*this)[0] = other.getX();
			(*this)[1] = other.getY();
			(*this)[2] = z;
			(*this)[3] = w;
	}

	void set(const vector4d<T>& other)
	{
		(*this)[0] = other[0];
		(*this)[1] = other[1];
		(*this)[2] = other[2];
		(*this)[3] = other[3];
	}

	void set(const vector3d<T>& other)
	{
		(*this)[0] = other[0];
		(*this)[1] = other[1];
		(*this)[2] = other[2];
		(*this)[3] = T(1);
	}

	T& operator[] (u32 i)
	{
		return Data[i];
	}

	const T& operator[] (u32 i) const
	{
		return Data[i];
	}

	T* getDataPtr()
	{
		return reinterpret_cast<T*>(this);
	}

	const T* getDataPtr() const
	{
		return reinterpret_cast<const T*>(this);
	}

	T& x() { return Data[0]; }
	T& y() { return Data[1]; }
	T& z() { return Data[2]; }
	T& w() { return Data[3]; }

	const T& x() const { return Data[0]; }
	const T& y() const { return Data[1]; }
	const T& z() const { return Data[2]; }
	const T& w() const { return Data[3]; }

	T getX() const { return Data[0]; }
	T getY() const { return Data[1]; }
	T getZ() const { return Data[2]; }
	T getW() const { return Data[3]; }

	void setX(T val) { Data[0] = val; }
	void setY(T val) { Data[1] = val; }
	void setZ(T val) { Data[2] = val; }
	void setW(T val) { Data[3] = val; }

	vector4d operator - () const
	{
		return vector4d(-Data[0],
						-Data[1],
						-Data[2],
						-Data[3]);
	}
	
	vector4d& operator = (const vector4d& other)
	{
		for(u32 i = 0; i < 4; ++i)
		{
			Data[i] = other.Data[i];
		}
		return *this;
	}

	vector4d& operator = (const vector3d<T>& other)
	{
		this->set(other);
		return *this;
	}

	bool operator <= (const vector4d& other) const
	{
		return this->getX() <= other.getX() &&
			this->getY() <= other.getY() &&
			this->getZ() <= other.getZ() &&
			this->getW() <= other.getW();
	}

	bool operator >= (const vector4d& other) const
	{
		return this->getX() >= other.getX() &&
			this->getY() >= other.getY() &&
			this->getZ() >= other.getZ() &&
			this->getW() >= other.getW();
	}

	bool operator < (const vector4d& other) const
	{
		return this->getX() < other.getX() &&
			this->getY() < other.getY() &&
			this->getZ() < other.getZ() &&
			this->getW() < other.getW();
	}

	bool operator > (const vector4d& other) const
	{
		return this->getX() > other.getX() &&
			this->getY() > other.getY() &&
			this->getZ() > other.getZ() &&
			this->getW() > other.getW();
	}

	vector4d operator + (const vector4d& other) const
	{
		return vector4d(Data[0] + other.Data[0],
						Data[1] + other.Data[1],
						Data[2] + other.Data[2],
						Data[3] + other.Data[3]);
	}

	bool operator == (const vector4d& other) const
	{
		return this->getX() == other.getX() &&
			this->getY() == other.getY() &&
			this->getZ() == other.getZ() &&
			this->getW() == other.getW();
	}

	bool operator != (const vector4d& other) const
	{
		return this->getX() != other.getX() ||
			this->getY() != other.getY() ||
			this->getZ() != other.getZ() ||
			this->getW() != other.getW();
	}

	vector4d& operator += (const vector4d& other)
	{
		for(u32 i = 0; i < 4; ++i)
		{
			Data[i] += other.Data[i];
		}
		return *this;
	}
	
	vector4d operator + (const T val) const
	{
		return vector4d(Data[0] + val,
						Data[1] + val,
						Data[2] + val,
						Data[3] + val);
	}

	vector4d& operator += (const T val)
	{
		for(u32 i = 0; i < 4; ++i)
		{
			Data[i] += val;
		}
		return *this;
	}

	vector4d operator - (const vector4d& other) const
	{
		return vector4d(Data[0] - other.Data[0],
						Data[1] - other.Data[1],
						Data[2] - other.Data[2],
						Data[3] - other.Data[3]);
	}

	vector4d& operator -= (const vector4d& other)
	{
		for(u32 i = 0; i < 4; ++i)
		{
			Data[i] -= other.Data[i];
		}
		return *this;
	}

	vector4d operator - (const T val) const
	{
		return vector4d(Data[0] - val, Data[1] - val, Data[2] - val, Data[3] - val);
	}
	
	vector4d& operator -= (const T val)
	{
		for(u32 i = 0; i < 4; ++i)
		{
			Data[i] -= val;
		}
		return *this;
	}

	vector4d operator * (const vector4d& other) const
	{
		return vector4d(Data[0] * other.Data[0],
						Data[1] * other.Data[1],
						Data[2] * other.Data[2],
						Data[3] * other.Data[3]);
	}
	
	vector4d& operator *= (const vector4d& other)
	{
		for(u32 i = 0; i < 4; ++i)
		{
			Data[i] *= other.Data[i];
		}
		return *this;
	}
	
	vector4d operator * (const T v) const
	{
		return vector4d(Data[0] * v,
						Data[1] * v,
						Data[2] * v,
						Data[3] * v);
	}
	
	vector4d& operator *= (const T v)
	{
		for(u32 i = 0; i < 4; ++i)
		{
			Data[i] *= v;
		}
		return *this;
	}

	vector4d operator / (const vector4d& other) const
	{
		return vector4d(Data[0] / other.Data[0],
						Data[1] / other.Data[1],
						Data[2] / other.Data[2],
						Data[3] / other.Data[3]);
	}
	
	vector4d& operator /= (const vector4d& other)
	{
		for(u32 i = 0; i < 4; ++i)
		{
			Data[i] /= other.Data[i];
		}
		return *this;
	}

	vector4d operator / (const T v) const
	{
		return Vector4Div(v);
	}

	vector4d& operator /= (const T v)
	{
		return Vector4DivEq(v);
	}

	T getLength() const
	{
		return T(Sqrt(f64(getLengthSQ())));
	}

	T dotProduct(const vector4d& other) const
	{
		return (Data[0] * other.Data[0]
				+ Data[1] * other.Data[1]
				+ Data[2] * other.Data[2]
				+ Data[3] * other.Data[3]);
	}

	T getLengthSQ() const
	{
		return dotProduct(*this);
	}


	vector4d& normalize()
	{
		T l = getLengthSQ();
		if (l != T(0))
		{
			l = T(reciprocal_squareroot(f32(l)));
			*this *= l;
		}
		return *this;
	}


	vector4d getInterpolated(const vector4d& other, const T d) const
	{
		return *this + (other - *this) * d;
	}

	bool equals(const vector4d& other,
		const T tolerance = T(ROUNDING_ERROR_32)) const
	{
		return (core::equals(this->getX(), other.getX(), tolerance) &&
			core::equals(this->getY(), other.getY(), tolerance) &&
			core::equals(this->getZ(), other.getZ(), tolerance) &&
			core::equals(this->getW(), other.getW(), tolerance));
	}

	vector3d<T> getXYZ() const
	{
		return vector3d<T>(this->getX(), this->getY(), this->getZ());
	}

	vector4d<T>& set(const T nx, const T ny, const T nz, const T nw)
	{
		this->setX(nx);
		this->setY(ny);
		this->setZ(nz);
		this->setW(nw);
		return *this;
	}

	//! Get distance from another point.
	/** Here, the vector is interpreted as point in 3 dimensional space. */
	T getDistanceFrom(const vector4d& other) const
	{
		return (*this - other).getLength();
	}

	//! Returns squared distance from another point.
	/** Here, the vector is interpreted as point in 3 dimensional space. */
	T getDistanceFromSQ(const vector4d& other) const
	{
		return (*this - other).getLengthSQ();
	}

	//! Returns if this vector interpreted as a point is on a line between two other points.
	/** It is assumed that the point is on the line.
	\param begin Beginning vector to compare between.
	\param end Ending vector to compare between.
	\return True if this vector is between begin and end, false if not. */
	bool isBetweenPoints(const vector4d& begin, const vector4d& end) const
	{
		const T f = end.getDistanceFromSQ(begin);
		return (getDistanceFromSQ(begin) <= f && getDistanceFromSQ(end) <= f);
	}

	//! Sets the length of the vector to a newly value
	vector4d& setLength(T newlength)
	{
		this->normalize();
		return (*this *= newlength);
	}

	//! Inverts the vector.
	vector4d& invert()
	{
		return *this = -*this;
	}

	//! Returns interpolated vector. ( quadratic )
	/** \param v2 Second vector to interpolate with
	\param v3 Third vector to interpolate with
	\param d Value between 0.0f and 1.0f. */
	vector4d<T> getInterpolated_quadratic(const vector4d& v2,
		const vector4d& v3,
		const T d) const
	{
		// this*(1-d)*(1-d) + 2 * v2 * (1-d) + v3 * d * d;
		const T inv = T(1) - d;
		const T mul0 = inv * inv;
		const T mul1 = T(2) * d * inv;
		const T mul2 = d * d;
		return vector4d(this->getX() * mul0 + v2.getX() * mul1 + v3.getX() * mul2,
			this->getY() * mul0 + v2.getY() * mul1 + v3.getY() * mul2,
			this->getZ() * mul0 + v2.getZ() * mul1 + v3.getZ() * mul2,
			this->getW() * mul0 + v2.getW() * mul1 + v3.getW() * mul2);
	}

	//! Fills an array of 4 values with the vector data (usually floats).
	/** Useful for setting in shader constants for example. The fourth value
	will always be 0. */
	void getAs4Values(T* ar) const
	{
		ar[0] = this->getX();
		ar[1] = this->getY();
		ar[2] = this->getZ();
		ar[3] = this->getW();
	}

	//! Rotates the vector by a specified number of degrees around the Y axis and the specified center.
	/** \param degrees Number of degrees to rotate around the Y axis.
	\param center The center of the rotation. */
	void rotateXZBy(f64 degrees, const vector3d<T>& center = vector3d<T>())
	{
		degrees *= DEGTORAD64;
		T cs = (T)glf::Cos(degrees);
		T sn = (T)glf::Sin(degrees);
		this->setX(this->getX() - center.getX());
		this->setZ(this->getZ() - center.getZ());
		set(this->getX() * cs - this->getZ() * sn, this->getY(), this->getX() * sn + this->getZ() * cs);
		this->setX(this->getX() + center.getX());
		this->setZ(this->getZ() + center.getZ());
	}

	//! Rotates the vector by a specified number of degrees around the Z axis and the specified center.
	/** \param degrees: Number of degrees to rotate around the Z axis.
	\param center: The center of the rotation. */
	void rotateXYBy(f64 degrees, const vector3d<T>& center = vector3d<T>())
	{
		degrees *= DEGTORAD64;
		T cs = (T)glf::Cos(degrees);
		T sn = (T)glf::Sin(degrees);
		this->setX(this->getX() - center.getX());
		this->setY(this->getY() - center.getY());
		set(this->getX() * cs - this->getY() * sn, this->getX() * sn + this->getY() * cs, this->getZ());
		this->setX(this->getX() + center.getX());
		this->setY(this->getY() + center.getY());
	}

	//! Rotates the vector by a specified number of degrees around the X axis and the specified center.
	/** \param degrees: Number of degrees to rotate around the X axis.
	\param center: The center of the rotation. */
	void rotateYZBy(f64 degrees, const vector3d<T>& center = vector3d<T>())
	{
		degrees *= DEGTORAD64;
		T cs = (T)glf::Cos(degrees);
		T sn = (T)glf::Sin(degrees);
		this->setZ(this->getZ() - center.getZ());
		this->setY(this->getY() - center.getY());
		set(this->getX(), this->getY() * cs - this->getZ() * sn, this->getY() * sn + this->getZ() * cs);
		this->setZ(this->getZ() + center.getZ());
		this->setY(this->getY() + center.getY());
	}

private:

	T Data[4];

	template <typename U>
	typename std::enable_if<std::is_floating_point<U>::value, vector4d>::type Vector4Div(const U divisor) const
	{
		U inv_divisor = U(1) / divisor;
		return vector4d(Data[0] * inv_divisor,
						Data[1] * inv_divisor,
						Data[2] * inv_divisor,
						Data[3] * inv_divisor);
	}

	template <typename U>
	typename std::enable_if<std::is_floating_point<U>::value == false, vector4d>::type Vector4Div(const U divisor) const
	{
		return vector4d(Data[0] / divisor,
			Data[1] / divisor,
			Data[2] / divisor,
			Data[3] / divisor);
	}

	template <typename U>
	typename std::enable_if<std::is_floating_point<U>::value, vector4d&>::type Vector4DivEq(const U divisor)
	{
		U inv_divisor = U(1) / divisor;

		Data[0] *= inv_divisor;
		Data[1] *= inv_divisor;
		Data[2] *= inv_divisor;
		Data[3] *= inv_divisor;

		return *this;
	}

	template <typename U>
	typename std::enable_if<std::is_floating_point<U>::value == false, vector4d&>::type Vector4DivEq(const U divisor)
	{
		Data[0] /= divisor;
		Data[1] /= divisor;
		Data[2] /= divisor;
		Data[3] /= divisor;

		return *this;
	}
};

typedef vector4d<f32> vector4df;
typedef vector4d<s32> vector4di;

} //jenny
