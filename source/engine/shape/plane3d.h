#pragma once

#include <math/math.h>
#include <math/vector3d.h>

namespace jenny
{

enum E_INTERSECTION_RELATION_3D
{
	EIR3D_FRONT,
	EIR3D_BACK,
	EIR3D_PLANAR,
	EIR3D_SPANNING,
	EIR3D_CLIPPED
};


template < typename T >
class plane3d
{
public:

	// Constructors

	plane3d();
	plane3d(const vector3d<T>& mpoint, const vector3d<T>& normal);
	plane3d(T px, T py, T pz, T nx, T ny, T nz);
	plane3d(const vector3d<T>& point1, const vector3d<T>& point2, const vector3d<T>& point3);

	// operators

	bool operator == (const plane3d<T>& other) const;
	bool operator != (const plane3d<T>& other) const;

	// functions

	void setPlane(const vector3d<T>& point, const vector3d<T>& nvector);
	void setPlane(const vector3d<T>& nvect, T d);
	void setPlane(const vector3d<T>& point1, const vector3d<T>& point2, const vector3d<T>& point3);

	bool getIntersectionWithLine(const vector3d<T>& linePoint,
								 const vector3d<T>& lineVect,
								 vector3d<T>& outIntersection) const;

	f32 getKnownIntersectionWithLine(const vector3d<T>& linePoint1,
									 const vector3d<T>& linePoint2) const;


	bool getIntersectionWithLimitedLine(const vector3d<T>& linePoint1,
										const vector3d<T>& linePoint2,
										vector3d<T>& outIntersection) const;

	E_INTERSECTION_RELATION_3D classifyPointRelation(const vector3d<T>& point) const;

	void recalculateD(const vector3d<T>& mpoint);

	vector3d<T> getMemberPoint() const;

	bool existsIntersection(const plane3d<T>& other) const;

	bool getIntersectionWithPlane(const plane3d<T>& other,
								  vector3d<T>& outLinePoint,
								  vector3d<T>& outLineVect) const;

	bool getIntersectionWithPlanes(const plane3d<T>& o1,
								   const plane3d<T>& o2,
								   vector3d<T>& outPoint) const;

	bool isFrontFacing(const vector3d<T>& lookDirection) const;

	T getDistanceTo(const vector3d<T>& point) const;

