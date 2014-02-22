#pragma once

#include <core/types.h>
#include <math/vector4d.h>
#include <math/matrix3.h>
#include <math/vector2d.h>
#include <math/math.h>
#include <shape/plane3d.h>
#include <shape/aabbox3d.h>
#include <shape/rect.h>

namespace jenny
{
		
//! 4x4 matrix. Mostly used as transformation matrix for 3d calculations.
/** The matrix is column major with translations in the 4th column. */

template <class T>
class CMatrix4 //: public detail::CMatrix4<T>
{
public:

	//! Constructor Flags
	enum eConstructor
	{
		EM4CONST_NOTHING = 0,		
		EM4CONST_COPY,
		EM4CONST_IDENTITY,
		EM4CONST_TRANSPOSED,
		EM4CONST_INVERSE,
		EM4CONST_INVERSE_TRANSPOSED,
		EM4CONST_CLEAR
	};

	//! Default constructor
	/** \param constructor Choose the initialization style */
	CMatrix4( eConstructor constructor = EM4CONST_IDENTITY );
	//! Copy constructor
	/** \param other Other matrix to copy from
		\param constructor Choose the initialization style */
	template<class U>
	CMatrix4(const CMatrix4<U>& other, eConstructor constructor = EM4CONST_COPY);

	CMatrix4(const CMatrix4<T>& other, eConstructor constructor = EM4CONST_COPY);

	template<typename U>
	explicit CMatrix4(const CMatrix3<U>& upperLeft3x3);

	T* pointer()
	{
		return M;
	}

	const T* pointer() const
	{
		return M;
	}

	T& operator [] (u32 index)
	{
		return M[index];
	}

	const T& operator [] (u32 index) const
	{
		return M[index];
	}
			
	//! Simple operator for directly accessing every element of the matrix.
	T& operator () (const s32 col, const s32 row)
	{
		return (*this)[col * 4 + row];
	}
			
	//! Simple operator for directly accessing every element of the matrix.
	const T& operator () (const s32 col, const s32 row) const
	{
		return (*this)[col * 4 + row];
	}
			
	//! Sets all elements of this matrix to the value.
	CMatrix4<T>& operator=(const T& scalar);

	//! Returns true if other matrix is equal to this matrix.
	bool operator == (const CMatrix4<T>& other) const;
			
	//! Returns true if other matrix is not equal to this matrix.
	bool operator != (const CMatrix4<T>& other) const;
			
	//! Add another matrix.
	CMatrix4<T> operator + (const CMatrix4<T>& other) const
	{
		return add(other);
	}
			
	//! Add another matrix.
	CMatrix4<T>& operator += (const CMatrix4<T>& other)
	{
		addEq(other);
		return *this;
	}
			
	//! Subtract another matrix.
	CMatrix4<T> operator - (const CMatrix4<T>& other) const
	{
		return sub(other);
	}
			
	//! Subtract another matrix.
	CMatrix4<T>& operator -= (const CMatrix4<T>& other)
	{
		subEq(other);
		return *this;
	}
			
	//! Multiply by another matrix.
	CMatrix4<T> operator * (const CMatrix4<T>& other) const
	{
		return this->mult(other);
	}
			
	//! Multiply by another matrix.
	CMatrix4<T>& operator *= (const CMatrix4<T>& other)
	{
		this->multEq(other);
		return *this;
	}
			
	//! Multiply by scalar.
	CMatrix4<T> operator * (const T& scalar) const
	{
		return this->multScal(scalar);
	}

	//! Multiply by scalar.
	CMatrix4<T>& operator *= (const T& scalar)
	{
		this->multEqScal(scalar);
		return *this;
	}

	//! Clear matrix (fill with 0).
	CMatrix4<T>& clear();
			
	//! Set matrix to identity.
	CMatrix4<T>& makeIdentity();
			
	//! Returns true if the matrix is the identity matrix
	bool isIdentity() const;
			
	//! Returns the c'th column of the matrix, without the lowest row.
	vector3d<T> getColumn(u32 c) const;
			
	//! Returns the c'th column of the matrix.
	vector4d<T> getFullColumn(u32 c) const;
			
	//! Sets the c'th column of the matrix, without the lowest row.
	CMatrix4<T>& setColumn(u32 c, const vector3d<T>& v);
			
	//! Returns the c'th row of the matrix, without the last column.
	vector3d<T> getRow(u32 c) const;
			
	//! Sets the c'th row of the matrix, without the lowest column.
	CMatrix4<T>& setRow(u32 c, const vector3d<T>& v);
			
	//! Sets the c'th column of the matrix.
	CMatrix4<T>& setFullColumn(u32 c, const vector4d<T>& v);
			
	//! Gets the current translation
	vector3d<T> getTranslation() const;
			
	//! Set the translation of the current matrix. Will erase any previous values.
	CMatrix4<T>& setTranslation(const vector3d<T>& translation);
			
	//! Set the inverse translation of the current matrix. Will erase any previous values.
	CMatrix4<T>& setInverseTranslation( const vector3d<T>& translation);
			
	//! Make a rotation matrix from Euler angles. The 4th row and column are unmodified.
	CMatrix4<T>& setRotationRadians(const vector3d<T>& rotation);
			
	//! Make a rotation matrix from Euler angles. The 4th row and column are unmodified.
	CMatrix4<T>& setRotationDegrees(const vector3d<T>& rotation);

	//! set this matrix to the product of two matrices
	CMatrix4<T>& setbyproduct(const CMatrix4<T>& other_a, const CMatrix4<T>& other_b);

	//! Set this matrix to the product of two matrices
	/** no optimization used,
	use it if you know you never have a identity matrix */
	CMatrix4<T>& setbyproduct_nocheck(const CMatrix4<T>& other_a, const CMatrix4<T>& other_b);
			
	//! Make a rotation matrix from Euler angles. The 4th row and column are cleared to identity.
	CMatrix4<T>& makeRotationRadians(const vector3d<T>& rotation);
			
	//! Make a rotation matrix from Euler angles. The 4th row and column are cleared to identity.
	CMatrix4<T>& makeRotationDegrees(const vector3d<T>& rotation);
			
	//! Returns the rotation, as set by setRotation().
	/** This code was orginally written by by Chev. */
	vector3d<T> getRotationDegrees() const;
			
	//! Make an inverted rotation matrix from Euler angles.
	/** The 4th row and column are unmodified. */
	inline CMatrix4<T>& setInverseRotationRadians(const vector3d<T>& rotation);
			
	//! Make an inverted rotation matrix from Euler angles.
	/** The 4th row and column are unmodified. */
	CMatrix4<T>& setInverseRotationDegrees(const vector3d<T>& rotation);
			
	//! Set Scale
	CMatrix4<T>& setScale(const vector3d<T>& scale);
			
	//! Set Scale
	CMatrix4<T>& setScale(const T scale)
	{
		return setScale(vector3d<T>(scale, scale ,scale));
	}
			
	//! Apply scale to this matrix as if multiplication was on the left.
	CMatrix4<T>& preScale(const vector3d<T>& scale);
			
	//! Apply scale to this matrix as if multiplication was on the right.
	CMatrix4<T>& postScale(const vector3d<T>& scale);
			
	//! Get Scale
	vector3d<T> getScale() const;
			
	//! Translate a vector by the inverse of the translation part of this matrix.
	void inverseTranslateVect(vector3d<T>& vect) const;
			
	//! Rotate a vector by the inverse of the rotation part of this matrix.
	void inverseRotateVect(vector3d<T>& vect) const;
			
	//! Rotate a vector by the rotation part of this matrix.
	void rotateVect(vector3d<T>& vect) const;
			
	//! An alternate transform vector method, writing into a second vector
	void rotateVect(vector3d<T>& out, const vector3d<T>& in) const;
			
