#define DEBUG_COCOS2D 2
#include "MyMenu.h"
#include "Macros.h"
#include "SingleGateWorld.h"
#include "SimpleAudioEngine.h"
#include "Paths.h"
#include "Globals.h"
#include "SingleEliminationWorld.h"
#include "EndlessWorld.h"
#include "Hud.h"
#include "AIOpponent.h"
#include "myLayout.h"
#include "myListView.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "DbReader.h"
#include "VisibleRect.h"
#include "soundManager.h"
#include "dialogReader.h"
#include "extensions/GUI/CCEditBox/CCEditBox.h"
#include "extensions/GUI/CCControlExtension/CCScale9Sprite.h"
#include "screw/screw.h"
#include "GlobalAdManager.h"
using namespace cocos2d;
using namespace ui;

bool MyMenu::init()
{
	if (!Layer::init())
	{
		return false;
	}
	preload();
	currOpponentsNumber   = 3;
	currDiffValue         = 1;
	currModeSelected      = 0;
	currGatesNumb         = 7;
	currPlayersNumber     = 2;
	m_currOpponentsNumber = 1;
	m_currPlayersNumber   = 2;
	m_currDiffValue       = 1;
	m_currModeSelected    = 0;
	m_currGatesNumb       = 7;
	playerboxFileNameIndex = 2;
	m_playersBoxesFileNamesIndexes[0] = 0;
	m_playersBoxesFileNamesIndexes[1] = 1;
	m_playersBoxesFileNamesIndexes[2] = 2;
	m_playersBoxesFileNamesIndexes[3] = 3;
	currMenu = L_MAINMENU;
	//bg//
	Sprite *bg = Sprite::createWithSpriteFrameName(R_mainMenuBg);
	bg->setAnchorPoint(Vec2(0, 0));
	G_scaleToFitScreen(bg);
	this->addChild(bg,-3);
	this->setPosition(VR::leftBottom());
	dr = DialogReader::getInstance();
		//****MAIN MENU******
		G_scaleNodeVerticallyToFit(dr->getMainWidgetFromJson("MainMenu.json", this));
		((Button*)dr->getWidget("MainMenu.json", "Single_PlayerBtn"))->addTouchEventListener(CC_CALLBACK_2(MyMenu::onPlaySinglePlayerBtn, this));
		((Button*)dr->getWidget("MainMenu.json", "Multi_PlayerBtn"))->addTouchEventListener(CC_CALLBACK_2(MyMenu::onPlayMultiBtn, this));
		((Button*)dr->getWidget("MainMenu.json", "OptionsBtn"))->addTouchEventListener(CC_CALLBACK_2(MyMenu::onOptionsBtn, this));
			//**OPTIONS**//
			createOptionsMenu();
			//****SINGLE PLAYER******
			G_scaleNodeVerticallyToFit(dr->getHiddenMainWidFromJson("PLAYSINGLE.json", this));
			((Button*)dr->getWidget("PLAYSINGLE.json", "CarrerBtn"))->addTouchEventListener(CC_CALLBACK_2(MyMenu::onCarrerBtn, this));
			((Button*)dr->getWidget("PLAYSINGLE.json", "FreeRunBtn"))->addTouchEventListener(CC_CALLBACK_2(MyMenu::onFreeRunBtn, this));
			((Button*)dr->getWidget("PLAYSINGLE.json", "BestRunBtn"))->addTouchEventListener(CC_CALLBACK_2(MyMenu::onBestScoreBtn, this));
				//****FREE RUN******
				G_scaleNodeVerticallyToFit(dr->getHiddenMainWidFromJson("FREERUN.json", this));
				((PageView*)dr->getWidget("FREERUN.json", "modeChooser"))->addEventListener(CC_CALLBACK_2(MyMenu::onSPmodeChooserPage, this));
				((Button*)dr->getWidget("FREERUN.json", "ContinueBtn"))->addTouchEventListener(CC_CALLBACK_2(MyMenu::onSPcontinueToBoxChooseBtn, this));
				createSpinner((Button*)dr->getWidget("FREERUN.json", "plus_gates"), (Button*)dr->getWidget("FREERUN.json", "minus_gates"), std::to_string(currGatesNumb), (Text*)dr->getWidget("FREERUN.json", "gatesNumber"), currGatesNumb, 25, 3);
				createSpinner((Button*)dr->getWidget("FREERUN.json", "plus_OpNum"), (Button*)dr->getWidget("FREERUN.json", "minus_OpNum"), std::to_string(currOpponentsNumber), (Text*)dr->getWidget("FREERUN.json", "opponentsNumber"), currOpponentsNumber, 5, 2);
				createSpinner((Button*)dr->getWidget("FREERUN.json", "plus_ai"), (Button*)dr->getWidget("FREERUN.json", "minus_ai"), std::to_string(currDiffValue), (Text*)dr->getWidget("FREERUN.json", "aiSmartness"), currDiffValue, 2, 0,CC_CALLBACK_1(MyMenu::difficultySpinnerChanged,this));
					//**CHOOSE NAME***//
					G_scaleNodeVerticallyToFit(dr->getHiddenMainWidFromJson("CHOOSENAMES.json", this));
                    auto textEdit = createTextEdit(G_playersDefaultNames[0], G_colors[playerboxFileNameIndex],NULL,1);
					dr->getWidget("CHOOSENAMES.json", "editBoxPlaceHolder")->addChild(textEdit,0,"playerTextEdit");
					textEdit->setNormalizedPosition(Point(0.5f, 0.5f));
					((PageView*)dr->getWidget("CHOOSENAMES.json", "PageView_6"))->addEventListener([this, textEdit](Ref *reff, PageView::EventType type)
					{
						defaultPageAction((PageView*)reff);
						onBoxChooseChanged((PageView*)reff);
						playerboxFileNameIndex = ((PageView*)dr->getWidget("CHOOSENAMES.json", "PageView_6"))->getCurPageIndex();
						textEdit->setFontColor(G_colors[playerboxFileNameIndex]);
					});	
					((PageView*)dr->getWidget("CHOOSENAMES.json", "PageView_6"))->scrollToPage(3);
					((Button*)dr->getWidget("CHOOSENAMES.json", "PlayNow"))->addTouchEventListener(CC_CALLBACK_2(MyMenu::playCustomNow, this));
				//**CARRER**//
				createLevelMapUI();	
			//**MUTLIFREERUN**//
			G_scaleNodeVerticallyToFit(dr->getHiddenMainWidFromJson("MULTIFREERUN.json", this));
			((PageView*)dr->getWidget("MULTIFREERUN.json", "modeChooser"))->addEventListener(CC_CALLBACK_2(MyMenu::onMPModeChooserPage, this));
			((Button*)dr->getWidget("MULTIFREERUN.json", "ContinueBtn"))->addTouchEventListener(CC_CALLBACK_2(MyMenu::onMPcontinueToBoxChoose, this));
			createSpinner((Button*)dr->getWidget("MULTIFREERUN.json", "plus_gates"), (Button*)dr->getWidget("MULTIFREERUN.json", "minus_gates"), std::to_string(m_currGatesNumb), (Text*)dr->getWidget("MULTIFREERUN.json", "gatesNumber"), m_currGatesNumb, 25, 3);
			createSpinner((Button*)dr->getWidget("MULTIFREERUN.json", "plus_OpNum"), (Button*)dr->getWidget("MULTIFREERUN.json", "minus_OpNum"), std::to_string(m_currOpponentsNumber), (Text*)dr->getWidget("MULTIFREERUN.json", "opponentsNumber"), m_currOpponentsNumber, 3, 0, CC_CALLBACK_1(MyMenu::checkIfNoOpponents, this));
			createSpinner((Button*)dr->getWidget("MULTIFREERUN.json", "plus_ai"), (Button*)dr->getWidget("MULTIFREERUN.json", "minus_ai"), std::to_string(m_currDiffValue), (Text*)dr->getWidget("MULTIFREERUN.json", "aiSmartness"), m_currDiffValue, 2, 0, CC_CALLBACK_1(MyMenu::m_difficultySpinnerChanged, this));
			createSpinner((Button*)dr->getWidget("MULTIFREERUN.json", "plus_PlNum"), (Button*)dr->getWidget("MULTIFREERUN.json", "minus_PlNum"), std::to_string(m_currPlayersNumber), (Text*)dr->getWidget("MULTIFREERUN.json", "playersNumber"), m_currPlayersNumber, 3, 2);
				//**MULTI CHOOSE NAMES**//
				G_scaleNodeVerticallyToFit(dr->getHiddenMainWidFromJson("MULTICHOOSENAMES.json", this));
				for (int i = 0; i < 3; i++)
				{
					auto textEdit = createTextEdit(G_playersDefaultNames[i], G_colors[m_playersBoxesFileNamesIndexes[i]],NULL,i+1);
					dr->getWidget("MULTICHOOSENAMES.json", "editBoxPlaceHolder"+to_string(i))->addChild(textEdit,0,("textEdit"+std::to_string(i)).c_str());
					auto page = ((PageView*)dr->getWidget("MULTICHOOSENAMES.json", "PageView_" + std::to_string(i)));
					page->addEventListener([this, textEdit,i](Ref *reff, PageView::EventType type)
					{
						defaultPageAction((PageView*)reff);
						onBoxChooseChanged((PageView*)reff);
						m_playersBoxesFileNamesIndexes[i] = ((PageView*)reff)->getCurPageIndex();
						textEdit->setFontColor(G_colors[m_playersBoxesFileNamesIndexes[i]]);
					});
					page->scrollToPage(i+1);
				}
				((Button*)dr->getWidget("MULTICHOOSENAMES.json", "PlayNow"))->addTouchEventListener(CC_CALLBACK_2(MyMenu::playMultiNow, this));
	//misc
	refreshFBAvatar(NULL);
    if(!this->getChildByTag(L_CHOOSENAMES)->getChildByTag(645))
    {
        auto scoresBtn = createBtn(R_btnOn,"",G_str("bstScores"), [this](Ref *reff,Widget::TouchEventType type)
                                   {
                                       if(type != Widget::TouchEventType::ENDED) return;
                                        SoundManager::getInstance()->playBtnEffect();
                                       if(!G_faceBookAvatarTex)
                                       {
                                           FB_login();
                                           return;
                                       }
                                       FB_showScores(this);
                                   }, 645, this->getChildByTag(L_CHOOSENAMES));
        auto playpos = dr->getWidget("CHOOSENAMES.json", "PlayNow")->getPosition();
        scoresBtn->setPosition(Vec2(playpos.x,playpos.y+45));
    }
    if(!this->getChildByTag(L_MAINMENU)->getChildByTag(646))
    {
        auto moregamesBtn = createBtn(R_btnOn,"",G_str("more_games"),[](Ref* reff,ui::Widget::TouchEventType type)
                                        {
                                            if(type != Widget::TouchEventType::ENDED) return;
                                            SoundManager::getInstance()->playBtnEffect();
                                            GlobalAdManager::showMoreGames();
                                        },646,this->getChildByTag(L_MAINMENU));
        moregamesBtn->setAnchorPoint(Vec2(1,0));
        moregamesBtn->setPosition(moregamesBtn->getParent()->convertToNodeSpace(Point(VR::right().x-15,VR::bottom().y+15)));
        auto fbButton = createBtn(R_fbIcon,"","",[](Ref* reff,ui::Widget::TouchEventType type)
                                  {
                                      if(type != Widget::TouchEventType::ENDED) return;
                                      SoundManager::getInstance()->playBtnEffect();
                                      GlobalAdManager::goToLink(R_fblink);
                                  },950,this->getChildByTag(L_MAINMENU));
        fbButton->setAnchorPoint(Vec2(1,0));
        fbButton->setPosition(moregamesBtn->getParent()->convertToNodeSpace(Point(VR::right().x-15,VR::bottom().y+48)));
    }
    auto backbtn = createBtn(R_btnBack,"","", CC_CALLBACK_2(MyMenu::goBack, this), B_BACK, this);
	backbtn->setAnchorPoint(Vec2(0, 1));
	backbtn->setPosition(Vec2(12,VR::leftTop().y-VR::bottom().y - 12));
	backbtn->setOpacity(0);
	backbtn->setVisible(false);
	this->getChildByTag(L_MAINMENU)->setOpacity(255);
	this->getChildByTag(L_MAINMENU)->setVisible(true);
	//if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)this->setScale(0.3f);
	//keyBack listener
	auto keylistener = EventListenerKeyboard::create();
	keylistener->onKeyReleased = CC_CALLBACK_2(MyMenu::onKeyReleased, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(keylistener, this);
	SoundManager::getInstance()->playBgMusicMenu();
	return true;
}
cocos2d::ui::Button* MyMenu::createBtn(const std::string &imgOn, const std::string &imgOf, const std::string &btnText, cocos2d::ui::Widget::ccWidgetTouchCallback callback, int typed, cocos2d::Node  *layout)
{
	cocos2d::ui::Button* btn = cocos2d::ui::Button::create(imgOn, imgOf, "", TextureResType::PLIST);
	btn->setTitleFontName(R_defaultFont);
	btn->setTitleFontSize(12);
	btn->setTouchEnabled(true);
	if (btnText != "")
	{
		btn->setTitleText(G_str(btnText));
	}
	btn->addTouchEventListener(callback);
	layout->addChild(btn, 15, typed);
    return btn;
}
cocos2d::Scene* MyMenu::createScene()
{
	Scene *scena = Scene::create();
	MyMenu *menu = MyMenu::create();
	scena->addChild(menu, 1, LAYER_HUD);
	return scena;
}
//**CREATING**//
void MyMenu::createSpinner(cocos2d::ui::Button* btnplus, cocos2d::ui::Button* btnMinus, const std::string &defaultText, cocos2d::ui::Text *labelText,int &changinVal, int maxVal, int minVal,std::function<void(cocos2d::ui::Text*)> additionalFunction/*=nullptr*/)
{

	btnMinus->addTouchEventListener([&changinVal, minVal, labelText, additionalFunction](Ref* pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) return;
		SoundManager::getInstance()->playBtnEffect();
		if (changinVal-1 >= minVal)
		{
			changinVal = changinVal - 1;
			labelText->setString(String::createWithFormat("%d", changinVal)->getCString());
			if (additionalFunction != nullptr)
				additionalFunction(labelText);
		}
	});
	btnplus->addTouchEventListener([&changinVal, maxVal, labelText, additionalFunction](Ref* pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) return;
		SoundManager::getInstance()->playBtnEffect();
		if (changinVal + 1 <= maxVal)
		{
			changinVal = changinVal + 1;
			labelText->setString(String::createWithFormat("%d", changinVal)->getCString());
			if (additionalFunction != nullptr)
				additionalFunction(labelText);
		}
	});
	labelText->setString(String::createWithFormat("%d", changinVal)->getCString());
	if (additionalFunction != nullptr) additionalFunction(labelText);
}
cocos2d::extension::EditBox* MyMenu::createTextEdit(std::string &text, cocos2d::Color3B textColor, int parenttag, int tag, std::function<void(const std::string &name)> callback/*=nullptr*/)
{
	auto editbox = extension::EditBox::create(Sprite::createWithSpriteFrameName(R_editBtn[0].c_str())->getContentSize(), extension::Scale9Sprite::createWithSpriteFrameName(R_editBtn[0].c_str()), extension::Scale9Sprite::createWithSpriteFrameName(R_editBtn[1].c_str()));
	editbox->setText(text.c_str());
	editbox->setFontColor(textColor);
	editbox->setPlaceHolder(text.c_str());
	editbox->setMaxLength(8);
	editbox->setReturnType(extension::EditBox::KeyboardReturnType::DONE);
	editbox->setDelegate(new TextHandler(text,tag));
	editbox->setNormalizedPosition(Vec2(0.5, 0.5));
	editbox->setFont(R_defaultFont.c_str(),12);
	//
	return editbox;
}
//**MISC**
void MyMenu::preload()
{
	G_srodek = VR::center();
	VR::setShouldLazyInit(false);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(R_res1);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(R_res2);
	SoundManager::getInstance()->preloadSounds();
	DbReader::getInstance()->initPlayersDefaultNames();
}
void MyMenu::hide(int menutypedef)
{
	Node *nodetiHide = this->getChildByTag(menutypedef);
	FiniteTimeAction *fadeout = FadeOut::create(0.5f);
	//EaseBackIn *fadeout = EaseBackIn::create(MoveTo::create(0.4f, Vec2(G_srodek.x * 2, nodetiHide->getPositionY())));
	FiniteTimeAction *disableTouch = CallFunc::create([nodetiHide](){nodetiHide->setVisible(false); });
	nodetiHide->runAction(Sequence::createWithTwoActions(fadeout, disableTouch));
}
void MyMenu::show(int menutypedef)
{
	Node *nodetoShow = this->getChildByTag(menutypedef);
	FiniteTimeAction *fadeIn = FadeIn::create(0.5f);
	//nodetoShow->setPosition(Vec2(-2*G_srodek.x, nodetoShow->getPositionY()));
	//EaseBackIn *fadeIn = EaseBackIn::create(MoveTo::create(0.4f, Vec2(G_srodek.x, nodetoShow->getPositionY())));
	FiniteTimeAction *doNothung = DelayTime::create(0.5f);
	FiniteTimeAction *enableTouch = CallFunc::create([nodetoShow](){nodetoShow->setVisible(true); });
	nodetoShow->runAction(Sequence::create(doNothung, enableTouch, fadeIn, NULL));
	currMenu = menutypedef;
}
void MyMenu::goBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;
	SoundManager::getInstance()->playBtnEffect();
	switch (currMenu)
	{
	case L_PLAYSINGLE:
		show(L_MAINMENU);
		hide(B_BACK);
		hide(L_PLAYSINGLE);
		break;
	case L_FREERUN:
		show(L_PLAYSINGLE);
		hide(L_FREERUN);
		break;
	case L_CARRER:
		show(L_PLAYSINGLE);
		hide(L_CARRER);
		break;
	case L_OPTIONS:
		show(L_MAINMENU);
		hide(B_BACK);
		hide(L_OPTIONS);
		break;
	case L_PLAYMULTI:
		show(L_MAINMENU);
		hide(B_BACK);
		hide(L_MULTIFREELOCALRUN);
		break;
	case L_M_CHOOSENAMES:
		show(L_MULTIFREELOCALRUN);
		hide(L_M_CHOOSENAMES);
		break;
	case L_MULTIFREELOCALRUN:
		show(L_MAINMENU);
		hide(L_MULTIFREELOCALRUN);
		hide(B_BACK);
		break;
	case L_CHOOSENAMES:
        if (spType == singlePlayerType::free) show(L_FREERUN);
		else show(L_PLAYSINGLE);
		hide(L_CHOOSENAMES);
		break;
	default:
		break;
	}
}
//**MAIN MENU EVENTS**//
void MyMenu::onPlaySinglePlayerBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;
	SoundManager::getInstance()->playBtnEffect();
	show(B_BACK);
	show(L_PLAYSINGLE);
	hide(L_MAINMENU);
}
void MyMenu::onPlayMultiBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;
	SoundManager::getInstance()->playBtnEffect();
	if (!DbReader::getInstance()->areBasicTutorialsCompleted())
	{
		G_scaleNodeVerticallyToFit(dr->getMainWidgetFromJson("completeTutsFirst.json", this));
		return;
	}
	show(B_BACK);
	show(L_MULTIFREELOCALRUN);
	hide(L_MAINMENU);
}
void MyMenu::onOptionsBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;
	SoundManager::getInstance()->playBtnEffect();
	show(B_BACK);
	show(L_OPTIONS);
	hide(L_MAINMENU);
}
//**SINGLE PLAYER EVENTS**//
void MyMenu::onCarrerBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;
	spType = singlePlayerType::carrer;
    this->getChildByTag(L_CHOOSENAMES)->getChildByTag(645)->setVisible(false);
    ((Text*)dr->getWidget("CHOOSENAMES.json", "Label_5_0"))->setString(G_str("Carrer"));
	((Button*)dr->getWidget("CHOOSENAMES.json", "PlayNow"))->setTitleText(G_str("Continue"));
     onSPcontinueToBoxChooseBtn(pSender, type);
}
void MyMenu::onFreeRunBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;
	SoundManager::getInstance()->playBtnEffect();
	if (!DbReader::getInstance()->areBasicTutorialsCompleted())
	{
		G_scaleNodeVerticallyToFit(dr->getMainWidgetFromJson("completeTutsFirst.json", this));
		return;
	}
	spType = singlePlayerType::free;
     this->getChildByTag(L_CHOOSENAMES)->getChildByTag(645)->setVisible(false);
    ((Text*)dr->getWidget("CHOOSENAMES.json", "Label_5_0"))->setString(G_str("FreeRun"));
	((Button*)dr->getWidget("CHOOSENAMES.json", "PlayNow"))->setTitleText(G_str("Play"));
	show(L_FREERUN);
	hide(L_PLAYSINGLE);
}
//**CUSTOM SINGLE PLAYER EVENTS**//
void MyMenu::playCustomNow(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;
	SoundManager::getInstance()->playBtnEffect();
	Scene *scene;
	if (spType == singlePlayerType::best)
	{
		scene = EndlessWorld::createScene(currOpponentsNumber + 1, currDiffValue, true);
		EndlessWorld *world = (EndlessWorld*)scene->getChildByTag(LAYER_GAMEPLAY);
		world->setSinglePlayer(Player::create(R_Box[playerboxFileNameIndex], G_playersDefaultNames[0], world->getGravitySpace(), G_colors[playerboxFileNameIndex]));
		world->setMinGates(0);
		G_dir()->replaceScene(scene);
		return;
	}
	else if (spType == singlePlayerType::carrer)
	{
		G_globalBoxFileNameIndex = playerboxFileNameIndex;
		G_globalPlayerName = G_playersDefaultNames[0];
		hide(L_CHOOSENAMES);
		show(L_CARRER);
		return;
	}
	else if (currModeSelected == 0)
	{
		scene = SingleGateWorld::createScene(currOpponentsNumber + 1, currGatesNumb, currDiffValue);
	}
	else if (currModeSelected == 1)
	{
		scene = SingleEliminationWorld::createScene(currOpponentsNumber + 1, currDiffValue);
	}
	else if (currModeSelected == 2)
	{
		scene = EndlessWorld::createScene(currOpponentsNumber + 1, currDiffValue, false);
		EndlessWorld *world = (EndlessWorld*)scene->getChildByTag(LAYER_GAMEPLAY);
		world->setSinglePlayer(Player::create(R_Box[playerboxFileNameIndex], G_playersDefaultNames[0], world->getGravitySpace(), G_colors[playerboxFileNameIndex]));
		world->setMinGates(currGatesNumb);
		G_dir()->replaceScene(scene);
		return;
	}
	World *world = (World*)scene->getChildByTag(LAYER_GAMEPLAY);
	world->setSinglePlayer(Player::create(R_Box[playerboxFileNameIndex], G_playersDefaultNames[0], world->getGravitySpace(),G_colors[playerboxFileNameIndex]));
	G_dir()->replaceScene(scene);
}
void MyMenu::onSPmodeChooserPage(cocos2d::Ref *reff, cocos2d::ui::PageView::EventType type)
{
	defaultPageAction((PageView*)reff);
	currModeSelected = ((PageView*)dr->getWidget("FREERUN.json", "modeChooser"))->getCurPageIndex();
	auto enable = [](bool val,Widget *nod)
	{
		if (val) nod->setColor(Color3B::WHITE);
		else nod->setColor(Color3B::GRAY);
		nod->setEnabled(val);
	};
	if (currModeSelected == 0)
	{
		enable(true , (Widget*)dr->getWidget("FREERUN.json", "gatesChooser"));
		enable(true, (Widget*)dr->getWidget("FREERUN.json", "opponentsChooser"));
		enable(true, (Widget*)dr->getWidget("FREERUN.json", "diffChooser"));
	}
	else if (currModeSelected == 1)
	{
		enable(false, (Widget*)dr->getWidget("FREERUN.json", "gatesChooser"));
		enable(true, (Widget*)dr->getWidget("FREERUN.json", "opponentsChooser"));
		enable(true, (Widget*)dr->getWidget("FREERUN.json", "diffChooser"));
	}
	else
	{
		enable(true, (Widget*)dr->getWidget("FREERUN.json", "gatesChooser"));
		enable(true, (Widget*)dr->getWidget("FREERUN.json", "opponentsChooser"));
		enable(true, (Widget*)dr->getWidget("FREERUN.json", "diffChooser"));
	}
}
void MyMenu::difficultySpinnerChanged(cocos2d::ui::Text *textTochange)
{
	std::string str = "";
	if (currDiffValue == 0) str = G_str("Easy");
	else if (currDiffValue == 1) str = G_str("Medium");
	else if (currDiffValue == 2) str = G_str("Hard");
	textTochange->setString(str);
}
//**CUSTOM MULTI PLAYER EVENTS**//
void MyMenu::onMPModeChooserPage(cocos2d::Ref* reff, cocos2d::ui::PageView::EventType type)
{
	defaultPageAction((PageView*)reff);
	m_currModeSelected = ((PageView*)dr->getWidget("MULTIFREERUN.json", "modeChooser"))->getCurPageIndex();
	auto enable = [](bool val, Widget *nod)
	{
		if (val) nod->setColor(Color3B::WHITE);
		else nod->setColor(Color3B::GRAY);
		nod->setEnabled(val);
	};
	if (m_currModeSelected == 0)
	{
		enable(true, (Widget*)dr->getWidget("MULTIFREERUN.json", "gatesChooser"));
		enable(true, (Widget*)dr->getWidget("MULTIFREERUN.json", "opponentsChooser"));
		enable(true, (Widget*)dr->getWidget("MULTIFREERUN.json", "diffChooser"));
	}
	else if (m_currModeSelected == 1)
	{
		enable(false, (Widget*)dr->getWidget("MULTIFREERUN.json", "gatesChooser"));
		enable(true, (Widget*)dr->getWidget("MULTIFREERUN.json", "opponentsChooser"));
		enable(true, (Widget*)dr->getWidget("MULTIFREERUN.json", "diffChooser"));
	}
	else
	{
		enable(true, (Widget*)dr->getWidget("MULTIFREERUN.json", "gatesChooser"));
		enable(true, (Widget*)dr->getWidget("MULTIFREERUN.json", "opponentsChooser"));
		enable(true, (Widget*)dr->getWidget("MULTIFREERUN.json", "diffChooser"));
	}
}
void MyMenu::onMPcontinueToBoxChoose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if(type != Widget::TouchEventType::ENDED) return;
	SoundManager::getInstance()->playBtnEffect();
	for (int i = 0; i< 3; i++)
	{
		auto boxChose = dr->getWidget("MULTICHOOSENAMES.json", "boxChoose" + std::to_string(i));
		auto textField = dr->getWidget("MULTICHOOSENAMES.json", "editBoxPlaceHolder" + std::to_string(i));
		if ( i+1 > m_currPlayersNumber)
		{
			boxChose->setVisible(false);
			textField->setVisible(false);
		}
		else
		{
			boxChose->setVisible(true);
			textField->setVisible(true);
		}
	}
	show(L_M_CHOOSENAMES);
	hide(L_MULTIFREELOCALRUN);
	
}
void MyMenu::m_difficultySpinnerChanged(cocos2d::ui::Text *textTochange)
{
	std::string str = "";
	if (m_currDiffValue == 0) str = G_str("Easy");
	else if (m_currDiffValue == 1) str = G_str("Medium");
	else if (m_currDiffValue == 2) str = G_str("Hard");
	textTochange->setString(str);
}
//*MUTLI CHOOSE BOXES *//
void MyMenu::playMultiNow(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;
	SoundManager::getInstance()->playBtnEffect();
	if (!m_checkPlayersOverlap()) return;
	Vector<Player*> players;
	Vector<AIOpponent*> opponentz;
	Scene *scene;
	if (m_currModeSelected == 0)
	{
		scene = SingleGateWorld::createScene(m_currOpponentsNumber +m_currPlayersNumber, m_currGatesNumb, currDiffValue);
	}
	else if (m_currModeSelected == 1)
	{
		scene = SingleEliminationWorld::createScene(m_currOpponentsNumber + m_currPlayersNumber, currDiffValue);
	}
	World *world = (World*)scene->getChildByTag(LAYER_GAMEPLAY);
	for (int i = 0; i < m_currPlayersNumber; i++)
	{
		players.pushBack(Player::create(R_Box[m_playersBoxesFileNamesIndexes[i]], G_playersDefaultNames[i], world->getGravitySpace(), G_colors[m_playersBoxesFileNamesIndexes[i]]));
	}
	world->setMultiplayer(players);
	G_dir()->replaceScene(scene);
}
void MyMenu::m_OpponentsChanged(cocos2d::ui::Text *textToChange)
{
	auto widget = ((Widget*)dr->getWidget("MULTIFREERUN.json", "diffChooser"));
	if (m_currOpponentsNumber == 0)
	{ 
		widget->setColor(Color3B::GRAY);
		widget->setEnabled(false);
	}
	else
	{
		widget->setColor(Color3B::WHITE);
		widget->setEnabled(true);
	}
}
void MyMenu::onSPcontinueToBoxChooseBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;
	SoundManager::getInstance()->playBtnEffect();
	if (currMenu == L_FREERUN)
	{
		hide(L_FREERUN);
	}
	else
	{
		hide(L_PLAYSINGLE);
	}
	show(L_CHOOSENAMES);
}
//FIXES
bool MyMenu::m_checkPlayersOverlap()
{
	bool ok = true;
	for (int i = 0; i < m_currPlayersNumber-1; i++)
	{
		for (int j = i + 1; j < m_currPlayersNumber; j++)
		{
			PageView *pageviewAtI = (PageView*)dr->getWidget("MULTICHOOSENAMES.json", "PageView_" + std::to_string(i));
			PageView *pageviewAtJ = (PageView*)dr->getWidget("MULTICHOOSENAMES.json", "PageView_" + std::to_string(j));
			if (pageviewAtI->getCurPageIndex() == pageviewAtJ->getCurPageIndex())
			{
				//REST
				pageviewAtJ->setColor(Color3B(255, 255, 254));//chujowyt kolor
				ok = false;
			}
		}
	}
	if (!ok)
	{
		//GUI HANDLING
		G_scaleNodeVerticallyToFit(dr->getMainWidgetFromJson("AutoCorrectDialog.json", this));
		dr->addActionHideAndSomething("AutoCorrectDialog.json", "autoCorrectBtn", [this]()
		{
			this->m_autocorrectWrongPlayerChoose();
		});
	}
	return ok;
}
void MyMenu::m_autocorrectWrongPlayerChoose()
{
	bool zajete[6] = { false, false, false, false, false, false };
	for (int i = 0; i < m_currPlayersNumber; i++)
	{
		PageView *pageviewAtI = (PageView*)dr->getWidget("MULTICHOOSENAMES.json", "PageView_" + std::to_string(i));
		zajete[pageviewAtI->getCurPageIndex()] = true;
	}
	for (int i = 0; i <  m_currPlayersNumber; i++)
	{
		PageView *pageviewAtI = (PageView*)dr->getWidget("MULTICHOOSENAMES.json", "PageView_" + std::to_string(i));
		if (pageviewAtI->getColor() == Color3B(255, 255, 254))	//chujowy kolor
		{
			bool moved = false;
			int k = 1;
			const int curpost = pageviewAtI->getCurPageIndex();
			while (moved == false)
			{
				if (curpost + k <= 5 && !zajete[curpost + k])
				{
					pageviewAtI->getColor() == Color3B(255, 255, 255);
					pageviewAtI->scrollToPage(curpost + k);
					zajete[curpost + k] = true;
					moved = true;
				}
				else if (curpost - k >= 0 && !zajete[curpost - k])
				{
					pageviewAtI->getColor() == Color3B(255, 255, 255);
					pageviewAtI->scrollToPage(curpost - k);
					zajete[curpost - k] = true;
					moved = true;
				}
				k++;
			}
		}
	}
}
void MyMenu::createSinglePlayerTutorialDialog()
{
		int taps=0;
		Layout *dialogRoot = dynamic_cast<Layout*>(cocostudio::GUIReader::shareReader()->widgetFromJsonFile(R_tutorialDialog.c_str()));
		dialogRoot->setTouchEnabled(false);
		auto button = (Button*)utils::findChildren(*dialogRoot, "//Button_3").at(0);
		auto text = (Text*)utils::findChildren(*dialogRoot, "//Label_4").at(0);
		cocostudio::ActionManagerEx::getInstance()->playActionByName(R_tutorialDialog.c_str(), "WelcomeAnimation");
		bool singleConfirmed=false;
		button->addTouchEventListener([text, taps](Ref*, Widget::TouchEventType type) mutable
		{
			if (type != Widget::TouchEventType::ENDED) return;
			SoundManager::getInstance()->playBtnEffect();
			switch (taps)
			{
			case 0:
				cocostudio::ActionManagerEx::getInstance()->playActionByName(R_tutorialDialog.c_str(), "MoveToTop");
				text->setString(G_str("Tutorial_Single/Multiplayer"));
				taps++;
				break;
			case 1:
				cocostudio::ActionManagerEx::getInstance()->playActionByName(R_tutorialDialog.c_str(), "Notification");
				text->setString(G_str("Tutorial_Single/Multiplayer"));
				taps++;
				break;
			}
		});
		this->addChild(dialogRoot, 1, E_TUTORIALDIALOG);
}
void MyMenu::createLevelMapUI()
{
	Layout *parent = NULL;
	if(!this->getChildByTag(L_CARRER))
	{
		parent = Layout::create();
		this->addChild(parent,0, L_CARRER);
		parent->setOpacity(0);
		parent->setVisible(false);
	}
	else parent = (Layout*)this->getChildByTag(L_CARRER);
	bool firstUncompletedFound=false;
	G_scaleNodeVerticallyToFit(dr->getMainWidgetFromJson(R_LevelMapUI, parent));
	for (int i = 1; i < 10; i++)
	{
		const std::string name = "Level" + std::to_string(i) + "Btn";
		auto btn = (Button*)DialogReader::getInstance()->getWidget(R_LevelMapUI, name);
		btn->setColor(Color3B::WHITE);
		if (!DbReader::getInstance()->isLevelUnlocked(i))
		{
			btn->setColor(Color3B::GRAY);
			btn->addTouchEventListener([this, parent,i](Ref* reff, Widget::TouchEventType type)
			{
				if (type != Widget::TouchEventType::ENDED) return;
				SoundManager::getInstance()->playBtnEffect();
				G_scaleNodeVerticallyToFit(dr->getMainWidgetFromJson("levelLockedDialog.json", this));
				dr->addActionHideAndSomething("levelLockedDialog.json", "unlockLvLBtn", [i]()
				{
					GlobalAdManager::unlockLevel(i);
				});
			});
		}
		else
		{
            if(!firstUncompletedFound && !DbReader::getInstance()->isLevelUnlocked(i+1))
			{
				firstUncompletedFound=true;
				btn->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(0.2f,1.3f),ScaleTo::create(0.2f,1))));
			}
			btn->addTouchEventListener([parent, i,this](Ref *reff, Widget::TouchEventType type)
			{
				if (type != Widget::TouchEventType::ENDED) return;
				SoundManager::getInstance()->playBtnEffect();
				curCarrerLevelChose = i;
				G_displayCorrectLevelStarter(i, this);
			});
		}
	}
}
void MyMenu::onBestScoreBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;
	SoundManager::getInstance()->playBtnEffect();
	if (!DbReader::getInstance()->areBasicTutorialsCompleted())
	{
		G_scaleNodeVerticallyToFit(dr->getMainWidgetFromJson("completeTutsFirst.json", this));
		return;
	}
	spType = singlePlayerType::best;
    this->getChildByTag(L_CHOOSENAMES)->getChildByTag(645)->setVisible(true);
    ((Text*)dr->getWidget("CHOOSENAMES.json", "Label_5_0"))->setString(G_str("BestRun"));
	((Button*)dr->getWidget("CHOOSENAMES.json", "PlayNow"))->setTitleText(G_str("Play"));
	onSPcontinueToBoxChooseBtn(pSender, type);
}
void MyMenu::goToLevelChooserMenu()
{
	show(B_BACK);
	hide(L_MAINMENU);
	show(L_CARRER);
}
void MyMenu::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	// Back button pressed
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		goBack(this, Widget::TouchEventType::ENDED);
	}
}
void MyMenu::createOptionsMenu()
{
	auto parent = Layout::create();
	this->addChild(parent,0, L_OPTIONS);
	parent->setVisible(false);
	G_scaleNodeVerticallyToFit(dr->getMainWidgetFromJson("Options.json", parent));
	//**end fb callback**//
	dr->addButtonAction("Options.json", "fbConnectBtn", []()//login
	{
		FB_login();
	});
	dr->addButtonAction("Options.json", "fbDisconnectBtn", []()//logout
	{
		FB_logOut();
	});
	dr->addButtonAction("Options.json", "CreditsBtn", [parent]()
	{
		auto dialgo = DialogReader::getInstance()->getMainWidgetFromJson("creditsDialog.json", parent);
		dialgo->setNormalizedPosition(Vec2(0, 0));
		G_scaleNodeVerticallyToFit(dialgo);
	});
	if(!DbReader::getInstance()->areAdsEnabled())
	{
		auto rmvBtn = (Button*)dr->getWidget("Options.json", "rmvAdsBtn");
		rmvBtn->setTouchEnabled(false);
		rmvBtn->setColor(Color3B(100,100,100));
	}
	dr->addButtonAction("Options.json", "rmvAdsBtn", []()
	{
		GlobalAdManager::rmvAds();
	});
	auto ofBtn = (Button*)dr->getWidget("Options.json", "TutorialsOffBtn");
	auto onBtn = (Button*)dr->getWidget("Options.json", "TutorialsOnButton");
	if(DbReader::getInstance()->areTutorialsEnabled())
	{
		onBtn->setTouchEnabled(false);
		onBtn->setColor(Color3B(100,100,100));
	}
	else
	{
		ofBtn->setTouchEnabled(false);
		ofBtn->setColor(Color3B(100,100,100));
	}
	dr->addButtonAction("Options.json", "TutorialsOnButton", [onBtn,ofBtn]
	                                                       {
	                                                    		   DbReader::getInstance()->setTutorialsEnabled(true);
	                                                    		   onBtn->setTouchEnabled(false);
	                                                    		   onBtn->setColor(Color3B(100,100,100));
	                                                    		   ofBtn->setTouchEnabled(true);
	                                                    		   ofBtn->setColor(Color3B(255,255,255));
	                                                       });
	dr->addButtonAction("Options.json", "TutorialsOffBtn", [ofBtn,onBtn]
	                                                        {
	                                                        		DbReader::getInstance()->setTutorialsEnabled(false);
	                                                        		ofBtn->setTouchEnabled(false);
	                                                        		ofBtn->setColor(Color3B(100,100,100));
	                                                        		onBtn->setTouchEnabled(true);
	                                                        		onBtn->setColor(Color3B(255,255,255));
	                                                        });
    auto connbtn = (Button*)dr->getWidget("Options.json", "fbConnectBtn");
    connbtn->setVisible(!FB_connected);
    connbtn->setTouchEnabled(!FB_connected);
    auto discoBtn = (Button*)dr->getWidget("Options.json", "fbDisconnectBtn");
    discoBtn->setVisible(FB_connected);
    discoBtn->setTouchEnabled(FB_connected);
}
void MyMenu::onEnter()
{
    Layer::onEnter();
    //*FB CALLBACKs**//
	auto kolbak = [](bool isLoggedIn)
	{
		CCLOG("login callback");
		if (isLoggedIn)
		{
			FB_connected = true;
		}
		else
		{
            if(G_faceBookAvatarTex != NULL)
            {
                
            }
			FB_connected = false;
		}
		if (G_dir()->getRunningScene() && dynamic_cast<MyMenu*>(G_dir()->getRunningScene()->getChildByTag(LAYER_HUD))) //menu
		{
			CCLOG("boolval = %d", FB_connected);
			auto connbtn = (Button*)DialogReader::getInstance()->getWidget("Options.json", "fbConnectBtn");
			connbtn->setVisible(!FB_connected);
			connbtn->setTouchEnabled(!FB_connected);
			auto discoBtn = (Button*)DialogReader::getInstance()->getWidget("Options.json", "fbDisconnectBtn");
			discoBtn->setVisible(FB_connected);
			discoBtn->setTouchEnabled(FB_connected);
		}
	};
    FB_setLoginCallBack(kolbak,this);
    FB_addDownloadFinishListener(this,CC_CALLBACK_1(MyMenu::refreshFBAvatar,this));
    FB_autLogin();
}
void MyMenu::UPDATEPLAYERNAME()
{
	((extension::EditBox*)dr->getWidget("CHOOSENAMES.json", "editBoxPlaceHolder")->getChildByName("playerTextEdit"))->setText(G_playersDefaultNames[0].c_str());
	((extension::EditBox*)dr->getWidget("MULTICHOOSENAMES.json", "editBoxPlaceHolder0")->getChildByName("textEdit0"))->setText(G_playersDefaultNames[0].c_str());
}
 void MyMenu::shareGame(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED) return;
	FB_showScores(this);
    FB_shareGame();
}
 void MyMenu::defaultPageAction(PageView* pgView)
 {
	 for (auto page : pgView->getPages())
	 {
		 if (page == pgView->getPage(pgView->getCurPageIndex()))
		 {
			 for (auto child : page->getChildren())
			 {
				child->runAction(Sequence::createWithTwoActions(ScaleTo::create(scaleTime, scaleFactor + 0.2f), ScaleTo::create(scaleTime / 2.0f, scaleFactor)));
			 }
		 }
		 else
		 {
			 for (auto child : page->getChildren())
			 {
				 if (child->getScale() != 1)
				 child->runAction(ScaleTo::create(scaleTime, 1));
			 }
		 }
	 }
 }
 void MyMenu::onBoxChooseChanged(cocos2d::ui::PageView *pgview)
 {
	 if (pgview->getCurPageIndex() == 5 && G_faceBookAvatarTex == NULL)
	 {
		 G_scaleNodeVerticallyToFit(dr->getMainWidgetFromJson("connectoToFbDialog.json", this));
		 dr->addActionHideAndSomething("connectoToFbDialog.json", "connectBtn", []()
		 {
			 FB_login();
		 });
		 pgview->scrollToPage(4);
		 return;
	 }
 }
