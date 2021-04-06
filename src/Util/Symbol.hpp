#ifndef UTIL_SYMBOL_HPP
#define UTIL_SYMBOL_HPP

#include <map>
#include <string>
#include "Elf.hpp"
#include <deque>

using E = ELF<DemanglerType::GNUCXXABI>;

class Library
{
public:
	std::map<std::string, std::uintptr_t> m_symbols;
	std::string path;
	std::string name;
	std::size_t address;
	std::size_t size;

	Library(const std::string& name, const std::deque<E::Segment>& segs, const std::deque<E::Symbol>& symbols, const std::deque<Maps::MapEntry>& entries);
	~Library();

	std::uintptr_t operator[](const std::string& key) const;
};

class SymbolTable
{
public:
	std::map<std::string, Library> m_table;
	
	SymbolTable();
	~SymbolTable();

	const Library& operator[](const std::string& key) const;
};

#endif // UTIL_SYMBOL_HPP
