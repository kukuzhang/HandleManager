#pragma once
#include <boost/signals2.hpp>
#include <boost/function.hpp>
#include <map>
template <
	typename R,
	typename T,
	typename K>
class HandleMgr
{
public:
	typedef typename boost::function<R(T)> Handle;
	typedef typename boost::signals2::signal<R(T)> Signal_t;
	typedef typename Signal_t::slot_type Signal_SlotType;
	typedef typename boost::shared_ptr<Signal_t> Signal_ptr;
	typedef typename std::map<K, Signal_ptr> HandleMap;
	typedef typename boost::function_traits<R(T)>::result_type result_type_t;
	HandleMgr() {};
	~HandleMgr(){
	
	};


	//boost::signals2::signal<void () > sig;

	bool registerHandle(K k, Handle handle, uint32 weight = 0)
	{
		typename HandleMap::iterator it = m_mHandle.find(k);
		if (it != m_mHandle.end())
		{
			it->second->connect(weight, handle);
		}
		else
		{
			Signal_ptr sig(new Signal_t);
			sig->connect(weight, handle);
			m_mHandle.insert(std::make_pair(k, sig));
		}

		return true;
	}
	template <typename T1>
	bool unregisterHandle(K k, T1 handle)
	{
		typename HandleMap::iterator it = m_mHandle.find(k);
		if (it != m_mHandle.end())
		{
			it->second->disconnect(handle);
			return true;
		}
		return false;
	}

	result_type_t callHandle(K k, T arg)
	{
		typename HandleMap::iterator it = m_mHandle.find(k);
		if (it != m_mHandle.end())
		{
			return (*it->second)(arg); 
		}
	}

private:

	HandleMap m_mHandle;
};

