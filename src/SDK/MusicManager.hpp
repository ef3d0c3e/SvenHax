#ifndef SDK_MUSICMANAGER_HPP
#define SDK_MUSICMANAGER_HPP

#include "IBaseInterface.hpp"
typedef int TrackType;

class MusicManager : public IBaseInterface
{
public:
	virtual ~MusicManager() = 0;

	virtual void StartTrack(TrackType track, bool looping) = 0;
	virtual void StopTrack() = 0;
	virtual void FadeOutTrack() = 0;
	virtual bool IsPlayingTrack() const = 0;
	virtual bool IsPlayingTrack(TrackType track) const = 0;
};

#endif // SDK_MUSICMANAGER_HPP
