#include "myLayout.h"
#include "Globals.h"
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

void myLayout::setType(int i)
{
	child = myLayout::create();
	if (i == 0)
	{
		child->setLayoutType(Layout::Type::VERTICAL);
		linearParam = LinearLayoutParameter::create();
		linearParam->setGravity(LinearGravity::CENTER_HORIZONTAL);
	}
	else 
	{
		child->setLayoutType(Layout::Type::HORIZONTAL);
		linearParam = LinearLayoutParameter::create();
		linearParam->setGravity(LinearGravity::CENTER_VERTICAL);
	}
	this->setLayoutType(Layout::Type::RELATIVE);
	relativeParam = RelativeLayoutParameter::create();
	relativeParam->setAlign(RelativeAlign::CENTER_IN_PARENT);
	child->setLayoutParameter(relativeParam);
	this->addChild(child);
}

void myLayout::setMargin(float x, float y)
{
	relativeParam->setMargin(Margin(x / 2, y / 2, x / 2, y / 2));
	child->setLayoutParameter(relativeParam);
}

void myLayout::setMargin(const float l, const float t, const float r, const float b)
{
	relativeParam->setMargin(Margin(l, t, r, b));
}

void myLayout::addWidget(cocos2d::ui::Widget* node)
{
	node->setLayoutParameter(linearParam);
	child->addChild(node);
	if (child->getLayoutType() == Layout::Type::VERTICAL)
	{
		child->autoResizeVertically();
		this->autoResizeVertically();
	}
	else
	{
		child->autoResizeHorizontally();
		this->autoResizeHorizontally();
	}
}

void myLayout::addWidgetCustomParam(cocos2d::ui::Widget *node)
{
	child->addChild(node);
	if (child->getLayoutType() == Layout::Type::VERTICAL)
	{
		child->autoResizeVertically();
		this->autoResizeVertically();
	}
	else
	{
		child->autoResizeHorizontally();
		this->autoResizeHorizontally();
	}
}

void myLayout::disableWidgets()
{

		this->setColor(Color3B(100, 100, 100));
		this->setEnabled(false);
}

void myLayout::enableWidgets()
{
	this->setColor(Color3B::WHITE);
	this->setEnabled(true);
}

void myLayout::removeAllWidgets()
{
	child->removeAllChildren();
}
