#include "Symbol.hpp"
#include "Util.hpp"
#include <elf.h>

Library::Library(const std::string& name, const std::deque<E::Symbol>& symbols, const std::deque<Maps::MapEntry>& entries)
{
	if (entries.empty())
		return;

	path = entries.front().path;
	this->name = name;

	address = entries.front().address;
	size = entries.back().address - address + entries.back().size; // Rough size

	for (const auto& s : symbols)
	{
		m_symbols.insert({s.name, E::Resolve(s, entries)});
	}
}

Library::~Library()
{
}

std::uintptr_t Library::operator[](const std::string& key) const
{
	const auto it = m_symbols.find(key);
	if (it == m_symbols.end())
		throw Exception("Library::operator[] Could not get symbol '{0}' in module '{1}'", key, name);

	return it->second;
}

SymbolTable::SymbolTable()
{
}

SymbolTable::~SymbolTable()
{
}

const Library& SymbolTable::operator[](const std::string& key) const
{
	const auto it = m_table.find(key);
	if (it == m_table.end())
		throw Exception("SymbolTable::operator[] Could not get library '{}'", key);

	return it->second;
}
