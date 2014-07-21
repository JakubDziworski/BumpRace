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
	void resumeBtnListener(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType);
	void repeatBtnListener(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType);
	void gotoMenuBtnListener(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType);
	virtual void gameIsOver();
public:
	void displayGameOver();
	virtual bool init();
	virtual void pointsChanged(cocos2d::Vector<Boxx*> *orderedByPointsBoxes);
	void displayInfo(const std::string &stringToDisplay);
};
#endif
