#pragma once

#include <math/vector3d.h>
#include <shape/line3d.h>
#include <shape/plane3d.h>
#include <shape/aabbox3d.h>

namespace jenny
{

template < typename T >
class triangle3d
{
public:

	triangle3d();

	triangle3d(const vector3d<T>& v1,
			   const vector3d<T>& v2,
			   const vector3d<T>& v3);

	bool operator == (const triangle3d<T>& other) const;

	bool operator != (const triangle3d<T>& other) const;

	bool isTotalInsideBox(const aabbox3d<T>& box) const;

	vector3d<T> closestPointOnTriangle(const vector3d<T>& p) const;

	bool isPointInside(const vector3d<T>& p) const;

	bool isPointInsideFast(const vector3d<T>& p) const;

	bool getIntersectionWithLimitedLine(const line3d<T>& line,
										vector3d<T>& outIntersection) const;

	bool getIntersectionWithLineSegment(const line3d<T>&   line,
										const f32          length,
										const vector3d<T>& vect,
										const aabbox3d<T>& box,
										vector3d<T>&       intersection) const;

	bool getIntersectionWithLineSegmentNearest(const line3d<T>&   line,
											   const f32          length,
											   const vector3d<T>& vect,
											   const aabbox3d<T>& box,
											   vector3d<T>&       intersection,
											   f32&               nearest) const;


	bool getIntersectionWithLine(const vector3d<T>& linePoint,
								 const vector3d<T>& lineVect,
								 vector3d<T>& outIntersection) const;


	bool getIntersectionOfPlaneWithLine(const vector3d<T>& linePoint,
										const vector3d<T>& lineVect,
										vector3d<T>& outIntersection) const;

	bool intersectsWithRay(
		const vector3d<T>& origin, 
		const vector3d<T>& dir, 
		T& u, 
		T& v, 
		T& t,
		T epsilon = ROUNDING_ERROR_32) const;

	bool intersectsWithRay(
		const vector3d<T>& origin, 
		const vector3d<T>& dir, 
		T& t,
		T epsilon = ROUNDING_ERROR_32) const;


	bool intersectsWithBoxWithoutBoxTest(const aabbox3d<T>& aabb) const;

	bool intersectsWithBox(const aabbox3d<T>& aabb) const;

	vector3d<T> getNormal() const;

	bool isFrontFacing(const vector3d<T>& lookDirection) const;

	plane3d<T> getPlane() const;

	T getArea() const;

	void set(const vector3d<T>& a, const vector3d<T>& b, const vector3d<T>& c);

	//!
	aabbox3d<T>& getBoundingBox(aabbox3d<T>& bbox) const;

	//!
	aabbox3d<T> getBoundingBox() const;

	//!
	bool getClippedBoundingBox(const aabbox3d<T>& aabb, aabbox3d<T>& out, int clippedPointCount = 9) const;

	vector3d<T>& getPoint(int index);

	const vector3d<T>& getPoint(int index) const;

	vector3d<T> PointA;
	vector3d<T> PointB;
	vector3d<T> PointC;

private:

