#pragma once

#include <core/types.h>
#include <math/math.h>
#include <math/vector3d.h>
#include <math/vector4d.h>
#include <math/matrix4.h>

namespace jenny
{

//! Quaternion class for representing rotations.
/** It provides cheap combinations and avoids gimbal locks.
	Also useful for interpolations. */
class quaternion
{
public:

	//! Default Constructor
	quaternion();

	//! Constructor
	quaternion(f32 x, f32 y, f32 z, f32 w);

	//! Constructor which converts euler angles (radians) to a quaternion
	quaternion(f32 x, f32 y, f32 z);

	//! Constructor which converts euler angles (radians) to a quaternion
	quaternion(const vector3df& vec);

	//! Constructor which converts a matrix to a quaternion
	quaternion(const matrix4& mat);

	//! Equalilty operator
	bool operator == (const quaternion& other) const;

	//! Inequalilty operator
	bool operator != (const quaternion& other) const;

	//! Assignment operator
	quaternion& operator = (const quaternion& other);

	//! Matrix assignment operator
	quaternion& operator = (const matrix4& other);

	//! Add operator
	quaternion operator + (const quaternion& other) const;

	//! Multiplication operator
	quaternion operator * (const quaternion& other) const;

	//! Multiplication operator with scalar
	quaternion operator * (f32 s) const;

	//! Multiplication operator with scalar
	quaternion& operator *= (f32 s);

	//! Multiplication operator
	vector3df operator * (const vector3df& v) const;

	//! Multiplication operator
	quaternion& operator *= (const quaternion& other);

	//!
	f32& operator [] (u32 i);

	//!
	const f32& operator [] (u32 i) const;

	//! Calculates the dot product
	f32 dotProduct(const quaternion& other) const;

	//! Sets newly quaternion
	quaternion& set(f32 x, f32 y, f32 z, f32 w);

	//! Sets newly quaternion based on euler angles (radians)
	quaternion& set(f32 x, f32 y, f32 z);

	//! Sets newly quaternion based on euler angles (radians)
	quaternion& set(const vector3df& vec);

	//! Normalizes the quaternion
	quaternion& normalize();

	//! Return an amplified rotation angle
	quaternion amplify(float factor);

	//! Creates a matrix from this quaternion
	matrix4 getMatrix() const;
	matrix4 getMatrix_transposed() const;

	//! Creates a matrix from this quaternion
	void getMatrix(matrix4& dest) const;

	//! Creates a matrix from this quaternion
	void getMatrix_transposed(matrix4& dest) const;

	//! Return true if it is an identity quaternion (0, 0, 0, 1)
	bool isIdentity() const;

	//! Inverts this quaternion
	quaternion& makeInverse();

	//! Set this quaternion to the result of the angular interpolation between two quaternions
	quaternion& slerp(quaternion q1, quaternion q2, f32 interpolate);

	//! Set this quaternion to the result of the linear interpolation between two quaternions
	quaternion& nlerp(quaternion q1, quaternion q2, f32 interpolate);
	
	//! Create quaternion from rotation angle and rotation axis.
	/** Axis must be unit length.
		The quaternion representing the rotation is
		q = Cos(A/2)+Sin(A/2)*(x*i+y*j+z*k).
		\param angle Rotation Angle in radians.
		\param axis Rotation axis. */
	quaternion& fromAngleAxis(f32 angle, const vector3df& axis);

	//! Fills an angle (radians) around an axis (unit vector)
	void toAngleAxis(f32& angle, vector3df& axis) const;

	//! Output this quaternion to an euler angle (radians)
	void toEuler(vector3df& euler) const;

	//! Output this quaternion to an euler angle (360 degree)
	void toEulerDegree(vector3df& euler) const;

	//! Set quaternion to identity
	quaternion& makeIdentity();

	//! Set quaternion to be the shortest rotation.
	/** If the angle is > 180 or < -180 it compute the complementary
	*/
	quaternion& makeShortest();

	//! Set quaternion to represent a rotation from one vector to another.
	quaternion& rotationFromTo(const vector3df& from, const vector3df& to);

