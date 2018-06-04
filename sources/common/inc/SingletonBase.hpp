/********************************************************************
**
**              SingletonBase.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef __SINGLETON_BASE__
#define __SINGLETON_BASE__

#include <cstdlib>

template<class T>
class SingletonBase
{
public:
	SingletonBase() {}
	virtual ~SingletonBase() {}

	static T* self(void)
	{
		static T instance;

		if (_init == false)
		{
			_instance = &instance;
			_init = true;
		}
		return _instance;
	}

protected:
	static bool _init;
	static T* _instance;
};

template<class T> T* SingletonBase<T>::_instance = NULL;
template<class T> bool SingletonBase<T>::_init = false;

#endif
