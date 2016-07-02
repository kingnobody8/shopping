#include "event.h"

static std::map<std::string, EventCallback> g_eventMap;

void RegisterEvent(const std::string& name, EventCallback callback)
{
	g_eventMap.insert(std::make_pair(name, callback));
}

void UnRegisterEvent(const std::string& name)
{
	std::map<std::string, EventCallback>::iterator itr = g_eventMap.find(name);
	if (itr != g_eventMap.end())
	{
		g_eventMap.erase(itr);
	}
}

void FireEvent(const std::string& name, void* clientData)
{
	std::map<std::string, EventCallback>::iterator itr = g_eventMap.find(name);
	if (itr != g_eventMap.end())
	{
		itr->second(clientData);
	}
}