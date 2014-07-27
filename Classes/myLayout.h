
#ifndef	__MYLAYOUT_H____
#define __MYLAYOUT_H____
#include "cocos2d.h"
#include "ui/CocosGUI.h"
class myLayout :public cocos2d::ui::Layout
{
private:
	myLayout *child;
	cocos2d::ui::LinearLayoutParameter *linearParam;
	cocos2d::ui::RelativeLayoutParameter *relativeParam;
public:
	void autoResizeVertically(const float marginWidth =0, const float marginHeight=0);
	void autoResizeHorizontally(float marginWidth=0, float marginHeight=0);
	void setType(int i);
	void setMargin(float x,float y);
	void setMargin(const float l, const float t, const float r, const float b);
	void addWidget(cocos2d::ui::Widget* node);
	void addWidgetCustomParam(cocos2d::ui::Widget *node);
	void removeAllWidgets();
	cocos2d::Vector<cocos2d::Node*> getWidgets(){ return child->getChildren(); }
	void disableWidgets();
	void enableWidgets();
	virtual bool init();
	CREATE_FUNC(myLayout);
};
#endif

