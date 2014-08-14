#ifndef __HUD_H__
#define  __HUD_H__

#include "cocos2d.h"
#include "UI/CocosGUI.h"
#include "Boxx.h"
#include "World.h"
#include "PowerUp.h"
class Hud : public cocos2d::Layer
{
protected:
	bool isGameOver;
	Node *pauseNode;
	Node *gameOverNode;
	cocos2d::Label *infoNode;
	void pauseTouchCallback(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType);
	void resumeBtnListenerBase(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType);
	void repeatBtnListenerBase(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType);
	void gotoMenuBtnListenerBase(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType);
	//TYLKO DZIEDZICZENIE//
	virtual void resumeBtnListenerExtended(){}
	virtual void gotoMenuBtnListenerExtended(){}
	virtual void repeatBtnListenerExtended(){}
	virtual void displayGameIsOverAdditional(bool win){}
	virtual void additionalMulti(int heightY){}
public:
	virtual void powerUpCollected(PowerUp::PowerUpType type, Boxx* box);
	void setMultiplayer(World *world);
	virtual void lateinit(World *world){}
	virtual void pointsChanged(cocos2d::Vector<Boxx*> *orderedByPointsBoxes){}
	void displayGameOver(bool win);
	virtual bool init();
	void displayInfo(const std::string &stringToDisplay, Boxx* boxabout = NULL);
};
#endif
