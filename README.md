# omp-dotenv

`.env` support for [open.mp](https://open.mp) — load configuration and secrets
from a `.env` file (and the OS environment) and read them from Pawn.

Keep secrets like database keys out of your committed `.pwn`/`config.json`.

## Natives

```pawn
#include <omp-dotenv>

native ENV_Load(const path[] = ".env");
native ENV_Get(const key[], dest[], size = sizeof(dest), const fallback[] = "");
native ENV_GetInt(const key[], fallback = 0);
native Float:ENV_GetFloat(const key[], Float:fallback = 0.0);
native bool:ENV_GetBool(const key[], bool:fallback = false);
native bool:ENV_Has(const key[]);
```

Lookup order for every key: **`.env` file → OS environment variable → fallback**.

## Example

`.env` (in the server root):

```dotenv
# comments and "export" prefixes are allowed
SUPABASE_URL=https://yourproject.supabase.co
SUPABASE_KEY="eyJ..."
MAX_PLAYERS=100
DEBUG=true
```

```pawn
public OnGameModeInit()
{
    new url[160], key[400];
    ENV_Get("SUPABASE_URL", url);
    ENV_Get("SUPABASE_KEY", key);

    new maxPlayers = ENV_GetInt("MAX_PLAYERS", 50);
    new bool:debug = ENV_GetBool("DEBUG", false);
    return 1;
}
```

## Install (server)

1. Build (see below) or grab a release `omp-dotenv.so` / `omp-dotenv.dll`.
2. Put it in your server's `components/` folder.
3. Put `include/omp-dotenv.inc` on your include path and `#include <omp-dotenv>`.
4. Create a `.env` file in the server root.

The component auto-loads `.env` from the server root on startup. Unlike Pawn's
own file functions (limited to `scriptfiles/`), this C++ component can read the
root and also falls back to real OS environment variables.

## Build

Requires a **32-bit** toolchain (open.mp servers are i386), CMake ≥ 3.19, and
the submodules.

```sh
git clone --recursive https://github.com/<you>/omp-dotenv
cd omp-dotenv
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
# -> build/omp-dotenv.so
```

On Arch Linux you need multilib (`lib32-gcc-libs`, `lib32-glibc`). On Debian/
Ubuntu: `gcc-multilib g++-multilib`.

### Submodules

| Path | Repo | Purpose |
|------|------|---------|
| `lib/omp-sdk` | openmultiplayer/open.mp-sdk | open.mp component SDK |
| `lib/pawn` | openmultiplayer/compiler | `amx/amx.h` headers |
| `lib/pawn-natives` | Y-Less/pawn-natives | used by the SDK's Pawn impl |

## License

MIT