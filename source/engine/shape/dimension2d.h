#pragma once
#include <core/types.h>

namespace jenny
{

template < typename T >
class dimension2d
{
public:

	dimension2d();

	dimension2d(T width, T height);

	bool operator == (const dimension2d<T>& other) const;

	bool operator != (const dimension2d<T>& other) const;

	dimension2d<T>& set(T width, T height);

	dimension2d<T>& operator /= (T scale);

	dimension2d<T> operator / (T scale) const;

	dimension2d<T>& operator *= (T scale);

	dimension2d<T> operator * (T scale) const;

	T getArea() const;

	bool isNull() const;

	T Width;
	T Height;
};

typedef dimension2d<f32> dimension2df;
typedef dimension2d<s32> dimension2di;

template < typename T >
inline
dimension2d<T>::dimension2d()
	: Width(T(0))
	, Height(T(0))
{
}

template < typename T >
inline
dimension2d<T>::dimension2d(T width, T height)
	: Width(width)
	, Height(height)
{
}

template < typename T >
inline
bool
dimension2d<T>::operator == (const dimension2d<T>& other) const
{
	return Width == other.Width && Height == other.Height;
}

template < typename T >
inline
bool
dimension2d<T>::operator != (const dimension2d<T>& other) const
{
	return !(*this == other);
}

template < typename T >
inline
dimension2d<T>&
dimension2d<T>::set(T width, T height)
{
	Width = width;
	Height = height;
	return *this;
}

template < typename T >
inline
dimension2d<T>&
dimension2d<T>::operator /= (T scale)
{
	Width /= scale;
	Height /= scale;
	return *this;
}

template < typename T >
inline
dimension2d<T>
dimension2d<T>::operator / (T scale) const
{
	dimension2d<T> result(*this);
	result /= scale;
	return result;
}

template < typename T >
inline
dimension2d<T>&
dimension2d<T>::operator *= (T scale)
{
	Width *= scale;
	Height *= scale;
	return *this;
}

template < typename T >
inline
dimension2d<T>
dimension2d<T>::operator * (T scale) const
{
	dimension2d<T> result(*this);
	result *= scale;
	return result;
}

template < typename T >
inline
T
dimension2d<T>::getArea() const
{
	return Width * Height;
}

template < typename T >
inline
bool
dimension2d<T>::isNull() const
{
	return Width == (T)0 && Height == (T)0;
}

//------------------------------------------------------------------------------

template <class T>
inline
dimension2d<T>
abs(const dimension2d<T>& dims)
{
	dimension2d<T> r(dims);
	if(r.Width < T())
	{
		r.Width = -r.Width;
	}
	if(r.Height < T())
	{
		r.Height = -r.Height;
	}
	return r;
}

} // end namespace jenny
