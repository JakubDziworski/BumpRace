#ifndef __MENU_H__
#define __MENU_H__

#include "cocos2d.h"
#include "UI/CocosGUI.h"

class MyMenu : public cocos2d::Layer
{
private:
	const float maxOpponentsNumber = 10;
	int currOpponentsNumber;
	int currDiffValue;
	int currModeSelected;
	int currMenu;
	cocos2d::Size srodek;
	cocos2d::ui::Layout* layout;
	void preload();
	void hide(int menutypedef);
	void show(int menutypedef);
	void createPages(const std::string title,const std::vector<const std::string> names, const std::vector<const std::string> filepaths, int defaultState, const int tag, int parent, cocos2d::ui::PageView::ccPageViewCallback callback);
	void createLabel(const std::string &text, int parenttag, int tag);
	void createLayout(int layoutTag);
	void createSlider(const std::string &defaultText, const float defaultval, const float maxVal, int &changingValue, cocos2d::ui::Slider::ccSliderCallback callback, int parenttag, int tag, int labelTag);
	void createBtn(const std::string &imgOn, const std::string &imgOf, const std::string &btnText, cocos2d::ui::Widget::ccWidgetTouchCallback callback, int typed,cocos2d::Node *layout);
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MyMenu);
	//main menu//
	void playSingleEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
		//single menu//
		void playCarrer(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
		void playElimination(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
		void playEndless(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
		void playCustom(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
			//custom single//
			void playCustomNow(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void playMultiEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void optionsEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	//shared//
	void goBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	//CALLBACKS
	//SLIDER VALUE CHANGED
	void opponentsSliderChanged(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type);
	void difficultySliderChanged(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type);
	void modeChooserPageChanged(cocos2d::Ref* pSender, cocos2d::ui::PageView::EventType type);
};
#endif // !__MENU_H__
