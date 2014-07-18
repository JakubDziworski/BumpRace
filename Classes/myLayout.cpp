#include "myLayout.h"

using namespace cocos2d;
using namespace ui;

void myLayout::autoResizeVertically(const float marginWidth,const float marginHeight)
{
	Size widgetsSize = Size(0, 0);
	for (auto child : (this->getChildren()))
	{
		Widget* widget = (Widget*)child;
		if (widget == NULL) continue;
		widgetsSize.height = widgetsSize.height + widget->getContentSize().height;
		widgetsSize.height = widgetsSize.height + widget->getLayoutParameter()->getMargin().top + widget->getLayoutParameter()->getMargin().bottom;
		const float width = widget->getLayoutParameter()->getMargin().right + widget->getLayoutParameter()->getMargin().left + widget->getContentSize().width;
		if (width > widgetsSize.width) widgetsSize.width = width;
	}
	widgetsSize = widgetsSize + Size(marginWidth, marginHeight);
	this->setContentSize(widgetsSize);
	this->setBackGroundImageCapInsets(Rect(0,0, widgetsSize.width - 20, widgetsSize.height - 20));
	this->setBackGroundImageScale9Enabled(true);
}

void myLayout::autoResizeHorizontally(const float marginWidth, const float marginHeight)
{
	Size widgetsSize = Size(0, 0);
	for (auto child : (this->getChildren()))
	{
		Widget* widget = (Widget*)child;
		if (widget == NULL) continue;
		widgetsSize.width = widgetsSize.width + widget->getContentSize().width;
		widgetsSize.width = widgetsSize.width + widget->getLayoutParameter()->getMargin().left + widget->getLayoutParameter()->getMargin().right;
		const float height = widget->getLayoutParameter()->getMargin().top + widget->getLayoutParameter()->getMargin().bottom + widget->getContentSize().height;
		if (height > widgetsSize.height) widgetsSize.height = height;
	}
	widgetsSize = widgetsSize + Size(marginWidth, marginHeight);
	this->setContentSize(widgetsSize);
	this->setBackGroundImageCapInsets(Rect(0, 0, widgetsSize.width - 20, widgetsSize.height - 20));
	this->setBackGroundImageScale9Enabled(true);
}

bool myLayout::init()
{
	if (!Layout::init())
	{
		return false;
	}
	return true;
}
