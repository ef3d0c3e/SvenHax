#include "Maps.hpp"
#include <linux/limits.h>
#include <fstream>
#include <cstring>

void Maps::ParseMaps(std::function<void(MapEntry&&)> callback)
{
	std::ifstream file("/proc/self/maps");
	if (!file.is_open())
		throw Exception("Maps::ParseMaps() Could not open /proc/self/maps");

	std::string line;
	while (std::getline(file, line))
	{
		MapEntry ent;

		// Address
		const auto pos1 = line.find('-');
		if (pos1 == std::string::npos)
			throw Exception("Maps::ParseMaps() Parse error while parsing begin address");
		try
		{
			std::size_t n = 0;
			ent.address = std::stoll(line.substr(0, pos1), &n, 16);
		}
		catch (std::invalid_argument& e)
		{
			throw Exception("Maps::ParseMaps() Begin address could not be converted");
		}
		catch (std::out_of_range& e)
		{
			throw Exception("Maps::ParseMaps() Begin address is out of range");
		}

		// Size
		const auto pos2 = line.find(' ', pos1);
		if (pos2 == std::string::npos)
			throw Exception("Maps::ParseMaps() Parse error while parsing end address");
		try
		{
			std::size_t n = 0;
			ent.size = std::stoll(line.substr(pos1+1, pos2-pos1-1), &n, 16) - ent.address;
		}
		catch (std::invalid_argument& e)
		{
			throw Exception("Maps::ParseMaps() End address could not be converted");
		}
		catch (std::out_of_range& e)
		{
			throw Exception("Maps::ParseMaps() End address is out of range");
		}

		// Permissions
		if (line.size() <= pos2+6)
			throw Exception("Maps::ParseMaps() Parse error while parsing permissions");
		if (line[pos2+1] == 'r')
			ent.perm |= Permissions::READ;
		if (line[pos2+2] == 'w')
			ent.perm |= Permissions::WRITE;
		if (line[pos2+3] == 'x')
			ent.perm |= Permissions::EXECUTE;
		if (line[pos2+4] == 'p')
			ent.perm |= Permissions::PRIVATE;

		// Offsets
		const auto pos3 = pos2+6;
		const auto pos4 = line.find(' ', pos3);
		if (pos4 == std::string::npos)
			throw Exception("Maps::ParseMaps() Parse error while parsing offset");
		try
		{
			std::size_t n = 0;
			ent.offset = std::stoll(line.substr(pos3, pos4-pos3), &n, 16);
		}
		catch (std::invalid_argument& e)
		{
			throw Exception("Maps::ParseMaps() The offset could not be converted");
		}
		catch (std::out_of_range& e)
		{
			throw Exception("Maps::ParseMaps() The offset is out of range");
		}

		// Dev
		const auto pos5 = line.find(' ', pos4+1);
		if (pos5 == std::string::npos)
			throw Exception("Maps::ParseMaps() Parse error while parsing dev");
		std::strncpy(ent.dev, line.substr(pos4+1, pos5-pos4-1).c_str(), pos5-pos4); //+1 to copy \x00

		// Inode
		const auto pos6 = line.find(' ', pos5+1);
		if (pos6 == std::string::npos)
			throw Exception("Maps::ParseMaps() Parse error while parsing inode");
		try
		{
			std::size_t n = 0;
			ent.inode = std::stoi(line.substr(pos5+1, pos6-pos5-1), &n, 10);
		}
		catch (std::invalid_argument& e)
		{
			throw Exception("Maps::ParseMaps() The inode could not be converted");
		}
		catch (std::out_of_range& e)
		{
			throw Exception("Maps::ParseMaps() The inode is out of range");
		}

		// Path
		auto i = pos6+1;
		while (line[i] == '\t' || line[i] == ' ')
			++i;
		const auto orig = i;
		ent.path.resize(line.size() - orig, '\x00');
		while (i < line.size() && line[i] != '\t' /*&& line[i] != ' '*/)
			ent.path[i-orig] = line[i], ++i;

		callback(std::move(ent));
	}
}
