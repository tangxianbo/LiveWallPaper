#include "math/Matrix.h"

using namespace Jenny::Math;

template<typename T>
Vector_T<T,4> Multiply(const Vector_T<T,4>& vector, const Matrix_T<T>& matrix)
{
	T x = vector[0]*matrix(0,0) + vector[1]*matrix(1,0) + vector[2]*matrix(2,0) + vector[3]*matrix(3,0);
	T y = vector[0]*matrix(0,1) + vector[1]*matrix(1,1) + vector[2]*matrix(2,1) + vector[3]*matrix(3,1);
	T z = vector[0]*matrix(0,2) + vector[1]*matrix(1,2) + vector[2]*matrix(2,2) + vector[3]*matrix(3,2);
	T w = vector[0]*matrix(0,3) + vector[1]*matrix(1,3) + vector[2]*matrix(2,3) + vector[3]*matrix(3,3);
	return Vector_T<T,4>(x,y,z,w);
}


template<typename T>
Matrix_T<T> Multiply(const Matrix_T<T>& lhs, const Matrix_T<T>& rhs)
{
	return Matrix_T<T>(
		lhs(0, 0) * rhs(0, 0) + lhs(0, 1) * rhs(1, 0) + lhs(0, 2) * rhs(2, 0) + lhs(0, 3) * rhs(3, 0),
		lhs(0, 0) * rhs(0, 1) + lhs(0, 1) * rhs(1, 1) + lhs(0, 2) * rhs(2, 1) + lhs(0, 3) * rhs(3, 1),
		lhs(0, 0) * rhs(0, 2) + lhs(0, 1) * rhs(1, 2) + lhs(0, 2) * rhs(2, 2) + lhs(0, 3) * rhs(3, 2),
		lhs(0, 0) * rhs(0, 3) + lhs(0, 1) * rhs(1, 3) + lhs(0, 2) * rhs(2, 3) + lhs(0, 3) * rhs(3, 3),

		lhs(1, 0) * rhs(0, 0) + lhs(1, 1) * rhs(1, 1) + lhs(1, 2) * rhs(2, 0) + lhs(1, 3) * rhs(3, 0),
		lhs(1, 0) * rhs(0, 1) + lhs(1, 1) * rhs(1, 1) + lhs(1, 2) * rhs(2, 1) + lhs(1, 3) * rhs(3, 1),
		lhs(1, 0) * rhs(0, 2) + lhs(1, 1) * rhs(1, 2) + lhs(1, 2) * rhs(2, 2) + lhs(1, 3) * rhs(3, 2),
		lhs(1, 0) * rhs(0, 3) + lhs(1, 1) * rhs(1, 3) + lhs(1, 2) * rhs(2, 3) + lhs(1, 3) * rhs(3, 3),

		lhs(2, 0) * rhs(0, 0) + lhs(2, 1) * rhs(1, 0) + lhs(2, 2) * rhs(2, 0) + lhs(2, 3) * rhs(3, 0),
		lhs(2, 0) * rhs(0, 1) + lhs(2, 1) * rhs(1, 1) + lhs(2, 2) * rhs(2, 1) + lhs(2, 3) * rhs(3, 1),
		lhs(2, 0) * rhs(0, 2) + lhs(2, 1) * rhs(1, 2) + lhs(2, 2) * rhs(2, 2) + lhs(2, 3) * rhs(3, 2),
		lhs(2, 0) * rhs(0, 3) + lhs(2, 1) * rhs(1, 3) + lhs(2, 2) * rhs(2, 3) + lhs(2, 3) * rhs(3, 3),

		lhs(3, 0) * rhs(0, 0) + lhs(3, 1) * rhs(1, 0) + lhs(3, 2) * rhs(2, 0) + lhs(3, 3) * rhs(3, 0),
		lhs(3, 0) * rhs(0, 1) + lhs(3, 1) * rhs(1, 1) + lhs(3, 2) * rhs(2, 1) + lhs(3, 3) * rhs(3, 1),
		lhs(3, 0) * rhs(0, 2) + lhs(3, 1) * rhs(1, 2) + lhs(3, 2) * rhs(2, 2) + lhs(3, 3) * rhs(3, 2),
		lhs(3, 0) * rhs(0, 3) + lhs(3, 1) * rhs(1, 3) + lhs(3, 2) * rhs(2, 3) + lhs(3, 3) * rhs(3, 3));
}