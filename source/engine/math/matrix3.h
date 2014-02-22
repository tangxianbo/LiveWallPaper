#pragma once

#include <core/types.h>
#include <math/math.h>
#include <math/vector2d.h>
#include <math/vector3d.h>
#include <shape/plane3d.h>
#include <shape/aabbox3d.h>

namespace jenny
{
		
template <class T>
class CMatrix3
{
public:

	//! Constructs an identity matrix
	CMatrix3();

	//! Copy constructor
	template < typename U >
	CMatrix3(const CMatrix3<U>& other);

	T& operator [] (u32 index);
	const T& operator [] (u32 index) const;
	T* pointer();
	const T* pointer() const;
			
	//! Simple operator for directly accessing every element of the matrix.
	T& operator () (const s32 col, const s32 row);
			
	//! Simple operator for directly accessing every element of the matrix.
	const T& operator () (const s32 col, const s32 row) const;
			
	//! Sets all elements of this matrix to the value.
	CMatrix3<T>& operator = (T scalar);
			
	//! Returns true if other matrix is equal to this matrix.
	bool operator == (const CMatrix3& other) const;
			
	//! Returns true if other matrix is not equal to this matrix.
	bool operator != (const CMatrix3& other) const;
			
	//! Add another matrix.
	CMatrix3& operator += (const CMatrix3& other);
			
	//! Add another matrix.
	CMatrix3 operator + (const CMatrix3& other) const;
			
	//! Subtract another matrix.
	CMatrix3& operator -= (const CMatrix3& other);
			
	//! Subtract another matrix.
	CMatrix3 operator - (const CMatrix3& other) const;
			
	//! Multiply by another matrix.
	CMatrix3 operator * (const CMatrix3& other) const;
			
	//! Multiply by another matrix.
	CMatrix3& operator *= (const CMatrix3& other);

	//! Multiply by scalar.
	CMatrix3& operator *= (T scalar);

	//! Clear matrix (fill with 0).
	CMatrix3& clear();
			
	//! Set matrix to identity.
	CMatrix3& makeIdentity();
			
	//! Returns true if the matrix is the identity matrix
	bool isIdentity() const;
			
	//! Returns the c'th column of the matrix.
	vector3d<T> getColumn(u32 c) const;
			
	//! Sets the c'th column of the matrix.
	CMatrix3& setColumn(u32 c, const vector3d<T>& v);
			
	//! Returns the c'th row of the matrix.
	vector3d<T> getRow(u32 c) const;
			
	//! Sets the c'th row of the matrix.
	CMatrix3& setRow(u32 c, const vector3d<T>& v);
			
	//! Gets the current translation
	vector2d<T> getTranslation() const;
			
	//! Set the translation of the current matrix. Will erase any previous values.
	CMatrix3& setTranslation(const vector2d<T>& translation);
			
	//! Set the inverse translation of the current matrix. Will erase any previous values.
	CMatrix3& setInverseTranslation(const vector2d<T>& translation);
			
	//! Make a 2D rotation matrix from angle. The 3rd row and column are unmodified.
	CMatrix3& setRotationRadians(T rotation);
			
	//! Make a 2D rotation matrix from angle. The 3rd row and column are unmodified.
	CMatrix3& setRotationDegrees(T rotation);
			
	//! Make a 2D rotation matrix from angle. The 3rd row and column are cleared to identity.
	CMatrix3& makeRotationRadians(T rotation);
			
	//! Make a 2D rotation matrix from angle. The 3rd row and column are cleared to identity.
	CMatrix3& makeRotationDegrees(T rotation);
			
	//! Returns the rotation, as set by setRotation().
	T getRotationDegrees() const;
			
	//! Make an inverted 2D rotation matrix from angle.
	/** The 3rd row and column are unmodified. */
	CMatrix3& setInverseRotationRadians(T rotation);
			
	//! Make an inverted 2D rotation matrix from angle.
	/** The 3rd row and column are unmodified. */
	CMatrix3& setInverseRotationDegrees(T rotation);
			
