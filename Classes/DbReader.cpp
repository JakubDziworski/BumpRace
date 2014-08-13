#define COCOS2D_DEBUG 2
#include "DbReader.h"
#include "Paths.h"
USING_NS_CC;
DbReader::DbReader()
{
	db = UserDefault::getInstance();
	CCLOG("%s", db->getXMLFilePath().c_str());
}

DbReader * DbReader::getInstance()
{
	if (me == NULL) me = new DbReader();
	return me;
}

int DbReader::getEndlessBestScore()
{
	return db->getIntegerForKey(R_endlessHighScore.c_str(),0);
}

void DbReader::setEndlessBestScore(const int value)
{
	db->setIntegerForKey(R_endlessHighScore.c_str(), value);
}

void DbReader::unlockLevel(const int val)
{
	String *tmp = String::createWithFormat("%s%d", R_levelCompleted.c_str(), val);
	db->setBoolForKey(tmp->getCString(), true);
}

bool DbReader::isLevelUnlocked(const int val)
{
	if (val == 1) return true;
	String *tmp = String::createWithFormat("%s%d", R_levelCompleted.c_str(), val);
	std::string str = db->getXMLFilePath();
	return db->getBoolForKey(tmp->getCString(), false);
}

void DbReader::unlockBox(const int val)
{
	String *tmp = String::createWithFormat("%s%d", R_boxUnlocked.c_str(), val);
	db->setBoolForKey(tmp->getCString(), true);
}

bool DbReader::isBoxUnlocked(const int val)
{
	if (val == 1) return true;
	String *tmp = String::createWithFormat("%s%d", R_boxUnlocked.c_str(), val);
	return db->getBoolForKey(tmp->getCString(),false);
}

void DbReader::flush()
{
	db->flush();
}

DbReader * DbReader::me = NULL;