	vector3d<T> Normal;
	T D;
};

typedef plane3d<f32> plane3df;
typedef plane3d<s32> plane3di;

template < typename T >
inline
plane3d<T>::plane3d()
	: Normal(T(0), T(1), T(0))
{
	recalculateD(vector3d<T>(T(0)));
}

template < typename T >
inline
plane3d<T>::plane3d(const vector3d<T>& mpoint, const vector3d<T>& normal)
	: Normal(normal)
{
	recalculateD(mpoint);
}

template < typename T >
inline
plane3d<T>::plane3d(T px, T py, T pz, T nx, T ny, T nz)
	: Normal(nx, ny, nz)
{
	recalculateD(vector3d<T>(px, py, pz));
}

template < typename T >
inline
plane3d<T>::plane3d(const vector3d<T>& point1, const vector3d<T>& point2, const vector3d<T>& point3)
{
	setPlane(point1, point2, point3);
}

template < typename T >
inline
bool
plane3d<T>::operator == (const plane3d<T>& other) const
{
	return D == other.D && Normal == other.Normal;
}

template < typename T >
inline
bool
plane3d<T>::operator != (const plane3d<T>& other) const
{
	return !(*this == other);
}

template < typename T >
inline
void
plane3d<T>::setPlane(const vector3d<T>& point, const vector3d<T>& nvector)
{
	Normal = nvector;
	recalculateD(point);
}

template < typename T >
inline
void
plane3d<T>::setPlane(const vector3d<T>& nvect, T d)
{
	Normal = nvect;
	D = d;
}

template < typename T >
inline
void
plane3d<T>::setPlane(const vector3d<T>& point1, const vector3d<T>& point2, const vector3d<T>& point3)
{
	// creates the plane from 3 memberpoints
	Normal = (point2 - point1).crossProduct(point3 - point1);
	Normal.normalize();

	recalculateD(point1);
}

template < typename T >
inline
bool
plane3d<T>::getIntersectionWithLine(const vector3d<T>& linePoint,
									const vector3d<T>& lineVect,
									vector3d<T>& outIntersection) const
{
	T t2 = Normal.dotProduct(lineVect);

	if(t2 == T(0))
	{
		return false;
	}
	T t = -(Normal.dotProduct(linePoint) + D) / t2;
	outIntersection = linePoint + (lineVect * t);
	return true;
}

template < typename T >
inline
f32
plane3d<T>::getKnownIntersectionWithLine(const vector3d<T>& linePoint1,
										 const vector3d<T>& linePoint2) const
{
	vector3d<T> vect = linePoint2 - linePoint1;
	f32 t2 = f32(Normal.dotProduct(vect));
	return -f32(Normal.dotProduct(linePoint1) + D) / t2;
}

template < typename T >
inline
bool
plane3d<T>::getIntersectionWithLimitedLine(const vector3d<T>& linePoint1,
										   const vector3d<T>& linePoint2,
										   vector3d<T>& outIntersection) const
{
	return (getIntersectionWithLine(linePoint1, linePoint2 - linePoint1, outIntersection)
			&& outIntersection.isBetweenPoints(linePoint1, linePoint2));
}

template < typename T >
inline
E_INTERSECTION_RELATION_3D
plane3d<T>::classifyPointRelation(const vector3d<T>& point) const
{
	const T d = Normal.dotProduct(point) + D;

	if(d < -T(ROUNDING_ERROR_32))
	{
		return EIR3D_BACK;
	}
	if(d > T(ROUNDING_ERROR_32))
	{
		return EIR3D_FRONT;
	}
	return EIR3D_PLANAR;
}

template < typename T >
inline
void
plane3d<T>::recalculateD(const vector3d<T>& mpoint)
{
	D = - mpoint.dotProduct(Normal);
}

template < typename T >
inline
vector3d<T>
plane3d<T>::getMemberPoint() const
{
	return Normal * -D;
}

template < typename T >
inline
bool
plane3d<T>::existsIntersection(const plane3d<T>& other) const
{
	vector3d<T> cross = other.Normal.crossProduct(Normal);
	return cross.getLength() > T(ROUNDING_ERROR_32);
}

template < typename T >
inline
bool
plane3d<T>::getIntersectionWithPlane(const plane3d<T>& other,
									 vector3d<T>& outLinePoint,
									 vector3d<T>& outLineVect) const
{
	const T fn00 = Normal.getLength();
	const T fn01 = Normal.dotProduct(other.Normal);
	const T fn11 = other.Normal.getLength();
	const f64 det = f64(fn00 * fn11 - fn01 * fn01);

	if(std::abs(det) < ROUNDING_ERROR_64)
	{
		return false;
	}

	const f64 invdet = 1.0 / det;
	const f64 fc0 = f64(fn11 * -D + fn01 * other.D) * invdet;
	const f64 fc1 = f64(fn00 * -other.D + fn01 * D) * invdet;

	outLineVect = Normal.crossProduct(other.Normal);
	outLinePoint = Normal * T(fc0) + other.Normal * T(fc1);
	return true;
}

template < typename T >
inline
bool
plane3d<T>::getIntersectionWithPlanes(const plane3d<T>& o1,
									  const plane3d<T>& o2,
									  vector3d<T>& outPoint) const
{
	vector3d<T> linePoint, lineVect;
	if(getIntersectionWithPlane(o1, linePoint, lineVect))
	{
		return o2.getIntersectionWithLine(linePoint, lineVect, outPoint);
	}
	return false;
}

template < typename T >
inline
bool
plane3d<T>::isFrontFacing(const vector3d<T>& lookDirection) const
{
	const f32 d = Normal.dotProduct(lookDirection);
	return d <= 0.0f;
}

template < typename T >
inline
T
plane3d<T>::getDistanceTo(const vector3d<T>& point) const
{
	return point.dotProduct(Normal) + D;
}

} // end namespace jenny
