#pragma once

#include <amx/amx.h>

// shared env natives, used by both the open.mp component and the samp plugin
namespace dotenv
{
	// register the env natives on a loaded script
	int RegisterNatives(AMX* amx);

	// wire the amx api from a raw export table (omp getAmxFunctions or samp ppData)
	void WireAmx(void* const* exports);
}