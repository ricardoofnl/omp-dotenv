# omp-dotenv

`.env` support for [open.mp](https://open.mp) load configuration and secrets
from a `.env` file (and the OS environment) and read them from Pawn.

Keep secrets like database keys out of your committed `.pwn` and `config.json`.

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

Lookup order for every key: the `.env` file first, then an OS environment
variable, then the fallback.

## Example

`.env` in the server root:

```dotenv
# comments and an optional "export" prefix are allowed
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

## Install

- Build it (see below) or download a release `omp-dotenv.so` / `omp-dotenv.dll`.
- Put the binary in your server's `components/` folder (it auto-loads).
- Put `include/omp-dotenv.inc` on your include path and `#include <omp-dotenv>`.
- Create a `.env` file in the server root.

The component auto-loads `.env` from the server root on startup. Unlike Pawn's
own file functions (limited to `scriptfiles/`), this C++ component reads the
root and also falls back to real OS environment variables.

## Build

Requires a 32-bit toolchain (open.mp servers are i386), CMake 3.19+, and the
submodules.

```sh
git clone --recursive https://github.com/ricardooofnl/omp-dotenv
cd omp-dotenv
CMAKE_POLICY_VERSION_MINIMUM=3.5 cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

The output is `build/omp-dotenv.so`.

The `CMAKE_POLICY_VERSION_MINIMUM` env var lets very new CMake configure the
older bundled submodules. On Windows PowerShell, set it first:
`$env:CMAKE_POLICY_VERSION_MINIMUM=3.5`, then `cmake -B build -A Win32`.

On Arch Linux you need multilib (`lib32-gcc-libs`, `lib32-glibc`). On Debian or
Ubuntu use `gcc-multilib g++-multilib`.

## Test

A sample Pawn gamemode is in `test/`.

- Copy `test/sample.env` to your server root as `.env`.
- Put `omp-dotenv.so` in `components/` and `omp-dotenv.inc` on the include path.
- Compile `test/test.pwn` as the gamemode and run the server.

The results are printed to the server console, ending with `X/X passed`.

## License

GNU General Public License v3.0. See [LICENSE](LICENSE).