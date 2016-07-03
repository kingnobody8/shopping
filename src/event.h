#pragma once

#include <map>
#include <vector>

#define EVENT_AS(TYPE, LOCAL) TYPE& LOCAL = *static_cast<TYPE*>(clientData)

typedef void (*EventCallback)(void*);

void RegisterEvent(const std::string& name, EventCallback callback);
void UnRegisterEvent(const std::string& name);
void FireEvent(const std::string& name, void* clientData = nullptr);