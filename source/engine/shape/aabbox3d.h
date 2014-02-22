#pragma once

#include <limits>
#include <stddef.h>
#include <type_traits>
#include <math/math.h>
#include <shape/plane3d.h>
#include <shape/line3d.h>
#include <shape/ECullingResult.h>

namespace jenny
{

template < typename T >
class aabbox3d
{
public:
	aabbox3d();

	aabbox3d(const vector3d<T>& min, const vector3d<T>& max);

	explicit aabbox3d(const vector3d<T>& init);

	aabbox3d(T minx, T miny, T minz, T maxx, T maxy, T maxz);

	bool operator == (const aabbox3d<T>& other) const;

	bool operator != (const aabbox3d<T>& other) const;

	aabbox3d<T>& operator += (const vector3d<T>& other);

	// functions
	void addInternalPoint(const vector3d<T>& p);

	void addInternalBox(const aabbox3d<T>& b);

	void addInternalBoxCheck(const aabbox3d<T>& b);

	void reset(T x, T y, T z);

	void reset(const aabbox3d<T>& initValue);

	void reset(const vector3d<T>& initValue);

	void invalidate();

	void addInternalPoint(T x, T y, T z);

	bool isPointInside(const vector3d<T>& p) const;

	bool isPointInside(const vector3d<T>& p, float epsilon) const;

	bool isPointTotalInside(const vector3d<T>& p) const;

	bool intersectsWithBox(const aabbox3d<T>& other) const;

	bool intersectsWithBox(const aabbox3d<T>& other, aabbox3d<T>& result) const;

	bool isFullInside(const aabbox3d<T>& other) const;

	bool isValid() const;

	E_CULLING_RESULT intersectsWithBoxEx(const aabbox3d<T>& other) const;
    
	bool intersectsWithPoint(const vector3d<T>& point) const;


	static bool intersectsWithLine_impl_1d(T bmin,	// min value of the bounding box
										   T bmax,	// max value of the bounding box
										   T si,	// start of the line segment
										   T ei,    // end of the line segment
										   T& fst,  // given start value to compare (start with 0)
										   T& fet); // given end value to compare (start with 1)
	

	bool intersectsWithLine(const line3d<T>& line) const;

	bool intersectsWithSegment(const line3d<T>& line, T& fst, T& fet) const;

	bool intersectsWithRay(const vector3d<T>& origin,
						   const vector3d<T>& dir,
						   T& a,
						   T& b,
						   bool boundedByOrigin = true) const;

	E_INTERSECTION_RELATION_3D classifyPlaneRelation(const plane3d<T>& plane) const;

	vector3d<T> getCenter() const;

	vector3d<T> getExtent() const;

	u32 getMaxExtentAxis(vector3d<T>& extent) const;

	u32 getMaxExtentAxis() const;

	vector3d<T>& getEdge(int zeroForMinAndOneForMax);

	const vector3d<T>& getEdge(int zeroForMinAndOneForMax) const;

	void getEdges(vector3d<T>* edges) const;

	void getEdges(vector3d<T>* edges, ptrdiff_t stride) const;

	bool isEmpty() const;

	void repair();

	aabbox3d<T> getInterpolated(const aabbox3d<T>& other, f32 d) const;

	T getVolume() const;

	T getArea() const;	

	vector3d<T> getClosestPointTo(const vector3d<T>& p) const;

	T getMinDistanceToSQ(const aabbox3d<T>& p) const;

	T getMaxDistanceSQ(const vector3d<T>& p) const;

