#include "debug_text.h"
#include <cstdarg>

static void DebugVPrintf(const char* fmt, va_list args)
{
	char buffer[32000];
	int len = vsnprintf(buffer, sizeof(buffer), fmt, args);
	buffer[len] = '\0';
	g_debugText.setString(buffer);
}

void DebugPrintf(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	DebugVPrintf(fmt, args);
	va_end(args);
}