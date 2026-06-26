/**
 * omp-dotenv - .env support for open.mp
 * --------------------------------------------------------------------------
 * Reads scriptfiles-free .env from the server root and the OS environment.
 * Lookup order for every key: .env file -> OS environment -> fallback.
 *
 *   #include <omp-dotenv>
 *
 *   new url[160];
 *   ENV_Get("SUPABASE_URL", url);
 */

#if defined _omp_dotenv_included
    #endinput
#endif
#define _omp_dotenv_included

/// Load (or reload) a .env file. Auto-loaded as ".env" on startup, so this is
/// only needed to load a different file or refresh. Returns 1 on success.
native ENV_Load(const path[] = ".env");

/// Copy the value of key into dest (or fallback if missing). Returns length.
native ENV_Get(const key[], dest[], size = sizeof(dest), const fallback[] = "");

/// Value parsed as an integer (or fallback if missing / not a number).
native ENV_GetInt(const key[], fallback = 0);

/// Value parsed as a float (or fallback if missing / not a number).
native Float:ENV_GetFloat(const key[], Float:fallback = 0.0);

/// Value parsed as a bool ("1/true/yes/on" or "0/false/no/off", else fallback).
native bool:ENV_GetBool(const key[], bool:fallback = false);

/// True if the key exists in the .env file or the OS environment.
native bool:ENV_Has(const key[]);