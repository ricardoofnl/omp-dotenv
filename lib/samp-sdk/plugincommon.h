#pragma once

// minimal subset of the sa-mp plugin sdk, enough to expose amx natives

#define SAMP_PLUGIN_VERSION 0x0200

enum SUPPORTS_FLAGS
{
	SUPPORTS_VERSION = SAMP_PLUGIN_VERSION,
	SUPPORTS_VERSION_MASK = 0xffff,
	SUPPORTS_AMX_NATIVES = 0x10000,
	SUPPORTS_PROCESS_TICK = 0x20000,
};

enum PLUGIN_DATA_TYPE
{
	PLUGIN_DATA_LOGPRINTF = 0x00,
	PLUGIN_DATA_AMX_EXPORTS = 0x10,
};

// host-provided logger, captured from ppData on load
typedef void (*logprintf_t)(const char* format, ...);

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
	#define PLUGIN_CALL __stdcall
	#define PLUGIN_EXPORT extern "C" __declspec(dllexport)
#else
	#define PLUGIN_CALL
	#define PLUGIN_EXPORT extern "C" __attribute__((visibility("default")))
#endif