	//! An alternate transform vector method, writing into an array of 3 floats
	void rotateVect(T* out, const vector3d<T>& in) const;
			
	//! Transforms the vector by this matrix
	void transformVect(vector3d<T>& vect) const;
			
	//! Full transforms of the vector (with projection)
	void transformVectProj(vector3d<T>& vect) const;

	//! Transforms input vector by this matrix and stores result in output vector
	void transformVect(vector3d<T>& out, const vector3d<T>& in) const;
			
	//! Transforms the vector by this matrix
	void transformVect(vector4d<T>& vect) const
	{
		this->transVect(vect);
	}
			
	//! Transforms input vector by this matrix and stores result in output vector
	void transformVect(vector4d<T>& out, const vector4d<T>& in) const
	{
		this->transVect(out, in);
	}
			
	//! Transforms the vector by this matrix as though it was in 2D (Z ignored).
	void transformVect2D(vector3d<T>& vect) const;

	//! Transforms input vector by this matrix and stores result in output vector as though it was in 2D (Z ignored).
	void transformVect2D(vector3d<T>& out, const vector3d<T>& in) const;
			
	//! Transforms the vector by this matrix
	void transformVect(vector2d<T>& vect) const;
			
	//! Transforms input vector by this matrix and stores result in output vector
	void transformVect(vector2d<T>& out, const vector2d<T>& in) const;
			
	//! Transforms the vector representing texture coordinates by this matrix
	void transformTexCoord(vector2d<T>& vect) const;
			
	//! Transforms input vector representing texture coordinates by this matrix and stores result in output vector
	void transformTexCoord(vector2d<T>& out, const vector2d<T>& in) const;
			
	//! An alternate transform vector method, writing into an array of 4 floats
	void transformVect(T* out, const vector3d<T>& in) const;
			
	//! Translate a vector by the translation part of this matrix.
	void translateVect(vector3d<T>& vect) const;

	//! Transforms a plane by this matrix
	void transformPlane(plane3d<T>& plane) const;
			
	//! Transforms a plane by this matrix ( some problems to solve..)
	void transformPlane_new(plane3d<T>& plane) const;
			
	//! Transforms a plane by this matrix
	void transformPlane(const plane3d<T>& in, plane3d<T>& out) const;
			
	//! Transforms a axis aligned bounding box
	/** The result box of this operation may not be accurate at all. For
		correct results, use transformBoxEx() */
	void transformBox(aabbox3d<T>& box) const;
			
	//! Transforms a axis aligned bounding box
	/** The result box of this operation should by accurate, but this operation
		is slower than transformBox(). */
	void transformBoxEx(aabbox3d<T>& box) const;
			
	//! Multiplies this matrix by a 1x4 matrix
	void multiplyWith1x4Matrix(T* matrix) const;
			
	//! Calculates inverse of matrix. Slow.
	/** \return Returns false if there is no inverse matrix.*/
	bool makeInverse();
			
	//! Computes the determinant of the matrix.
	T getDeterminant() const;
			
	//! Inverts a primitive matrix which only contains a translation and a rotation
	/** \param out: where result matrix is written to. */
	bool getInversePrimitive(CMatrix4<T>& out) const;
			
	//! Gets the inversed matrix of this one
	/** \param out: where result matrix is written to.
		\return Returns false if there is no inverse matrix. */
	bool getInverse(CMatrix4<T>& out) const;
			
	//! Creates a newly matrix as interpolated matrix from two other ones.
	/** \param b: other matrix to interpolate with
		\param time: Must be a value between 0 and 1. */
	CMatrix4<T> interpolate(const CMatrix4<T>& b, T time) const;
			
	//! Gets transposed matrix
	CMatrix4<T> getTransposed() const;
			
	//! Gets transposed matrix
	void getTransposed(CMatrix4<T>& dest) const;


	//! Multiply by another matrix as if both matrices where 3x4.
	CMatrix4<T> mult34(const CMatrix4<T>& m2) const;

	//! Multiply by another matrix as if both matrices where 3x4.
	CMatrix4<T>& mult34(const CMatrix4<T>& m2, CMatrix4<T>& out) const;

	//! Multiply by another matrix as if both matrices where 3x3.
	CMatrix4<T> mult33(const CMatrix4<T>& m2) const;

	//! Multiply by another matrix as if both matrices where 3x3.
	CMatrix4<T>& mult33(const CMatrix4<T>& m2, CMatrix4<T>& out) const;
			
	//! Set texture transformation rotation
	/** Rotate about z axis, recenter at (0.5,0.5).
		Doesn't clear other elements than those affected
		\param radAngle Angle in radians
		\return Altered matrix */
	CMatrix4<T>& setTextureRotationCenter(T radAngle);
			
	//! Set texture transformation translation
	/** Doesn't clear other elements than those affected.
		\param x Offset on x axis
		\param y Offset on y axis
		\return Altered matrix */
	CMatrix4<T>& setTextureTranslate(T x, T y);
			
	//! Set texture transformation translation, using a transposed representation
	/** Doesn't clear other elements than those affected.
		\param x Offset on x axis
		\param y Offset on y axis
		\return Altered matrix */
	CMatrix4<T>& setTextureTranslateTransposed(T x, T y);
			
	//! Set texture transformation scale
	/** Doesn't clear other elements than those affected.
		\param sx Scale factor on x axis
		\param sy Scale factor on y axis
		\return Altered matrix. */
	CMatrix4<T>& setTextureScale(T sx, T sy);
			
	//! Set texture transformation scale, and recenter at (0.5,0.5)
	/** Doesn't clear other elements than those affected.
		\param sx Scale factor on x axis
		\param sy Scale factor on y axis
		\return Altered matrix. */
	CMatrix4<T>& setTextureScaleCenter(T sx, T sy);
			
	//! Applies a texture post scale.
	/**	\param sx Scale factor on x axis
		\param sy Scale factor on y axis
		\return Altered matrix. */
	CMatrix4<T>& postTextureScale (T sx, T sy);
			
	//! Sets all matrix data members at once
	CMatrix4<T>& setM(const T* data);
			
	//! Gets all matrix data members at once
	/** \returns data */
	T* getM(T* data) const;

protected:
	//! Add another matrix.
	CMatrix4<T> add(const CMatrix4<T>& other) const;

	//! Add another matrix.
	CMatrix4<T>& addEq(const CMatrix4<T>& other);

	//! Subtract another matrix.
	CMatrix4<T> sub(const CMatrix4<T>& other) const;

	//! Subtract another matrix.
	CMatrix4<T>& subEq(const CMatrix4<T>& other);

	//! Multiply by another matrix.
	CMatrix4<T> mult(const CMatrix4<T>& other) const;

	//! Multiply by another matrix.
	CMatrix4<T>& multEq(const CMatrix4<T>& other);

	//! Multiply by scalar.
	CMatrix4<T> multScal(const T& scalar) const;

	//! Multiply by scalar.
	CMatrix4<T>& multEqScal(const T& scalar);

	//! Transforms the vector by this matrix
	void transVect(vector4d<T>& vect) const;

	//! Transforms input vector by this matrix and stores result in output vector
	void transVect(vector4d<T>& out, const vector4d<T>& in) const;

	//! Transforms the vector by this matrix
	void transVect(vector3d<T>& vect) const;

	//! Transforms input vector by this matrix and stores result in output vector
	void transVect(vector3d<T>& out, const vector3d<T>& in) const;

	//! Rotate the vector by this matrix
	void rotVect(vector3d<T>& vect) const;

	//! Rotate input vector by this matrix and stores result in output vector
	void rotVect(vector3d<T>& out, const vector3d<T>& in) const;

private:

