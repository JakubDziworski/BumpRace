#ifndef __HUD_H__
#define  __HUD_H__

#include "cocos2d.h"
#include "UI/CocosGUI.h"
#include "Boxx.h"
#include "World.h"
#include "PowerUp.h"
class myLayout;
class Hud : public cocos2d::Layer
{
protected:
	bool visiblePause = false;
	bool isGameOver;
	Node *pauseNode;
	Node *cocostudioNode;
	Node *gameOverNode;
	Node *scoreTableNode;
	cocos2d::Label *infoNode;
	cocos2d::ui::Button *activatorBtns[4];
	cocos2d::ui::Button *multiBtns[4];
	void pauseTouchCallback(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType);
	void resumeBtnListenerBase(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType);
	void repeatBtnListenerBase(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType);
	void gotoMenuBtnListenerBase(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType);
	void displayNextLevel(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType);
	//TYLKO DZIEDZICZENIE//
	virtual void resumeBtnListenerExtended(){}
	virtual void gotoMenuBtnListenerExtended(){}
	virtual void repeatBtnListenerExtended(){}
	virtual void displayGameIsOverAdditional(bool win){}
	virtual void additionalMulti(int heightY){}
    void addGameOverButtons(bool win,myLayout *parent);
public:
	Node *getCocostudioNode() const { return cocostudioNode; }
	void keyBackClickedHUD();
	virtual void powerUpCollected(PowerUp::PowerUpType type, Boxx* box);
	void setMultiplayer(World *world);
	virtual void lateinit(World *world){}
	virtual void pointsChanged(cocos2d::Vector<Boxx*> *orderedByPointsBoxes){}
	void displayGameOver(bool win);
	virtual bool init();
	void displayInfo(const std::string &stringToDisplay, Boxx* boxabout = NULL);
};
#endif
