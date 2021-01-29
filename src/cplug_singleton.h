#pragma once

template <typename T>
class Cplug_Singleton
{
public:
	static T& getInstance();
	Cplug_Singleton(const Cplug_Singleton&) = delete;
	Cplug_Singleton& operator = (const Cplug_Singleton&) = delete;
private:

};

template<typename T>
inline T& Cplug_Singleton<T>::getInstance()
{
	static T g_instance;
	return g_instance;
}
