#pragma once
#include <math/Vector.h>

namespace Jenny
{
	namespace Math
	{
		template<typename T>
		class Matrix_T
		{
		public:
			Matrix_T();
			~Matrix_T();

			Matrix_T(const T& v00, const T& v01, const T& v02, const T& v03,
					const T& v10, const T& v11, const T& v12, const T& v13,
					const T& v20, const T& v21, const T& v22, const T& v23,
					const T& v30, const T& v31, const T& v32, const T& v33);

			Matrix_T(const Matrix_T& rhs);
			Matrix_T(Matrix_T&& rhs);

			T operator()(u32 row, u32 colume);

			const T& operator()(u32 row, u32 colume) const;

			Matrix_T& operator=(const Matrix_T& rhs);
			Matrix_T& operator=(Matrix_T&& rhs);

		private:
			T mData[4][4];
		};

		typedef Matrix_T<float> Matrix4x4f;


		template<typename T>
		inline
			Matrix_T<T>::Matrix_T()
		{
			ZeroMemory(mData, sizeof(T)*16);
		}


		template<typename T>
		inline
			Matrix_T<T>::~Matrix_T()
		{

		}

		template<typename T>
		Matrix_T<T>::Matrix_T(const T& v00, const T& v01, const T& v02, const T& v03,
			const T& v10, const T& v11, const T& v12, const T& v13,
			const T& v20, const T& v21, const T& v22, const T& v23,
			const T& v30, const T& v31, const T& v32, const T& v33)
		{
			mData[0][0] = v00; mData[0][1] = v01; mData[0][2] = v02; mData[0][3] = v03;
			mData[1][0] = v10; mData[1][1] = v11; mData[1][2] = v12; mData[1][3] = v13;
			mData[2][0] = v20; mData[2][1] = v21; mData[2][2] = v22; mData[2][3] = v23;
			mData[3][0] = v30; mData[3][1] = v31; mData[3][2] = v32; mData[3][3] = v33;
		}


		template<typename T>
		inline
			Matrix_T<T>::Matrix_T(const Matrix_T& rhs)
		{

		}


		template<typename T>
		inline
			Matrix_T<T>::Matrix_T(Matrix_T&& rhs)
		{

		}

		template<typename T>
		inline T 
			Matrix_T<T>::operator()(u32 row, u32 colume)
		{
			return mData[row][colume];
		}

		template<typename T>
		inline const T& 
			Matrix_T<T>::operator()(u32 row, u32 colume) const
		{
			return mData[row][colume];
		}


		template<typename T>
		inline Matrix_T<T>& 
			Matrix_T<T>::operator=(const Matrix_T& rhs)
		{
			memcpy(&this->mData[0], &rhs.mData[0], sizeof(T)*16);
			return *this;
		}


		template<typename T>
		inline Matrix_T<T>& 
			Matrix_T<T>::operator=(Matrix_T&& rhs)
		{
			memcpy(&this->mData[0], &rhs.mData[0], sizeof(T)*16);
			return *this;
		}





		#include <math/Matrix.cpp>
		template<typename T>
		Vector_T<T,4> Multiply(const Vector_T<T,4>& vector, const Matrix_T<T>& matrix);

		template<typename T>
		Matrix_T<T> Multiply(const Matrix_T<T>& lhs, const Matrix_T<T>& rhs);

	}
}