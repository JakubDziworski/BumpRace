#include "dialogReader.h"
#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Globals.h"
#include "soundManager.h"
#include "myListView.h"
#include "Paths.h"
#include "World.h"
#include "DbReader.h"
USING_NS_CC;
using namespace ui;
DialogReader::DialogReader()
{
}
DialogReader* DialogReader::me = NULL;
DialogReader* DialogReader::getInstance()
{
	if (me == NULL)
	{
		me = new DialogReader();
		me->retain();
	}
	return me;
}
cocos2d::ui::Layout* DialogReader::getMainWidgetFromJson(const std::string &fileName, cocos2d::Node* parent)
{
	Layout* root=NULL;
	if (root = loadedWidgets.at(fileName))
	{
		if (root->getParent() != nullptr || root->getParent() != NULL)
		{
			if (parent)
			{
				root->removeFromParent();
				parent->addChild(root);
			}
		}
		else if(parent) parent->addChild(root);
		auto animIn = cocostudio::ActionManagerEx::getInstance()->getActionByName(fileName.c_str(), "animIn");
		if (animIn)
		{
			SoundManager::getInstance()->playEffect(R_popUp);
			animIn->play();
			root->setTouchEnabled(true);
		}
		return root;
	}
	root = (Layout*)cocostudio::GUIReader::getInstance()->widgetFromJsonFile(fileName.c_str());
	if (parent) parent->addChild(root);
	loadedWidgets.insert(fileName, root);
	auto animIn = cocostudio::ActionManagerEx::getInstance()->getActionByName(fileName.c_str(), "animIn");
	if (animIn) animIn->play();
	//set correct text font size
	root->enumerateChildren("//.*", [this, fileName,root](Node *node)
	{
		//IF IS A TEXT
		if (dynamic_cast<Text*>(node))
		{
			Text *nod = (Text*)node;
			G_enableShadow(nod);
			nod->setString(G_str(nod->getString()));
		}
		//IF IS A BUTTON
		else if (dynamic_cast<Button*>(node))
		{
			Button *nod = (Button*)node;
			nod->setTitleText(G_str(nod->getTitleText()));
			const int size = nod->getName().length();
			//IF IS A CLOSE BUTTON
			if (nod->getName() == "closeBtn" || nod->getName() == "okBtn")
			{
				nod->addTouchEventListener([this, fileName,root](Ref* reff, Widget::TouchEventType type)
				{
					if (type != Widget::TouchEventType::ENDED) return;
					SoundManager::getInstance()->playBtnEffect();
					auto prevAction = cocostudio::ActionManagerEx::getInstance()->getActionByName(fileName.c_str(), "animIn");
					if (prevAction && prevAction->isPlaying())
						prevAction->stop();
					auto action = cocostudio::ActionManagerEx::getInstance()->getActionByName(fileName.c_str(), "animOut");
					if (action)
					{
						action->play();
						root->setTouchEnabled(false);
					}
				});
			}
		}
		else if (dynamic_cast<PageView*>(node))
		{
			PageView *nod = (PageView*)node;
			PageViewController *ctrl = PageViewController::create();
			ctrl->setControlledpageView(nod);
		}
		return false;
	});
	return root;
}
void DialogReader::addActionHideAndSomething(const std::string& cocosFileName, const std::string &name, std::function<void()> additionalFeature)
{
	std::string fullName = "//" + name;
	auto root = getMainWidgetFromJson(cocosFileName,NULL);
	loadedWidgets.at(cocosFileName)->enumerateChildren(fullName, [cocosFileName, additionalFeature,root](Node *node)
	{
		auto nod = (Button*)node;
		nod->addTouchEventListener([cocosFileName, additionalFeature,root](Ref* reff, Widget::TouchEventType type)
		{
			if (type != Widget::TouchEventType::ENDED) return;
			SoundManager::getInstance()->playBtnEffect();
			auto prevAction = cocostudio::ActionManagerEx::getInstance()->getActionByName(cocosFileName.c_str(), "animIn");
			if (prevAction && prevAction->isPlaying()) prevAction->stop();
			auto action = cocostudio::ActionManagerEx::getInstance()->getActionByName(cocosFileName.c_str(), "animOut");
			if (action)
			{
				action->play();
				root->setTouchEnabled(false);
			}
			additionalFeature();
		});
		return true;
	});
}
cocos2d::Node* DialogReader::getWidget(const std::string &cocosFileName, const std::string &name)
{
	std::string fullName = "//"+name;
	Node *returnNode=NULL;
	loadedWidgets.at(cocosFileName)->enumerateChildren(fullName, [&returnNode](Node *node)
		{
			returnNode = node;
			return true; //terminate
		});
	return returnNode;
}
void DialogReader::flush()
{
	loadedWidgets.clear();
	cocostudio::ActionManagerEx::getInstance()->releaseActions();
}
void DialogReader::addButtonAction(const std::string& cocosFileName, const std::string &name, std::function<void()> additionalFeature)
{
	((Button*)getWidget(cocosFileName, name))->addTouchEventListener([additionalFeature](Ref* reff, Button::TouchEventType type)
	{
		if (type != Button::TouchEventType::ENDED) return;
		SoundManager::getInstance()->playBtnEffect();
		additionalFeature();
	});
}
cocos2d::ui::Layout* DialogReader::getHiddenMainWidFromJson(const std::string &fileName, cocos2d::Node* parent)
{
	auto ret = getMainWidgetFromJson(fileName, parent);
	ret->setVisible(false);
	ret->setOpacity(0);
	return ret;
}
void DialogReader::getTutorialDialog(const std::string &cocosFileName, cocos2d::Node *parent, World *worldToPause)
{
	if (DbReader::getInstance()->isTutorialCmpltd(cocosFileName)) return;
	Director::getInstance()->getScheduler()->setTimeScale(1);
	auto main = me->getMainWidgetFromJson(cocosFileName, parent);
	worldToPause->setIsDisplayingTutorial(true);
	cocos2d::Vector<Widget*> widgets;

	Widget* widget = NULL;
	int i = 1;
	do
	{
		widget = (Widget*)me->getWidget(cocosFileName, ".*" + to_string(i));
		if (widget != NULL)
		{
			//widget->setAnchorPoint(Point(0.5f, 0.5f));
			widgets.pushBack(widget);
			if (i != 1)widget->setScale(0);
			else widget->setScale(1);
		}
		i++;
	} while (widget != NULL);
	int touches = 0;
	((Button*)me->getWidget(cocosFileName, "continueBtn"))->addTouchEventListener([worldToPause, widgets, touches, main,cocosFileName](Ref* reff, Widget::TouchEventType type) mutable
	{
		if (type != Widget::TouchEventType::ENDED) return;
		SoundManager::getInstance()->playBtnEffect();
		if (widgets.size() == touches) return;
		widgets.at(touches)->runAction(ScaleTo::create(0.3f, 0));
		touches++;
		if (widgets.size() > touches)
			widgets.at(touches)->runAction(Sequence::createWithTwoActions(DelayTime::create(0.3f),ScaleTo::create(0.3f, 1)));
		else
		{
			worldToPause->setIsDisplayingTutorial(false);
			auto prevAction = cocostudio::ActionManagerEx::getInstance()->getActionByName(cocosFileName.c_str(), "animIn");
			if (prevAction && prevAction->isPlaying()) prevAction->stop();
			auto action = cocostudio::ActionManagerEx::getInstance()->getActionByName(cocosFileName.c_str(), "animOut");
			if (action)
			{
				action->play();
				main->setTouchEnabled(false);
			}
			DbReader::getInstance()->setTutorialCmpltd(cocosFileName);
		}
	});
}
