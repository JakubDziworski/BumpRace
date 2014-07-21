#include "EndlessHud.h"
USING_NS_CC;
using namespace ui;

bool EndlessHud::init(EndlessWorld* worldd)
{
	if (!Hud::init())
	{
		return false;
	}
	return true;
}

EndlessHud* EndlessHud::create(EndlessWorld* worldd)
{
	EndlessHud *pRet = new EndlessHud();
	if (pRet && pRet->init(worldd))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

void EndlessHud::pointsChanged(cocos2d::Vector<Boxx*> *orderedByPointsBoxes)
{
	
}

void EndlessHud::gameIsOver()
{
}

