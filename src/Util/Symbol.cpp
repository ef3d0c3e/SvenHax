#include "Symbol.hpp"
#include "Util.hpp"
#include <elf.h>

Library::Library(const std::string& name)
{
	auto getName = [](const char* name) {
		const std::string s(name);
		const auto pos = s.rfind('/');
		if (pos == std::string::npos)
			return s;

		return s.substr(pos + 1);

		/* Gets the name for dlopen() (somewhat)
		if (strlen(name) < 5) // "/home"
			return name;

		if (strcmp(name, "/home") <= 0)
			return name;
		std::string s(name);

		const auto pos = s.find("Sven Co-op/");
		if (pos == std::string::npos)
			return s;

		return s.substr(pos + 11);*/
	};
}

Library::~Library()
{
}

const E::Symbol& Library::operator[](const std::string& key) const
{
	const auto it = m_symbols.find(key);
	if (it == m_symbols.end())
		throw Exception("Library::operator[] Could not get symbol '{0}' in file '{1}'", key, name);

	return it->second;
}

SymbolTable::SymbolTable()
{
}

SymbolTable::~SymbolTable()
{
}

void SymbolTable::BuildTable()
{
}

const Library& SymbolTable::operator[](const std::string& key) const
{
	const auto it = m_table.find(key);
	if (it == m_table.end())
		throw Exception("SymbolTable::operator[] Could not get library '{}'", key);

	return it->second;
}
