#ifndef PATTERNS_HPP
#define PATTERNS_HPP

#include <string>
#include <tuple>
#include "Hooker.hpp"
#include "Maps.hpp"
class Damage;


namespace Hooker
{
	std::tuple<bool, std::uintptr_t, std::size_t> GetLibraryInformation(const char* library);
	std::uintptr_t FindPattern(const std::function<bool(const std::string&)> pathFn, Maps::Permissions permMask, std::uintptr_t beg, std::uintptr_t end, const std::u8string_view& pattern, const std::u8string_view& mask);
}

#endif // PATTERNS_HPP
