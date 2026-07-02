#pragma once

#include <string>
#include <map>
#include <istream>
#include <filesystem>

class IniReader {
private:
	std::map<std::string, std::map<std::string, std::string>> data;

	static std::string trim(const std::string& str);
	bool loadStream(std::istream& stream);

public:
	bool load(const std::string& filename);
	bool load(const std::wstring& filename);

	std::string getString(const std::string& section, const std::string& key,
		const std::string& defaultValue = "") const;

	int getInt(const std::string& section, const std::string& key, int defaultValue = 0) const;

	double getDouble(const std::string& section, const std::string& key, double defaultValue = 0.0) const;

	bool getBool(const std::string& section, const std::string& key, bool defaultValue = false) const;

	bool hasKey(const std::string& section, const std::string& key) const;
	bool hasSection(const std::string& section) const;
};