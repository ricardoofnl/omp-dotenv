// omp-dotenv - dotenv support for open.mp
// lookup order for every key: .env file, os environment, then fallback

#if defined _omp_dotenv_included
    #endinput
#endif
#define _omp_dotenv_included

// load or reload a .env file (already auto-loaded as .env on startup)
native ENV_Load(const path[] = ".env");

// copy the value of key into dest, or fallback when the key is missing
native ENV_Get(const key[], dest[], size = sizeof(dest), const fallback[] = "");

// value parsed as an integer, or fallback when missing or not a number
native ENV_GetInt(const key[], fallback = 0);

// value parsed as a float, or fallback when missing or not a number
native Float:ENV_GetFloat(const key[], Float:fallback = 0.0);

// value parsed as a bool, true for 1/true/yes/on and false for 0/false/no/off
native bool:ENV_GetBool(const key[], bool:fallback = false);

// true if the key exists in the .env file or the os environment
native bool:ENV_Has(const key[]);