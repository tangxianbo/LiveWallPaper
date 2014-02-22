#pragma once

#include <math/vector2d.h>
#include <math/matrix4.h>
#include <shape/aabbox3d.h>

namespace jenny
{

//!
template <typename T>
inline
void
computeBoundingBox(const T* pos,
				   u32 arraySize,
				   u32 stride,
				   u32 count,
				   aabbox3d<T>& aabb)
{
	JENNY_DEBUG_BREAK_IF(pos == NULL || stride < sizeof(vector3d<T>));
	if(count > 0)
	{
		if(arraySize > 3)
		{
			arraySize = 3;
		}
		for(u32 i = 0; i < arraySize; ++i)
		{
			aabb.MinEdge[i] = pos[i];
			aabb.MaxEdge[i] = pos[i];
		}
		for(u32 i = arraySize; i < 3; ++i)
		{
			aabb.MinEdge[i] = T();
			aabb.MaxEdge[i] = T();
		}
		pos = stepPointer(pos, stride);
		for(const T* end = stepPointer(pos, (count - 1) * stride);
			pos != end;
			pos = stepPointer(pos, stride))
		{
			for(u32 i = 0; i < arraySize; ++i)
			{
				if(pos[i] < aabb.MinEdge[i])
				{
					aabb.MinEdge[i] = pos[i];
				}
				if(aabb.MaxEdge[i] < pos[i])
				{
					aabb.MaxEdge[i] = pos[i];
				}
			}
		}
	}
	else
	{
		aabb.MinEdge = aabb.MaxEdge = core::vector3d<T>();
	}
}

//!
template <typename T>
inline
void
computeBoundingBoxWithTransformation(const T* pos,
				   					 u32 arraySize,
				   					 u32 stride,
				   					 u32 count,
				   					 aabbox3d<T>& aabb,
									 const CMatrix4<T>& transformationMatrix)
{
	JENNY_DEBUG_BREAK_IF(pos == NULL || stride < sizeof(vector3d<T>));
	if(count > 0)
	{
		if(arraySize > 3)
		{
			arraySize = 3;
		}
		vector3d<T> tPos, outPos;
		for(u32 i = 0; i < arraySize; ++i)
		{
			tPos[i] = pos[i];
		}
		for(u32 i = arraySize; i < 3; ++i)
		{
			tPos[i] = T();
		}
		transformationMatrix.transformVect(outPos, tPos);
		aabb.MinEdge = outPos;
		aabb.MaxEdge = outPos;
		pos = stepPointer(pos, stride);
		for(const T* end = stepPointer(pos, (count - 1) * stride);
			pos != end;
			pos = stepPointer(pos, stride))
		{
			for(u32 i = 0; i < arraySize; ++i)
			{
				tPos[i] = pos[i];
			}
			for(u32 i = arraySize; i < 3; ++i)
			{
				tPos[i] = T();
			}
			transformationMatrix.transformVect(outPos, tPos);
			for(u32 i = 0; i < arraySize; ++i)
			{
				if(outPos[i] < aabb.MinEdge[i])
				{
					aabb.MinEdge[i] = outPos[i];
				}
				if(aabb.MaxEdge[i] < outPos[i])
				{
					aabb.MaxEdge[i] = outPos[i];
				}
			}
		}
	}
	else
	{
		aabb.MinEdge = aabb.MaxEdge = core::vector3d<T>();
	}
}

//!
template <typename T>
inline
aabbox3d<T>
computeBoundingBox(const T* pos,
				   u32 arraySize,
				   u32 stride,
				   u32 count)
{
	aabbox3d<T> aabb;
	computeBoundingBox(pos, arraySize, stride, count, aabb);
	return aabb;
}

//!
template <typename T>
inline
void
computeBoundingBox(const vector3d<T>* pos,
				   u32 stride,
				   u32 count,
				   aabbox3d<T>& aabb)
{
	computeBoundingBox(reinterpret_cast<const T*>(pos), 3, stride, count, aabb);
}

//!
template <typename T>
inline
aabbox3d<T>
computeBoundingBox(const vector3d<T>* pos,
				   u32 stride,
				   u32 count)
{
	aabbox3d<T> aabb;
	computeBoundingBox(pos, stride, count, aabb);
	return aabb;
}

//!
template <typename T>
inline
aabbox3d<T>
computeBoundingBox(const vector2d<T>* pos,
				   u32 stride,
				   u32 count)
{
	return computeBoundingBox(pos->getDataPtr(), 2, stride, count);
}


// Clipping --------------------------------------------------------------------

template < typename VECTOR >
VECTOR
intersectAAPlane(const VECTOR& p0, 
				 const VECTOR& p1, 
				 u32 axis, 
				 typename VECTOR::SValueType pos)
{
	const typename VECTOR::SValueType d = p1[axis] - p0[axis];
	if(glf::Abs(d) < core::ROUNDING_ERROR_32)
	{
		return p0;
	}
	const typename VECTOR::SValueType t = (pos - p0[axis]) / d;
	return p0 + t * (p1 - p0);
}

template < bool PlusSideInside, typename VECTOR >
inline
u32
clipWithAAPlane(const VECTOR* in, 
				u32 count, 
				u32 axis, 
				typename VECTOR::SValueType pos, 
				VECTOR* const outBegin,
				VECTOR* const outEnd)
{
	if(count == 0)
	{
		return count;
	}
	const VECTOR* const end = in + count;
	const VECTOR* prev = end - 1;
	bool prevInside = PlusSideInside ? ((*prev)[axis] >= pos) : ((*prev)[axis] <= pos);
	VECTOR* out = outBegin;
	GLITCH_ASSERT(out != outEnd);
	for(; in != end; ++in)
	{
		const bool inside = PlusSideInside ? ((*in)[axis] >= pos) : ((*in)[axis] <= pos);
		if(inside)
		{
			if(!prevInside)
			{
				GLITCH_ASSERT(out != outEnd);
				*(out++) = intersectAAPlane(*prev, *in, axis, pos);				
			}
			GLITCH_ASSERT(out != outEnd);
			*(out++) = *in;			
		}
		else if(prevInside)
		{
			GLITCH_ASSERT(out != outEnd);
			*(out++) = intersectAAPlane(*in, *prev, axis, pos);			
		}
		prev = in;
		prevInside = inside;
	}
	return u32(out - outBegin);
}

} // end namespace jenny
