#include "dotenv.hpp"

#include <cstdlib>
#include <fstream>

namespace
{
	std::string trim(const std::string& s)
	{
		const char* ws = " \t\r\n";
		const auto begin = s.find_first_not_of(ws);
		if (begin == std::string::npos)
			return "";
		const auto end = s.find_last_not_of(ws);
		return s.substr(begin, end - begin + 1);
	}

	// Remove a matching pair of surrounding single or double quotes.
	std::string unquote(const std::string& s)
	{
		if (s.size() >= 2)
		{
			const char f = s.front(), b = s.back();
			if ((f == '"' && b == '"') || (f == '\'' && b == '\''))
				return s.substr(1, s.size() - 2);
		}
		return s;
	}
}

Dotenv& Dotenv::instance()
{
	static Dotenv inst;
	return inst;
}

bool Dotenv::load(const std::string& path)
{
	std::ifstream file(path);
	if (!file.is_open())
		return false;

	std::string line;
	while (std::getline(file, line))
	{
		line = trim(line);
		if (line.empty() || line[0] == '#')
			continue;

		// Allow an optional "export " prefix.
		if (line.rfind("export ", 0) == 0)
			line = trim(line.substr(7));

		const auto eq = line.find('=');
		if (eq == std::string::npos)
			continue;

		const std::string key = trim(line.substr(0, eq));
		const std::string value = unquote(trim(line.substr(eq + 1)));
		if (!key.empty())
			values_[key] = value;
	}
	return true;
}

std::optional<std::string> Dotenv::get(const std::string& key) const
{
	const auto it = values_.find(key);
	if (it != values_.end())
		return it->second;

	if (const char* env = std::getenv(key.c_str()))
		return std::string(env);

	return std::nullopt;
}

bool Dotenv::has(const std::string& key) const
{
	if (values_.find(key) != values_.end())
		return true;
	return std::getenv(key.c_str()) != nullptr;
}