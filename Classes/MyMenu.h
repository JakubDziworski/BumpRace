#ifndef __MENU_H__
#define __MENU_H__

#include "cocos2d.h"
#include "UI/CocosGUI.h"
#include "extensions/GUI/CCEditBox/CCEditBox.h"
#include "DbReader.h"
class DialogReader;
class TextHandler : public cocos2d::extension::EditBoxDelegate
{
	std::string *txt;
    int pnum;
public:
	TextHandler(std::string &textToEdit,int pNumber)
	{
		txt = &textToEdit;
        pnum = pNumber;
	}
	virtual void editBoxReturn(cocos2d::extension::EditBox* editBox)
	{
		*txt = editBox->getText();
        DbReader::getInstance()->setPlayerDefaultName(pnum,*txt);
	}
};
class MyMenu : public cocos2d::Layer
{
private:
	enum singlePlayerType {best,carrer,free};
	singlePlayerType spType;
	//UI DIALOGS
	cocos2d::Node *cocoStudioNode;
	cocos2d::ui::Widget *levelLockedDialog = NULL;
	//single
	bool tryBestScore = false;
	const float maxOpponentsNumber   = 10;
	const float scaleFactor          = 1.3f;
	const float scaleTime            = 0.2f;
	const float m_maxOpponentsNumber = 2;
	const float m_maxPlayersNumber   = 4;
	int currChangingPlayer           = 0;
	int curCarrerLevelChose = 1;
	int currOpponentsNumber;
	int currDiffValue;
	int currModeSelected;
	int currMenu;
	int currGatesNumb;
	int currPlayersNumber;
	std::string *currChangingValue;
	int playerboxFileNameIndex;
    cocos2d::extension::EditBox* playerEditBoxListener = NULL;
	int m_playersBoxesFileNamesIndexes[4];
	//multi
	int m_currOpponentsNumber;
	int m_currDiffValue;
	int m_currModeSelected;
	int m_currGatesNumb;
	int m_currPlayersNumber;
	//dla zmieniania wartosc playerow;
	cocos2d::ui::Layout* layout;
	DialogReader* dr;
	void preload();
	void hide(int menutypedef);
	void show(int menutypedef);
	void createSpinner(cocos2d::ui::Button* btnplus, cocos2d::ui::Button* btnMinus, const std::string &defaultText,cocos2d::ui::Text *labelText, int &changinVal, int maxVal, int minVal,std::function<void(cocos2d::ui::Text*)> additionalFunction=nullptr);
	cocos2d::extension::EditBox* createTextEdit(std::string &text, cocos2d::Color3B textColor, int parenttag, int tag,std::function<void(const std::string &name)> callback=nullptr);
	void createSinglePlayerTutorialDialog();
	void createLevelMapUI();
public:
    void UPDATEPLAYERNAME();
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MyMenu);
	//main menu//
    void shareGame(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onPlaySinglePlayerBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
		//single menu//
		void onCarrerBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
		void onFreeRunBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
			//custom single//
			void playCustomNow(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
			void onBestScoreBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onPlayMultiBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
		void onMPcontinueToBoxChoose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onOptionsBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	//shared//
	void goBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	//CALLBACKS
	//SINGLE PLAYER SLIDER VALUE CHANGED
	void onSPmodeChooserPage(cocos2d::Ref *reff, cocos2d::ui::PageView::EventType type);
	void onSPcontinueToBoxChooseBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	//MULTIPLAYER SLIDER VALUE CHANGED
	void onMPModeChooserPage(cocos2d::Ref* reff, cocos2d::ui::PageView::EventType type);
	//wybieranie gracza
	void m_textFieldChanged(cocos2d::Ref *psender, cocos2d::ui::TextField::EventType type);
	bool m_checkPlayersOverlap();
	void playMultiNow(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	//additional callbacks
	void m_OpponentsChanged(cocos2d::ui::Text *textToChange);
	void difficultySpinnerChanged(cocos2d::ui::Text *textTochange);
	void m_difficultySpinnerChanged(cocos2d::ui::Text *textTochange);
	void m_autocorrectWrongPlayerChoose();
	void goToLevelChooserMenu();
	//SETUPO
	void m_setupAutoCorrectDialog(cocos2d::ui::Layout *root);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	//options
	void createOptionsMenu();
    virtual void onEnter();
	void defaultPageAction(cocos2d::ui::PageView* pgView);
	void onBoxChooseChanged(cocos2d::ui::PageView *pgview);
	void refreshFBAvatar(cocos2d::Texture2D *tex);
	void checkIfNoOpponents(cocos2d::ui::Text* txt);
    cocos2d::ui::Button* createBtn(const std::string &imgOn, const std::string &imgOf, const std::string &btnText, cocos2d::ui::Widget::ccWidgetTouchCallback callback, int typed, cocos2d::Node *layout);
};
#endif // !__MENU_H__
