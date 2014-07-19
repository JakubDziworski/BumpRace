#ifndef __HUD_H__
#define  __HUD_H__

#include "cocos2d.h"
#include "UI/CocosGUI.h"
#include "Boxx.h"
class Hud : public cocos2d::Layer
{
protected:
	Node *pauseNode;
	Node *gameOverNode;
	void pauseTouchCallback(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType);
	void resumeBtnListener(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType);
	void repeatBtnListener(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType);
	void gotoMenuBtnListener(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType);
public:
	void switchToGameOverInput();
	virtual void gameIsOver();
	virtual bool init();
	virtual void pointsChanged(cocos2d::Vector<Boxx*> *orderedByPointsBoxes);

};
#endif
