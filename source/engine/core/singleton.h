#ifndef _SINGLETON_
#define _SINGLETON_

#include "assert.h"

template <typename T> 
class Singleton
{
	static T* s_instance;

public:
	static T* newInstance() 
	{
		return new T();
	}

	static void deleteInstance() 
	{
		delete s_instance;
		s_instance = 0;
	}


	static T* instance() 
	{
		return (s_instance);
	}

protected:
	Singleton() 
	{
		s_instance = (T*)this;
	}

	virtual ~Singleton() 
	{
		s_instance = 0;
	}
private:
	Singleton(const Singleton& source) 
	{}

};

template<typename T>
T* Singleton<T>::s_instance = 0;

#endif
