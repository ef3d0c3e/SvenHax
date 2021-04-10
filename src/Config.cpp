#include "Config.hpp"
#include "Util/Util.hpp"
#include <fstream>
#include <cstring>
#include <wordexp.h>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std::literals;

std::map<std::string, Config::Var> Config::varList;
static constexpr std::string_view ConfigDir = "$HOME/.config/CunnyWare";

static bool FileExists(const char* path)
{
	struct stat st;
	return stat(path, &st) == 0 && S_ISREG(st.st_mode);
}

static bool DirExists(const char* path)
{
	struct stat st;
	return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

static std::string Resolve(std::string path)
{
	for (std::size_t i = 0; i < path.size(); ++i)
	{
		if (path[i] == ' ')
		{
			path.insert(i, 1, '\\');
			++i;
		}
	}

	wordexp_t p;
	if (wordexp(path.c_str(), &p, 0) != 0)
		throw Exception("Resolve(): wordexp() failed");
	std::string expanded = "";
	for (std::size_t i = 0; i < p.we_wordc; ++i)
		expanded += p.we_wordv[i];
	wordfree(&p);

	return expanded;
}

static std::vector<std::string> Split(const std::string& path, char delim)
{
	std::vector<std::string> words;

	std::size_t pos, lastpos = 0;
	pos = path.find(delim, lastpos);
	while (pos != std::string::npos)
	{
		words.push_back(path.substr(pos, lastpos-pos));

		lastpos = pos + 1;
		pos = path.find(delim, lastpos);
	}

	return words;
}

static std::ofstream CreateFileR(const std::string& path, const std::string& filename)
{
	const auto dirs = Split(Resolve(path), '/');

	std::string cur = "/";
	for (auto dir : dirs)
	{
		cur += dir + "/";
		if (DirExists(dir.c_str()))
			continue;

		if (mkdir(cur.c_str(),  0b111111010) != 0) // 774
			throw Exception("CreateFileR() mkdir() failed to create '{}'", cur);
	}

	return std::ofstream(cur + filename, std::ios::binary);
}

static std::ifstream OpenFile(const std::string& path, const std::string& filename)
{
	const auto file = Resolve(path) + "/" + filename;

	if (FileExists(file.c_str()))
		throw Exception("OpenFile(): Tried to open non existing file '{}'", file);

	return std::ifstream(file);
}

void Config::Load(const std::string& name)
{
	auto file = OpenFile(std::string(ConfigDir), name);
	const std::string content( (std::istreambuf_iterator<char>(file)),
		(std::istreambuf_iterator<char>()) );
	file.close();

	std::size_t i = 0;
	while (i != content.size())
	{
		std::size_t ilen; // Identifier length
		std::size_t dlen; // Data length
		std::string identifier;
		// Type info
		{
			std::size_t pos = content.find(' ', i);
			if (pos == std::string::npos || pos-i == 0)
				throw Exception("Config::Load(): Could not get type info for file '{}' at position '{}'", name, i);
			std::string type = content.substr(i, pos-i);

			if (type == "B")
				dlen = sizeof(bool);
			else if (type == "I")
				dlen = sizeof(i32);
			else if (type == "F")
				dlen = sizeof(f32);
			else if (type == "IV4")
				dlen = sizeof(ImVec4);
			else if (type == "S")
			{
				try
				{
					dlen = std::stoi(content.substr(i+1, i-pos-1));
				}
				catch (std::invalid_argument& e)
				{
					throw Exception("Config::Load(): Could not get string length for file '{}' at position '{}' (invalid argument) '{}'", name, i, e.what());
				}
				catch (std::out_of_range& e)
				{
					throw Exception("Config::Load(): Could not get string length for file '{}' at position '{}' (out of range) '{}'", name, i, e.what());
				}
			}

			i = pos+1;
		}
		//Identifier length
		{
			std::size_t pos = content.find(' ', i);
			if (pos == std::string::npos || pos-i == 0)
				throw Exception("Config::Load(): Could not get identifier length for file '{}' at position '{}'", name, i);
			std::string type = content.substr(i, pos-i);

			try
			{
				ilen = std::stoi(content.substr(i+1, i-pos-1));
			}
			catch (std::invalid_argument& e)
			{
				throw Exception("Config::Load(): Could not get identifier length for file '{}' at position '{}' (invalid argument) '{}'", name, i, e.what());
			}
			catch (std::out_of_range& e)
			{
				throw Exception("Config::Load(): Could not get identifier length for file '{}' at position '{}' (out of range) '{}'", name, i, e.what());
			}

			i = pos+1;
		}
		//Identifier
		{
			if (i+ilen+1 >= content.size())
				throw Exception("Config::Load(): Could not get identifier for file '{}' at position '{}'", name, i);

			identifier = content.substr(i, ilen);

			i += ilen+1;
		}
		//Data
		{
			if (i+dlen >= content.size())
				throw Exception("Config::Load(): Could not get data for file '{}' at position '{}'", name, i);


			std::memcpy(reinterpret_cast<void*>(&varList[identifier]), reinterpret_cast<const void*>(&content.data()[i]), dlen);

			i += dlen;
		}

		++i;
	}
}

void Config::Save(const std::string& name)
{
	/*
	 * <type>.<Nid><value>
	 * eg: B 17 misc.bhop.enabled 1 // for 'bool Settings::BHop::enabled = true'
	 * S9 12 clantag.name MyClanTag // '9' is the length of the string
	*/

	auto GetType = [&](const Var& v) -> std::string
	{
		const static std::array<std::string, Var::Type::size> typeArray
		{
			"B"s, // bool
			"I"s, // i32
			"F"s, // f32
			"IV4"s, // ImVec4
			"S"s, // char*
		};

		if (v.type == Var::STRING)
			return typeArray[v.type] + std::to_string(strlen(reinterpret_cast<const char*>(v.p)));

		return typeArray[v.type];
	};

	auto file = CreateFileR(std::string(ConfigDir), name);

	for (const auto& e : varList)
	{
		const auto type = GetType(e.second);
		file.write(type.data(), type.size()); // type info
		file.put(' ');
		file << e.first.size(); // size of identifier
		file.put(' ');
		file.write(e.first.data(), e.first.size()); // identifier
		file.put(' ');
		switch (e.second.type)
		{
			case Var::BOOL:
				file.put(*reinterpret_cast<const char(&)[sizeof(bool)]>(e.second.p));
				break;
			case Var::I32:
				file.write(reinterpret_cast<const char(&)[sizeof(i32)]>(e.second.p), sizeof(i32));
				break;
			case Var::F32:
				file.write(reinterpret_cast<const char(&)[sizeof(f32)]>(e.second.p), sizeof(f32));
				break;
			case Var::IMVEC4:
				file.write(reinterpret_cast<const char(&)[sizeof(ImVec4)]>(e.second.p), sizeof(ImVec4));
				break;
			case Var::STRING:
				file.write(reinterpret_cast<const char*>(e.second.p), strlen(reinterpret_cast<const char*>(e.second.p)));
				break;
			default:
				break;
		}
		file.put('\n');
	}

	file.close();
}
