#include "Hud.h"
#include "Macros.h"
#include "SingleGateWorld.h"
#include "Globals.h"
USING_NS_CC;
using namespace ui;
bool Hud::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Button* pauseBtn = Button::create("pauseBtnOn.png", "pauseBtnOf.png");
	pauseBtn->setPosition(Vec2(2 * G_srodek.x - pauseBtn->getContentSize().height*0.75f, 2 * G_srodek.y - pauseBtn->getContentSize().width*0.75f));
	pauseBtn->addTouchEventListener(CC_CALLBACK_2(Hud::pauseTouchCallback,this));
	this->addChild(pauseBtn);
	return true;
}

void Hud::pauseTouchCallback(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType)
{
	CCLOG("pauseClicked");
}

void Hud::pointsChanged(cocos2d::Vector<Boxx*> *orderedByPointsBoxes)
{

}

