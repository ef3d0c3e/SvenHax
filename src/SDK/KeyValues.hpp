#ifndef SDK_KEYVALUES_HPP
#define SDK_KEYVALUES_HPP

#include "CBaseFileSystem.hpp""

class KeyValues
{
	virtual void /*?*/ GetName() const = 0;
	virtual void /*?*/ GetNameSymbol() const = 0;
	virtual void /*?*/ LoadFromFile(CBaseFileSystem*, char const*, char const*) = 0;
	virtual void /*?*/ SaveToFile(CBaseFileSystem*, char const*, char const*) = 0;
	virtual void /*?*/ FindKey(char const*, bool) = 0;
	virtual void /*?*/ FindKey(int) const = 0;
	virtual void /*?*/ CreateNewKey() = 0;
	virtual void /*?*/ RemoveSubKey(KeyValues*) = 0;
	virtual void /*?*/ GetFirstSubKey() = 0;
	virtual void /*?*/ GetNextKey() = 0;
	virtual void /*?*/ GetInt(char const*, int) = 0;
	virtual void /*?*/ GetFloat(char const*, float) = 0;
	virtual void /*?*/ GetString(char const*, char const*) = 0;
	virtual void /*?*/ GetWString(char const*, wchar_t const*) = 0;
	virtual void /*?*/ GetPtr(char const*, void*) = 0;
	virtual void /*?*/ IsEmpty(char const*) = 0;
	virtual void /*?*/ SetWString(char const*, wchar_t const*) = 0;
	virtual void /*?*/ SetString(char const*, char const*) = 0;
	virtual void /*?*/ SetInt(char const*, int) = 0;
	virtual void /*?*/ SetFloat(char const*, float) = 0;
	virtual void /*?*/ SetPtr(char const*, void*) = 0;
	virtual void /*?*/ MakeCopy() const = 0;
	virtual void /*?*/ Clear() = 0;
	virtual void /*?*/ GetDataType(char const*) = 0;
	virtual void /*?*/ deleteThis() = 0;
};

#endif // SDK_KEYVALUES_HPP