void MyMenu::refreshFBAvatar(cocos2d::Texture2D *tex)
 {
	UPDATEPLAYERNAME();
	 for (int i = 0; i < 4; i++)
	 {
		 Sprite* spr;
		 if (G_faceBookAvatarTex == NULL)
		 {
			 spr = Sprite::createWithSpriteFrameName(R_faceBookFaceLocked);
		 }
		 else
		 {
			spr = Sprite::createWithTexture(G_faceBookAvatarTex);
		 }
		 spr->setNormalizedPosition(Vec2(0.5f, 0.5f));
		 Node *parent;
		 if (i != 3) parent = dr->getWidget("MULTICHOOSENAMES.json", "fbAvatarEmptyLayout" + std::to_string(i));
		 else     parent = dr->getWidget("CHOOSENAMES.json", "fbAvatarEmptyLayout");
		 parent->removeAllChildren();
		 parent->addChild(spr);
	 }

 }
 void MyMenu::checkIfNoOpponents(cocos2d::ui::Text* txt)
 {
	 auto text = (Layout*)dr->getWidget("MULTIFREERUN.json", "diffChooser");
	 if (m_currOpponentsNumber == 0)
	 {
		 text->setColor(Color3B::GRAY);
		 text->setEnabled(false);
	 }
	 else
	 {
		 text->setColor(Color3B::WHITE);
		 text->setEnabled(true);
	 }
 }
