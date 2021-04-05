#ifndef UTIL_SYMBOL_HPP
#define UTIL_SYMBOL_HPP

#include <map>
#include <string>
#include "Elf.hpp"

using E = ELF<DemanglerType::GNUCXXABI>;

class Library
{
public:
	std::map<std::string, E::Symbol> m_symbols;
	std::string path;
	std::string name;
	std::size_t address;
	std::size_t size;

	Library(const std::string& name);
	~Library();

	const E::Symbol& operator[](const std::string& key) const;
};

class SymbolTable
{
public:
	std::map<std::string, Library> m_table;
	
	SymbolTable();
	~SymbolTable();

	void BuildTable();

	const Library& operator[](const std::string& key) const;
};

extern SymbolTable symbols;

#endif // UTIL_SYMBOL_HPP
