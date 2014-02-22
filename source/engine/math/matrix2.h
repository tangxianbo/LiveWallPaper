/******************************************************************************
	Glitch Engine
	Mailing List: World-Glitch@gameloft.com
	Documentation: https://docs.gameloft.org/category/development/tools-engines/glitch-glf/
******************************************************************************/
#pragma once

#include <core/types.h>
#include <math/math.h>
#include <math/vector2d.h>

namespace jenny
{
		
//! 2x2 matrix.
/** The matrix is column major. */
template <class T>
class CMatrix2
{
public:

	//! Constructs an identity matrix
	CMatrix2();

	//! Copy constructor
	template < typename U >
	CMatrix2(const CMatrix2<U>& other);

	T& operator [] (u32 index);
	const T& operator [] (u32 index) const;
	T* pointer();
	const T* pointer() const;
			
	//! Simple operator for directly accessing every element of the matrix.
	T& operator () (const s32 col, const s32 row);
			
	//! Simple operator for directly accessing every element of the matrix.
	const T& operator () (const s32 col, const s32 row) const;
			
	//! Sets all elements of this matrix to the value.
	CMatrix2<T>& operator = (T scalar);
			
	//! Returns true if other matrix is equal to this matrix.
	bool operator == (const CMatrix2& other) const;
			
	//! Returns true if other matrix is not equal to this matrix.
	bool operator != (const CMatrix2& other) const;
			
	//! Add another matrix.
	CMatrix2& operator += (const CMatrix2& other);
			
	//! Add another matrix.
	CMatrix2 operator + (const CMatrix2& other) const;
			
	//! Subtract another matrix.
	CMatrix2& operator -= (const CMatrix2& other);
			
	//! Subtract another matrix.
	CMatrix2 operator - (const CMatrix2& other) const;
			
	//! Multiply by another matrix.
	CMatrix2 operator * (const CMatrix2& other) const;
			
	//! Multiply by another matrix.
	CMatrix2& operator *= (const CMatrix2& other);

	//! Multiply by scalar.
	CMatrix2& operator *= (T scalar);

	//! Clear matrix (fill with 0).
	CMatrix2& clear();
			
	//! Set matrix to identity.
	CMatrix2& makeIdentity();
			
	//! Returns true if the matrix is the identity matrix
	bool isIdentity() const;
			
	//! Returns the c'th column of the matrix.
	vector2d<T> getColumn(u32 c) const;
			
	//! Sets the c'th column of the matrix.
	CMatrix2& setColumn(u32 c, const vector2d<T>& v);
			
	//! Returns the c'th row of the matrix.
	vector2d<T> getRow(u32 c) const;
			
	//! Sets the c'th row of the matrix.
	CMatrix2& setRow(u32 c, const vector2d<T>& v);

	//! Transforms input vector by this matrix and stores result in output vector
	void transformVect(vector2d<T>& out, const vector2d<T>& in) const;
			
	//! Transforms the vector by this matrix
	void transformVect(vector2d<T>& vect) const;
			
	//! An alternate transform vector method, writing into an array of 2 floats
	void transformVect(T* out, const vector2d<T>& in) const;
			
	//! Calculates inverse of matrix. Slow.
	/** \return Returns false if there is no inverse matrix.*/
	bool makeInverse();
			
	//! Computes the determinant of the matrix.
	T getDeterminant() const;
			
	//! Gets the inversed matrix of this one
	/** \param out: where result matrix is written to.
		\return Returns false if there is no inverse matrix. */
	bool getInverse(CMatrix2& out) const;
			
	//! Gets transposed matrix
	CMatrix2 getTransposed() const;
			
	//! Gets transposed matrix
	void getTransposed(CMatrix2& dest) const;
			
	//! Sets all matrix data members at once
	CMatrix2& setM(const T* data);
			
	//! Gets all matrix data members at once
	/** \returns data */
	T* getM(T* data) const;

private:

