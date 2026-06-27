#include <sdk.hpp>
#include <Server/Components/Pawn/pawn.hpp>

#include "natives.hpp"
#include "dotenv.hpp"

// the open.mp component: loads .env and registers the natives
class DotenvComponent final : public IComponent, public PawnEventHandler
{
public:
	PROVIDE_UID(0xD07E27C0FFEE1001ULL);

	StringView componentName() const override { return "omp-dotenv"; }

	SemanticVersion componentVersion() const override { return SemanticVersion(1, 0, 0, 0); }

	void onLoad(ICore* c) override
	{
		core_ = c;
		// .env lives in the server root (c++ file access isn't sandboxed)
		if (Dotenv::instance().load(".env"))
			core_->printLn("omp-dotenv: loaded .env");
		else
			core_->printLn("omp-dotenv: no .env file found (os environment still works)");
	}

	void onInit(IComponentList* components) override
	{
		pawn_ = components->queryComponent<IPawnComponent>();
		if (pawn_)
		{
			// wire the amx api so amx_* calls route through the server
			dotenv::WireAmx(pawn_->getAmxFunctions().data());
			pawn_->getEventDispatcher().addEventHandler(this);
		}
		else if (core_)
		{
			core_->printLn("omp-dotenv: pawn component not found, natives unavailable");
		}
	}

	// register the natives on every loaded script
	void onAmxLoad(IPawnScript& script) override
	{
		dotenv::RegisterNatives(script.GetAMX());
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