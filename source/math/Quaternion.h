#pragma once
#include <math/Vector.h>
#include <math/EulerAngle.h>
#include <math/Math.h>

namespace Jenny
{
	namespace Math
	{
		template<typename T>
		class Quaternion_T
		{
		public:
			Quaternion_T();
			~Quaternion_T();

			//rotation order: heading pitch bank
			explicit Quaternion_T(const EulerAngle_T<T>& eulerAngle);

			Quaternion_T(const Vector_T<T,3>& vec, T const& s);
			Quaternion_T(const T& x, const T& y, const T& z, const T& w);

			T operator[](u32 index);
			const T& operator[](u32 index) const;
			Quaternion_T operator*(T f) const;

			EulerAngle_T<T> ToEulerAngle();

		private:
			T mData[4];
		};

		typedef Quaternion_T<float> Quaternionf;


		template<typename T>
		inline
		Quaternion_T<T>::Quaternion_T()
		{

		}


		template<typename T>
		Quaternion_T<T>::~Quaternion_T()
		{

		}


		template<typename T>
		inline
		Quaternion_T<T>::Quaternion_T(const EulerAngle_T<T>& eulerAngle)
		{
			const T angX(eulerAngle[0]/2), angY(eulerAngle[1]/2), angZ(eulerAngle[2]/2);

			T sx = sin(angX);
			T sy = sin(angY);
			T sz = sin(angZ);
			T cx = cos(angX);
			T cy = cos(angY);
			T cz = cos(angZ);

			
			mData[0] = 	sx * cy * cz + cx * sy * sz;
			mData[1] =	cx * sy * cz - sx * cy * sz;
			mData[2] =	cx * cy * sz - sx * sy * cz;
			mData[3] =	sx * sy * sz + cx * cy * cz;
		}


		template<typename T>
		inline 
		Quaternion_T<T>::Quaternion_T(const Vector_T<T,3>& vec, T const& s)
		{
			mData[0] = vec[0];
			mData[1] = vec[1];
			mData[2] = vec[2];
			mData[3] = s;
		}

		template<typename T>
		inline
		Quaternion_T<T>::Quaternion_T(const T& x, const T& y, const T& z, const T& w)
		{
			mData[0] = x;
			mData[1] = y;
			mData[2] = z;
			mData[3] = w;
		}


		template<typename T>
		inline T 
		Quaternion_T<T>::operator[](u32 index)
		{
			return mData[index];
		}


		template<typename T>
		inline const T& 
		Quaternion_T<T>::operator[](u32 index) const
		{
			return mData[index];
		}


		template<typename T>
		Quaternion_T<T> 
		Quaternion_T<T>::operator*(T f) const
		{
			Quaternion_T<T> ret = *this;
			ret.mData[0] *= f;
			ret.mData[1] *= f;
			ret.mData[2] *= f;
			ret.mData[3] *= f;
			return ret;
		}


		template<typename T>
		inline
		T Dot(const Quaternion_T<T>& lhs, const Quaternion_T<T>& rhs)
		{
			return lhs[0]*rhs[0] + lhs[1]*rhs[1] + lhs[2]*rhs[2] + lhs[3]*rhs[3];
		}


		template<typename T>
		inline EulerAngle_T<T> 
		Quaternion_T<T>::ToEulerAngle()
		{
			T x = T(0);
			T y = T(0);
			T z = T(0);

			T sqx = mData[0] * mData[0];
			T sqy = mData[1] * mData[1];
			T sqz = mData[2] * mData[2];
			T sqw = mData[3] * mData[3];
			T unit = sqx + sqy + sqz + sqw;
			T test = mData[3] * mData[0] - mData[1] * mData[2];
			if (test > T(0.499) * unit)
			{
				// singularity at north pole
				y = 2 * atan2(mData[2], mData[3]);
				z = PI / 2;
				x = 0;
			}
			else
			{
				if (test < -T(0.499) * unit)
				{
					// singularity at south pole
					y = -2 * atan2(mData[2], mData[3]);
					z = -PI / 2;
					x = 0;
				}
				else
				{
					y = atan2(2 * (mData[1] * mData[3] + mData[0] * mData[2]), -sqx - sqy + sqz + sqw);
					x = asin(2 * test / unit);
					z = atan2(2 * (mData[2] * mData[3] + mData[0] * mData[1]), -sqx + sqy - sqz + sqw);
				}
			}
			return EulerAngle_T<T>(x,y,z);
		}


		#include <math/Quaternion.cpp>
		template<typename T>
		Quaternion_T<T> Slerp(const Quaternion_T<T>& lhs, const Quaternion_T<T>& rhs, T s);

		template<typename T>
		Quaternion_T<T> mul(const Quaternion_T<T>& lhs, const Quaternion_T<T>& rhs);

		template<typename T>
		inline Quaternion_T<T> operator*(float f, const Quaternion_T<T> q)
		{
			return q*f;
		}

		template<typename T>
		inline Quaternion_T<T> operator+(const Quaternion_T<T>& lhs, const Quaternion_T<T>& rhs)
		{
			return Quaternion_T<T>(
								lhs[0] + rhs[0], 
								lhs[1] + rhs[1], 
								lhs[2] + rhs[2], 
								lhs[3] + rhs[3]);
		}
	}
}