	//! Set Scale
	CMatrix3& setScale(const vector2d<T>& scale);
			
	//! Set Scale
	CMatrix3& setScale(T scale);
			
	//! Apply scale to this matrix as if multiplication was on the left.
	CMatrix3& preScale(const vector2d<T>& scale);
			
	//! Apply scale to this matrix as if multiplication was on the right.
	CMatrix3& postScale(const vector2d<T>& scale);
			
	//! Get Scale
	vector2d<T> getScale() const;
			
	//! Rotate a vector by the inverse of the rotation part of this matrix.
	void inverseRotateVect(vector2d<T>& vect) const;
			
	//! Rotate a vector by the rotation part of this matrix.
	void rotateVect(vector2d<T>& vect) const;
			
	//! An alternate transform vector method, writing into a second vector
	void rotateVect(vector2d<T>& out, const vector2d<T>& in) const;

	//! An alternate transform vector method, writing into an array of 2 floats
	void rotateVect(T* out, const vector2d<T>& in) const;
			
	//! Transforms the vector by this matrix
	void transformVect(vector2d<T>& vect) const;
			
	//! Full transforms of the vector (with projection)
	void transformVectProj(vector2d<T>& vect) const;

	//! Transforms input vector by this matrix and stores result in output vector
	void transformVect(vector2d<T>& out, const vector2d<T>& in) const;
			
	//! Transforms input vector by this matrix and stores result in output vector
	void transformVect(vector3d<T>& out, const vector3d<T>& in) const;
			
	//! Transforms the vector by this matrix
	void transformVect(vector3d<T>& vect) const;
			
	//! An alternate transform vector method, writing into an array of 3 floats
	void transformVect(T* out, const vector2d<T>& in) const;
			
	//! Transforms a plane by this matrix. The matrix is interpreted as a rotation/scale matrix.
	void transformPlane(const plane3d<T>& in, plane3d<T>& out) const;

	//! Transforms a plane by this matrix. The matrix is interpreted as a rotation/scale matrix.
	void transformPlane(plane3d<T>& plane) const;
			
	//! Transforms a axis aligned bounding box. The matrix is interpreted as a rotation/scale matrix.
	/** The result box of this operation may not be accurate at all. For
		correct results, use transformBoxEx() */
	void transformBox(aabbox3d<T>& box) const;
			
	//! Transforms a axis aligned bounding box. The matrix is interpreted as a rotation/scale matrix.
	/** The result box of this operation should by accurate, but this operation
		is slower than transformBox(). */
	void transformBoxEx(aabbox3d<T>& box) const;
			
	//! Calculates inverse of matrix. Slow.
	/** \return Returns false if there is no inverse matrix.*/
	bool makeInverse();
			
	//! Computes the determinant of the matrix.
	T getDeterminant() const;
			
	//! Inverts a primitive matrix which only contains a rotation
	/** \param out: where result matrix is written to. */
	bool getInversePrimitive(CMatrix3& out) const;
			
	//! Gets the inversed matrix of this one
	/** \param out: where result matrix is written to.
		\return Returns false if there is no inverse matrix. */
	bool getInverse(CMatrix3& out) const;
			
	//! Gets transposed matrix
	CMatrix3 getTransposed() const;
			
	//! Gets transposed matrix
	void getTransposed(CMatrix3& dest) const;
			
	//! Sets all matrix data members at once
	CMatrix3& setM(const T* data);
			
	//! Gets all matrix data members at once
	/** \returns data */
	T* getM(T* data) const;

private:

