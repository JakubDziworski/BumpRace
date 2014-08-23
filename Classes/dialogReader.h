#ifndef __dialog__READER__
#define __dialog__READER__
#include "cocos2d.h"
namespace cocos2d { namespace ui{ class Layout; }}
class DialogReader : public cocos2d::Node
{
private:
	static DialogReader* me;
	DialogReader();
	cocos2d::Map<std::string, cocos2d::ui::Layout*> loadedWidgets;
public:
	static DialogReader* getInstance();
	cocos2d::ui::Layout* getMainWidgetFromJson(const std::string &fileName, cocos2d::Node* parent);
	void addActionHideAndSomething(const std::string& cocosFileName, const std::string &name, std::function<void()> additionalFeature);
	cocos2d::Node* getWidget(const std::string &cocosFileName, const std::string &name);
	void flush();
};
#endif
