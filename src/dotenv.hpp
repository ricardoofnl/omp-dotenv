#pragma once

#include <optional>
#include <string>
#include <unordered_map>

// loads key/value pairs from a .env file, with a fallback to os environment
class Dotenv
{
public:
	static Dotenv& instance();

	// parse a .env file, returns false if it could not be opened
	bool load(const std::string& path);

	// value from the file, else from getenv, else nullopt
	std::optional<std::string> get(const std::string& key) const;

	// true if the key exists in the file or the os environment
	bool has(const std::string& key) const;

private:
	std::unordered_map<std::string, std::string> values_;
};