	T M[9];
};
		
template <typename T>
inline
CMatrix3<T>::CMatrix3()
{
	makeIdentity();
}
		
template <typename T>
template <typename U>
inline
CMatrix3<T>::CMatrix3(const CMatrix3<U>& other)
{
	M[0] = T(other.M[0]);
	M[1] = T(other.M[1]);
	M[2] = T(other.M[2]);
	M[3] = T(other.M[3]);
	M[4] = T(other.M[4]);
	M[5] = T(other.M[5]);
	M[6] = T(other.M[6]);
	M[7] = T(other.M[7]);
	M[8] = T(other.M[8]);
}

template <typename T>
inline
T&
CMatrix3<T>::operator [] (u32 index)
{
	return M[index];
}

template <typename T>
inline
const T&
CMatrix3<T>::operator [] (u32 index) const
{
	return M[index];
}

template <typename T>
inline
T*
CMatrix3<T>::pointer()
{
	return M;
}

template <typename T>
inline
const T*
CMatrix3<T>::pointer() const
{
	return M;
}
			
template <typename T>
inline
T&
CMatrix3<T>::operator () (const s32 col, const s32 row)
{
	return (*this)[col * 3 + row];
}
			
template <typename T>
inline
const T&
CMatrix3<T>::operator () (const s32 col, const s32 row) const
{
	return (*this)[col * 3 + row];
}

template <typename T>
inline
CMatrix3<T>&
CMatrix3<T>::operator += (const CMatrix3& other)
{
	M[0] += other.M[0];
	M[1] += other.M[1];
	M[2] += other.M[2];
	M[3] += other.M[3];
	M[4] += other.M[4];
	M[5] += other.M[5];
	M[6] += other.M[6];
	M[7] += other.M[7];
	M[8] += other.M[8];
	return *this;
}
			
template <typename T>
inline
CMatrix3<T>
CMatrix3<T>::operator + (const CMatrix3& other) const
{
	CMatrix3 result(*this);
	result += other;
	return result;
}
			
template <typename T>
inline
CMatrix3<T>&
CMatrix3<T>::operator -= (const CMatrix3& other)
{
	M[0] -= other.M[0];
	M[1] -= other.M[1];
	M[2] -= other.M[2];
	M[3] -= other.M[3];
	M[4] -= other.M[4];
	M[5] -= other.M[5];
	M[6] -= other.M[6];
	M[7] -= other.M[7];
	M[8] -= other.M[8];
	return *this;
}
			
template <typename T>
inline
CMatrix3<T>
CMatrix3<T>::operator - (const CMatrix3& other) const
{
	CMatrix3 result(*this);
	result -= other;
	return result;
}
			
template <typename T>
inline
CMatrix3<T>
CMatrix3<T>::operator * (const CMatrix3& other) const
{
	CMatrix3 result(ECT_NOTHING);
	result.M[0] = M[0] * other.M[0] + M[3] * other.M[1] + M[6] * other.M[2];
	result.M[1] = M[1] * other.M[0] + M[4] * other.M[1] + M[7] * other.M[2];
	result.M[2] = M[2] * other.M[0] + M[5] * other.M[1] + M[8] * other.M[2];

	result.M[3] = M[0] * other.M[3] + M[3] * other.M[4] + M[6] * other.M[5];
	result.M[4] = M[1] * other.M[3] + M[4] * other.M[4] + M[7] * other.M[5];
	result.M[5] = M[2] * other.M[3] + M[5] * other.M[4] + M[8] * other.M[5];

	result.M[6] = M[0] * other.M[6] + M[3] * other.M[7] + M[6] * other.M[8];
	result.M[7] = M[1] * other.M[6] + M[4] * other.M[7] + M[7] * other.M[8];
	result.M[8] = M[2] * other.M[6] + M[5] * other.M[7] + M[8] * other.M[8];
	return *this;
}
			
template <typename T>
inline
CMatrix3<T>&
CMatrix3<T>::operator *= (const CMatrix3& other)
{
	return *this = (*this) * other;
}

template <typename T>
inline
CMatrix3<T>&
CMatrix3<T>::operator *= (T scalar)
{
	M[0] *= scalar;
	M[1] *= scalar;
	M[2] *= scalar;
	M[3] *= scalar;
	M[4] *= scalar;
	M[5] *= scalar;
	M[6] *= scalar;
	M[7] *= scalar;
	M[8] *= scalar;
	return *this;
}

template <typename T>
inline
CMatrix3<T>&
CMatrix3<T>::clear()
{
	memset(this->pointer(), 0, 9 * sizeof(T));
	return *this;
}

template <typename T>
inline
CMatrix3<T>&
CMatrix3<T>::makeIdentity()
{
	M[0] = T(1);
	M[1] = T(0);
	M[2] = T(0);

	M[3] = T(0);
	M[4] = T(1);
	M[5] = T(0);

	M[6] = T(0);
	M[7] = T(0);
	M[8] = T(1);

	return *this;
}
		
template <typename T>
inline
bool
CMatrix3<T>::isIdentity() const
{
	return (!equals(M[0], T(1))
			&& !equals(M[4], T(1))
			&& !equals(M[8], T(1))
			&& iszero(M[1])
			&& iszero(M[2])
			&& iszero(M[3])
			&& iszero(M[5])
			&& iszero(M[6])
			&& iszero(M[7]));
}

template <typename T>
inline
vector3d<T>
CMatrix3<T>::getColumn(u32 c) const
{
	const T* v = this->pointer() + (c * 3);
	return vector3d<T>(v[0], v[1], v[2]);
}
		
template <typename T>
inline
CMatrix3<T>&
CMatrix3<T>::setColumn(u32 c, const vector3d<T>& v)
{
	T* dst = this->pointer() + (c * 3);
	dst[0] = v.getX();
	dst[1] = v.getY();
	dst[2] = v.getZ();
	return *this;
}

template <typename T>
inline
vector3d<T>
CMatrix3<T>::getRow(u32 c) const
{
	const T* v = this->pointer() + c;
	return vector3d<T>(v[0], v[3], v[6]);
}
		
template <typename T>
inline
CMatrix3<T>&
CMatrix3<T>::setRow(u32 c, const vector3d<T>& v)
{
	T* dst = this->pointer() + c;
	dst[0] = v.getX();
	dst[3] = v.getY();
	dst[6] = v.getZ();
	return *this;
}
		
template <typename T>
inline
vector2d<T>
CMatrix3<T>::getTranslation() const
{
	return vector2d<T>((*this)[6], (*this)[7]);
}
		
template <typename T>
inline
CMatrix3<T>&
CMatrix3<T>::setTranslation(const vector2d<T>& translation)
{
	(*this)[6] = translation.getX();
	(*this)[7] = translation.getY();
	return *this;
}
		
template <typename T>
inline
CMatrix3<T>&
CMatrix3<T>::setInverseTranslation(const vector2d<T>& translation)
{
	(*this)[6] = -translation.getX();
	(*this)[7] = -translation.getY();
	return *this;
}
		
template <typename T>
inline
CMatrix3<T>&
CMatrix3<T>::setScale(const vector2d<T>& scale)
{
	(*this)[0] = scale.getX();
	(*this)[4] = scale.getY();
	return *this;
}

template <typename T>
inline
CMatrix3<T>&
CMatrix3<T>::setScale(T scale)
{
	return setScale(vector2d<T>(scale, scale));
}
		
template <typename T>
inline
CMatrix3<T>&
CMatrix3<T>::preScale(const vector2d<T>& scale)
{
	(*this)[0] *= scale.getX();
	(*this)[1] *= scale.getY();
				
	(*this)[3] *= scale.getX();
	(*this)[4] *= scale.getY();
				
	(*this)[6] *= scale.getX();
	(*this)[7] *= scale.getY();

	return *this;
}
		
template <typename T>
inline
CMatrix3<T>&
CMatrix3<T>::postScale(const vector2d<T>& scale)
{
	(*this)[0] *= scale.getX();
	(*this)[1] *= scale.getX();
				
	(*this)[3] *= scale.getY();
	(*this)[4] *= scale.getY();

	return *this;
}
		
template <typename T>
inline
vector2d<T>
CMatrix3<T>::getScale() const
{
	vector2d<T> vScale(ECT_NOTHING);
	vScale.setX(vector2d<T>((*this)[0], (*this)[1]).getLength());
	vScale.setY(vector2d<T>((*this)[3], (*this)[4]).getLength());
	return vScale;
}
		
template <typename T>
inline
CMatrix3<T>&
CMatrix3<T>::setRotationDegrees(T rotation)
{
	return setRotationRadians(rotation * DEGTORAD);
}
		
template <typename T>
inline
CMatrix3<T>&
CMatrix3<T>::setInverseRotationDegrees(T rotation)
{
	return setInverseRotationRadians(rotation * DEGTORAD);
}
		
template <typename T>
inline
CMatrix3<T>&
CMatrix3<T>::setRotationRadians(T rotation)
{
    const f64 cr = glf::Cos(f64(rotation));
	const f64 sr = glf::Sin(f64(rotation));
	(*this)[0] = T(cr);
	(*this)[1] = T(sr);
	(*this)[3] = T(-sr);
	(*this)[4] = T(cr);
	return *this;
}
		
template <typename T>
inline
CMatrix3<T>&
CMatrix3<T>::makeRotationRadians(T rotation)
{
	setRotationRadians(rotation);
	setTranslation(vector2d<T>(T(0)));
	(*this)[2] = (*this)[5] = T(0);
	(*this)[8] = T(1);
	return *this;
}
		
template <typename T>
inline
CMatrix3<T>&
CMatrix3<T>::makeRotationDegrees(T rotation)
{
	return makeRotationRadians(rotation * DEGTORAD);
}
		
template <typename T>
inline
T
CMatrix3<T>::getRotationDegrees() const
{
	const CMatrix3<T>& mat = *this;

	f64 r = glf::Atan2(f64(M[1]), f64(M[0])) * RADTODEG64;
	if(r < 0.0)
	{
		r += 360.0;
	}
	return T(r);
}		
		
template <typename T>
inline
CMatrix3<T>&
CMatrix3<T>::setInverseRotationRadians(T rotation)
{
	return setRotationRadians(-rotation);
}
		
template <typename T>
inline
void
CMatrix3<T>::rotateVect(vector2d<T>& out, const vector2d<T>& in) const
{
	out.setX(in.getX() * (*this)[0] + in.getY() * (*this)[3]);
	out.setY(in.getX() * (*this)[1] + in.getY() * (*this)[4]);
}
		
template <typename T>
inline
void
CMatrix3<T>::rotateVect(vector2d<T>& vect) const
{
	vector2d<T> tmp = vect;
	rotateVect(vect, tmp);
}
		
template <typename T>
inline
void
CMatrix3<T>::rotateVect(T* out, const vector2d<T>& in) const
{
	out[0] = in.getX() * (*this)[0] + in.getY() * (*this)[3];
	out[1] = in.getX() * (*this)[1] + in.getY() * (*this)[4];
}
		
template <typename T>
inline
void
CMatrix3<T>::inverseRotateVect(vector2d<T>& vect) const
{
	vector2d<T> tmp = vect;
	vect.setX(tmp.getX() * (*this)[0] + tmp.getY() * (*this)[1]);
	vect.setY(tmp.getX() * (*this)[3] + tmp.getY() * (*this)[4]);
}
		
template <typename T>
inline
void
CMatrix3<T>::transformVect(vector2d<T>& out, const vector2d<T>& in) const
{
	out.setX(in.getX() * (*this)[0] + in.getY() * (*this)[3] + (*this)[6]);
	out.setY(in.getX() * (*this)[1] + in.getY() * (*this)[4] + (*this)[7]);
}
		
template <typename T>
inline
void
CMatrix3<T>::transformVect(vector2d<T>& vect) const
{
	T vector[2];

	vector[0] = vect.getX() * (*this)[0] + vect.getY() * (*this)[3] + (*this)[6];
	vector[1] = vect.getX() * (*this)[1] + vect.getY() * (*this)[4] + (*this)[7];
			
	vect.setX(vector[0]);
	vect.setY(vector[1]);
}

template <typename T>
inline
void
CMatrix3<T>::transformVectProj(vector2d<T>& vect) const
{
	T vector[3];

	vector[0] = vect.getX() * (*this)[0] + vect.getY() * (*this)[3] + (*this)[6];
	vector[1] = vect.getX() * (*this)[1] + vect.getY() * (*this)[4] + (*this)[7];
	vector[2] = vect.getX() * (*this)[2] + vect.getY() * (*this)[5] + (*this)[8];
			
	vect.setX(vector[0] / vector[2]);
	vect.setY(vector[1] / vector[2]);
}
		
template <typename T>
inline
void
CMatrix3<T>::transformVect(vector3d<T>& out, const vector3d<T>& in) const
{
	out.setX(in.getX() * (*this)[0] + in.getY() * (*this)[3] + in.getZ() * (*this)[6]);
	out.setY(in.getX() * (*this)[1] + in.getY() * (*this)[4] + in.getZ() * (*this)[7]);
	out.setZ(in.getX() * (*this)[2] + in.getY() * (*this)[4] + in.getZ() * (*this)[7]);
}
		
template <typename T>
inline
void
CMatrix3<T>::transformVect(vector3d<T>& vect) const
{
	vector3d<T> tmp(vect);
	transformVect(vect, tmp);
}
		
template <typename T>
inline
void
CMatrix3<T>::transformVect(T* out, const vector2d<T>& in) const
{
	out[0] = in.getX() * (*this)[0] + in.getY() * (*this)[3] + (*this)[6];
	out[1] = in.getX() * (*this)[1] + in.getY() * (*this)[4] + (*this)[7];
	out[2] = in.getX() * (*this)[2] + in.getY() * (*this)[5] + (*this)[8];
}
		
template <typename T>
inline
void
CMatrix3<T>::transformPlane(const plane3d<T>& in, plane3d<T>& out) const
{
	vector3d<T> member(ECT_NOTHING);
	transformVect(member, in.getMemberPoint());
			
	transformVect(out.Normal, in.Normal);
		
	out.D = -member.dotProduct(in.Normal);
}
		
template <typename T>
inline
void
CMatrix3<T>::transformPlane(plane3d<T>& plane) const
{
	plane3d<T> tmp(plane);
	transformPlane(plane, tmp);
}
		
template <typename T>
inline
void
CMatrix3<T>::transformBox(aabbox3d<T>& box) const
{
	transformVect(box.MinEdge);
	transformVect(box.MaxEdge);
	box.repair();
}
		
template <typename T>
inline
void
CMatrix3<T>::transformBoxEx(aabbox3d<T>& box) const
{
	const T amin[3] = {box.MinEdge.getX(), box.MinEdge.getY(), box.MinEdge.getZ()};
	const T amax[3] = {box.MaxEdge.getX(), box.MaxEdge.getY(), box.MaxEdge.getZ()};
			
	T bmin[3] = { T(0), T(0), T(0) };
	T bmax[3] = { T(0), T(0), T(0) };
			
	const CMatrix3& m = *this;
			
	for(u32 i = 0; i < 3; ++i)
	{
		for(u32 j = 0; j < 3; ++j)
		{
			const T a = m(j, i) * amin[j];
			const T b = m(j, i) * amax[j];
					
			if(a < b)
			{
				bmin[i] += a;
				bmax[i] += b;
			}
			else
			{
				bmin[i] += b;
				bmax[i] += a;
			}
		}
	}
			
	box.MinEdge.setX(bmin[0]);
	box.MinEdge.setY(bmin[1]);
	box.MinEdge.setZ(bmin[2]);
			
	box.MaxEdge.setX(bmax[0]);
	box.MaxEdge.setY(bmax[1]);
	box.MaxEdge.setZ(bmax[2]);
}
		
template <typename T>
inline
T
CMatrix3<T>::getDeterminant() const
{
	T t0 = (*this)[4] * (*this)[8] - (*this)[7] * (*this)[5];
	T t1 = (*this)[1] * (*this)[8] - (*this)[7] * (*this)[2];
	T t2 = (*this)[1] * (*this)[5] - (*this)[4] * (*this)[2];
			
	return (*this)[0] * t0 - (*this)[3] * t1 + (*this)[6] * t2;
}
		
template <typename T>
inline
bool
CMatrix3<T>::getInverse(CMatrix3& out) const
{
	GLITCH_ASSERT(this != &out);

	// Cramer's rule.
	double t0 = double((*this)[4] * (*this)[8] - (*this)[7] * (*this)[5]);
	double t1 = double((*this)[7] * (*this)[2] - (*this)[1] * (*this)[8]);
	double t2 = double((*this)[1] * (*this)[5] - (*this)[4] * (*this)[2]);

	double det = (*this)[0] * t0 + (*this)[3] * t1 + (*this)[6] * t2;

	if(iszero(det))
	{
		return false;
	}

	det = 1.0 / det;

	out[0] = T(t0 * det);
	out[1] = T(t1 * det);
	out[2] = T(t2 * det);

	out[3] = T(double((*this)[6] * (*this)[5] - (*this)[3] * (*this)[8]) * det);
	out[4] = T(double((*this)[0] * (*this)[8] - (*this)[6] * (*this)[2]) * det);
	out[5] = T(double((*this)[3] * (*this)[2] - (*this)[0] * (*this)[5]) * det);

	out[6] = T(double((*this)[3] * (*this)[7] - (*this)[6] * (*this)[4]) * det);
	out[7] = T(double((*this)[6] * (*this)[1] - (*this)[0] * (*this)[7]) * det);
	out[8] = T(double((*this)[0] * (*this)[4] - (*this)[3] * (*this)[1]) * det);

	return true;
}
		
template <typename T>
inline
bool
CMatrix3<T>::getInversePrimitive(CMatrix3& out) const
{
	getTransposed(out);
	return true;
}
		
template <typename T>
inline
bool
CMatrix3<T>::makeInverse()
{
	CMatrix3 temp(ECT_NOTHING);
	if(getInverse(temp))
	{
		*this = temp;
		return true;
	}
	return false;
}
		
template <typename T>
inline
CMatrix3<T>&
CMatrix3<T>::operator = (T scalar)
{
	for(s32 i = 0; i < 9; ++i)
	{
		(*this)[i] = scalar;
	}
	return *this;
}
		
template <typename T>
inline
bool
CMatrix3<T>::operator == (const CMatrix3& other) const
{
	for(s32 i = 0; i < 9; ++i)
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
CMatrix3<T>::operator != (const CMatrix3& other) const
{
	return !(*this == other);
}
		
template <typename T>
inline
CMatrix3<T>
CMatrix3<T>::getTransposed() const
{
	CMatrix3<T> t(ECT_NOTHING);
	getTransposed(t);
	return t;
}
		
template <typename T>
inline
void
CMatrix3<T>::getTransposed(CMatrix3& o) const
{
	GLITCH_ASSERT(this != &o);
	
	o[0] = (*this)[0];
	o[1] = (*this)[3];
	o[2] = (*this)[6];
			
	o[3] = (*this)[1];
	o[4] = (*this)[4];
	o[5] = (*this)[7];
			
	o[6] = (*this)[2];
	o[7] = (*this)[5];
	o[8] = (*this)[8];
}

template <typename T>
inline
CMatrix3<T>&
CMatrix3<T>::setM(const T* data)
{
	memcpy(this->pointer(), data, 9 * sizeof(T));
	return *this;
}

template <typename T>
inline
T*
CMatrix3<T>::getM(T* data) const
{
	memcpy(data, this->pointer(), 9 * sizeof(T));
	return data;
}

template <typename T>
inline
CMatrix3<T>
operator * (const T scalar, const CMatrix3<T>& mat)
{
	return mat * scalar;
}

//! Typedef for f32 matrix
typedef CMatrix3<f32> matrix3;

//! global const identity matrix
extern const matrix3 IdentityMatrix3;

} // end namespace jenny