	f32 getX() const;
	f32 getY() const;
	f32 getZ() const;
	f32 getW() const;

	void setX(f32 val);
	void setY(f32 val);
	void setZ(f32 val);
	void setW(f32 val);

private:

	//! Quaternion elements.
	vector4df Data;
};

inline
f32
quaternion::getX() const
{
	return Data.getX();
}

inline
f32
quaternion::getY() const
{
	return Data.getY();
}

inline
f32
quaternion::getZ() const
{
	return Data.getZ();
}

inline
f32
quaternion::getW() const
{
	return Data.getW();
}

inline
void
quaternion::setX(f32 val)
{
	Data.setX(val);
}

inline
void
quaternion::setY(f32 val)
{
	Data.setY(val);
}

inline
void
quaternion::setZ(f32 val)
{
	Data.setZ(val);
}

inline
void
quaternion::setW(f32 val)
{
	Data.setW(val);
}


inline
quaternion::quaternion()
	: Data(0.0f, 0.0f, 0.0f, 1.0f)
{
}

inline
quaternion::quaternion(f32 x, f32 y, f32 z, f32 w)
	: Data(x, y, z, w)
{
}

inline
quaternion&
quaternion::normalize()
{
	Data.normalize();
	return *this;
}

inline
quaternion&
quaternion::set(f32 x, f32 y, f32 z, f32 w)
{
	Data.set(x, y, z, w);
	return *this;
}

inline
quaternion&
quaternion::set(f32 x, f32 y, f32 z)
{
	f64 angle;

	angle = f64(x) * 0.5;
	const f64 sr = Sin(angle);
	const f64 cr = Cos(angle);

	angle = f64(y) * 0.5;
	const f64 sp = Sin(angle);
	const f64 cp = Cos(angle);

	angle = f64(z) * 0.5;
	const f64 sy = Sin(angle);
	const f64 cy = Cos(angle);

	const f64 cpcy = cp * cy;
	const f64 spcy = sp * cy;
	const f64 cpsy = cp * sy;
	const f64 spsy = sp * sy;

	set(f32(sr * cpcy - cr * spsy),
		f32(cr * spcy + sr * cpsy),
		f32(cr * cpsy - sr * spcy),
		f32(cr * cpcy + sr * spsy));

	return normalize();
}

inline
quaternion::quaternion(f32 x, f32 y, f32 z)
	: Data(0)
{
	set(x, y, z);
}

inline
quaternion::quaternion(const vector3df& vec)
	: Data(0)
{
	set(vec.getX(), vec.getY(), vec.getZ());
}

inline
bool
quaternion::operator == (const quaternion& other) const
{
	return Data == other.Data;
}

inline
bool
quaternion::operator != (const quaternion& other) const
{
	return Data != other.Data;
}

inline
quaternion&
quaternion::operator = (const quaternion& other)
{
	Data = other.Data;
	return *this;
}

inline
quaternion&
quaternion::makeInverse()
{
	setX(-getX());
	setY(-getY());
	setZ(-getZ());
	return *this;
}

inline
quaternion&
quaternion::operator = (const matrix4& m)
{
	const f32 diag = m(0, 0) + m(1, 1) + m(2, 2);

	if(diag > 0.0f)
	{
		f32 scale = Sqrt(1.0f + diag); // get scale from diagonal
		f32 invScale = 0.5f / scale;

		// TODO: speed this up
		Data.set((m(2, 1) - m(1, 2)) * invScale,
				 (m(0, 2) - m(2, 0)) * invScale,
				 (m(1, 0) - m(0, 1)) * invScale,
				 0.5f * scale);
	}
	else if(m(0, 0) > m(1, 1) && m(0, 0) > m(2, 2))
	{
		// 1st element of diag is greatest value
		// find scale according to 1st element
		f32 scale = Sqrt(1.0f + m(0, 0) - m(1, 1) - m(2, 2));
		f32 invScale = 0.5f / scale;

		Data.set(0.5f * scale,
				 (m(0, 1) + m(1, 0)) * invScale,
				 (m(2, 0) + m(0, 2)) * invScale,
				 (m(2, 1) - m(1, 2)) * invScale);
	}
	else if(m(1, 1) > m(2, 2))
	{
		// 2nd element of diag is greatest value
		// find scale according to 2nd element
		f32 scale = Sqrt(1.0f + m(1, 1) - m(0, 0) - m(2, 2));
		f32 invScale = 0.5f / scale;

		// TODO: speed this up
		Data.set((m(0, 1) + m(1, 0)) * invScale,
				 0.5f * scale,
				 (m(1, 2) + m(2, 1)) * invScale,
				 (m(0, 2) - m(2, 0)) * invScale);
	}
	else
	{
		// 3rd element of diag is greatest value
		// find scale according to 3rd element
		f32 scale = Sqrt(1.0f + m(2, 2) - m(0, 0) - m(1, 1));
		f32 invScale = 0.5f / scale;

		// TODO: speed this up
		Data.set((m(0, 2) + m(2, 0)) * invScale,
				 (m(1, 2) + m(2, 1)) * invScale,
				 0.5f * scale,
				 (m(1, 0) - m(0, 1)) * invScale);
	}
	makeInverse();

	return normalize();
}

inline
quaternion::quaternion(const matrix4& mat)
	: Data(0)
{
	*this = mat;
}

inline
quaternion
quaternion::operator * (const quaternion& o) const
{
	quaternion tmp(	
		getW() * o.getX() + getX() * o.getW() + getY() * o.getZ() - getZ() * o.getY(),
		getW() * o.getY() + getY() * o.getW() + getZ() * o.getX() - getX() * o.getZ(),
		getW() * o.getZ() + getZ() * o.getW() + getX() * o.getY() - getY() * o.getX(),
		getW() * o.getW() - getX() * o.getX() - getY() * o.getY() - getZ() * o.getZ()
	);
	return tmp;
}

inline
quaternion&
quaternion::operator *= (f32 s)
{
	Data *= s;
	return *this;
}

inline
quaternion
quaternion::operator * (f32 s) const
{
	quaternion tmp = *this;
	tmp *= s;
	return tmp;
}

inline
quaternion&
quaternion::operator *= (const quaternion& other)
{
	return (*this = (*this) * other);
}

inline
quaternion
quaternion::operator + (const quaternion& b) const
{
	quaternion tmp = *this;
	tmp.Data += b.Data;
	return tmp;
}

inline
f32&
quaternion::operator [] (u32 i)
{
	JENNY_ASSERT(i < 4);
	return Data[i];
}

inline
const f32&
quaternion::operator [] (u32 i) const
{
	JENNY_ASSERT(i < 4);
	return Data[i];
}

inline
void
quaternion::getMatrix_transposed(matrix4& dest) const
{
	const f32 _2xx = 2.0f * getX() * getX();
	const f32 _2yy = 2.0f * getY() * getY();
	const f32 _2zz = 2.0f * getZ() * getZ();
	const f32 _2xy = 2.0f * getX() * getY();
	const f32 _2xz = 2.0f * getX() * getZ();
	const f32 _2xw = 2.0f * getX() * getW();
	const f32 _2yz = 2.0f * getY() * getZ();
	const f32 _2yw = 2.0f * getY() * getW();
	const f32 _2zw = 2.0f * getZ() * getW();

	dest[0] = 1.0f - _2yy - _2zz;
	dest[4] = _2xy + _2zw;
	dest[8] = _2xz - _2yw;
	dest[12] = 0.0f;

	dest[1] = _2xy - _2zw;
	dest[5] = 1.0f - _2xx - _2zz;
	dest[9] = _2yz + _2xw;
	dest[13] = 0.0f;

	dest[2] = _2xz + _2yw;
	dest[6] = _2yz - _2xw;
	dest[10] = 1.0f - _2yy - _2xx;
	dest[14] = 0.0f;

	dest[3] = 0.f;
	dest[7] = 0.f;
	dest[11] = 0.f;
	dest[15] = 1.f;
}

inline
matrix4
quaternion::getMatrix_transposed() const
{
	matrix4 m(matrix4::EM4CONST_NOTHING);
	getMatrix_transposed(m);
	return m;
}

inline
void
quaternion::getMatrix(matrix4& dest) const
{
	const f32 _2xx = 2.0f * getX() * getX();
	const f32 _2yy = 2.0f * getY() * getY();
	const f32 _2zz = 2.0f * getZ() * getZ();
	const f32 _2xy = 2.0f * getX() * getY();
	const f32 _2xz = 2.0f * getX() * getZ();
	const f32 _2xw = 2.0f * getX() * getW();
	const f32 _2yz = 2.0f * getY() * getZ();
	const f32 _2yw = 2.0f * getY() * getW();
	const f32 _2zw = 2.0f * getZ() * getW();

	dest[0] = 1.0f - _2yy - _2zz;
	dest[1] = _2xy + _2zw;
	dest[2] = _2xz - _2yw;
	dest[3] = 0.0f;

	dest[4] = _2xy - _2zw;
	dest[5] = 1.0f - _2xx - _2zz;
	dest[6] = _2yz + _2xw;
	dest[7] = 0.0f;

	dest[8] = _2xz + _2yw;
	dest[9] = _2yz - _2xw;
	dest[10] = 1.0f - _2yy - _2xx;
	dest[11] = 0.0f;

	dest[12] = 0.f;
	dest[13] = 0.f;
	dest[14] = 0.f;
	dest[15] = 1.f;
}

inline
matrix4
quaternion::getMatrix() const
{
	matrix4 m(matrix4::EM4CONST_NOTHING);
	getMatrix(m);
	return m;
}

inline
quaternion&
quaternion::set(const vector3df& vec)
{
	return set(vec.getX(), vec.getY(), vec.getZ());
}

inline
f32
quaternion::dotProduct(const quaternion& q2) const
{
	return Data.dotProduct(q2.Data);
}

inline
quaternion&
quaternion::nlerp(quaternion q1, quaternion q2, f32 time)
{
	f32 angle = q1.dotProduct(q2);

	if(angle < 0.0f)
	{
		q1.Data = -q1.Data;
		angle = -angle;
	}
	
	Data = q1.Data.getInterpolated(q2.Data, time);
	normalize();

	return *this;
}

inline
quaternion
quaternion::amplify(float factor)
{
	f32 angle;
	vector3df axis;
	toAngleAxis(angle, axis);
	quaternion q;
	q.fromAngleAxis(angle * factor, axis);
	return q;
}

inline
quaternion&
quaternion::slerp(quaternion q1, quaternion q2, f32 time)
{
	f32 angle = q1.dotProduct(q2);

	if(angle < 0.0f)
	{
		q1.Data = -q1.Data;
		angle = -angle;
	}

	if((angle + 1.0f) > 0.05f)
	{
		if((1.0f - angle) >= 0.05f) // spherical interpolation
		{
			const f32 theta = Acos(angle);
			const f32 invsintheta = 1.0f / Sin(theta);
			f32 scale = Sin(theta * (1.0f - time)) * invsintheta;
			f32 invscale = Sin(theta * time) * invsintheta;
			*this = q1 * scale + q2 * invscale;
		}
		else // linear interploation
		{
			Data = q1.Data.getInterpolated(q2.Data, time);
			normalize();
		}
	}
	else
	{
		q2.set(-q1.getY(), q1.getX(), -q1.getW(), q1.getZ());
		f32 scale = Sin(PI * (0.5f - time));
		f32 invscale = Sin(PI * time);
		*this = q1 * scale + q2 * invscale;
	}

	return *this;
}

inline
quaternion&
quaternion::fromAngleAxis(f32 angle, const vector3df& axis)
{
	const f32 fHalfAngle = 0.5f * angle;
	const f32 fSin = Sin(fHalfAngle);
	Data.set(fSin * axis.getX(),
			 fSin * axis.getY(),
			 fSin * axis.getZ(),
			 Cos(fHalfAngle));
	return *this;
}

inline
void
quaternion::toAngleAxis(f32& angle, vector3df& axis) const
{
	const f32 scale = Sqrt(getX() * getX() + getY() * getY() + getZ() * getZ());

	if(iszero(scale) || getW() > 1.0f || getW() < -1.0f)
	{
		angle = 0.0f;
		axis.set(0.0f, 1.0f, 0.0f);
	}
	else
	{
		const f32 invscale = 1.0f / scale;
		angle = 2.0f * Acos(getW());
		axis.set(getX() * invscale, getY() * invscale, getZ() * invscale);
	}
}

inline
quaternion&
quaternion::makeShortest()
{
	// First we test if the rotation angle is the shortest one
	// [-1 -> 0 -> 1] w => [360 -> 180 -> 0] degree
	if(getW() < 0.0f)
	{
		const f32 squaredScale = 1.0f - getW() * getW();
		if(iszero(squaredScale))
		{
			Data.set(0.0,
				 0.0,
				 0.0,
				 1.0);
		}
		else
		{
			f32 adjustedHalfAngle = Acos(getW());
			adjustedHalfAngle -= PI;
			const f32 adjustedScale = Sin(adjustedHalfAngle) / sqrt(squaredScale);
			Data.set(adjustedScale * getX(),
				 adjustedScale * getY(),
				 adjustedScale * getZ(),
				 Cos(adjustedHalfAngle));
		}
	}
	return *this;
}

inline
void
quaternion::toEulerDegree(vector3df& euler) const
{
	//TODO:yin ze hong: optimize
	matrix4 m(matrix4::EM4CONST_NOTHING);
	getMatrix(m);
	euler = m.getRotationDegrees();
}

inline
void
quaternion::toEuler(vector3df& euler) const
{
	//TODO:yin ze hong: optimize
	toEulerDegree(euler);
	euler *= DEGTORAD;
}

inline
vector3df
quaternion::operator * (const vector3df& v) const
{
	// nVidia SDK implementation

	vector3df qvec(getX(), getY(), getZ());
	vector3df uv = qvec.crossProduct(v);
	vector3df uuv = qvec.crossProduct(uv);
	uv *= 2.0f * getW();
	uuv *= 2.0f;

	return v + uv + uuv;
}

inline
quaternion&
quaternion::makeIdentity()
{
	set(0.0f, 0.0f, 0.0f, 1.0f);
	return *this;
}

inline
bool
quaternion::isIdentity() const
{
	return 
		getX() == 0.0f &&
		getY() == 0.0f &&
		getZ() == 0.0f &&
		getW() == 1.0f;
}

inline
quaternion&
quaternion::rotationFromTo(const vector3df& from, const vector3df& to)
{
	// Based on Stan Melax's article in Game Programming Gems
	// Copy, since cannot modify local
	vector3df v0 = from;
	vector3df v1 = to;
	v0.normalize();
	v1.normalize();

	const f32 d = v0.dotProduct(v1);
	if(d >= 1.0f - ROUNDING_ERROR_32) // If dot == 1, vectors are the same
	{
		return makeIdentity();
	}

	if(d <= -1.0f + ROUNDING_ERROR_32) // if dot == -1, vectors are opossite
	{
		vector3df axis(1.0f, 0.0f, 0.0f);
		axis = axis.crossProduct(from);
		if(iszero(axis.getLengthSQ())) // pick another if colinear
		{
			axis.set(0.0f, 1.0f, 0.0f);
			axis = axis.crossProduct(from);
		}
		axis.normalize();
		return fromAngleAxis(PI, axis);
	}

	const f32 s = Sqrt((1.0f + d) * 2.0f); // optimize inv_sqrt
	const f32 invs = 1.0f / s;
	const vector3df c = v0.crossProduct(v1) * invs;

	set(c.getX(), c.getY(), c.getZ(), s * 0.5f);

	return *this;
}

} // end namespace jenny
