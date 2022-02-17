#include "Temp.hpp"

bool Temp::parseSharedString()
{
	if (gSharedString == nullptr)
		return false;

	std::string s(gSharedString);

	const auto p1 = s.find(':');
	if (p1 == std::string::npos)
		return false;

	Temp::address = s.substr(0, p1);

	const auto p2 = s.rfind(':');
	if (p1 == p2) // Default port
		Temp::port = 27015, Temp::map = s.substr(p2+1);
	else
	{
		const auto p3 = s.find(':', p1+1);
		try
		{
			Temp::port = std::stoul(s.substr(p1+1, p3-p1-1));
		}
		catch (const std::out_of_range& e)
		{
			return false;
		}
		catch (const std::invalid_argument& e)
		{
			return false;
		}

		Temp::map = s.substr(p3+1);
	}
	
	return true;
}
