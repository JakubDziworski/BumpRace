#ifndef __dialog__READER__
#define __dialog__READER__
#include "cocos2d.h"
class World;
namespace cocos2d { namespace ui{ class Layout; }}
class DialogReader : public cocos2d::Node
{
private:
	float touchTimeElapsed = 0;
	void touchTimer(float dt);
	static DialogReader* me;
	DialogReader();
	cocos2d::Map<std::string, cocos2d::ui::Layout*> loadedWidgets;
public:
	static DialogReader* getInstance();
	cocos2d::ui::Layout* getMainWidgetFromJson(const std::string &fileName, cocos2d::Node* parent);
	cocos2d::ui::Layout* getHiddenMainWidFromJson(const std::string &fileName, cocos2d::Node* parent);
	void addActionHideAndSomething(const std::string& cocosFileName, const std::string &name, std::function<void()> additionalFeature);
	void addButtonAction(const std::string& cocosFileName, const std::string &name, std::function<void()> additionalFeature);
	cocos2d::Node* getWidget(const std::string &cocosFileName, const std::string &name);
	void getTutorialDialog(const std::string &cocosFileName, cocos2d::Node *parent, World *worldToPause,bool once = true);
	void flush();
};
#endif
