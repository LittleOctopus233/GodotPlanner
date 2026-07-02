#include "IniReader.h"
#include <fstream>
#include <algorithm>
#include <cctype>

std::string IniReader::trim(const std::string& str) {
	if (str.empty()) return "";
	size_t start = str.find_first_not_of(" \t\r");
	size_t end = str.find_last_not_of(" \t\r");
	if (start == std::string::npos) return "";
	return str.substr(start, end - start + 1);
}

bool IniReader::loadStream(std::istream& stream) {
	data.clear();
	std::string line, currentSection;

	while (std::getline(stream, line)) {
		std::string trimmed = trim(line);
		if (trimmed.empty() || trimmed[0] == ';') continue;
		if (trimmed[0] == '[' && trimmed.back() == ']') {
			currentSection = trim(trimmed.substr(1, trimmed.size() - 2));
			continue;
		}
		size_t eqPos = trimmed.find('=');
		if (eqPos != std::string::npos && eqPos > 0) {
			std::string key = trim(trimmed.substr(0, eqPos));
			std::string value = trim(trimmed.substr(eqPos + 1));
			if (!key.empty()) data[currentSection][key] = value;
		}
	}
	return true;
}

bool IniReader::load(const std::string& filename) {
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open()) return false;
	return loadStream(file);
}

bool IniReader::load(const std::wstring& filename) {
	std::ifstream file(std::filesystem::path(filename), std::ios::binary);
	if (!file.is_open()) return false;
	return loadStream(file);
}

std::string IniReader::getString(const std::string& section, const std::string& key,
	const std::string& defaultValue) const {
	auto secIt = data.find(section);
	if (secIt != data.end()) {
		auto keyIt = secIt->second.find(key);
		if (keyIt != secIt->second.end()) return keyIt->second;
	}
	return defaultValue;
}

int IniReader::getInt(const std::string& section, const std::string& key, int defaultValue) const {
	std::string s = getString(section, key);
	if (s.empty()) return defaultValue;
	try { return std::stoi(s); }
	catch (...) { return defaultValue; }
}

double IniReader::getDouble(const std::string& section, const std::string& key, double defaultValue) const {
	std::string s = getString(section, key);
	if (s.empty()) return defaultValue;
	try { return std::stod(s); }
	catch (...) { return defaultValue; }
}

bool IniReader::getBool(const std::string& section, const std::string& key, bool defaultValue) const {
	std::string s = getString(section, key);
	if (s.empty()) return defaultValue;
	std::string lower = s;
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	return (lower == "true" || lower == "1" || lower == "yes");
}

bool IniReader::hasKey(const std::string& section, const std::string& key) const {
	auto secIt = data.find(section);
	return secIt != data.end() && secIt->second.find(key) != secIt->second.end();
}

bool IniReader::hasSection(const std::string& section) const {
	return data.find(section) != data.end();
}