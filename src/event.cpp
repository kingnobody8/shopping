#include "event.h"

static std::map<std::string, std::vector<EventCallback>> g_eventMap;

// Registers an event to a callback
// e.g. RegisterEvent("GOO GOO", ([](void *x) {printf("GOO GOO EVENT BODY\n"); }));
void RegisterEvent(const std::string& name, EventCallback callback)
{
	std::map<std::string, std::vector<EventCallback>>::iterator itr = g_eventMap.find(name);
	if (itr != g_eventMap.end())
	{
		itr->second.push_back(callback);
	}
	else
	{
		g_eventMap.insert(std::make_pair(name, std::vector<EventCallback>(1, callback)));
	}
}

void UnRegisterEvent(const std::string& name)
{
	// This currently erases all the events bound to the event name specified
	std::map<std::string, std::vector<EventCallback>>::iterator itr = g_eventMap.find(name);
	if (itr != g_eventMap.end())
	{
		g_eventMap.erase(itr);
	}
}

void FireEvent(const std::string& name, void* clientData)
{
	std::map<std::string, std::vector<EventCallback>>::iterator itr = g_eventMap.find(name);
	if (itr != g_eventMap.end())
	{
		for (auto vitr = itr->second.begin(); vitr != itr->second.end(); vitr++)
		{
			if(*vitr != nullptr) (*vitr)(clientData);
		}
	}
}