	T M[4];
};
		
template <typename T>
inline
CMatrix2<T>::CMatrix2()
{
	makeIdentity();
}
		
template <typename T>
template <typename U>
inline
CMatrix2<T>::CMatrix2(const CMatrix2<U>& other)
{
	M[0] = T(other.M[0]);
	M[1] = T(other.M[1]);
	M[2] = T(other.M[2]);
	M[3] = T(other.M[3]);
}

template <typename T>
inline
T&
CMatrix2<T>::operator [] (u32 index)
{
	return M[index];
}

template <typename T>
inline
const T&
CMatrix2<T>::operator [] (u32 index) const
{
	return M[index];
}

template <typename T>
inline
T*
CMatrix2<T>::pointer()
{
	return M;
}

template <typename T>
inline
const T*
CMatrix2<T>::pointer() const
{
	return M;
}
			
template <typename T>
inline
T&
CMatrix2<T>::operator () (const s32 col, const s32 row)
{
	return (*this)[col * 2 + row];
}
			
template <typename T>
inline
const T&
CMatrix2<T>::operator () (const s32 col, const s32 row) const
{
	return (*this)[col * 2 + row];
}

template <typename T>
inline
CMatrix2<T>&
CMatrix2<T>::operator += (const CMatrix2& other)
{
	M[0] += other.M[0];
	M[1] += other.M[1];
	M[2] += other.M[2];
	M[3] += other.M[3];
	return *this;
}
			
template <typename T>
inline
CMatrix2<T>
CMatrix2<T>::operator + (const CMatrix2& other) const
{
	CMatrix2 result(*this);
	result += other;
	return result;
}
			
template <typename T>
inline
CMatrix2<T>&
CMatrix2<T>::operator -= (const CMatrix2& other)
{
	M[0] -= other.M[0];
	M[1] -= other.M[1];
	M[2] -= other.M[2];
	M[3] -= other.M[3];
	return *this;
}
			
template <typename T>
inline
CMatrix2<T>
CMatrix2<T>::operator - (const CMatrix2& other) const
{
	CMatrix2 result(*this);
	result -= other;
	return result;
}
			
template <typename T>
inline
CMatrix2<T>
CMatrix2<T>::operator * (const CMatrix2& other) const
{
	CMatrix2 result(ECT_NOTHING);
	result.M[0] = M[0] * other.M[0] + M[2] * other.M[1];
	result.M[1] = M[1] * other.M[0] + M[3] * other.M[1];

	result.M[3] = M[0] * other.M[2] + M[2] * other.M[3];
	result.M[4] = M[1] * other.M[2] + M[3] * other.M[3];
	return *this;
}
			
template <typename T>
inline
CMatrix2<T>&
CMatrix2<T>::operator *= (const CMatrix2& other)
{
	return *this = (*this) * other;
}

template <typename T>
inline
CMatrix2<T>&
CMatrix2<T>::operator *= (T scalar)
{
	M[0] *= scalar;
	M[1] *= scalar;
	M[2] *= scalar;
	M[3] *= scalar;
	return *this;
}

template <typename T>
inline
CMatrix2<T>&
CMatrix2<T>::clear()
{
	M[0] = M[1] = M[2] = M[3] = T(0);
	return *this;
}

template <typename T>
inline
CMatrix2<T>&
CMatrix2<T>::makeIdentity()
{
	M[0] = T(1);
	M[1] = T(0);
	M[2] = T(0);
	M[3] = T(1);
	return *this;
}
		
template <typename T>
inline
bool
CMatrix2<T>::isIdentity() const
{
	return (!equals(M[0], T(1))
			&& !equals(M[3], T(1))
			&& iszero(M[1])
			&& iszero(M[2]));
}

template <typename T>
inline
vector2d<T>
CMatrix2<T>::getColumn(u32 c) const
{
	const T* v = this->pointer() + (c * 2);
	return vector2d<T>(v[0], v[1]);
}
		
template <typename T>
inline
CMatrix2<T>&
CMatrix2<T>::setColumn(u32 c, const vector2d<T>& v)
{
	T* dst = this->pointer() + (c * 2);
	dst[0] = v.getX();
	dst[1] = v.getY();
	return *this;
}

template <typename T>
inline
vector2d<T>
CMatrix2<T>::getRow(u32 c) const
{
	const T* v = this->pointer() + c;
	return vector2d<T>(v[0], v[2]);
}
		
template <typename T>
inline
CMatrix2<T>&
CMatrix2<T>::setRow(u32 c, const vector2d<T>& v)
{
	T* dst = this->pointer() + c;
	dst[0] = v.getX();
	dst[2] = v.getY();
	return *this;
}
		
template <typename T>
inline
void
CMatrix2<T>::transformVect(vector2d<T>& out, const vector2d<T>& in) const
{
	out.setX(in.getX() * (*this)[0] + in.getY() * (*this)[2]);
	out.setY(in.getX() * (*this)[1] + in.getY() * (*this)[3]);
}
		
template <typename T>
inline
void
CMatrix2<T>::transformVect(vector2d<T>& vect) const
{
	vector2d<T> tmp(vect);
	transformVect(vect, tmp);
}
		
template <typename T>
inline
void
CMatrix2<T>::transformVect(T* out, const vector2d<T>& in) const
{
	out[0] = in.getX() * (*this)[0] + in.getY() * (*this)[2];
	out[1] = in.getX() * (*this)[1] + in.getY() * (*this)[3];
}
		
template <typename T>
inline
T
CMatrix2<T>::getDeterminant() const
{
	return (*this)[0] * (*this)[3] - (*this)[1] * (*this)[2];
}
		
template <typename T>
inline
bool
CMatrix2<T>::getInverse(CMatrix2& out) const
{
	GLITCH_ASSERT(this != &out);

	double det = getDeterminant();

	if(iszero(det))
	{
		return false;
	}

	det = 1.0 / det;

	out[0] = T(double((*this)[3]) * det);
	out[1] = T(double(-(*this)[1]) * det);
	out[2] = T(double(-(*this)[2]) * det);
	out[3] = T(double((*this)[0]) * det);

	return true;
}
		
template <typename T>
inline
bool
CMatrix2<T>::makeInverse()
{
	CMatrix2 temp(ECT_NOTHING);
	if(getInverse(temp))
	{
		*this = temp;
		return true;
	}
	return false;
}
		
template <typename T>
inline
CMatrix2<T>&
CMatrix2<T>::operator = (T scalar)
{
	M[0] = M[1] = M[2] = M[3] = scalar;
	return *this;
}
		
template <typename T>
inline
bool
CMatrix2<T>::operator == (const CMatrix2& other) const
{
	for(s32 i = 0; i < 4; ++i)
	{
		if((*this)[i] != other[i])
		{
			return false;
		}
	}
	return true;
}
		
template <typename T>
inline
bool
CMatrix2<T>::operator != (const CMatrix2& other) const
{
	return !(*this == other);
}
		
template <typename T>
inline
CMatrix2<T>
CMatrix2<T>::getTransposed() const
{
	CMatrix2<T> t(ECT_NOTHING);
	getTransposed(t);
	return t;
}
		
template <typename T>
inline
void
CMatrix2<T>::getTransposed(CMatrix2& o) const
{
	GLITCH_ASSERT(this != &o);
	
	o[0] = (*this)[0];
	o[1] = (*this)[2];
	o[2] = (*this)[1];
	o[3] = (*this)[3];
}

template <typename T>
inline
CMatrix2<T>&
CMatrix2<T>::setM(const T* data)
{
	M[0] = data[0];
	M[1] = data[1];
	M[2] = data[2];
	M[3] = data[3];
	return *this;
}

template <typename T>
inline
T*
CMatrix2<T>::getM(T* data) const
{
	data[0] = M[0];
	data[1] = M[1];
	data[2] = M[2];
	data[3] = M[3];
	return data;
}

template <typename T>
inline
CMatrix2<T>
operator * (const T scalar, const CMatrix2<T>& mat)
{
	return mat * scalar;
}

//! Typedef for f32 matrix
typedef CMatrix2<f32> matrix2;

//! global const identity matrix
extern const matrix2 IdentityMatrix2;

} // end namespace jenny