	bool isOnSameSide(const vector3d<T>& p1,
					  const vector3d<T>& p2,
					  const vector3d<T>& a,
					  const vector3d<T>& b) const;
};


typedef triangle3d<f32> triangle3df;
typedef triangle3d<s32> triangle3di;

template < typename T >
inline
triangle3d<T>::triangle3d()
{
}
	
template < typename T >
inline
triangle3d<T>::triangle3d(const vector3d<T>& v1,
						  const vector3d<T>& v2,
						  const vector3d<T>& v3)
	: PointA(v1)
	, PointB(v2)
	, PointC(v3)
{
}

template < typename T >
inline
bool
triangle3d<T>::operator == (const triangle3d<T>& other) const
{
	return other.PointA == PointA && other.PointB == PointB && other.PointC == PointC;
}

template < typename T >
inline
bool
triangle3d<T>::operator != (const triangle3d<T>& other) const
{
	return !(*this == other);
}

template < typename T >
inline
bool
triangle3d<T>::isTotalInsideBox(const aabbox3d<T>& box) const
{
	return (box.isPointInside(PointA)
			&& box.isPointInside(PointB)
			&& box.isPointInside(PointC));
}

template < typename T >
inline
vector3d<T>
triangle3d<T>::closestPointOnTriangle(const vector3d<T>& p) const
{
	const vector3d<T> rab = line3d<T>(PointA, PointB).getClosestPoint(p);
	const vector3d<T> rbc = line3d<T>(PointB, PointC).getClosestPoint(p);
	const vector3d<T> rca = line3d<T>(PointC, PointA).getClosestPoint(p);

	const T d1 = rab.getDistanceFrom(p);
	const T d2 = rbc.getDistanceFrom(p);
	const T d3 = rca.getDistanceFrom(p);

	if(d1 < d2)
	{
		return d1 < d3 ? rab : rca;
	}
	return d2 < d3 ? rbc : rca;
}

template < typename T >
inline
bool
triangle3d<T>::isPointInside(const vector3d<T>& p) const
{
	return (isOnSameSide(p, PointA, PointB, PointC)
			&& isOnSameSide(p, PointB, PointA, PointC)
			&& isOnSameSide(p, PointC, PointA, PointB));
}

template < typename T >
inline
bool
triangle3d<T>::isPointInsideFast(const vector3d<T>& p) const
{
	const vector3d<T> f = PointB - PointA;
	const vector3d<T> g = PointC - PointA;

	const f32 a = f.dotProduct(f);
	const f32 b = f.dotProduct(g);
	const f32 c = g.dotProduct(g);

	const vector3d<T> vp = p - PointA;
	const f32 d = vp.dotProduct(f);
	const f32 e = vp.dotProduct(g);

	f32 x = (d * c) - (e * b);
	f32 y = (e * a) - (d * b);
	const f32 ac_bb = (a * c) - (b * b);
	f32 z = x + y - ac_bb;

	// return sign(z) && !(sign(x)||sign(y))
	return ((((u32&)z) & ~(((u32&)x) | ((u32&)y))) & 0x80000000) != 0;
}

template < typename T >
inline
bool
triangle3d<T>::getIntersectionWithLimitedLine(const line3d<T>& line,
											  vector3d<T>& outIntersection) const
{
	return (getIntersectionWithLine(line.Start, line.getVector(), outIntersection)
			&& outIntersection.isBetweenPoints(line.Start, line.End));
}

template < typename T >
inline
bool
triangle3d<T>::getIntersectionWithLineSegment(const line3d<T>&   line,
											  const f32          length,
											  const vector3d<T>& vect,
											  const aabbox3d<T>& box,
											  vector3d<T>&       intersection) const
{
	if(box.MinEdge.getX() > PointA.getX() && box.MinEdge.getX() > PointB.getX() && box.MinEdge.getX() > PointC.getX())
	{
		return false;
	}
	if(box.MaxEdge.getX() < PointA.getX() && box.MaxEdge.getX() < PointB.getX() && box.MaxEdge.getX() < PointC.getX())
	{
		return false;
	}
	if(box.MinEdge.getY() > PointA.getY() && box.MinEdge.getY() > PointB.getY() && box.MinEdge.getY() > PointC.getY())
	{
		return false;
	}
	if(box.MaxEdge.getY() < PointA.getY() && box.MaxEdge.getY() < PointB.getY() && box.MaxEdge.getY() < PointC.getY())
	{
		return false;
	}
	if(box.MinEdge.getZ() > PointA.getZ() && box.MinEdge.getZ() > PointB.getZ() && box.MinEdge.getZ() > PointC.getZ())
	{
		return false;
	}
	if(box.MaxEdge.getZ() < PointA.getZ() && box.MaxEdge.getZ() < PointB.getZ() && box.MaxEdge.getZ() < PointC.getZ())
	{
		return false;
	}

	if(getIntersectionWithLine(line.Start, vect, intersection))
	{
		const f32 tmp1 = intersection.getDistanceFromSQ(line.Start);
		const f32 tmp2 = intersection.getDistanceFromSQ(line.End);

		if(tmp1 < length && tmp2 < length)
		{
			return true;
		}
	}
	
	return false;
}

template < typename T >
inline
bool
triangle3d<T>::getIntersectionWithLineSegmentNearest(const line3d<T>&   line,
													 const f32          length,
													 const vector3d<T>& vect,
													 const aabbox3d<T>& box,
													 vector3d<T>&       intersection,
													 f32&               nearest) const
{
	if(box.MinEdge.getX() > PointA.getX() && box.MinEdge.getX() > PointB.getX() && box.MinEdge.getX() > PointC.getX())
	{
		return false;
	}
	if(box.MaxEdge.getX() < PointA.getX() && box.MaxEdge.getX() < PointB.getX() && box.MaxEdge.getX() < PointC.getX())
	{
		return false;
	}
	if(box.MinEdge.getY() > PointA.getY() && box.MinEdge.getY() > PointB.getY() && box.MinEdge.getY() > PointC.getY())
	{
		return false;
	}
	if(box.MaxEdge.getY() < PointA.getY() && box.MaxEdge.getY() < PointB.getY() && box.MaxEdge.getY() < PointC.getY())
	{
		return false;
	}
	if(box.MinEdge.getZ() > PointA.getZ() && box.MinEdge.getZ() > PointB.getZ() && box.MinEdge.getZ() > PointC.getZ())
	{
		return false;
	}
	if(box.MaxEdge.getZ() < PointA.getZ() && box.MaxEdge.getZ() < PointB.getZ() && box.MaxEdge.getZ() < PointC.getZ())
	{
		return false;
	}

	if(line.Start.getDistanceFromSQ(PointA) >= nearest
	   && line.Start.getDistanceFromSQ(PointB) >= nearest
	   && line.Start.getDistanceFromSQ(PointC) >= nearest)
	{
		return false;
	}

	if(getIntersectionWithLine(line.Start, vect, intersection))
	{
		const f32 tmp1 = intersection.getDistanceFromSQ(line.Start);
		const f32 tmp2 = intersection.getDistanceFromSQ(line.End);

		if(tmp1 < length && tmp2 < length && tmp1 < nearest)
		{
			nearest = tmp1;
			return true;
		}
	}
	
	return false;
}

template < typename T >
inline
bool
triangle3d<T>::getIntersectionWithLine(const vector3d<T>& linePoint,
									   const vector3d<T>& lineVect,
									   vector3d<T>& outIntersection) const
{
	if(getIntersectionOfPlaneWithLine(linePoint, lineVect, outIntersection))
	{
		return isPointInside(outIntersection);
	}
	return false;
}

template < typename T >
inline
bool
triangle3d<T>::getIntersectionOfPlaneWithLine(const vector3d<T>& linePoint,
											  const vector3d<T>& lineVect,
											  vector3d<T>& outIntersection) const
{
	const vector3d<T> normal = getNormal().normalize();
	T t2 = normal.dotProduct(lineVect);
	if(iszero(t2))
	{
		return false;
	}

	T d = PointA.dotProduct(normal);
	T t = -(normal.dotProduct(linePoint) - d) / t2;
	outIntersection = linePoint + (lineVect * t);
	return true;
}

template < typename T >
inline
bool
triangle3d<T>::intersectsWithRay(const vector3d<T>& origin,
								 const vector3d<T>& dir,
								 T& u,
								 T& v,
								 T& t,
								 T epsilon) const
{
	// epsilon for values near 1.0
	static const T epsilonNearOne = std::numeric_limits<T>::epsilon() * T(10);
	// Algorithm from "Real-Time Rendering 3rd Edition", chapter 16, section 16.8.2
	const vector3d<T> e1 = PointB - PointA;
	const vector3d<T> e2 = PointC - PointA;
	const vector3d<T> q = dir.crossProduct(e2);
	const T a = e1.dotProduct(q);	
	if (iszero(a, epsilon * epsilon))
	{
		return false;
	}
	const T f = T(1) / a;
	const vector3d<T> s = origin - PointA;
	const T localu = s.dotProduct(q) * f;
	if (localu < -epsilonNearOne)
	{
		return false;
	}
	const vector3d<T> r = s.crossProduct(e1);
	const T localv = dir.dotProduct(r) * f;
	if (localv < -epsilonNearOne || 
		localu + localv > T(1) + epsilonNearOne)
	{
		return false;
	}
	u = localu;
	v = localv;
	t = e2.dotProduct(r) * f;
	return true;
}

template < typename T >
inline
bool
triangle3d<T>::intersectsWithRay(const vector3d<T>& origin,
								 const vector3d<T>& dir,
								 T& t,
								 T epsilon) const
{
	T u, v;
	return intersectsWithRay(origin, dir, u, v, t);
}

template < typename T >
inline
bool
triangle3d<T>::intersectsWithBoxWithoutBoxTest(const aabbox3d<T>& aabb) const
{
	// Algorithm from "Real-Time Rendering 3rd Edition", chapter 16, section 16.12, but skipping point 1.

	// compute box center and half extents
	vector3d<T> c = aabb.getCenter();
	vector3d<T> h = aabb.getExtent() * T(0.5);

	// actually, we want s = c.dotProduct(n) + d, where d is the plane's "D";
	// however, d would be computed as -PointA.dotProduct(n), so doing as below is
	// cheaper in multiplies; we would have 6 multiples, 5 add/subs, but now we
	// have 3 multiples and 5 add/subs

	// translate triangle such that c is the origin (v0 already computed above)
	vector3d<T> v[3] = { PointA - c, PointB - c, PointC - c };
	vector3d<T> e[3] = { v[1] - v[0], v[2] - v[1], v[0] - v[2] };
	T r, p0, p1;

#define JENNY_TRIANGLE_AXIS_TEST(j, ea, eb, v0, v1, va, vb)	\
	p0 = e[j][ea] * v[v0][va] - e[j][eb] * v[v0][vb]; \
	p1 = e[j][ea] * v[v1][va] - e[j][eb] * v[v1][vb]; \
	if(p1 < p0) \
	{ \
		boost::swap(p0, p1); \
	} \
	r = absE[ea] * h[va] + absE[eb] * h[vb]; \
	if(p0 > r || p1 < -r) \
	{ \
		return false; \
	}

#define JENNY_TRIANGLE_AXIS_TEST_NEG(j, ea, eb, v0, v1, va, vb) \
	p0 = e[j][eb] * v[v0][vb] - e[j][ea] * v[v0][va]; \
	p1 = e[j][eb] * v[v1][vb] - e[j][ea] * v[v1][va]; \
	if(p1 < p0) \
	{ \
		boost::swap(p0, p1); \
	} \
	r = absE[ea] * h[va] + absE[eb] * h[vb]; \
	if(p0 > r || p1 < -r) \
	{ \
		return false; \
	}

	vector3d<T> absE = abs(e[0]);
	JENNY_TRIANGLE_AXIS_TEST    (0, 2, 1, 0, 2, 1, 2); // X01
	JENNY_TRIANGLE_AXIS_TEST_NEG(0, 2, 0, 0, 2, 0, 2); // Y02
	JENNY_TRIANGLE_AXIS_TEST    (0, 1, 0, 1, 2, 0, 1); // Z12

	absE = abs(e[1]);
	JENNY_TRIANGLE_AXIS_TEST    (1, 2, 1, 0, 2, 1, 2); // X01
	JENNY_TRIANGLE_AXIS_TEST_NEG(1, 2, 0, 0, 2, 0, 2); // Y02
	JENNY_TRIANGLE_AXIS_TEST    (1, 1, 0, 0, 1, 0, 1); // Z0

	absE = abs(e[2]);
	JENNY_TRIANGLE_AXIS_TEST    (2, 2, 1, 0, 1, 1, 2); // X2
	JENNY_TRIANGLE_AXIS_TEST_NEG(2, 2, 0, 0, 1, 0, 2); // Y1
	JENNY_TRIANGLE_AXIS_TEST    (2, 1, 0, 1, 2, 0, 1); // Z12

#undef JENNY_TRIANGLE_AXIS_TEST
#undef JENNY_TRIANGLE_AXIS_TEST_NEG

	// now, check if the triangle's plane intersects the box
	vector3d<T> n = e[0].crossProduct(e[1]);
	r = h.dotProduct(abs(n));
	T s = -v[0].dotProduct(n);

	if(s > r || s < -r)
	{
		return false;
	}

	return true;
}

template < typename T >
inline
bool
triangle3d<T>::intersectsWithBox(const aabbox3d<T>& aabb) const
{
	if(getBoundingBox().intersectsWithBox(aabb))
	{
		return intersectsWithBoxWithoutBoxTest(aabb);
	}
	return false;
}

template < typename T >
inline
vector3d<T>
triangle3d<T>::getNormal() const
{
	return (PointB - PointA).crossProduct(PointC - PointA);
}

template < typename T >
inline
bool
triangle3d<T>::isFrontFacing(const vector3d<T>& lookDirection) const
{
	const vector3d<T> n = getNormal();//.normalize();
	const f32 d = (f32)n.dotProduct(lookDirection);
	return d <= 0.0f;
}

template < typename T >
inline
plane3d<T>
triangle3d<T>::getPlane() const
{
	return plane3d<T>(PointA, PointB, PointC);
}

template < typename T >
inline
T
triangle3d<T>::getArea() const
{
	return (PointB - PointA).crossProduct(PointC - PointA).getLength() * T(0.5);
}

template < typename T >
inline
void
triangle3d<T>::set(const vector3d<T>& a, const vector3d<T>& b, const vector3d<T>& c)
{
	PointA = a;
	PointB = b;
	PointC = c;
}

template < typename T >
inline
bool
triangle3d<T>::isOnSameSide(const vector3d<T>& p1,
							const vector3d<T>& p2,
							const vector3d<T>& a,
							const vector3d<T>& b) const
{
	vector3d<T> bminusa = b - a;
	vector3d<T> cp1 = bminusa.crossProduct(p1 - a);
	vector3d<T> cp2 = bminusa.crossProduct(p2 - a);
	return (cp1.dotProduct(cp2) >= 0.0f);
}

template < typename T >
inline
aabbox3d<T>&
triangle3d<T>::getBoundingBox(aabbox3d<T>& bbox) const
{
	for(int i = 0; i < 3; ++i)
	{
		minmax_(PointA[i], PointB[i], PointC[i], bbox.MinEdge[i], bbox.MaxEdge[i]);
	}
	return bbox;
}

template < typename T >
inline
aabbox3d<T>
triangle3d<T>::getBoundingBox() const
{
	aabbox3d<T> result(ECT_NOTHING);
	getBoundingBox(result);
	return result;
}

template < typename T >
inline
bool
triangle3d<T>::getClippedBoundingBox(const aabbox3d<T>& aabb, 
									 aabbox3d<T>& out, 
									 int clippedPointCount) const
{
	// This is far from being the most optimal solution, this is works :S
	SScopedProcessArray< vector3d<T> > clippedPointBuffer(2 * clippedPointCount);
	vector3d<T>* clipped1 = clippedPointBuffer.get();
	vector3d<T>* clipped2 = clippedPointBuffer.get() + clippedPointCount;
	u32 count = clipWithAAPlane<true>(&PointA, 3, 0, aabb.MinEdge[0], clipped1, clipped1 + clippedPointCount);
	if(count == 0)
	{
		return false;
	}
	count = clipWithAAPlane<true>(clipped1, count, 1, aabb.MinEdge[1], clipped2, clipped2 + clippedPointCount);
	if(count == 0)
	{
		return false;
	}
	count = clipWithAAPlane<true>(clipped2, count, 2, aabb.MinEdge[2], clipped1, clipped1 + clippedPointCount);
	if(count == 0)
	{
		return false;
	}
	count = clipWithAAPlane<false>(clipped1, count, 0, aabb.MaxEdge[0], clipped2, clipped2 + clippedPointCount);
	if(count == 0)
	{
		return false;
	}
	count = clipWithAAPlane<false>(clipped2, count, 1, aabb.MaxEdge[1], clipped1, clipped1 + clippedPointCount);
	if(count == 0)
	{
		return false;
	}
	count = clipWithAAPlane<false>(clipped1, count, 2, aabb.MaxEdge[2], clipped2, clipped2 + clippedPointCount);
	if(count == 0)
	{
		return false;
	}

	computeBoundingBox(clipped2, sizeof(vector3d<T>), count, out);
	return true;
}

template < typename T >
inline
vector3d<T>& 
triangle3d<T>::getPoint(int index)
{
	return *(&PointA + index);
}

template < typename T >
inline
const vector3d<T>& 
triangle3d<T>::getPoint(int index) const
{
	return *(&PointA + index);
}

} // end namespace jenny
