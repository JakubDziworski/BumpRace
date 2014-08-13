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
	void flush();
};
#endif

