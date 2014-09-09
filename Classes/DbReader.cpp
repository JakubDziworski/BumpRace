#define COCOS2D_DEBUG 2
#include "DbReader.h"
#include "Paths.h"
#include "MyMenu.h"
#include "Globals.h"
#include "Macros.h"
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
void DbReader::initPlayersDefaultNames()
{
	G_playersDefaultNames[0] = db->getStringForKey("Player1Name",(G_str("Player")+std::to_string(1)).c_str());
    G_playersDefaultNames[1] = db->getStringForKey("Player2Name",(G_str("Player")+std::to_string(2)).c_str());
    G_playersDefaultNames[2] = db->getStringForKey("Player3Name",(G_str("Player")+std::to_string(3)).c_str());
    G_playersDefaultNames[3] = db->getStringForKey("Player4Name",(G_str("Player")+std::to_string(4)).c_str());
}
void DbReader::setPlayerDefaultName(const int plyr, const std::string &name)
{
	db->setStringForKey(("Player"+std::to_string(plyr)+"Name").c_str(),name.c_str());
	G_playersDefaultNames[plyr - 1] = name;
    MyMenu *menu;
    if (G_dir()->getRunningScene() && (menu = dynamic_cast<MyMenu*>(G_dir()->getRunningScene()->getChildByTag(LAYER_HUD)))) //menu
    {
        menu->UPDATEPLAYERNAME();
    }
}
bool DbReader::isTutorialCmpltd(const std::string &tutorialName)
{
	if (db->getBoolForKey("TUTORIALS_ENABLED", true))
	{
		return db->getBoolForKey(tutorialName.c_str(), false); //tut completed
	}
	return true;
}
void DbReader::setTutorialCmpltd(const std::string &tutorialName)
{
	db->setBoolForKey(tutorialName.c_str(), true);
}
bool DbReader::areBasicTutorialsCompleted()
{
	return isTutorialCmpltd("firstTut.json");
}
void DbReader::setTutorialsEnabled(bool vl)
{
	db->setBoolForKey("TUTORIALS_ENABLED", vl);
}

bool DbReader::isRatedOrLiked()
{
	return db->getBoolForKey("RATEDORLIKED", false);
}

void DbReader::setRatedOrLiked(bool val)
{
	db->setBoolForKey("RATEDORLIKED", val);
}

bool DbReader::areAdsEnabled()
{
	db->getBoolForKey("ADSENABLED", true);
}

void DbReader::setAdsEnabled(bool val)
{
	db->setBoolForKey("ADSENABLED", val);
}

DbReader * DbReader::me = NULL;
