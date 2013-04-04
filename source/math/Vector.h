#pragma once
#include <base/Types.h>

namespace Jenny
{
	namespace Math
	{
	
		template<typename T, int N>
		class Vector_T
		{
			friend class Vector_T;

		public:
			Vector_T();
			~Vector_T();


			Vector_T(const T& x, const T& y, const T& z);

			Vector_T(const T& x, const T& y, const T& z, const T& w);


			//copy constructor
			template<int M>
			Vector_T(const Vector_T<T,M>& rhs);


			//move constructor
			template<int M>
			Vector_T(Vector_T<T,M>&& rhs);


			T operator[](u32 index);


			const T& operator[](u32 index) const;


			//copy assignment
			template<int M>
			Vector_T& operator=(const Vector_T<T,M>& rhs);

			
			//move assignment
			template<int M>
			Vector_T& operator=(Vector_T<T,M>&& rhs);

		private:
			T mData[N];
		};


		typedef Vector_T<float,2> Vector2f;
		typedef Vector_T<float,3> Vector3f;
		typedef Vector_T<float,4> Vector4f;


		template<typename T, int N> 
		inline Vector_T<T,N>::Vector_T()
		{
		}

		template<typename T, int N>
		inline Vector_T<T,N>::~Vector_T()
		{
		}

		template<typename T, int N>
		inline 
		Vector_T<T,N>::Vector_T(const T& x, const T& y, const T& z)
		{
			JENNY_STATIC_ASSERT(3 == N);
			mData[0] = x;
			mData[1] = y;
			mData[2] = z;
		}


		template<typename T, int N>
		inline 
		Vector_T<T,N>::Vector_T(const T& x, const T& y, const T& z, const T& w)
		{
			JENNY_STATIC_ASSERT(4 == N);
			mData[0] = x;
			mData[1] = y;
			mData[2] = z;
			mData[3] = w;
		}


		//copy constructor
		template<typename T, int N>
		template<int M>
		inline Vector_T<T,N>::Vector_T(const Vector_T<T,M>& rhs)
		{
			JENNY_STATIC_ASSERT(M>N);
			memcpy(this->mData, &rhs.mData, sizeof(T)*N);
		}


		//move constructor
		template<typename T, int N>
		template<int M> 
		inline
		Vector_T<T,N>::Vector_T(Vector_T<T,M>&& rhs)
		{
			JENNY_STATIC_ASSERT(M>N);
			memcpy(this->mData, rhs.mData, sizeof(T)*N);
		}


		template<typename T, int N>
		inline T 
		Vector_T<T,N>::operator[](u32 index)
		{
			return mData[index];
		}


		template<typename T, int N>
		inline const T& 
		Vector_T<T,N>::operator[](u32 index) const
		{
			return mData[index];
		}


		//copy assignment
		template<typename T, int N>
		template<int M>
		inline Vector_T<T,N>& 
		Vector_T<T,N>::operator=(const Vector_T<T,M>& rhs)
		{
			JENNY_STATIC_ASSERT(M>N);
			memcpy(this->mData,&rhs.mData,sizeof(T)*N);
			return *this;
		}


		//move assignment
		template<typename T, int N>
		template<int M>
		inline Vector_T<T,N>& 
		Vector_T<T,N>::operator=(Vector_T<T,M>&& rhs)
		{
			JENNY_STATIC_ASSERT(M>N);
			memcpy(this->mData, rhs.mData, sizeof(T)*N);
			return *this;
		}




		
		template<typename T>
		inline
		Vector_T<T,3> CrossProduct(const Vector_T<T,3>& lhs, const Vector_T<T,3>& rhs)
		{
			return Vector_T<T,3>(	lhs[1]*rhs[2] - lhs[2]*rhs[1],
				lhs[2]*rhs[1] - lhs[0]*rhs[2],
				lhs[0]*rhs[1] - lhs[1]*rhs[0]);
		}
	}
}