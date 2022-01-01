#ifndef ENGINE_PRIMITIVES_CUSTOMIZATION_HPP
#define ENGINE_PRIMITIVES_CUSTOMIZATION_HPP

#include "Resource.hpp"

struct Customization
{
	QBool inUse;         // Is this customization in use;
	Resource resource;   // The resource_t for this customization
	QBool translated;    // Has the raw data been translated into a useable format?  
	                     //  (e.g., raw decal .wad make into texture_t *)
	i32 userData1;       // Customization specific data
	i32 userData2;       // Customization specific data
	void* info;          // Buffer that holds the data structure that references the data (e.g., the cachewad_t)
	void* buffer;        // Buffer that holds the data for the customization (the raw .wad data)
	Customization *next; // Next in chain
};

#endif // ENGINE_PRIMITIVES_CUSTOMIZATION_HPP
