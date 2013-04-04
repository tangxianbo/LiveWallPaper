#pragma once
#include <base/Types.h>

namespace Jenny
{
	namespace Math
	{
		template<typename T>
		class EulerAngle_T
		{
		public:
			EulerAngle_T();
			~EulerAngle_T();

			EulerAngle_T(const T& x, const T& y, const T& z);

			T operator[](u32 index);
			const T& operator[](u32 index) const;

		private:
			T mData[3];
		};

		typedef EulerAngle_T<float> EulerAnglef;

		template<typename T>
		inline
		EulerAngle_T<T>::EulerAngle_T()
		{
			ZeroMemory(mData,sizeof(T)*3);
		}

		template<typename T>
		EulerAngle_T<T>::~EulerAngle_T()
		{

		}


		template<typename T>
		EulerAngle_T<T>::EulerAngle_T(const T& x, const T& y, const T& z)
		{
			mData[0] = x;
			mData[1] = y;
			mData[2] = z;
		}


		template<typename T>
		inline T 
		EulerAngle_T<T>::operator[](u32 index)
		{
			return mData[index];
		}


		template<typename T>
		const T& 
		EulerAngle_T<T>::operator[](u32 index) const
		{
			return mData[index];
		}
	}
}