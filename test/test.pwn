// sample test gamemode for omp-dotenv
// 1. copy test/sample.env to your server root as .env
// 2. put omp-dotenv.so in components/ and omp-dotenv.inc on the include path
// 3. compile this as the gamemode and run, results print to the console

#include <open.mp>
#include <omp-dotenv>

static g_pass = 0;
static g_total = 0;

// print a single check result and tally it
Check(const name[], bool:ok)
{
    g_total++;
    if (ok) g_pass++;
    printf("%s %s", ok ? "[ok]  " : "[fail]", name);
}

main()
{
    new value[128];

    // string value
    ENV_Get("APP_NAME", value);
    Check("ENV_Get APP_NAME == open.mp", !strcmp(value, "open.mp"));

    // integer value
    Check("ENV_GetInt MAX_PLAYERS == 100", ENV_GetInt("MAX_PLAYERS") == 100);

    // bool value
    Check("ENV_GetBool DEBUG == true", ENV_GetBool("DEBUG") == true);

    // float value (roughly 3.14)
    new Float:pi = ENV_GetFloat("PI");
    Check("ENV_GetFloat PI ~= 3.14", pi > 3.13 && pi < 3.15);

    // surrounding quotes are stripped
    ENV_Get("QUOTED", value);
    Check("ENV_Get QUOTED == hello world", !strcmp(value, "hello world"));

    // existence checks
    Check("ENV_Has APP_NAME", ENV_Has("APP_NAME"));
    Check("ENV_Has missing key", ENV_Has("DOES_NOT_EXIST") == false);

    // fallback when the key is missing
    ENV_Get("DOES_NOT_EXIST", value, sizeof(value), "fallback");
    Check("ENV_Get fallback", !strcmp(value, "fallback"));

    printf("omp-dotenv test: %d/%d passed", g_pass, g_total);
}