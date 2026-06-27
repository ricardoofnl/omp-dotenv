#include <plugincommon.h>

#include "natives.hpp"
#include "dotenv.hpp"

// on win32 the stdcall exports are decorated (_name@bytes), alias them to plain names
#if defined(_WIN32) && !defined(_WIN64)
	#define EXPORT_PLAIN(name, size) __pragma(comment(linker, "/EXPORT:" #name "=_" #name "@" #size))
#else
	#define EXPORT_PLAIN(name, size)
#endif

// host logger captured on load
static logprintf_t gLogprintf = nullptr;

EXPORT_PLAIN(Supports, 0)
PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

EXPORT_PLAIN(Load, 4)
PLUGIN_EXPORT bool PLUGIN_CALL Load(void** ppData)
{
	gLogprintf = reinterpret_cast<logprintf_t>(ppData[PLUGIN_DATA_LOGPRINTF]);
	// the amx exports table matches open.mp's getAmxFunctions layout
	dotenv::WireAmx(static_cast<void* const*>(ppData[PLUGIN_DATA_AMX_EXPORTS]));

	// .env lives in the server root
	if (Dotenv::instance().load(".env"))
		gLogprintf("omp-dotenv: loaded .env");
	else
		gLogprintf("omp-dotenv: no .env file found (os environment still works)");
	return true;
}

EXPORT_PLAIN(Unload, 0)
PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
}

EXPORT_PLAIN(AmxLoad, 4)
PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX* amx)
{
	return dotenv::RegisterNatives(amx);
}

EXPORT_PLAIN(AmxUnload, 4)
PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX* amx)
{
	return AMX_ERR_NONE;
}