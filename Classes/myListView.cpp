#include "myListView.h"
#include "Globals.h"
USING_NS_CC;
using namespace ui;
bool PageViewController::init()
{
	if (!Layout::init())
	{
		return false;
	}
	pageview = NULL;
	return true;
}

void PageViewController::setControlledpageView(PageView *pgv)
{
	pageview = pgv;
	pageview->setTouchEnabled(false);
	this->setAnchorPoint(Vec2(0.5, 0));
	this->setSize(Size(8*G_srodek.x,pgv->getSize().height));
	pageview->addChild(this);
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](Touch* touch, Event* eventt)
	{
		return this->Layout::onTouchBegan(touch, eventt);
	};
	listener->onTouchMoved = [this](Touch* touch, Event* eventt)
	{
		if (pageview)
		{
			pageview->onTouchMoved(touch, eventt);
		}
	};
	listener->onTouchCancelled = [this](Touch* touch, Event* eventt)
	{
		if (pageview)
		{
			pageview->onTouchCancelled(touch, eventt);
		}
	};
	listener->onTouchEnded = [this](Touch* touch, Event* eventt)
	{
		if (pageview)
		{
			pageview->onTouchEnded(touch, eventt);
		}
	};
	G_dir()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

