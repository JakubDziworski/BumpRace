#ifndef __DBREADER__
#define __DBREADER__
#include "cocos2d.h"
class DbReader
{
private:
	cocos2d::UserDefault *db;
	DbReader();
	static DbReader *me;
public:
	static DbReader *getInstance();
	//ENDLESS
	int getEndlessBestScore();
	void setEndlessBestScore(const int value);
	//UNLOCKED / LOCKED / CARRER
	void unlockLevel(const int val);
	bool isLevelUnlocked(const int val);
	void unlockBox(const int val);
	bool isBoxUnlocked(const int val);
	void setPlayerDefaultName(const int plyr, const std::string &name);
	void initPlayersDefaultNames();
	void flush();
	bool isTutorialCmpltd(const std::string &tutorialName);
	void setTutorialCmpltd(const std::string &tutorialName);
	void setTutorialsEnabled(bool vl);
	bool areBasicTutorialsCompleted();
};
#endif

