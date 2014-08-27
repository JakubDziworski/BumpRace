#ifndef __MENU_H__
#define __MENU_H__

#include "cocos2d.h"
#include "UI/CocosGUI.h"

class MyMenu : public cocos2d::Layer
{
private:
	//UI DIALOGS
	cocos2d::ui::Widget *levelLockedDialog = NULL;
	//single
	const float maxOpponentsNumber   = 10;
	const float scaleFactor          = 1.3f;
	const float scaleTime            = 0.2f;
	const float m_maxOpponentsNumber = 2;
	const float m_maxPlayersNumber   = 4;
	int currChangingPlayer           = 0;
	int currOpponentsNumber;
	int currDiffValue;
	int currModeSelected;
	int currMenu;
	int currGatesNumb;
	int currPlayersNumber;
	std::string playerName;
	int playerboxFileNameIndex;
	int m_playersBoxesFileNamesIndexes[4];
	std::string m_playersNames[4];
	//multi
	int m_currOpponentsNumber;
	int m_currDiffValue;
	int m_currModeSelected;
	int m_currGatesNumb;
	int m_currPlayersNumber;
	//dla zmieniania wartosc playerow;
	cocos2d::ui::Layout* layout;
	void preload();
	void hide(int menutypedef);
	void show(int menutypedef);
	void createSpinner(const std::string &defaultText,const std::string &labelText, int &changinVal, int maxVal, int minVal,int tag, int parenttag ,std::function<void(cocos2d::ui::TextBMFont*)> additionalFunction=nullptr);
	cocos2d::ui::TextField* createTextEdit(std::string &text, cocos2d::Color3B textColor, int parenttag, int tag,std::function<void(cocos2d::ui::TextField*)> callback=nullptr);
	cocos2d::ui::PageView* createPages(const std::string title, const std::vector<const std::string> names, const std::vector<const std::string> filepaths, int &defaultState, const int tag, int parent, std::function<void(cocos2d::ui::PageView*)> callback = nullptr);
	void createLabel(const std::string &text, int parenttag, int tag);
	void createLayout(int layoutTag);
	void createSlider(const char *defaultText, const float defaultval, const float maxVal, int &changingValue, cocos2d::ui::Slider::ccSliderCallback callback, int parenttag, int tag, int labelTag);
	void createBtn(const std::string &imgOn, const std::string &imgOf, const std::string &btnText, cocos2d::ui::Widget::ccWidgetTouchCallback callback, int typed,cocos2d::Node *layout);
	void createSinglePlayerTutorialDialog();
	void createLevelMapUI();
	void resizeLayouts();
public:
	virtual void retain();
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
			void playtBestScoreNow(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void playMultiEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
		void m_continueToBoxChoose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void optionsEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	//shared//
	void goBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	//CALLBACKS
	//SINGLE PLAYER SLIDER VALUE CHANGED
	void modeChooserPageChanged(cocos2d::ui::PageView *page);
	void continueToBoxChoose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	//MULTIPLAYER SLIDER VALUE CHANGED
	void m_ModeChooserPageChanged(cocos2d::ui::PageView *page);
	//wybieranie gracza
	void m_textFieldChanged(cocos2d::Ref *psender, cocos2d::ui::TextField::EventType type);
	bool m_checkPlayersOverlap();
	void playMultiNow(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	//additional callbacks
	void m_OpponentsChanged(cocos2d::ui::TextBMFont *textToChange);
	void difficultySpinnerChanged(cocos2d::ui::TextBMFont* textTochange);
	void m_difficultySpinnerChanged(cocos2d::ui::TextBMFont* textTochange);
	void m_autocorrectWrongPlayerChoose();
	void goToLevelChooserMenu();
	//SETUPO
	void m_setupAutoCorrectDialog(cocos2d::ui::Layout *root);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	
};
#endif // !__MENU_H__
