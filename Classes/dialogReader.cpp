#include "dialogReader.h"
#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Globals.h"
#include "soundManager.h"
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
			root->removeFromParent();
			parent->addChild(root);
		}
		else parent->addChild(root);
		auto animIn = cocostudio::ActionManagerEx::getInstance()->getActionByName(fileName.c_str(), "animIn");
		if (animIn) animIn->play();
		return root;
	}
	root = (Layout*)cocostudio::GUIReader::getInstance()->widgetFromJsonFile(fileName.c_str());
	parent->addChild(root);
	loadedWidgets.insert(fileName, root);
	auto animIn = cocostudio::ActionManagerEx::getInstance()->getActionByName(fileName.c_str(), "animIn");
	if (animIn) animIn->play();
	//set correct text font size
	root->enumerateChildren("//.*", [this, fileName](Node *node)
	{
		//IF IS A TEXT
		if (dynamic_cast<Text*>(node))
		{
			Text *nod = (Text*)node;
			nod->setString(G_str(nod->getString()));
		}
		//IF IS A BUTTON
		if (dynamic_cast<Button*>(node))
		{
			Button *nod = (Button*)node;
			nod->setTitleText(G_str(nod->getTitleText()));
			const int size = nod->getName().length();
			//IF IS A CLOSE BUTTON
			if (nod->getName() == "closeBtn" || nod->getName() == "okBtn")
			{
				nod->addTouchEventListener([this, fileName](Ref* reff, Widget::TouchEventType type)
				{
					if (type != Widget::TouchEventType::ENDED) return;
					SoundManager::getInstance()->playBtnEffect();
					auto prevAction = cocostudio::ActionManagerEx::getInstance()->getActionByName(fileName.c_str(), "animIn");
					if (prevAction && prevAction->isPlaying())
						prevAction->stop();
					auto action = cocostudio::ActionManagerEx::getInstance()->getActionByName(fileName.c_str(), "animOut");
					if(action) action->play();
				});
			}
		}
		return false;
	});
	return root;
}
void DialogReader::addActionHideAndSomething(const std::string& cocosFileName, const std::string &name, std::function<void()> additionalFeature)
{
	std::string fullName = "//" + name;
	loadedWidgets.at(cocosFileName)->enumerateChildren(fullName, [cocosFileName, additionalFeature](Node *node)
	{
		auto nod = (Button*)node;
		nod->addTouchEventListener([cocosFileName, additionalFeature](Ref* reff, Widget::TouchEventType type)
		{
			if (type != Widget::TouchEventType::ENDED) return;
			SoundManager::getInstance()->playBtnEffect();
			auto prevAction = cocostudio::ActionManagerEx::getInstance()->getActionByName(cocosFileName.c_str(), "animIn");
			if (prevAction && prevAction->isPlaying()) prevAction->stop();
			auto action = cocostudio::ActionManagerEx::getInstance()->getActionByName(cocosFileName.c_str(), "animOut");
			if(action) action->play();
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
