#ifndef __HUD_H__
#define  __HUD_H__

#include "cocos2d.h"
#include "UI/CocosGUI.h"
#include "Boxx.h"
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
	virtual void resumeBtnListenerExtended(){}
	virtual void gotoMenuBtnListenerExtended(){}
	virtual void repeatBtnListenerExtended(){}
	virtual void gameIsOver();
public:
	void displayGameOver();
	virtual bool init();
	virtual void pointsChanged(cocos2d::Vector<Boxx*> *orderedByPointsBoxes);
	void displayInfo(const std::string &stringToDisplay);
};
#endif