	T M[16];
};
		
template <class T>
inline
CMatrix4<T>::CMatrix4( eConstructor constructor )
{
	switch(constructor)
	{
		case EM4CONST_NOTHING:
		case EM4CONST_COPY:
			break;

		case EM4CONST_CLEAR:
			clear();
			break;

		case EM4CONST_IDENTITY:
		case EM4CONST_INVERSE:
		default:
		{
			makeIdentity();
		}
		break;
	}
}
	
template <class T>
template <class U>
inline
CMatrix4<T>::CMatrix4( const CMatrix4<U>& other,eConstructor constructor)
{
	for(int i = 0; i < 16; ++i)
	{
		(*this)[i] = (T)other[i];
	}
	(*this) = CMatrix4<T>(*this, constructor);
}

template <class T>
inline
CMatrix4<T>::CMatrix4( const CMatrix4<T>& other, eConstructor constructor)
{
	switch(constructor)
	{
		case EM4CONST_IDENTITY:
		{
			makeIdentity();
		}
		break;

		case EM4CONST_NOTHING:
			break;

		case EM4CONST_COPY:
		{
			*this = other;
		}
		break;

		case EM4CONST_CLEAR:
		{
			clear();
		}
		break;

		case EM4CONST_TRANSPOSED:
		{
			other.getTransposed(*this);
		}
		break;

		case EM4CONST_INVERSE:
		{
			if(!other.getInverse(*this))
			{
				memset(this->pointer(), 0, 16 * sizeof(T));
			}
		}
		break;

		case EM4CONST_INVERSE_TRANSPOSED:
		{
			if(!other.getInverse(*this))
			{
				memset(this->pointer(), 0, 16 * sizeof(T));
			}
			else
			{
				*this = getTransposed();
			}
		}
		break;
	}
}

template<typename T>
template<typename U>
inline
CMatrix4<T>::CMatrix4(const CMatrix3<U>& upperLeft3x3)
{
	(*this)[0] = upperLeft3x3[0];
	(*this)[1] = upperLeft3x3[1];
	(*this)[2] = upperLeft3x3[2];
	(*this)[3] = T(0);

	(*this)[4] = upperLeft3x3[3];
	(*this)[5] = upperLeft3x3[4];
	(*this)[6] = upperLeft3x3[5];
	(*this)[7] = T(0);

	(*this)[8] = upperLeft3x3[6];
	(*this)[9] = upperLeft3x3[7];
	(*this)[10] = upperLeft3x3[9];
	(*this)[11] = T(0);

	(*this)[12] = T(0);
	(*this)[13] = T(0);
	(*this)[14] = T(0);
	(*this)[15] = T(1);
}
		
template <class T>
inline
vector3d<T>
CMatrix4<T>::getColumn(u32 c) const
{
	const T* v = this->pointer() + (c * 4);
	return vector3d<T>(v[0], v[1], v[2]);
}
		
template <class T>
inline
vector4d<T>
CMatrix4<T>::getFullColumn(u32 c) const
{
	const T* v = this->pointer() + (c * 4);
	return vector4d<T>(v[0], v[1], v[2], v[3]);
}
		
template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::setColumn(u32 c, const vector3d<T>& v)
{
	T* dst = this->pointer() + (c * 4);
	dst[0] = v.getX();
	dst[1] = v.getY();
	dst[2] = v.getZ();
	return *this;
}

template <class T>
inline
vector3d<T>
CMatrix4<T>::getRow(u32 c) const
{
	const T* v = this->pointer() + c;
	return vector3d<T>(v[0], v[4], v[8]);
}
		
template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::setRow(u32 c, const vector3d<T>& v)
{			
	T* dst = &(*this)[c];
	dst[0] = v.getX();
	dst[4] = v.getY();
	dst[8] = v.getZ();
	return *this;
}
		
template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::setFullColumn(u32 c, const vector4d<T>& v)
{			
	T* dst = &(*this)[c * 4];
	dst[0] = v.getX();
	dst[1] = v.getY();
	dst[2] = v.getZ();
	dst[3] = v.getW();
	return *this;
}
		
template <class T>
inline
vector3d<T>
CMatrix4<T>::getTranslation() const
{
	return vector3d<T>((*this)[12], (*this)[13], (*this)[14]);
}
		
template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::setTranslation(const vector3d<T>& translation)
{
	(*this)[12] = translation.getX();
	(*this)[13] = translation.getY();
	(*this)[14] = translation.getZ();

	return *this;
}
		
template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::setInverseTranslation(const vector3d<T>& translation)
{
	(*this)[12] = -translation.getX();
	(*this)[13] = -translation.getY();
	(*this)[14] = -translation.getZ();

	return *this;
}
		
template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::setScale(const vector3d<T>& scale)
{
	(*this)[0] = scale.getX();
	(*this)[5] = scale.getY();
	(*this)[10] = scale.getZ();

	return *this;
}
		
template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::preScale(const vector3d<T>& scale)
{
	if(this->getDefinitelyIdentityMatrix())
	{
		setScale(scale);
	}
	else
	{
		(*this)[0] *= scale.getX();
		(*this)[1] *= scale.getY();
		(*this)[2] *= scale.getZ();
				
		(*this)[4] *= scale.getX();
		(*this)[5] *= scale.getY();
		(*this)[6] *= scale.getZ();
				
		(*this)[8] *= scale.getX();
		(*this)[9] *= scale.getY();
		(*this)[10] *= scale.getZ();
				
		(*this)[12] *= scale.getX();
		(*this)[13] *= scale.getY();
		(*this)[14] *= scale.getZ();

	}
	return *this;
}
		
template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::postScale(const vector3d<T>& scale)
{
	if(this->getDefinitelyIdentityMatrix())
	{
		setScale(scale);
	}
	else
	{
		(*this)[0] *= scale.getX();
		(*this)[1] *= scale.getX();
		(*this)[2] *= scale.getX();
				
		(*this)[4] *= scale.getY();
		(*this)[5] *= scale.getY();
		(*this)[6] *= scale.getY();
				
		(*this)[8] *= scale.getZ();
		(*this)[9] *= scale.getZ();
		(*this)[10] *= scale.getZ();

	}
	return *this;
}
		
template <class T>
inline
vector3d<T>
CMatrix4<T>::getScale() const
{
	vector3d<T> vScale(ECT_NOTHING);
	vScale.setX(vector3d<T>((*this)[0], (*this)[1], (*this)[2]).getLength());
	vScale.setY(vector3d<T>((*this)[4], (*this)[5], (*this)[6]).getLength());
	vScale.setZ(vector3d<T>((*this)[8], (*this)[9], (*this)[10]).getLength());
	return vScale;
}
		
template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::setRotationDegrees(const vector3d<T>& rotation)
{
	return setRotationRadians(rotation * DEGTORAD);
}
		
template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::setInverseRotationDegrees(const vector3d<T>& rotation)
{
	return setInverseRotationRadians(rotation * DEGTORAD);
}
		
template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::setRotationRadians(const vector3d<T>& rotation)
{
    const f64 cr = glf::Cos(f64(rotation.getX()));
	const f64 sr = glf::Sin(f64(rotation.getX()));
	const f64 cp = glf::Cos(f64(rotation.getY()));
	const f64 sp = glf::Sin(f64(rotation.getY()));
	const f64 cy = glf::Cos(f64(rotation.getZ()));
	const f64 sy = glf::Sin(f64(rotation.getZ()));
			
	(*this)[0] = T(cp * cy);
	(*this)[1] = T(cp * sy);
	(*this)[2] = T(-sp);
			
	const f64 srsp = sr * sp;
	const f64 crsp = cr * sp;
			
	(*this)[4] = T(srsp * cy - cr * sy);
	(*this)[5] = T(srsp * sy + cr * cy);
	(*this)[6] = T(sr * cp);
			
	(*this)[8] = T(crsp * cy + sr * sy);
	(*this)[9] = T(crsp * sy - sr * cy);
	(*this)[10] = T(cr * cp);

	return *this;
}
		
template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::makeRotationRadians(const vector3d<T>& rotation)
{
	setRotationRadians(rotation);
	setTranslation(vector3d<T>(T(0)));
	(*this)[3] = (*this)[7] = (*this)[11] = T(0);
	(*this)[15] = T(1);
	return *this;
}
		
template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::makeRotationDegrees(const vector3d<T>& rotation)
{
	return makeRotationRadians(rotation * DEGTORAD);
}
		
template <class T>
inline
vector3d<T>
CMatrix4<T>::getRotationDegrees() const
{
	const CMatrix4<T>& mat = *this;
			
	f64 y = -SafeAsin(mat(0, 2));
	const f64 c = Cos(y);
	y *= RADTODEG64;
			
	f64 rotx, roty, x, z;
			
	if(std::abs(c) > ROUNDING_ERROR_64)
	{
		const f64 invC = 1.0 / c;
		rotx = mat(2, 2) * invC;
		roty = mat(1, 2) * invC;
		x = Atan2(roty, rotx) * RADTODEG64;
		rotx = mat(0, 0) * invC;
		roty = mat(0, 1) * invC;
		z = Atan2(roty, rotx) * RADTODEG64;
	}
	else
	{
		x = 0.0;
		rotx = mat(1, 1);
		roty = -mat(1, 0);
		z = Atan2(roty, rotx) * RADTODEG64;
	}
			
	// fix values that get below zero
	// before it would set (!) values to 360
	// that where above 360:
	if(x < 0.0) x += 360.0;
	if(y < 0.0) y += 360.0;
	if(z < 0.0) z += 360.0;
			
	return vector3d<T>(T(x), T(y), T(z));
}		
		
template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::setInverseRotationRadians(const vector3d<T>& rotation)
{
	f64 cr = glf::Cos(rotation.getX());
	f64 sr = glf::Sin(rotation.getX());
	f64 cp = glf::Cos(rotation.getY());
	f64 sp = glf::Sin(rotation.getY());
	f64 cy = glf::Cos(rotation.getZ());
	f64 sy = glf::Sin(rotation.getZ());
			
	(*this)[0] = T(cp * cy);
	(*this)[4] = T(cp * sy);
	(*this)[8] = T(-sp);
			
	f64 srsp = sr * sp;
	f64 crsp = cr * sp;
			
	(*this)[1] = T(srsp * cy - cr * sy);
	(*this)[5] = T(srsp * sy + cr * cy);
	(*this)[9] = T(sr * cp);
			
	(*this)[2] = T(crsp * cy + sr * sy);
	(*this)[6] = T(crsp * sy - sr * cy);
	(*this)[10] = T(cr * cp);

	return *this;
}

template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::clear()
{
	memset(this->pointer(), 0, 16 * sizeof(T));
	return *this;
}

template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::makeIdentity()
{
	memset(this->pointer(), 0, 16 * sizeof(T));
	(*this)[0] = T(1);
	(*this)[5] = T(1);
	(*this)[10] = T(1);
	(*this)[15] = T(1);

	return *this;
}
		
template <class T>
inline
bool
CMatrix4<T>::isIdentity() const
{
	if(!equals((*this)[0], T(1))
	   || !equals((*this)[5], T(1))
	   || !equals((*this)[10], T(1))
	   || !equals((*this)[15], T(1)))
	{
		return false;
	}
			
	for(s32 i = 0; i < 4; ++i)
	{
		for(s32 j = 0; j < 4; ++j)
		{
			if((j != i) && (!iszero((*this)(i, j))))
			{
				return false;
			}
		}
	}

	return true;
}
		
template <class T>
inline
void
CMatrix4<T>::rotateVect(vector3d<T>& vect) const
{
	vector3d<T> tmp = vect;
	vect.setX(tmp.getX() * (*this)[0] + tmp.getY() * (*this)[4] + tmp.getZ() * (*this)[8]);
	vect.setY(tmp.getX() * (*this)[1] + tmp.getY() * (*this)[5] + tmp.getZ() * (*this)[9]);
	vect.setZ(tmp.getX() * (*this)[2] + tmp.getY() * (*this)[6] + tmp.getZ() * (*this)[10]);
}
		
template <class T>
inline
void
CMatrix4<T>::rotateVect(vector3d<T>& out, const vector3d<T>& in) const
{
	out.setX(in.getX() * (*this)[0] + in.getY() * (*this)[4] + in.getZ() * (*this)[8]);
	out.setY(in.getX() * (*this)[1] + in.getY() * (*this)[5] + in.getZ() * (*this)[9]);
	out.setZ(in.getX() * (*this)[2] + in.getY() * (*this)[6] + in.getZ() * (*this)[10]);
}
		
template <class T>
inline
void
CMatrix4<T>::rotateVect(T *out, const vector3d<T>& in) const
{
	out[0] = in.getX() * (*this)[0] + in.getY() * (*this)[4] + in.getZ() * (*this)[8];
	out[1] = in.getX() * (*this)[1] + in.getY() * (*this)[5] + in.getZ() * (*this)[9];
	out[2] = in.getX() * (*this)[2] + in.getY() * (*this)[6] + in.getZ() * (*this)[10];
}
		
template <class T>
inline
void
CMatrix4<T>::inverseRotateVect(vector3d<T>& vect ) const
{
	vector3d<T> tmp = vect;
	vect.setX(tmp.getX() * (*this)[0] + tmp.getY() * (*this)[1] + tmp.getZ() * (*this)[2]);
	vect.setY(tmp.getX() * (*this)[4] + tmp.getY() * (*this)[5] + tmp.getZ() * (*this)[6]);
	vect.setZ(tmp.getX() * (*this)[8] + tmp.getY() * (*this)[9] + tmp.getZ() * (*this)[10]);
}
		
template <class T>
inline
void
CMatrix4<T>::transformVect(vector3d<T>& vect) const
{
	T vector[3];

	vector[0] = vect.getX() * (*this)[0] + vect.getY() * (*this)[4] + vect.getZ() * (*this)[8] + (*this)[12];
	vector[1] = vect.getX() * (*this)[1] + vect.getY() * (*this)[5] + vect.getZ() * (*this)[9] + (*this)[13];
	vector[2] = vect.getX() * (*this)[2] + vect.getY() * (*this)[6] + vect.getZ() * (*this)[10] + (*this)[14];
			
	vect.setX(vector[0]);
	vect.setY(vector[1]);
	vect.setZ(vector[2]);
}

template <class T>
inline
void
CMatrix4<T>::transformVectProj(vector3d<T>& vect) const
{
	T vector[4];

	vector[0] = vect.getX() * (*this)[0] + vect.getY() * (*this)[4] + vect.getZ() * (*this)[8] + (*this)[12];
	vector[1] = vect.getX() * (*this)[1] + vect.getY() * (*this)[5] + vect.getZ() * (*this)[9] + (*this)[13];
	vector[2] = vect.getX() * (*this)[2] + vect.getY() * (*this)[6] + vect.getZ() * (*this)[10] + (*this)[14];
	vector[3] = vect.getX() * (*this)[3] + vect.getY() * (*this)[7] + vect.getZ() * (*this)[11] + (*this)[15];
			
	vect.setX(vector[0] / vector[3]);
	vect.setY(vector[1] / vector[3]);
	vect.setZ(vector[2] / vector[3]);
}
		
template <class T>
inline
void
CMatrix4<T>::transformVect2D(vector3d<T>& vect) const
{
	T vector[2];
			
	vector[0] = vect.getX() * (*this)[0] + vect.getY() * (*this)[4] + (*this)[12];
	vector[1] = vect.getX() * (*this)[1] + vect.getY() * (*this)[5] + (*this)[13];
			
	vect.setX(vector[0]);
	vect.setY(vector[1]);
}
		
template <class T>
inline
void
CMatrix4<T>::transformVect2D(vector3d<T>& out, const vector3d<T>& in) const
{
	out.setX(in.getX() * (*this)[0] + in.getY() * (*this)[4] + (*this)[12]);
	out.setY(in.getX() * (*this)[1] + in.getY() * (*this)[5] + (*this)[13]);
}
		
template <class T>
inline
void
CMatrix4<T>::transformVect(vector3d<T>& out, const vector3d<T>& in) const
{
	out.setX(in.getX() * (*this)[0] + in.getY() * (*this)[4] + in.getZ() * (*this)[8] + (*this)[12]);
	out.setY(in.getX() * (*this)[1] + in.getY() * (*this)[5] + in.getZ() * (*this)[9] + (*this)[13]);
	out.setZ(in.getX() * (*this)[2] + in.getY() * (*this)[6] + in.getZ() * (*this)[10] + (*this)[14]);
}
		
template <class T>
inline
void
CMatrix4<T>::transformVect(vector2d<T>& vect) const
{
	T vector[2];
			
	vector[0] = vect.getX() * (*this)[0] + vect.getY() * (*this)[4] + (*this)[12];
	vector[1] = vect.getX() * (*this)[1] + vect.getY() * (*this)[5] + (*this)[13];
			
	vect.setX(vector[0]);
	vect.setY(vector[1]);
}
		
template <class T>
inline
void
CMatrix4<T>::transformVect(vector2d<T>& out, const vector2d<T>& in) const
{
	out.setX(in.getX() * (*this)[0] + in.getY() * (*this)[4] + (*this)[12]);
	out.setY(in.getX() * (*this)[1] + in.getY() * (*this)[5] + (*this)[13]);
}
		
template <class T>
inline
void
CMatrix4<T>::transformTexCoord(vector2d<T>& vect) const
{
	T vector[2];
			
	// following buildTextureTransform, translation part is not in the
	// fourth but third column
	vector[0] = vect.getX() * (*this)[0] + vect.getY() * (*this)[4] + (*this)[8 /*12*/];
	vector[1] = vect.getX() * (*this)[1] + vect.getY() * (*this)[5] + (*this)[9 /*13*/];
			
	vect.setX(vector[0]);
	vect.setY(vector[1]);
}
		
template <class T>
inline
void
CMatrix4<T>::transformTexCoord(vector2d<T>& out, const vector2d<T>& in) const
{
	// following buildTextureTransform, translation part is not in the
	// fourth but third column
	out.setX(in.getX() * (*this)[0] + in.getY() * (*this)[4] + (*this)[8 /*12*/]);
	out.setY(in.getX() * (*this)[1] + in.getY() * (*this)[5] + (*this)[9 /*13*/]);
}
		
template <class T>
inline
void
CMatrix4<T>::transformVect(T* out, const vector3d<T>& in) const
{
	out[0] = in.getX() * (*this)[0] + in.getY() * (*this)[4] + in.getZ() * (*this)[8] + (*this)[12];
	out[1] = in.getX() * (*this)[1] + in.getY() * (*this)[5] + in.getZ() * (*this)[9] + (*this)[13];
	out[2] = in.getX() * (*this)[2] + in.getY() * (*this)[6] + in.getZ() * (*this)[10] + (*this)[14];
	out[3] = in.getX() * (*this)[3] + in.getY() * (*this)[7] + in.getZ() * (*this)[11] + (*this)[15];
}
		
template <class T>
inline
void
CMatrix4<T>::transformPlane(plane3d<T>& plane) const
{
	vector3d<T> member(ECT_NOTHING);
	transformVect(member, plane.getMemberPoint());
			
	vector3d<T> origin(ECT_NOTHING);
	transformVect(plane.Normal);
	transformVect(origin);
			
	plane.Normal -= origin;
	plane.D = -member.dotProduct(plane.Normal);
}
		
template <class T>
inline
void
CMatrix4<T>::transformPlane_new(plane3d<T>& plane) const
{
	// rotate normal -> rotateVect ( plane.n );
	vector3d<T> n(ECT_NOTHING);
	n.setX(plane.Normal.getX() * (*this)[0] + plane.Normal.getY() * (*this)[4] + plane.Normal.getZ() * (*this)[8]);
	n.setY(plane.Normal.getX() * (*this)[1] + plane.Normal.getY() * (*this)[5] + plane.Normal.getZ() * (*this)[9]);
	n.setZ(plane.Normal.getX() * (*this)[2] + plane.Normal.getY() * (*this)[6] + plane.Normal.getZ() * (*this)[10]);
			
	// compute newly d. -> getTranslation(). dotproduct ( plane.n )
	plane.D -= (*this)[12] * n.getX() + (*this)[13] * n.getY() + (*this)[14] * n.getZ();
	plane.Normal.setX(n.getX());
	plane.Normal.setY(n.getY());
	plane.Normal.setZ(n.getZ());
}
		
template <class T>
inline
void
CMatrix4<T>::transformPlane(const plane3d<T>& in, plane3d<T>& out) const
{
	out = in;
	transformPlane(out);
}
		
template <class T>
inline
void
CMatrix4<T>::transformBox(aabbox3d<T>& box) const
{
	if(this->getDefinitelyIdentityMatrix())
	{
		return;
	}
			
	transformVect(box.MinEdge);
	transformVect(box.MaxEdge);
	box.repair();
}
		
template <class T>
inline
void
CMatrix4<T>::transformBoxEx(aabbox3d<T>& box) const
{
	const T amin[3] = {box.MinEdge.getX(), box.MinEdge.getY(), box.MinEdge.getZ()};
	const T amax[3] = {box.MaxEdge.getX(), box.MaxEdge.getY(), box.MaxEdge.getZ()};
			
	T bmin[3];
	T bmax[3];
			
	bmin[0] = bmax[0] = (*this)[12];
	bmin[1] = bmax[1] = (*this)[13];
	bmin[2] = bmax[2] = (*this)[14];
			
	const CMatrix4<T>& m = *this;
			
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
		
template <class T>
inline
void
CMatrix4<T>::multiplyWith1x4Matrix(T* matrix) const
{
	/*
	  0  1  2  3
	  4  5  6  7
	  8  9  10 11
	  12 13 14 15
	*/
			
	T mat[4];
	mat[0] = matrix[0];
	mat[1] = matrix[1];
	mat[2] = matrix[2];
	mat[3] = matrix[3];
			
	matrix[0] = (*this)[0] * mat[0] + (*this)[4] * mat[1] + (*this)[8] * mat[2] + (*this)[12] * mat[3];
	matrix[1] = (*this)[1] * mat[0] + (*this)[5] * mat[1] + (*this)[9] * mat[2] + (*this)[13] * mat[3];
	matrix[2] = (*this)[2] * mat[0] + (*this)[6] * mat[1] + (*this)[10] * mat[2] + (*this)[14] * mat[3];
	matrix[3] = (*this)[3] * mat[0] + (*this)[7] * mat[1] + (*this)[11] * mat[2] + (*this)[15] * mat[3];
}
		
template <class T>
inline
void
CMatrix4<T>::inverseTranslateVect(vector3d<T>& vect) const
{
	vect.setX(vect.getX() - (*this)[12]);
	vect.setY(vect.getY() - (*this)[13]);
	vect.setZ(vect.getZ() - (*this)[14]);
}
		
template <class T>
inline
void
CMatrix4<T>::translateVect(vector3d<T>& vect) const
{
	vect.setX(vect.getX() + (*this)[12]);
	vect.setY(vect.getY() + (*this)[13]);
	vect.setZ(vect.getZ() + (*this)[14]);
}
		
template <class T>
inline
T
CMatrix4<T>::getDeterminant() const
{
	if(this->getDefinitelyIdentityMatrix())
	{
		return T(1);
	}
			
	T t0 = (*this)[10] * (*this)[15] - (*this)[11] * (*this)[14];
	T t1 = (*this)[6] * (*this)[15] - (*this)[7] * (*this)[14];
	T t2 = (*this)[6] * (*this)[11] - (*this)[7] * (*this)[10];
	T t3 = (*this)[2] * (*this)[15] - (*this)[3] * (*this)[14];
	T t4 = (*this)[2] * (*this)[11] - (*this)[3] * (*this)[10];
	T t5 = (*this)[2] * (*this)[7] - (*this)[3] * (*this)[6];
			
	T t6 = (*this)[8] * (*this)[13] - (*this)[9] * (*this)[12];
	T t7 = (*this)[4] * (*this)[13] - (*this)[5] * (*this)[12];
	T t8 = (*this)[4] * (*this)[9] - (*this)[5] * (*this)[8];
	T t9 = (*this)[0] * (*this)[13] - (*this)[1] * (*this)[12];
	T t10 = (*this)[0] * (*this)[9] - (*this)[1] * (*this)[8];
	T t11 = (*this)[0] * (*this)[5] - (*this)[1] * (*this)[4];
			
	return t0 * t11 - t1 * t10 + t2 * t9 + t3 * t8 - t4 * t7 + t5 * t6;
}
		
template <class T>
inline
bool
CMatrix4<T>::getInverse(CMatrix4<T>& out) const
{
	JENNY_ASSERT(this != &out);
			
	// Cramer's rule.
	double t0 = double((*this)[10] * (*this)[15] - (*this)[11] * (*this)[14]);
	double t1 = double((*this)[6] * (*this)[15] - (*this)[7] * (*this)[14]);
	double t2 = double((*this)[6] * (*this)[11] - (*this)[7] * (*this)[10]);
	double t3 = double((*this)[2] * (*this)[15] - (*this)[3] * (*this)[14]);
	double t4 = double((*this)[2] * (*this)[11] - (*this)[3] * (*this)[10]);
	double t5 = double((*this)[2] * (*this)[7] - (*this)[3] * (*this)[6]);
			
	double t6 = double((*this)[8] * (*this)[13] - (*this)[9] * (*this)[12]);
	double t7 = double((*this)[4] * (*this)[13] - (*this)[5] * (*this)[12]);
	double t8 = double((*this)[4] * (*this)[9] - (*this)[5] * (*this)[8]);
	double t9 = double((*this)[0] * (*this)[13] - (*this)[1] * (*this)[12]);
	double t10 = double((*this)[0] * (*this)[9] - (*this)[1] * (*this)[8]);
	double t11 = double((*this)[0] * (*this)[5] - (*this)[1] * (*this)[4]);
			
	double det = t0 * t11 - t1 * t10 + t2 * t9 + t3 * t8 - t4 * t7 + t5 * t6;
	if(iszero(det))
	{
		return false;
	}

	T ft0 = T(t0);
	T ft1 = T(t1);
	T ft2 = T(t2);
	T ft3 = T(t3);
	T ft4 = T(t4);
	T ft5 = T(t5);
	T ft6 = T(t6);
	T ft7 = T(t7);
	T ft8 = T(t8);
	T ft9 = T(t9);
	T ft10 = T(t10);
	T ft11 = T(t11);
			
	out[0] = (*this)[5] * ft0 - (*this)[9] * ft1 + (*this)[13] * ft2;
	out[1] = (*this)[9] * ft3 - (*this)[1] * ft0 - (*this)[13] * ft4;
	out[2] = (*this)[1] * ft1 - (*this)[5] * ft3 + (*this)[13] * ft5;
	out[3] = (*this)[5] * ft4 - (*this)[1] * ft2 - (*this)[9] * ft5;
			
	out[4] = (*this)[8] * ft1 - (*this)[4] * ft0 - (*this)[12] * ft2;
	out[5] = (*this)[0] * ft0 - (*this)[8] * ft3 + (*this)[12] * ft4;
	out[6] = (*this)[4] * ft3 - (*this)[0] * ft1 - (*this)[12] * ft5;
	out[7] = (*this)[0] * ft2 - (*this)[4] * ft4 + (*this)[8] * ft5;
			
	out[8] = (*this)[7] * ft6 - (*this)[11] * ft7 + (*this)[15] * ft8;
	out[9] = (*this)[11] * ft9 - (*this)[3] * ft6 - (*this)[15] * ft10;
	out[10] = (*this)[3] * ft7 - (*this)[7] * ft9 + (*this)[15] * ft11;
	out[11] = (*this)[7] * ft10 - (*this)[3] * ft8 - (*this)[11] * ft11;
			
	out[12] = (*this)[10] * ft7 - (*this)[6] * ft6 - (*this)[14] * ft8;
	out[13] = (*this)[2] * ft6 - (*this)[10] * ft9 + (*this)[14] * ft10;
	out[14] = (*this)[6] * ft9 - (*this)[2] * ft7 - (*this)[14] * ft11;
	out[15] = (*this)[2] * ft8 - (*this)[6] * ft10 + (*this)[10] * ft11;
			

	det = 1.0 / det;
	for(int i = 0; i < 16; ++i)
	{
		out[i] = T(out[i] * det);
	}

	return true;
}
		
template <class T>
inline
bool
CMatrix4<T>::getInversePrimitive(CMatrix4<T>& out) const
{
	out[0] = (*this)[0];
	out[1] = (*this)[4];
	out[2] = (*this)[8];
	out[3] = 0;
			
	out[4] = (*this)[1];
	out[5] = (*this)[5];
	out[6] = (*this)[9];
	out[7] = 0;
			
	out[8] = (*this)[2];
	out[9] = (*this)[6];
	out[10] = (*this)[10];
	out[11] = 0;
			
	out[12] = -((*this)[12] * (*this)[0] + (*this)[13] * (*this)[1] + (*this)[14] * (*this)[2]);
	out[13] = -((*this)[12] * (*this)[4] + (*this)[13] * (*this)[5] + (*this)[14] * (*this)[6]);
	out[14] = -((*this)[12] * (*this)[8] + (*this)[13] * (*this)[9] + (*this)[14] * (*this)[10]);
	out[15] = T(1);
	out.setDefinitelyIdentityMatrix(this->getDefinitelyIdentityMatrix());
	return true;
}
		
template <class T>
inline bool CMatrix4<T>::makeInverse()
{
	if(this->getDefinitelyIdentityMatrix())
		return true;
			
	CMatrix4<T> temp (EM4CONST_NOTHING);
			
	if(getInverse(temp))
	{
		*this = temp;
		return true;
	}
			
	return false;
}
		
template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::operator = (const T& scalar)
{
	for(s32 i = 0; i < 16; ++i)
	{
		(*this)[i] = scalar;
	}
	this->setDefinitelyIdentityMatrix(false);
	return *this;
}
		
template <class T>
inline
bool
CMatrix4<T>::operator == (const CMatrix4<T>& other) const
{
	if(this->getDefinitelyIdentityMatrix() && other.getDefinitelyIdentityMatrix())
	{
		return true;
	}
	for(s32 i = 0; i < 16; ++i)
	{
		if((*this)[i] != other[i])
		{
			return false;
		}
	}
			
	return true;
}
		
template <class T>
inline
bool
CMatrix4<T>::operator != (const CMatrix4<T>& other) const
{
	return !(*this == other);
}
		
template <class T>
inline
CMatrix4<T>
CMatrix4<T>::interpolate(const CMatrix4<T>& b, T time) const
{
	CMatrix4<T> mat(EM4CONST_NOTHING);
			
	for(u32 i = 0; i < 16; i += 4)
	{
		mat[i + 0] = (T)((*this)[i + 0] + (b[i + 0] - (*this)[i + 0]) * time);
		mat[i + 1] = (T)((*this)[i + 1] + (b[i + 1] - (*this)[i + 1]) * time);
		mat[i + 2] = (T)((*this)[i + 2] + (b[i + 2] - (*this)[i + 2]) * time);
		mat[i + 3] = (T)((*this)[i + 3] + (b[i + 3] - (*this)[i + 3]) * time);
	}
	return mat;
}
		
template <class T>
inline
CMatrix4<T>
CMatrix4<T>::getTransposed() const
{
	CMatrix4<T> t(EM4CONST_NOTHING);
	getTransposed(t);
	return t;
}
		
template <class T>
inline
void
CMatrix4<T>::getTransposed(CMatrix4<T>& o) const
{
	JENNY_ASSERT(this != &o);
	
	o[ 0] = (*this)[ 0];
	o[ 1] = (*this)[ 4];
	o[ 2] = (*this)[ 8];
	o[ 3] = (*this)[12];
			
	o[ 4] = (*this)[ 1];
	o[ 5] = (*this)[ 5];
	o[ 6] = (*this)[ 9];
	o[ 7] = (*this)[13];
			
	o[ 8] = (*this)[ 2];
	o[ 9] = (*this)[ 6];
	o[10] = (*this)[10];
	o[11] = (*this)[14];
			
	o[12] = (*this)[ 3];
	o[13] = (*this)[ 7];
	o[14] = (*this)[11];
	o[15] = (*this)[15];
}
		
template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::setTextureRotationCenter(T rotateRad)
{
	const float c = glf::Cos(float(rotateRad));
	const float s = glf::Sin(float(rotateRad));
	(*this)[0] = T(c);
	(*this)[1] = T(s);
			
	(*this)[4] = T(-s);
	(*this)[5] = T(c);
			
	(*this)[8] = T(0.5f * (s - c) + 0.5f);
	(*this)[9] = T(-0.5f * (s + c) + 0.5f);
	this->setDefinitelyIdentityMatrix(this->getDefinitelyIdentityMatrix()
									  && (rotateRad == T(0)));
	return *this;
}
		
template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::setTextureTranslate(T x, T y)
{
	(*this)[8] = x;
	(*this)[9] = y;
	this->setDefinitelyIdentityMatrix(this->getDefinitelyIdentityMatrix() && x == T(0) && y == T(0));
	return *this;
}
		
template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::setTextureTranslateTransposed(T x, T y)
{
	(*this)[2] = x;
	(*this)[6] = y;
	this->setDefinitelyIdentityMatrix(this->getDefinitelyIdentityMatrix() && x == T(0) && y == T(0));
	return *this;
}
		
template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::setTextureScale(T sx, T sy)
{
	(*this)[0] = sx;
	(*this)[5] = sy;
	this->setDefinitelyIdentityMatrix(this->getDefinitelyIdentityMatrix() && sx == T(0) && sy == T(0));
	return *this;
}
		
template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::postTextureScale(T sx, T sy)
{
	(*this)[0] *= sx;
	(*this)[1] *= sx;
	(*this)[4] *= sy;
	(*this)[5] *= sy;
	this->setDefinitelyIdentityMatrix(this->getDefinitelyIdentityMatrix() && sx == T(1) && sy == T(1));
	return *this;
}

template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::setTextureScaleCenter(T sx, T sy)
{
	(*this)[0] = sx;
	(*this)[5] = sy;
	(*this)[8] = T(0.5f - 0.5f * sx);
	(*this)[9] = T(0.5f - 0.5f * sy);
	this->setDefinitelyIdentityMatrix(this->getDefinitelyIdentityMatrix() && sx == T(1) && sy == T(1));
	return *this;
}

template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::setM(const T* data)
{
	memcpy(this->pointer(), data, 16*sizeof(T));

	this->setDefinitelyIdentityMatrix(false);
	return *this;
}

template <class T>
inline
T*
CMatrix4<T>::getM(T* data) const
{
	memcpy(data, this->pointer(), 16 * sizeof(T));
	return data;
}

template <class T>
inline
CMatrix4<T>
operator * (const T scalar, const CMatrix4<T>& mat)
{
	return mat * scalar;
}

////////////////////////////
template <class T>
inline
CMatrix4<T>
CMatrix4<T>::add(const CMatrix4<T>& other) const
{
	CMatrix4<T> temp;

	temp[0] = M[0] + other[0];
	temp[1] = M[1] + other[1];
	temp[2] = M[2] + other[2];
	temp[3] = M[3] + other[3];
	temp[4] = M[4] + other[4];
	temp[5] = M[5] + other[5];
	temp[6] = M[6] + other[6];
	temp[7] = M[7] + other[7];
	temp[8] = M[8] + other[8];
	temp[9] = M[9] + other[9];
	temp[10] = M[10] + other[10];
	temp[11] = M[11] + other[11];
	temp[12] = M[12] + other[12];
	temp[13] = M[13] + other[13];
	temp[14] = M[14] + other[14];
	temp[15] = M[15] + other[15];

	return temp;
}

template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::addEq(const CMatrix4<T>& other)
{
	M[0] += other[0];
	M[1] += other[1];
	M[2] += other[2];
	M[3] += other[3];
	M[4] += other[4];
	M[5] += other[5];
	M[6] += other[6];
	M[7] += other[7];
	M[8] += other[8];
	M[9] += other[9];
	M[10] += other[10];
	M[11] += other[11];
	M[12] += other[12];
	M[13] += other[13];
	M[14] += other[14];
	M[15] += other[15];

	return *this;
}

template <class T>
inline
CMatrix4<T>
CMatrix4<T>::sub(const CMatrix4<T>& other) const
{
	CMatrix4<T> temp;

	temp[0] = M[0] - other[0];
	temp[1] = M[1] - other[1];
	temp[2] = M[2] - other[2];
	temp[3] = M[3] - other[3];
	temp[4] = M[4] - other[4];
	temp[5] = M[5] - other[5];
	temp[6] = M[6] - other[6];
	temp[7] = M[7] - other[7];
	temp[8] = M[8] - other[8];
	temp[9] = M[9] - other[9];
	temp[10] = M[10] - other[10];
	temp[11] = M[11] - other[11];
	temp[12] = M[12] - other[12];
	temp[13] = M[13] - other[13];
	temp[14] = M[14] - other[14];
	temp[15] = M[15] - other[15];

	return temp;
}

template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::subEq(const CMatrix4<T>& other)
{
	M[0] -= other[0];
	M[1] -= other[1];
	M[2] -= other[2];
	M[3] -= other[3];
	M[4] -= other[4];
	M[5] -= other[5];
	M[6] -= other[6];
	M[7] -= other[7];
	M[8] -= other[8];
	M[9] -= other[9];
	M[10] -= other[10];
	M[11] -= other[11];
	M[12] -= other[12];
	M[13] -= other[13];
	M[14] -= other[14];
	M[15] -= other[15];

	return *this;
}

template <class T>
inline
CMatrix4<T>
CMatrix4<T>::multScal(const T& scalar) const
{
	CMatrix4<T> temp;

	temp[0] = M[0] * scalar;
	temp[1] = M[1] * scalar;
	temp[2] = M[2] * scalar;
	temp[3] = M[3] * scalar;
	temp[4] = M[4] * scalar;
	temp[5] = M[5] * scalar;
	temp[6] = M[6] * scalar;
	temp[7] = M[7] * scalar;
	temp[8] = M[8] * scalar;
	temp[9] = M[9] * scalar;
	temp[10] = M[10] * scalar;
	temp[11] = M[11] * scalar;
	temp[12] = M[12] * scalar;
	temp[13] = M[13] * scalar;
	temp[14] = M[14] * scalar;
	temp[15] = M[15] * scalar;

	return temp;
}

template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::multEqScal(const T& scalar)
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
	M[9] *= scalar;
	M[10] *= scalar;
	M[11] *= scalar;
	M[12] *= scalar;
	M[13] *= scalar;
	M[14] *= scalar;
	M[15] *= scalar;

	return *this;
}

template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::multEq(const CMatrix4<T>& other)
{
	CMatrix4<T> temp(*this);
	return setbyproduct_nocheck(temp, other);
}

template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::setbyproduct_nocheck(const CMatrix4<T>& other_a, const CMatrix4<T>& other_b)
{
	const T* m1 = other_a.M;
	const T* m2 = other_b.M;

	M[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
	M[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
	M[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
	M[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];

	M[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
	M[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
	M[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
	M[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];

	M[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
	M[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
	M[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
	M[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];

	M[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
	M[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
	M[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
	M[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];

	return *this;
}

template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::setbyproduct(const CMatrix4<T>& other_a, const CMatrix4<T>& other_b)
{
	return setbyproduct_nocheck(other_a, other_b);
}

template <class T>
inline
CMatrix4<T>
CMatrix4<T>::mult(const CMatrix4<T>& m2) const
{
	CMatrix4<T> m3;

	const T* m1 = M;

	m3[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
	m3[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
	m3[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
	m3[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];

	m3[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
	m3[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
	m3[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
	m3[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];

	m3[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
	m3[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
	m3[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
	m3[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];

	m3[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
	m3[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
	m3[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
	m3[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];

	return m3;
}

template <class T>
inline
CMatrix4<T>
CMatrix4<T>::mult34(const CMatrix4<T>& m2) const
{
	CMatrix4<T> out;
	mult34(m2, out);
	return out;
}

template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::mult34(const CMatrix4<T>& m2, CMatrix4<T>& out) const
{
	const T* m1 = M;

	out.M[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2];
	out.M[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2];
	out.M[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2];
	out.M[3] = T(0);

	out.M[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6];
	out.M[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6];
	out.M[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6];
	out.M[7] = T(0);

	out.M[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10];
	out.M[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10];
	out.M[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10];
	out.M[11] = T(0);

	out.M[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12];
	out.M[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13];
	out.M[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14];
	out.M[15] = T(1);

	return out;
}

template <class T>
inline
CMatrix4<T>
CMatrix4<T>::mult33(const CMatrix4<T>& m2) const
{
	CMatrix4<T> out;
	mult33(m2, out);
	return out;
}

template <class T>
inline
CMatrix4<T>&
CMatrix4<T>::mult33(const CMatrix4<T>& m2, CMatrix4<T>& out) const
{

	const T* m1 = M;

	out.M[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2];
	out.M[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2];
	out.M[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2];
	out.M[3] = T(0);

	out.M[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6];
	out.M[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6];
	out.M[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6];
	out.M[7] = T(0);

	out.M[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10];
	out.M[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10];
	out.M[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10];
	out.M[11] = T(0);

	out.M[12] = T(0);
	out.M[13] = T(0);
	out.M[14] = T(0);
	out.M[15] = T(1);

	return out;
}

template <class T>
inline
void
CMatrix4<T>::transVect(vector4d<T>& vect) const
{
	T vector[4];

	vector[0] = vect.getX() * (*this)[0] + vect.getY() * (*this)[4] + vect.getZ() * (*this)[8] + vect.getW() * (*this)[12];
	vector[1] = vect.getX() * (*this)[1] + vect.getY() * (*this)[5] + vect.getZ() * (*this)[9] + vect.getW() * (*this)[13];
	vector[2] = vect.getX() * (*this)[2] + vect.getY() * (*this)[6] + vect.getZ() * (*this)[10] + vect.getW() * (*this)[14];
	vector[3] = vect.getX() * (*this)[3] + vect.getY() * (*this)[7] + vect.getZ() * (*this)[11] + vect.getW() * (*this)[15];

	vect.setX(vector[0]);
	vect.setY(vector[1]);
	vect.setZ(vector[2]);
	vect.setW(vector[3]);
}

template <class T>
inline
void
CMatrix4<T>::transVect(vector4d<T>& out, const vector4d<T>& in) const
{
	out.setX(in.getX() * (*this)[0] + in.getY() * (*this)[4] + in.getZ() * (*this)[8] + in.getW() * (*this)[12]);
	out.setY(in.getX() * (*this)[1] + in.getY() * (*this)[5] + in.getZ() * (*this)[9] + in.getW() * (*this)[13]);
	out.setZ(in.getX() * (*this)[2] + in.getY() * (*this)[6] + in.getZ() * (*this)[10] + in.getW() * (*this)[14]);
	out.setW(in.getX() * (*this)[3] + in.getY() * (*this)[7] + in.getZ() * (*this)[11] + in.getW() * (*this)[15]);
}

template <class T>
inline
void
CMatrix4<T>::transVect(vector3d<T>& vect) const
{
	T vector[3];

	vector[0] = vect.getX() * (*this)[0] + vect.getY() * (*this)[4] + vect.getZ() * (*this)[8] + (*this)[12];
	vector[1] = vect.getX() * (*this)[1] + vect.getY() * (*this)[5] + vect.getZ() * (*this)[9] + (*this)[13];
	vector[2] = vect.getX() * (*this)[2] + vect.getY() * (*this)[6] + vect.getZ() * (*this)[10] + (*this)[14];

	vect.setX(vector[0]);
	vect.setY(vector[1]);
	vect.setZ(vector[2]);
}

template <class T>
inline
void
CMatrix4<T>::transVect(vector3d<T>& out, const vector3d<T>& in) const
{
	out.setX(in.getX() * (*this)[0] + in.getY() * (*this)[4] + in.getZ() * (*this)[8] + (*this)[12]);
	out.setY(in.getX() * (*this)[1] + in.getY() * (*this)[5] + in.getZ() * (*this)[9] + (*this)[13]);
	out.setZ(in.getX() * (*this)[2] + in.getY() * (*this)[6] + in.getZ() * (*this)[10] + (*this)[14]);
}

template <class T>
inline
void
CMatrix4<T>::rotVect(vector3d<T>& vect) const
{
	vector3d<T> tmp = vect;
	vect.setX(tmp.getX() * (*this)[0] + tmp.getY() * (*this)[4] + tmp.getZ() * (*this)[8]);
	vect.setY(tmp.getX() * (*this)[1] + tmp.getY() * (*this)[5] + tmp.getZ() * (*this)[9]);
	vect.setZ(tmp.getX() * (*this)[2] + tmp.getY() * (*this)[6] + tmp.getZ() * (*this)[10]);
}

template <class T>
inline
void
CMatrix4<T>::rotVect(vector3d<T>& out, const vector3d<T>& in) const
{
	out.setX(in.getX() * (*this)[0] + in.getY() * (*this)[4] + in.getZ() * (*this)[8]);
	out.setY(in.getX() * (*this)[1] + in.getY() * (*this)[5] + in.getZ() * (*this)[9]);
	out.setZ(in.getX() * (*this)[2] + in.getY() * (*this)[6] + in.getZ() * (*this)[10]);
}


typedef CMatrix4<f32> matrix4;
extern const matrix4 IdentityMatrix;

} // end namespace jenny

#include <math/matrix4_ops.h>
