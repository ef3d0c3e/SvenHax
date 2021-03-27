#ifndef SDK_CSCHEMEMANAGER_HPP
#define SDK_CSCHEMEMANAGER_HPP

#include "IBaseInterface.hpp"

typedef unsigned long HScheme;
typedef unsigned long HTexture;
class IScheme;
class IImage;

class CSchemeManager : public IBaseInterface
{
public:
	virtual ~CSchemeManager() = 0;

	// loads a scheme from a file
	// first scheme loaded becomes the default scheme, and all subsequent loaded scheme are derivitives of that
	virtual HScheme LoadSchemeFromFile(const char *fileName, const char *tag) = 0;

	// reloads the scheme from the file - should only be used during development
	virtual void ReloadSchemes() = 0;

	// returns a handle to the default (first loaded) scheme
	virtual HScheme GetDefaultScheme() = 0;

	// returns a handle to the scheme identified by "tag"
	virtual HScheme GetScheme(const char *tag) = 0;

	// returns a pointer to an image
	//virtual IImage *GetImage(const char *imageName, bool hardwareFiltered = false) = 0;
	virtual HTexture GetImageID(const char *imageName, bool hardwareFiltered = false) = 0;

	// This can only be called at certain times, like during paint()
	// It will assert-fail if you call it at the wrong time...

	// FIXME: This interface should go away!!! It's an icky back-door
	// If you're using this interface, try instead to cache off the information
	// in ApplySchemeSettings
	virtual IScheme *GetIScheme(HScheme scheme) = 0;

	// unload all schemes
	virtual void Shutdown(bool full = true) = 0;

	// gets the proportional coordinates for doing screen-size independant panel layouts
	// use these for font, image and panel size scaling (they all use the pixel height of the display for scaling)
	virtual int GetProportionalScaledValue(int normalizedValue) = 0;
	virtual int GetProportionalNormalizedValue(int scaledValue) = 0;
};

#endif // SDK_CSCHEMEMANAGER_HPP
