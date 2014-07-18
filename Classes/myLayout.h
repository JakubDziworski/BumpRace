
#ifndef	__MYLAYOUT_H____
#define __MYLAYOUT_H____
#include "cocos2d.h"
#include "ui/CocosGUI.h"
class myLayout :public cocos2d::ui::Layout
{
public:
	void autoResizeVertically(const float marginWidth =0, const float marginHeight=0);
	void autoResizeHorizontally(float marginWidth=0, float marginHeight=0);
	virtual bool init();
	CREATE_FUNC(myLayout);
};
#endif

