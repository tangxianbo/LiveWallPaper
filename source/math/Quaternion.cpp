#include <math\Quaternion.h>
#include <base\Types.h>

using namespace Jenny::Math;

template<typename T>
Quaternion_T<T> Slerp(const Quaternion_T<T>& lhs, const Quaternion_T<T>& rhs, T t)
{
	T cosine = Dot(lhs,rhs);

	T dir = T(1);
	if (cosine < 0)
	{
		dir = T(-1);
		cosine = -cosine;
	}


	T scale0, scale1;
	if (cosine < T(1) - std::numeric_limits<T>::epsilon())
	{
		const T omega = acos(cosine);
		const T isinom = T(1)/sin(omega);
		scale0 = sin((T(1) - t) * omega) * isinom;
		scale1 = sin(t * omega) * isinom;
	} 
	else
	{
		scale0 = T(1) - t;
		scale1 = t;
	}

	return scale0*lhs + dir*scale1*rhs;
}


template<typename T>
Quaternion_T<T> mul(const Quaternion_T<T>& lhs, const Quaternion_T<T>& rhs)
{
	return Quaternion_T<T>(
		lhs[0] * rhs[3] - lhs[1] * rhs[2] + lhs[2] * rhs[1] + lhs[3] * rhs[0],
		lhs[0] * rhs[2] + lhs[1] * rhs[3] - lhs[2] * rhs[0] + lhs[3] * rhs[1],
		lhs[1] * rhs[0] - lhs[0] * rhs[1] + lhs[2] * rhs[3] + lhs[3] * rhs[2],
		lhs[3] * rhs[3] - lhs[0] * rhs[0] - lhs[1] * rhs[1] - lhs[2] * rhs[2]);
}