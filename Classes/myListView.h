
#ifndef _MYSCROLL_H__
#define _MYSCROLL_H__
#include "ui/UILayout.h"
#include "ui/UIPageView.h"
#include "cocos2d.h"
class PageViewController : public cocos2d::ui::Layout
{
private:
	cocos2d::ui::PageView *pageview;
public:
	virtual bool init();
	void setControlledpageView(cocos2d::ui::PageView*);
	CREATE_FUNC(PageViewController);
};
#endif
