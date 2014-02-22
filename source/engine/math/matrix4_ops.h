#pragma once

#include <math/matrix4.h>

namespace jenny
{

//MT must have operator[]
template<class T, typename MT>
inline
CMatrix4<T>
operator * (const CMatrix4<T>& mat, const MT& other)
{
	CMatrix4<T> mat2(CMatrix4<T>::EM4CONST_NOTHING);
	
	if(mat.getDefinitelyIdentityMatrix())
	{
		for(u32 i = 0; i < 16; ++i)
		{
			mat2[i] = other[i];
		}
	}
	else
	{
		rowMatrixProduct34(mat2, mat, other.M);
	}
	
	return mat2;
}

//MT must have operator[]
template<typename T, typename MT>
inline
CMatrix4<T>
operator * (const MT& other, const CMatrix4<T>& mat)
{
	CMatrix4<T> mat2(CMatrix4<T>::EM4CONST_NOTHING);

	if(mat.getDefinitelyIdentityMatrix())
	{
		for(u32 i = 0; i < 16; ++i)
		{
			mat2[i] = other[i];
		}
	}
	else
	{
		CMatrix4<T>::rowMatrixProduct34(mat2, other.M, mat);
	}
	
	return mat2;
}

//! Builds a right-handed perspective projection matrix based on a field of view
template<typename T>
inline
CMatrix4<T>&
buildProjectionMatrixPerspectiveFov(CMatrix4<T>& out, T fieldOfViewRadians, T aspectRatio, T zNear, T zFar)
{
	f64 h = 1.0 / glf::Tan(f64(fieldOfViewRadians) / 2.0);
	T w = T(h / f64(aspectRatio));
	T d = 1.0f / (zFar - zNear);

	out(0, 0) = w;
	out(0, 1) = T(0);
	out(0, 2) = T(0);
	out(0, 3) = T(0);

	out(1, 0) = T(0);
	out(1, 1) = T(h);
	out(1, 2) = T(0);
	out(1, 3) = T(0); 

	out(2, 0) = T(0);
	out(2, 1) = T(0);
	out(2, 2) = -(zFar + zNear) * d; // equiv. to -(zFar + zNear) / (zFar - zNear)
	out(2, 3) = T(-1);

	out(3, 0) = T(0);
	out(3, 1) = T(0);
	out(3, 2) = T(-2) * zNear * zFar * d; // equiv. to -2 * zFar * zNear / (zFar - zNear);
	out(3, 3) = T(0);
	return out;
}

//! Builds a right-handed perspective projection matrix based on a field of view
template<typename T>
inline
CMatrix4<T>
buildProjectionMatrixPerspectiveFov(T fieldOfViewRadians, T aspectRatio, T zNear, T zFar)
{
	CMatrix4<T> m(CMatrix4<T>::EM4CONST_NOTHING);
	buildProjectionMatrixPerspectiveFov(m, fieldOfViewRadians, aspectRatio, zNear, zFar);
	return m;
}

//! Builds a perspective projection matrix.
template<typename T>
inline
CMatrix4<T>&
buildProjectionMatrixPerspectiveFovInfinity(CMatrix4<T>& out, T fieldOfViewRadians, T aspectRatio, T zNear)
{
	f64 h = 1.0 / glf::Tan(f64(fieldOfViewRadians) / 2.0);
	T w = T(h / f64(aspectRatio));

	out(0, 0) = w;
	out(0, 1) = T(0);
	out(0, 2) = T(0);
	out(0, 3) = T(0);

	out(1, 0) = T(0);
	out(1, 1) = T(h);
	out(1, 2) = T(0);
	out(1, 3) = T(0);

	out(2, 0) = T(0);
	out(2, 1) = T(0);
	out(2, 2) = T(-1);
	out(2, 3) = T(-1);

	out(3, 0) = T(0);
	out(3, 1) = T(0);
	out(3, 2) = T(-2) * zNear;
	out(3, 3) = T(0);

	return out;
}
//! Builds a perspective projection matrix.
template<typename T>
inline
CMatrix4<T>
buildProjectionMatrixPerspectiveFovInfinity(T fieldOfViewRadians, T aspectRatio, T zNear)
{
	CMatrix4<T> m(CMatrix4<T>::EM4CONST_NOTHING);
	buildProjectionMatrixPerspectiveFovInfinity(m, fieldOfViewRadians, aspectRatio, zNear);
	return m;
}

//! Builds a perspective projection matrix.
template<typename T>
inline
CMatrix4<T>&
buildProjectionMatrixPerspective(CMatrix4<T>& out, T widthOfViewVolume, T heightOfViewVolume, T zNear, T zFar)
{
	T zNearTimes2 = T(2) * zNear;
	T d = zNear - zFar;

	out(0, 0) = zNearTimes2 / widthOfViewVolume;
	out(0, 1) = T(0);
	out(0, 2) = T(0);
	out(0, 3) = T(0);

	out(1, 0) = T(0);
	out(1, 1) = zNearTimes2 / heightOfViewVolume;
	out(1, 2) = T(0);
	out(1, 3) = T(0); 

	out(2, 0) = T(0);
	out(2, 1) = T(0);
	out(2, 2) = (zFar + zNear) / d; // equiv. to - (zFar + zNear) / (zFar - zNear);
	out(2, 3) = T(-1);

	out(3, 0) = T(0);
	out(3, 1) = T(0);
	out(3, 2) = zNearTimes2 * zFar / d; // equiv. to -2 * zFar * zNear / (zFar - zNear);
	out(3, 3) = T(0);

	return out;
}

//! Builds a generic perspective projection matrix.
template<typename T>
inline
CMatrix4<T>&
buildProjectionMatrixPerspective(CMatrix4<T>& out, T left, T right, T bottom, T top, T zNear, T zFar)
{
	T zNearTimes2 = T(2) * zNear;
	T d = zNear - zFar;
	T widthOfViewVolume = right - left;
	T heightOfViewVolume = top - bottom;

	out(0, 0) = zNearTimes2 / widthOfViewVolume;
	out(0, 1) = T(0);
	out(0, 2) = (right + left) / widthOfViewVolume;
	out(0, 3) = T(0);

	out(1, 0) = T(0);
	out(1, 1) = zNearTimes2 / heightOfViewVolume;
	out(1, 2) = (top + bottom) / heightOfViewVolume;
	out(1, 3) = T(0); 

	out(2, 0) = T(0);
	out(2, 1) = T(0);
	out(2, 2) = (zFar + zNear) / d; // equiv. to - (zFar + zNear) / (zFar - zNear);
	out(2, 3) = T(-1);

	out(3, 0) = T(0);
	out(3, 1) = T(0);
	out(3, 2) = zNearTimes2 * zFar / d; // equiv. to -2 * zFar * zNear / (zFar - zNear);
	out(3, 3) = T(0);

	return out;
}

//! Builds a perspective projection matrix.
template<typename T>
inline
CMatrix4<T>
buildProjectionMatrixPerspective(T widthOfViewVolume, T heightOfViewVolume, T zNear, T zFar)
{
	CMatrix4<T> m(CMatrix4<T>::EM4CONST_NOTHING);
	buildProjectionMatrixPerspective(m, widthOfViewVolume, heightOfViewVolume, zNear, zFar);
	return m;
}

//! Builds a perspective projection matrix.
template<typename T>
inline
CMatrix4<T>&
buildProjectionMatrixPerspectiveInfinity(CMatrix4<T>& out, T widthOfViewVolume, T heightOfViewVolume, T zNear)
{
	T zNearTimes2 = T(2) * zNear;

	out(0, 0) = zNearTimes2 / widthOfViewVolume;
	out(0, 1) = T(0);
	out(0, 2) = T(0);
	out(0, 3) = T(0);

	out(1, 0) = T(0);
	out(1, 1) = zNearTimes2 / heightOfViewVolume;
	out(1, 2) = T(0);
	out(1, 3) = T(0); 

	out(2, 0) = T(0);
	out(2, 1) = T(0);
	out(2, 2) = T(-1);
	out(2, 3) = T(-1);

	out(3, 0) = T(0);
	out(3, 1) = T(0);
	out(3, 2) = -zNearTimes2;
	out(3, 3) = T(0);

	return out;
}

//! Builds a perspective projection matrix.
template<typename T>
inline
CMatrix4<T>
buildProjectionMatrixPerspectiveInfinity(T widthOfViewVolume, T heightOfViewVolume, T zNear)
{
	CMatrix4<T> m(CMatrix4<T>::EM4CONST_NOTHING);
	buildProjectionMatrixPerspectiveInfinity(m, widthOfViewVolume, heightOfViewVolume, zNear);
	return m;
}

//! Builds a centered right-handed orthogonal projection matrix.
template<typename T>
inline
CMatrix4<T>&
buildProjectionMatrixOrtho(CMatrix4<T>& out, T widthOfViewVolume, T heightOfViewVolume, T zNear, T zFar)
{
	T d = zNear - zFar;

	out(0, 0) = T(2) / widthOfViewVolume;
	out(0, 1) = T(0);
	out(0, 2) = T(0);
	out(0, 3) = T(0);

	out(1, 0) = T(0);
	out(1, 1) = T(2) / heightOfViewVolume;
	out(1, 2) = T(0);
	out(1, 3) = T(0);

	out(2, 0) = T(0);
	out(2, 1) = T(0);
	out(2, 2) = T(2) / d; // equiv. to -1 / (zFar - zNear)
	out(2, 3) = 0;

	out(3, 0) = 0;
	out(3, 1) = 0;
	out(3, 2) = (zFar + zNear) / d; // equiv. to - (zFar + zNear) / (zFar - zNear)
	out(3, 3) = 1;

	return out;
}

//! Builds a centered right-handed orthogonal projection matrix.
template<typename T>
inline
CMatrix4<T>
buildProjectionMatrixOrtho(T widthOfViewVolume, T heightOfViewVolume, T zNear, T zFar)
{
	CMatrix4<T> m(CMatrix4<T>::EM4CONST_NOTHING);
	buildProjectionMatrixOrtho(m, widthOfViewVolume, heightOfViewVolume, zNear, zFar);
	return m;
}

//! Builds a right-handed orthogonal projection matrix.
template<typename T>
inline
CMatrix4<T>&
buildProjectionMatrixOrtho(CMatrix4<T>& out, T left, T right, T bottom, T top, T zNear, T zFar)
{
	T w = right - left;
	T h = top - bottom;
	T d = zNear - zFar;

	out(0,0) = T(2) / w;
	out(0,1) = T(0);
	out(0,2) = T(0);
	out(0,3) = T(0);

	out(1,0) = T(0);
	out(1,1) = T(2) / h;
	out(1,2) = T(0);
	out(1,3) = T(0);

	out(2, 0) = T(0);
	out(2, 1) = T(0);
	out(2, 2) = T(2) / d; // equiv. to -2 / (zFar - zNear)
	out(2, 3) = 0;

	out(3, 0) = -(right + left) / w;
	out(3, 1) = -(top + bottom) / h;
	out(3, 2) = (zFar + zNear) / d; // equiv. to - (zFar + zNear) / (zFar - zNear)
	out(3, 3) = 1;

	return out;
}

//! Builds a right-handed orthogonal projection matrix.
template<typename T>
inline
CMatrix4<T>
buildProjectionMatrixOrtho(T left, T right, T bottom, T top, T zNear, T zFar)
{
	CMatrix4<T> m(CMatrix4<T>::EM4CONST_NOTHING);
	buildProjectionMatrixOrtho(m, left, right, bottom, top, zNear, zFar);
	return m;
}

//! Builds a right-handed look-at matrix.
template <typename T>
inline
CMatrix4<T>&
buildCameraLookAtMatrix(CMatrix4<T>& out,
						const vector3d<T>& position,
						const vector3d<T>& target,
						const vector3d<T>& upVector)
{
	vector3d<T> zaxis = position - target;
	zaxis.normalize();

	vector3d<T> xaxis = upVector.crossProduct(zaxis);
	xaxis.normalize();

	vector3d<T> yaxis = zaxis.crossProduct(xaxis);

	out.setRow(0, xaxis);
	out.setRow(1, yaxis);
	out.setRow(2, zaxis);

	out(3, 0) = -xaxis.dotProduct(position);
	out(3, 1) = -yaxis.dotProduct(position);
	out(3, 2) = -zaxis.dotProduct(position);

	out(0, 3) = T(0);
	out(1, 3) = T(0);
	out(2, 3) = T(0);
	out(3, 3) = T(1);

	return out;
}

//! Builds a right-handed look-at matrix.
template <typename T>
inline
CMatrix4<T>
buildCameraLookAtMatrix(const vector3d<T>& position,
						const vector3d<T>& target,
						const vector3d<T>& upVector)
{
	CMatrix4<T> m(CMatrix4<T>::EM4CONST_NOTHING);
	buildCameraLookAtMatrix(m, position, target, upVector);
	return m;
}

//! Builds a matrix that flattens geometry into a plane.
/** \param out: matrix to fill
	\param light: light source
	\param plane: plane into which the geometry if flattened into
	\param point: value between 0 and 1, describing the light source.
	If this is 1, it is a point light, if it is 0, it is a directional light. */
template<typename T>
inline
CMatrix4<T>&
buildShadowMatrix(CMatrix4<T>& out, const vector3d<T>& light, plane3d<T> plane, T point = T(1))
{
	plane.Normal.normalize();
	T d = plane.Normal.dotProduct(light);

	out(0, 0) = -plane.Normal.getX() * light.getX() + d;
	out(0, 1) = -plane.Normal.getX() * light.getY();
	out(0, 2) = -plane.Normal.getX() * light.getZ();
	out(0, 3) = -plane.Normal.getX() * point;

	out(1, 0) = -plane.Normal.getY() * light.getX();
	out(1, 1) = -plane.Normal.getY() * light.getY() + d;
	out(1, 2) = -plane.Normal.getY() * light.getZ();
	out(1, 3) = -plane.Normal.getY() * point;

	out(2, 0) = -plane.Normal.getZ() * light.getX();
	out(2, 1) = -plane.Normal.getZ() * light.getY();
	out(2, 2) = -plane.Normal.getZ() * light.getZ() + d;
	out(2, 3) = -plane.Normal.getZ() * point;

	out(3, 0) = -plane.D * light.getX();
	out(3, 1) = -plane.D * light.getY();
	out(3, 2) = -plane.D * light.getZ();
	out(3, 3) = -plane.D * point + d;

	return out;
}

//! Builds a matrix that flattens geometry into a plane.
/** \param light: light source
	\param plane: plane into which the geometry if flattened into
	\param point: value between 0 and 1, describing the light source.
	If this is 1, it is a point light, if it is 0, it is a directional light. */
template<typename T>
inline
CMatrix4<T>
buildShadowMatrix(const vector3d<T>& light, const plane3d<T>& plane, T point = T(1))
{
	CMatrix4<T> m(CMatrix4<T>::EM4CONST_NOTHING);
	buildShadowMatrix(m, light, plane, point);
	return m;
}

//! Builds a matrix which transforms a normalized Device Coordinate to Device Coordinates.
/** Used to scale <-1,-1><1,1> to viewport, for example from von <-1,-1> <1,1> to the viewport <0,0><0,640> */
template<typename T>
inline
CMatrix4<T>&
buildNDCToDCMatrix(CMatrix4<T>& out, const rect<s32>& area, T zScale)
{
	T scaleX = (area.getWidth() - T(0.75)) / T(2);
	T scaleY = -(area.getHeight() - T(0.75)) / T(2);

	T dx = T(-0.5) + ((area.UpperLeftCorner.getX() + area.LowerRightCorner.getX() ) / T(2));
	T dy = T(-0.5) + ((area.UpperLeftCorner.getY() + area.LowerRightCorner.getY() ) / T(2));

	out.setScale(vector3d<T>(scaleX, scaleY, zScale));
	out(0, 1) = T(0);
	out(0, 2) = T(0);
	out(0, 3) = T(0);
	out(1, 0) = T(0);
	out(1, 2) = T(0);
	out(1, 3) = T(0);
	out(2, 0) = T(0);
	out(2, 1) = T(0);
	out(2, 3) = T(0);
	out(3, 0) = dx;
	out(3, 1) = dy;
	out(3, 2) = T(0);
	out(3, 3) = T(1);

	return out;
}

//! Builds a matrix which transforms a normalized Device Coordinate to Device Coordinates.
/** Used to scale <-1,-1><1,1> to viewport, for example from von <-1,-1> <1,1> to the viewport <0,0><0,640> */
template<typename T>
inline
CMatrix4<T>
buildNDCToDCMatrix(const rect<s32>& area, T zScale)
{
	CMatrix4<T> m(CMatrix4<T>::EM4CONST_NOTHING);
	buildNDCToDCMatrix(m);
	return m;
}

/*
  construct 2D Texture transformations
  rotate about center, scale, and transform.
*/
//! Set to a texture transformation matrix with the given parameters.
template<typename T>
inline
CMatrix4<T>
buildTextureTransform(T rotateRad,
					  const vector2d<T>& rotatecenter,
					  const vector2d<T>& translate,
					  const vector2d<T>& scale)
{
	T c = glf::Cos(rotateRad);
	T s = glf::Sin(rotateRad);

	CMatrix4<T> m(CMatrix4<T>::EM4CONST_NOTHING);

	m(0, 0) = c * scale.getX();
	m(0, 1) = s * scale.getY();
	m(0, 2) = T(0);
	m(0, 3) = T(0);

	m(1, 0) = -s * scale.getX();
	m(1, 1) = c * scale.getY();
	m(1, 2) = T(0);
	m(1, 3) = T(0); 

	T dx = -rotatecenter.getX() + translate.getX();
	T dy = -rotatecenter.getY() + translate.getY();

	m(2, 0) = scale.getX() * (c * dx + -s * dy) + rotatecenter.getX();
	m(2, 1) = scale.getY() * (s * dx + c * dy) + rotatecenter.getY();
	m(2, 2) = T(1);
	m(2, 3) = T(0);

	m(3, 0) = T(0);
	m(3, 1) = T(0);
	m(3, 2) = T(0);
	m(3, 3) = T(1);

	return m;
}

//! Fix the projection for a 3DS. Depth :[-1,1] -> [0,-1]
template<typename T>
inline
CMatrix4<T>&
fixProjectionFor3DS(CMatrix4<T>& mat)
{
    mat[2] = (mat[2] + mat[3]) * -0.5f;
    mat[6] = (mat[6] + mat[7]) * -0.5f;
    mat[10] = (mat[10] + mat[11]) * -0.5f;
    mat[14] = (mat[14] + mat[15]) * -0.5f;
	mat[14] /= 100.0f; // scale down far/near planes
	return mat;
}

//! extract the absolute world position of the camera from it's view matrix.
template<typename T>
inline
vector3d<T>&
absoluteCameraPositionFromViewMatrix(vector3d<T>&out, const CMatrix4<T>& mat)
{
	out.setX(-mat[12]);
	out.setY(-mat[13]);
	out.setZ(-mat[14]);
	mat.getTransposed().transformVect(out);
	return out;
}

//! creates a matrix that will offset a RGBA vector with the specified HSL
template<typename T>
inline
CMatrix4<T>
buildHSLOffsetMatrix(T hueOffset, T satOffset, T lumOffset)
{
	// Hue : rotation around Z axis

	// Step1 : setup rotation for gray vector into positive Z (TODO: should be precomputed)
	CMatrix4<T> xMat(CMatrix4<T>::EM4CONST_NOTHING);
	xMat.makeRotationDegrees(vector3d<T>(T(45), T(0), T(0)));

	CMatrix4<T> rMat(CMatrix4<T>::EM4CONST_NOTHING);
	rMat.makeRotationDegrees(vector3d<T>(T(0), T(-35.2643), T(0)));	
	rMat *= xMat;

	// Step2 : setup hue rotation
	CMatrix4<T> hueMat(CMatrix4<T>::EM4CONST_NOTHING);
	hueMat.makeRotationDegrees(vector3d<T>(T(0), T(0), hueOffset));

	// Step3 : apply it all
	hueMat = rMat.getTransposed() * hueMat * rMat;

	// Saturation : linear interpolation to gray
	const T mirSat = T(1) - satOffset;

	// Color factors for grey
	const T rF = T(0.3086);
	const T gF = T(0.6094);
	const T bF = T(0.0820);

	CMatrix4<T> satMat(CMatrix4<T>::EM4CONST_NOTHING);
	satMat[0]  = mirSat * rF + satOffset;
	satMat[1]  = mirSat * rF;
	satMat[2]  = mirSat * rF;
	satMat[3]  = T(0);

	satMat[4]  = mirSat * gF;
	satMat[5]  = mirSat * gF + satOffset;
	satMat[6]  = mirSat * gF;
	satMat[7]  = T(0);

	satMat[8]  = mirSat * bF;
	satMat[9]  = mirSat * bF;
	satMat[10] = mirSat * bF + satOffset;
	satMat[11] = T(0);

	satMat[12] = T(0);
	satMat[13] = T(0);
	satMat[14] = T(0);
	satMat[15] = T(1);

	// Luminosity : scale sat; then multiply with hueMat for final result
	return satMat.preScale(vector3d<T>(lumOffset)) * hueMat;
};

//! creates a matrix that will offset a RGBA vector with the specified HSL and channels offsets
template<typename T>
inline
CMatrix4<T>
buildHSLOffsetMatrix(const vector3d<T>& hsl,
					 const vector3d<T>& satOffset,
					 const vector3d<T>& lumOffset,
					 const vector3d<T>& brgOffset,
					 T brg)
{
	// Hue : rotation around Z axis

	// Step1 : setup rotation for gray vector into positive Z (TODO: should be precomputed)
	CMatrix4<T> xMat(CMatrix4<T>::EM4CONST_NOTHING);
	xMat.makeRotationDegrees(vector3d<T>(T(45), T(0), T(0)));

	CMatrix4<T> rMat(CMatrix4<T>::EM4CONST_NOTHING);
	rMat.makeRotationDegrees(vector3d<T>(T(0), T(-35.2643), T(0)));	
	rMat *= xMat;

	// Step2 : setup hue rotation
	CMatrix4<T> hueMat(CMatrix4<T>::EM4CONST_NOTHING);
	hueMat.makeRotationDegrees(vector3d<T>(T(0), T(0), hsl.x()));

	// Step3 : apply it all
	hueMat = rMat.getTransposed() * hueMat * rMat;

	const T satR = hsl.y() + satOffset.x();
	const T satG = hsl.y() + satOffset.y();
	const T satB = hsl.y() + satOffset.z();

	// Saturation : linear interpolation to gray
	const T mirSatR = T(1) - satR;
	const T mirSatG = T(1) - satG;
	const T mirSatB = T(1) - satB;

	// Color factors for grey
	const T rF = T(0.3086);
	const T gF = T(0.6094);
	const T bF = T(0.0820);

	CMatrix4<T> satMat(CMatrix4<T>::EM4CONST_NOTHING);
	satMat[0]  = mirSatR * rF + satR;
	satMat[1]  = mirSatR * rF;
	satMat[2]  = mirSatR * rF;
	satMat[3]  = T(0);

	satMat[4]  = mirSatG * gF;
	satMat[5]  = mirSatG * gF + satG;
	satMat[6]  = mirSatG * gF;
	satMat[7]  = T(0);

	satMat[8]  = mirSatB * bF;
	satMat[9]  = mirSatB * bF;
	satMat[10] = mirSatB * bF + satB;
	satMat[11] = T(0);

	satMat[12] = T(0);
	satMat[13] = T(0);
	satMat[14] = T(0);
	satMat[15] = T(1);

	// Brightness : scale sat; then multiply with hueMat
	const vector3d<T> brgVal(brg + brgOffset.x(), brg + brgOffset.y(), brg + brgOffset.z());
	CMatrix4<T> rez = satMat.preScale(brgVal) * hueMat;

	// Add Luminosity as translation. 1.0 luminosity means default value
	const vector3d<T> lum(hsl.z() - T(1) + lumOffset.x(),
						  hsl.z() - T(1) + lumOffset.y(),
						  hsl.z() - T(1) + lumOffset.z());
	rez.setTranslation(lum);

	return rez;
};

//all matrices types must have operator[]
template<typename MT1, typename MT2, typename MT3>
inline
void
rowMatrixProduct34(MT1& out, const MT2& m1, const MT3& m2)
{
	//GLITCH_DEBUG_BREAK_IF(&out == &m1 || &out == &m2);
	out[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2];
	out[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2];
	out[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2];
	out[3] = 0;

	out[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6];
	out[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6];
	out[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6];
	out[7] = 0;

	out[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10];
	out[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10];
	out[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10];
	out[11] = 0;

	out[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12];
	out[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13];
	out[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14];
	out[15] = 1;
}

} // end namespace jenny
