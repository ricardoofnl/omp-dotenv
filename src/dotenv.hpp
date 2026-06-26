#pragma once

#include <optional>
#include <string>
#include <unordered_map>

// Loads and stores key/value pairs from a .env file. Lookups fall back to the
// process environment (getenv) so the same API works for file values and real
// OS environment variables (handy for Docker / hosting).
class Dotenv
{
public:
	static Dotenv& instance();

	// Parse a .env file (KEY=VALUE per line, '#' comments, optional quotes).
	// Returns false if the file could not be opened. Adds to / overrides the
	// existing values.
	bool load(const std::string& path);

	// value from the file, else from getenv, else std::nullopt.
	std::optional<std::string> get(const std::string& key) const;

	// True if the key exists in the file or the OS environment.
	bool has(const std::string& key) const;

private:
	std::unordered_map<std::string, std::string> values_;
};