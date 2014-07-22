#ifndef __MENU_H__
#define __MENU_H__

#include "cocos2d.h"
#include "UI/CocosGUI.h"

class MyMenu : public cocos2d::Layer
{
private:
	//single
	const float maxOpponentsNumber = 10;
	int currOpponentsNumber;
	int currDiffValue;
	int currModeSelected;
	int currMenu;
	int currGatesNumb;
	int currPlayersNumber;
	//multi
	const float m_maxOpponentsNumber = 2;
	const float m_maxPlayersNumber = 4;
	int m_currOpponentsNumber;
	int m_currDiffValue;
	int m_currModeSelected;
	int m_currGatesNumb;
	int m_currPlayersNumber;
	//dla zmieniania wartosc playerow;
	int currChangingPlayer = 0;
	cocos2d::Size srodek;
	cocos2d::ui::Layout* layout;
	void preload();
	void hide(int menutypedef);
	void show(int menutypedef);
	void createTextEdit(const std::string &text, cocos2d::ui::TextField::ccTextFieldCallback callback, int parenttag, int tag);
	void createPages(const std::string title,const std::vector<const std::string> names, const std::vector<const std::string> filepaths, int defaultState, const int tag, int parent, cocos2d::ui::PageView::ccPageViewCallback callback);
	void createLabel(const std::string &text, int parenttag, int tag);
	void createLayout(int layoutTag);
	void createSlider(const char *defaultText, const float defaultval, const float maxVal, int &changingValue, cocos2d::ui::Slider::ccSliderCallback callback, int parenttag, int tag, int labelTag);
	void createBtn(const std::string &imgOn, const std::string &imgOf, const std::string &btnText, cocos2d::ui::Widget::ccWidgetTouchCallback callback, int typed,cocos2d::Node *layout);
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MyMenu);
	//main menu//
	void playSingleEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
		//single menu//
		void playCarrer(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
		void playCustom(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
			//custom single//
			void playCustomNow(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void playMultiEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
		void m_continueToBoxChoose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void optionsEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	//shared//
	void goBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	//CALLBACKS
	//SINGLE PLAYER SLIDER VALUE CHANGED
	void opponentsSliderChanged(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type);
	void difficultySliderChanged(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type);
	void modeChooserPageChanged(cocos2d::Ref* pSender, cocos2d::ui::PageView::EventType type);
	void gatesSliderChanged(cocos2d::Ref* pSender, cocos2d::ui::Slider::EventType type);
	//MULTIPLAYER SLIDER VALUE CHANGED
	void m_PlayerSliderChanged(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type);
	void m_OpponentsSliderChanged(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type);
	void m_DifficultySliderChanged(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type);
	void m_ModeChooserPageChanged(cocos2d::Ref* pSender, cocos2d::ui::PageView::EventType type);
	void m_GatesSliderChanged(cocos2d::Ref* pSender, cocos2d::ui::Slider::EventType type);
	//wybieranie gracza
	void m_textFieldChanged(cocos2d::Ref *psender, cocos2d::ui::TextField::EventType type);
	void m_pageBoxChosechanged(cocos2d::Ref* pSender, cocos2d::ui::PageView::EventType type);
	void playMultiNow(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};
#endif // !__MENU_H__