	vector3d<T> MinEdge;
	vector3d<T> MaxEdge;
};


typedef aabbox3d<f32> aabbox3df;
typedef aabbox3d<s32> aabbox3di;


template < typename T >
inline
aabbox3d<T>::aabbox3d()
	: MinEdge(SBounded<T>::ubound())
 	, MaxEdge(SBounded<T>::lbound())
{
}


template < typename T >
inline
aabbox3d<T>::aabbox3d(const vector3d<T>& min,
					  const vector3d<T>& max)
  : MinEdge(min)
  , MaxEdge(max)
{
}

template < typename T >
inline
aabbox3d<T>::aabbox3d(const vector3d<T>& init)
  : MinEdge(init)
  , MaxEdge(init)
{
}

template < typename T >
inline
aabbox3d<T>::aabbox3d(T minx, T miny, T minz, T maxx, T maxy, T maxz)
  : MinEdge(minx, miny, minz)
  , MaxEdge(maxx, maxy, maxz)
{
}

template < typename T >
inline
bool
aabbox3d<T>::operator == (const aabbox3d<T>& other) const
{
	return MinEdge == other.MinEdge && other.MaxEdge == MaxEdge;
}

template < typename T >
inline
bool
aabbox3d<T>::operator != (const aabbox3d<T>& other) const
{
	return !(*this == other);
}

template < typename T >
inline
aabbox3d<T>&
aabbox3d<T>::operator += (const vector3d<T>& other) 
{ 
	MinEdge += other;
	MaxEdge += other;
	return *this;
}
	
template < typename T >
inline
void
aabbox3d<T>::addInternalPoint(const vector3d<T>& p)
{
	addInternalPoint(p.getX(), p.getY(), p.getZ());
}


template < typename T >
inline
void
aabbox3d<T>::addInternalBox(const aabbox3d<T>& b)
{
	addInternalPoint(b.MaxEdge);
	addInternalPoint(b.MinEdge);
}

template < typename T >
inline
void
aabbox3d<T>::addInternalBoxCheck(const aabbox3d<T>& b)
{
	if(b.isValid())
	{
		addInternalBox(b);
	}
}

template < typename T >
inline
void
aabbox3d<T>::reset(T x, T y, T z)
{
	MaxEdge.set(x,y,z);
	MinEdge = MaxEdge;
}

template < typename T >
inline
void
aabbox3d<T>::reset(const aabbox3d<T>& initValue)
{
	*this = initValue;
}

template < typename T >
inline
void
aabbox3d<T>::reset(const vector3d<T>& initValue)
{
	MaxEdge = initValue;
	MinEdge = initValue;
}

template < typename T >
inline
void
aabbox3d<T>::invalidate()
{
	MaxEdge = vector3d<T>(SBounded<T>::lbound(),
						  SBounded<T>::lbound(),
						  SBounded<T>::lbound());
	MinEdge = vector3d<T>(SBounded<T>::ubound(),
						  SBounded<T>::ubound(),
						  SBounded<T>::ubound());
}

template < typename T >
inline
void
aabbox3d<T>::addInternalPoint(T x, T y, T z)
{
	if(x > MaxEdge.getX())
	{
		MaxEdge.setX(x);
	}
	if(y > MaxEdge.getY())
	{
		MaxEdge.setY(y);
	}
	if(z > MaxEdge.getZ())
	{
		MaxEdge.setZ(z);
	}

	if(x < MinEdge.getX())
	{
		MinEdge.setX(x);
	}
	if(y < MinEdge.getY())
	{
		MinEdge.setY(y);
	}
	if(z < MinEdge.getZ())
	{
		MinEdge.setZ(z);
	}
}

template < typename T >
inline
bool
aabbox3d<T>::isPointInside(const vector3d<T>& p) const
{
	return (p.getX() >= MinEdge.getX() && p.getX() <= MaxEdge.getX()
			&& p.getY() >= MinEdge.getY() && p.getY() <= MaxEdge.getY()
			&& p.getZ() >= MinEdge.getZ() && p.getZ() <= MaxEdge.getZ());
}

template < typename T >
inline
bool
aabbox3d<T>::isPointInside(const vector3d<T>& p, float epsilon) const
{
	return
		(p.getX() + epsilon - MinEdge.getX()) > (T)0 &&
		(p.getX() - epsilon - MaxEdge.getX()) < (T)0 &&
		(p.getY() + epsilon - MinEdge.getY()) > (T)0 &&
		(p.getY() - epsilon - MaxEdge.getY()) < (T)0 &&
		(p.getZ() + epsilon - MinEdge.getZ()) > (T)0 &&
		(p.getZ() - epsilon - MaxEdge.getZ()) < (T)0;
}

template < typename T >
inline
bool
aabbox3d<T>::isPointTotalInside(const vector3d<T>& p) const
{
	return (p.getX() > MinEdge.getX() && p.getX() < MaxEdge.getX()
			&& p.getY() > MinEdge.getY() && p.getY() < MaxEdge.getY()
			&& p.getZ() > MinEdge.getZ() && p.getZ() < MaxEdge.getZ());
}

template < typename T >
inline
bool
aabbox3d<T>::intersectsWithBox(const aabbox3d<T>& other) const
{
	return (MinEdge <= other.MaxEdge && MaxEdge >= other.MinEdge);
}

template < typename T >
inline
bool
aabbox3d<T>::intersectsWithBox(const aabbox3d<T>& other, aabbox3d<T>& result) const
{
	bool intersect = intersectsWithBox(other);
	
	if (intersect)
	{
		// On each axis, compute maximum of minimums and minimums of maximums
		result.MinEdge = core::vector3d<T>(	std::max(MinEdge.getX(), other.MinEdge.getX()),
											std::max(MinEdge.getY(), other.MinEdge.getY()),
											std::max(MinEdge.getZ(), other.MinEdge.getZ())
										);

		result.MaxEdge = core::vector3d<T>( std::min(MaxEdge.getX(), other.MaxEdge.getX()),
											std::min(MaxEdge.getY(), other.MaxEdge.getY()),
											std::min(MaxEdge.getZ(), other.MaxEdge.getZ())
										);
	}

	return intersect;
}

template < typename T >
inline
bool
aabbox3d<T>::isFullInside(const aabbox3d<T>& other) const
{
	return MinEdge >= other.MinEdge && MaxEdge <= other.MaxEdge;
}

template < typename T >
inline
bool
aabbox3d<T>::isValid() const
{
	return MinEdge[0] <= MaxEdge[0] && MinEdge[1] <= MaxEdge[1] && MinEdge[2] <= MaxEdge[2];
}

template < typename T >
inline
E_CULLING_RESULT
aabbox3d<T>::intersectsWithBoxEx(const aabbox3d<T>& other) const
{
	if(isFullInside(other))
	{
		return scene::ECR_INSIDE;
	}
	if(intersectsWithBox(other))
	{
		return scene::ECR_INTERSECT;
	}
	return scene::ECR_OUTSIDE;
}

template < typename T >
inline
bool
aabbox3d<T>::intersectsWithPoint(const vector3d<T>& point) const
{
	bool r;
	r = MinEdge.getX() <= point.getX() && point.getX() <= MaxEdge.getX();
	r &= MinEdge.getY() <= point.getY() && point.getY() <= MaxEdge.getY();
	r &= MinEdge.getZ() <= point.getZ() && point.getZ() <= MaxEdge.getZ();
	return r;
}

template < typename T >
inline
bool
aabbox3d<T>::intersectsWithLine_impl_1d(T bmin,	// min value of the bounding box
										T bmax,	// max value of the bounding box
										T si,	// start of the line segment
										T ei,   // end of the line segment
										T& fst, // given start value to compare (start with 0)
										T& fet) // given end value to compare (start with 1)
{
	// The algorithm need to know which of the start or the end of the 
	// segment is smaller; the variable could be swapped, but it's faster
	// to duplicate the code.
	T st, et;
	T di = ei - si;  

	if(si < ei) 
	{  
		if(si > bmax || ei < bmin) 
		{
			return false;   // outside AABB
		}
		st = si < bmin ? (bmin - si) / di : T(0); // cut / inclusion 
		et = ei > bmax ? (bmax - si) / di : T(1); // cut / inclusion 
	} 
	else 
	{  
		if(ei > bmax || si < bmin)  
		{
			return false;   //  outside AABB
		}
		st = si > bmax ? (bmax - si) / di : T(0);  // cut / inclusion  
		et = ei < bmin ? (bmin - si) / di : T(1);  // cut / inclusion
	}

	if(st > fst)   // Compare with prev results - the furthest the start, the better
	{
		fst = st;
	}
	if(et < fet)   // Compare with prev results - the closest the end, the better
	{
		fet = et;
	}
	if(fet < fst)  // result turned to be outside.
	{
		return false;   
	}
	return true;    // collision exist
}

template < typename T >
inline
bool
aabbox3d<T>::intersectsWithLine(const line3d<T>& line) const
{
	T fst, fet;
	return intersectsWithSegment(line, fst, fet);
}

template < typename T >
inline
bool
aabbox3d<T>::intersectsWithSegment(const line3d<T>& line, T& fst, T& fet) const
{
	fst = 0;
	fet = 1;
	return (intersectsWithLine_impl_1d(MinEdge.getX(), MaxEdge.getX(), line.Start.getX(), line.End.getX(), fst, fet)
			&& intersectsWithLine_impl_1d(MinEdge.getY(), MaxEdge.getY(), line.Start.getY(), line.End.getY(), fst, fet)
			&& intersectsWithLine_impl_1d(MinEdge.getZ(), MaxEdge.getZ(), line.Start.getZ(), line.End.getZ(), fst, fet));
}

template < typename T >
inline
bool
aabbox3d<T>::intersectsWithRay(const vector3d<T>& origin,
							   const vector3d<T>& dir,
							   T& a,
							   T& b,
							   bool boundedByOrigin) const
{
	// Algorithm from "Real-Time Rendering 3rd Edition", chapter 16, section
	// 16.7.1, simplified for the case of AABB
	T tmin = boundedByOrigin ? T(0) : SBounded<T>::lbound();
	T tmax = SBounded<T>::ubound();
	T t1, t2;
	for(int i = 0; i < 3; ++i)
	{
		T e0 = MinEdge[i] - origin[i];
		T e1 = MaxEdge[i] - origin[i];
		T f = dir[i];
		if(!iszero(f))
		{
			// this if will disappear because the condition value is known at
			// compile-time
			if(boost::is_floating_point<T>::value)
			{
				f = T(1) / f;
				t1 = e1 * f;
				t2 = e0 * f;
			}
			else
			{
				t1 = e1 / f;
				t2 = e0 / f;
			}
			if(t1 > t2)
			{
				boost::swap(t1, t2);
			}
			if(t1 > tmin)
			{
				tmin = t1;
			}
			if(t2 < tmax)
			{
				tmax = t2;
			}
			if(tmin > tmax || tmax < T(0))
			{
				return false;
			}
		}
		else if(e1 < T(0) || e0 > T(0))
		{
			return false;
		}
	}
	a = tmin;
	b = tmax;
	return true;
}

template < typename T >
inline
E_INTERSECTION_RELATION_3D
aabbox3d<T>::classifyPlaneRelation(const plane3d<T>& plane) const
{
	vector3d<T> nearPoint(MaxEdge);
	vector3d<T> farPoint(MinEdge);

	if(plane.Normal.getX() > T(0))
	{
		nearPoint.setX(MinEdge.getX());
		farPoint.setX(MaxEdge.getX());
	}

	if(plane.Normal.getY() > T(0))
	{
		nearPoint.getY() = MinEdge.getY();
		farPoint.getY() = MaxEdge.getY();
	}

	if(plane.Normal.getZ() > T(0))
	{
		nearPoint.Z = MinEdge.getZ();
		farPoint.Z = MaxEdge.getZ();
	}

	if(plane.Normal.dotProduct(nearPoint) + plane.D > T(0))
	{
		return EIR3D_FRONT;
	}
	if(plane.Normal.dotProduct(farPoint) + plane.D > T(0))
	{
		return EIR3D_CLIPPED;
	}
	return EIR3D_BACK;
}

template < typename T >
inline
vector3d<T>
aabbox3d<T>::getCenter() const
{
	return (MinEdge + MaxEdge) / T(2);
}

template < typename T >
inline
vector3d<T>
aabbox3d<T>::getExtent() const
{
	return MaxEdge - MinEdge;
}

template < typename T >
inline
u32
aabbox3d<T>::getMaxExtentAxis(vector3d<T>& extent) const
{
	extent = getExtent();
	return extent.getMaxAxis();
}

template < typename T >
inline
u32
aabbox3d<T>::getMaxExtentAxis() const
{
	vector3d<T> extent(ECT_NOTHING);
	return getMaxExtentAxis(extent);
}

template < typename T >
inline
vector3d<T>& 
aabbox3d<T>::getEdge(int zeroForMinAndOneForMax)
{
	return *(&MinEdge + zeroForMinAndOneForMax);
}

template < typename T >
inline
const vector3d<T>& 
aabbox3d<T>::getEdge(int zeroForMinAndOneForMax) const
{
	return *(&MinEdge + zeroForMinAndOneForMax);
}

template < typename T >
inline
void
aabbox3d<T>::getEdges(vector3d<T>* edges) const
{
	const core::vector3d<T> middle = getCenter();
	const core::vector3d<T> diag = middle - MaxEdge;

	/*
	  Edges are stored in this way:
	     3---------/7
	    /|        / |
	   / |       /  |
	  1---------5   |
	  |  2- - - |- -6
	  | /       |  /
	  |/        | /
	  0---------4/
	*/

	edges[0].set(middle.getX() + diag.getX(), middle.getY() + diag.getY(), middle.getZ() + diag.getZ());
	edges[1].set(middle.getX() + diag.getX(), middle.getY() - diag.getY(), middle.getZ() + diag.getZ());
	edges[2].set(middle.getX() + diag.getX(), middle.getY() + diag.getY(), middle.getZ() - diag.getZ());
	edges[3].set(middle.getX() + diag.getX(), middle.getY() - diag.getY(), middle.getZ() - diag.getZ());
	edges[4].set(middle.getX() - diag.getX(), middle.getY() + diag.getY(), middle.getZ() + diag.getZ());
	edges[5].set(middle.getX() - diag.getX(), middle.getY() - diag.getY(), middle.getZ() + diag.getZ());
	edges[6].set(middle.getX() - diag.getX(), middle.getY() + diag.getY(), middle.getZ() - diag.getZ());
	edges[7].set(middle.getX() - diag.getX(), middle.getY() - diag.getY(), middle.getZ() - diag.getZ());
}

template < typename T >
inline
void
aabbox3d<T>::getEdges(vector3d<T>* edges, ptrdiff_t stride) const
{
	const core::vector3d<T> middle = getCenter();
	const core::vector3d<T> diag = middle - MaxEdge;

	/*
	  Edges are stored in this way:
	     3---------/7
	    /|        / |
	   / |       /  |
	  1---------5   |
	  |  2- - - |- -6
	  | /       |  /
	  |/        | /
	  0---------4/
	*/

	util::SStridedIterator<vector3d<T> > iter(edges, stride);
	(iter++)->set(middle.getX() + diag.getX(), middle.getY() + diag.getY(), middle.getZ() + diag.getZ());
	(iter++)->set(middle.getX() + diag.getX(), middle.getY() - diag.getY(), middle.getZ() + diag.getZ());
	(iter++)->set(middle.getX() + diag.getX(), middle.getY() + diag.getY(), middle.getZ() - diag.getZ());
	(iter++)->set(middle.getX() + diag.getX(), middle.getY() - diag.getY(), middle.getZ() - diag.getZ());
	(iter++)->set(middle.getX() - diag.getX(), middle.getY() + diag.getY(), middle.getZ() + diag.getZ());
	(iter++)->set(middle.getX() - diag.getX(), middle.getY() - diag.getY(), middle.getZ() + diag.getZ());
	(iter++)->set(middle.getX() - diag.getX(), middle.getY() + diag.getY(), middle.getZ() - diag.getZ());
	iter->set(middle.getX() - diag.getX(), middle.getY() - diag.getY(), middle.getZ() - diag.getZ());
}

template < typename T >
inline
bool
aabbox3d<T>::isEmpty() const
{
	return MinEdge.equals(MaxEdge);
}

template < typename T >
inline
void
aabbox3d<T>::repair()
{
	if(MinEdge.getX() > MaxEdge.getX())
	{
		boost::swap(MinEdge[0], MaxEdge[0]);
	}
	if(MinEdge.getY() > MaxEdge.getY())
	{
		boost::swap(MinEdge[1], MaxEdge[1]);
	}
	if(MinEdge.getZ() > MaxEdge.getZ())
	{
		boost::swap(MinEdge[2], MaxEdge[2]);
	}
}

template < typename T >
inline
aabbox3d<T>
aabbox3d<T>::getInterpolated(const aabbox3d<T>& other, f32 d) const
{
	return aabbox3d<T>(other.MinEdge.getInterpolated(MinEdge, d),
					   other.MaxEdge.getInterpolated(MaxEdge, d));
}

template < typename T >
inline
T
aabbox3d<T>::getVolume() const
{
	const vector3d<T> e = getExtent();
	return e.getX() * e.getY() * e.getZ();
}

template < typename T >
inline
T
aabbox3d<T>::getArea() const
{
	const vector3d<T> e = getExtent();
	return T(2) * (e.getX() * e.getY() + e.getX() * e.getZ() + e.getY() * e.getZ());
}

template < typename T >
inline
vector3d<T>
aabbox3d<T>::getClosestPointTo(const vector3d<T>& p) const
{
	return p.clip(MinEdge, MaxEdge);
}

template < typename T >
inline
T
aabbox3d<T>::getMinDistanceToSQ(const aabbox3d<T>& other) const
{
	const aabbox3df* boxes[2] = {this, &other};
	float distanceSQ = 0.0f;
	for (int i = 0; i < 2; ++i)
	{
		const aabbox3df& b1 = *boxes[i];
		const aabbox3df& b2 = *boxes[1-i];
		for (int k = 0; k < 3; ++k)
		{
			const float edgeDistance = b2.MinEdge[k] - b1.MaxEdge[k];
			distanceSQ += edgeDistance > 0.0f ? edgeDistance * edgeDistance : 0.0f;
		}
	}
	return distanceSQ;
}

template < typename T >
inline
T
aabbox3d<T>::getMaxDistanceSQ(const vector3d<T>& p) const
{
	vector3d<T> res(
		core::max_(std::abs(MinEdge.getX() - p.getX()), std::abs(MaxEdge.getX() - p.getX())),
		core::max_(std::abs(MinEdge.getY() - p.getY()), std::abs(MaxEdge.getY() - p.getY())),
		core::max_(std::abs(MinEdge.getZ() - p.getZ()), std::abs(MaxEdge.getZ() - p.getZ())));

	return res.getLengthSQ();
		
}

} // end namespace jenny

