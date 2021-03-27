#ifndef SDK_CCAREERGAME_HPP
#define SDK_CCAREERGAME_HPP

#include "../Util/Cenum.hpp"
#include "IBaseInterface.hpp"
#include <vector>

struct MapInfo
{
	const char *name;
	bool defeated;
};


MAKE_CENUM_Q(CareerDifficultyType, int,
	CAREER_DIFFICULTY_EASY, 0,
	CAREER_DIFFICULTY_NORMAL, 1,
	CAREER_DIFFICULTY_HARD, 2,
	CAREER_DIFFICULTY_EXPERT, 3,

	MAX_CAREER_DIFFICULTY, 4,
);

class ICareerTask
{
public:
	virtual bool IsComplete() = 0;
	virtual bool IsCompletedThisRound() = 0;
	virtual bool IsCompletedThisMatch() = 0;
	virtual const char *GetMap() = 0;
	virtual const char *GetTaskName() = 0;
	virtual const wchar_t *GetLocalizedTaskName() = 0;
	virtual const wchar_t *GetLocalizedTaskNameWithCompletion() = 0;
	virtual void Reset() = 0;
	virtual void Set(bool thisRound) = 0;
	virtual void StartRound() = 0;
	virtual const char *GetWeaponName() = 0;
	virtual int GetRepeat() = 0;
	virtual bool MustSurvive() = 0;
	virtual bool InARow() = 0;
	virtual int GetPartial() = 0;
	virtual void SetPartial(int num) = 0;
	virtual bool IsMaxRoundTime() = 0;
};

class CCareerGame : public IBaseInterface
{
public:
	virtual ~CCareerGame() = 0;

	virtual bool IsPlayingMatch() = 0;
	virtual std::vector<ICareerTask*> *GetCurrentTaskVec() = 0;
	virtual bool PlayAsCT() = 0;
	virtual int GetReputationGained() = 0;
	virtual int GetNumMapsUnlocked() = 0;
	virtual bool DoesWinUnlockAll() = 0;
	virtual int GetRoundTimeLength() = 0;
	virtual int GetWinfastLength() = 0;
	virtual CareerDifficultyType GetDifficulty() const = 0;
	virtual int GetCurrentMapTriplet(MapInfo *maps) = 0;
	virtual void OnRoundEndMenuOpen(bool didWin) = 0;
	virtual void OnMatchEndMenuOpen(bool didWin) = 0;
	virtual void OnRoundEndMenuClose(bool stillPlaying) = 0;
	virtual void OnMatchEndMenuClose(bool stillPlaying) = 0;
};

#endif // SDK_CCAREERGAME_HPP
