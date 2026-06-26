#include <sdk.hpp>
#include <Server/Components/Pawn/Impl/pawn_impl.hpp>

#include <cctype>
#include <string>

#include "dotenv.hpp"

// ---------------------------------------------------------------------------
//  AMX string helpers
// ---------------------------------------------------------------------------
namespace
{
	std::string AmxGetString(AMX* amx, cell param)
	{
		cell* addr = nullptr;
		if (amx_GetAddr(amx, param, &addr) != AMX_ERR_NONE || addr == nullptr)
			return "";

		int len = 0;
		amx_StrLen(addr, &len);
		if (len <= 0)
			return "";

		std::string out(static_cast<size_t>(len), '\0');
		amx_GetString(&out[0], addr, 0, static_cast<size_t>(len) + 1);
		return out;
	}

	void AmxSetString(AMX* amx, cell param, const std::string& value, cell size)
	{
		cell* addr = nullptr;
		if (amx_GetAddr(amx, param, &addr) != AMX_ERR_NONE || addr == nullptr)
			return;
		amx_SetString(addr, value.c_str(), 0, 0, static_cast<size_t>(size));
	}
}

// ---------------------------------------------------------------------------
//  Natives
// ---------------------------------------------------------------------------
// native ENV_Load(const path[] = ".env");
static cell AMX_NATIVE_CALL n_ENV_Load(AMX* amx, const cell* params)
{
	const std::string path = AmxGetString(amx, params[1]);
	return Dotenv::instance().load(path) ? 1 : 0;
}

// native ENV_Get(const key[], dest[], size = sizeof(dest), const fallback[] = "");
static cell AMX_NATIVE_CALL n_ENV_Get(AMX* amx, const cell* params)
{
	const std::string key = AmxGetString(amx, params[1]);
	const std::string fallback = AmxGetString(amx, params[4]);
	const std::string value = Dotenv::instance().get(key).value_or(fallback);
	AmxSetString(amx, params[2], value, params[3]);
	return static_cast<cell>(value.size());
}

// native ENV_GetInt(const key[], fallback = 0);
static cell AMX_NATIVE_CALL n_ENV_GetInt(AMX* amx, const cell* params)
{
	const std::string key = AmxGetString(amx, params[1]);
	const auto value = Dotenv::instance().get(key);
	if (!value)
		return params[2];
	try
	{
		return static_cast<cell>(std::stoi(*value));
	}
	catch (...)
	{
		return params[2];
	}
}

// native Float:ENV_GetFloat(const key[], Float:fallback = 0.0);
static cell AMX_NATIVE_CALL n_ENV_GetFloat(AMX* amx, const cell* params)
{
	const std::string key = AmxGetString(amx, params[1]);
	const auto value = Dotenv::instance().get(key);
	float result = amx_ctof(params[2]);
	if (value)
	{
		try
		{
			result = std::stof(*value);
		}
		catch (...)
		{
		}
	}
	return amx_ftoc(result);
}

// native bool:ENV_GetBool(const key[], bool:fallback = false);
static cell AMX_NATIVE_CALL n_ENV_GetBool(AMX* amx, const cell* params)
{
	const std::string key = AmxGetString(amx, params[1]);
	const auto value = Dotenv::instance().get(key);
	if (!value)
		return params[2];

	std::string s = *value;
	for (auto& c : s)
		c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

	if (s == "1" || s == "true" || s == "yes" || s == "on")
		return 1;
	if (s == "0" || s == "false" || s == "no" || s == "off")
		return 0;
	return params[2];
}

// native bool:ENV_Has(const key[]);
static cell AMX_NATIVE_CALL n_ENV_Has(AMX* amx, const cell* params)
{
	const std::string key = AmxGetString(amx, params[1]);
	return Dotenv::instance().has(key) ? 1 : 0;
}

static const AMX_NATIVE_INFO kNatives[] = {
	{ "ENV_Load", n_ENV_Load },
	{ "ENV_Get", n_ENV_Get },
	{ "ENV_GetInt", n_ENV_GetInt },
	{ "ENV_GetFloat", n_ENV_GetFloat },
	{ "ENV_GetBool", n_ENV_GetBool },
	{ "ENV_Has", n_ENV_Has },
	{ nullptr, nullptr },
};

// ---------------------------------------------------------------------------
//  Component
// ---------------------------------------------------------------------------
class DotenvComponent final : public IComponent, public PawnEventHandler
{
public:
	PROVIDE_UID(0xD07E27C0FFEE1001ULL);

	StringView componentName() const override { return "omp-dotenv"; }

	SemanticVersion componentVersion() const override { return SemanticVersion(1, 0, 0, 0); }

	void onLoad(ICore* c) override
	{
		core_ = c;
		// .env lives in the server root (C++ file access isn't sandboxed).
		if (Dotenv::instance().load(".env"))
			core_->printLn("omp-dotenv: loaded .env");
		else
			core_->printLn("omp-dotenv: no .env file found (OS environment still works)");
	}

	void onInit(IComponentList* components) override
	{
		pawn_ = components->queryComponent<IPawnComponent>();
		if (pawn_)
		{
			setAmxFunctions(pawn_->getAmxFunctions());
			pawn_->getEventDispatcher().addEventHandler(this);
		}
		else if (core_)
		{
			core_->printLn("omp-dotenv: Pawn component not found, natives unavailable");
		}
	}

	void onAmxLoad(IPawnScript& script) override
	{
		amx_Register(script.GetAMX(), kNatives, -1);
	}

	void onAmxUnload(IPawnScript& script) override { }

	void onFree(IComponent* component) override { }

	void free() override { delete this; }

	void reset() override { }

	~DotenvComponent()
	{
		if (pawn_)
			pawn_->getEventDispatcher().removeEventHandler(this);
	}

private:
	ICore* core_ = nullptr;
	IPawnComponent* pawn_ = nullptr;
};

COMPONENT_ENTRY_POINT()
{
	return new DotenvComponent();
}