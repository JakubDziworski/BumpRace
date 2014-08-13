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
using namespace cocos2d;
using namespace ui;

bool MyMenu::init()
{
	if (!Layer::init())
	{
		return false;
	}
	preload();
	/*Button *btnn = Button::create(R_multiBtn,"","",TextureResType::PLIST);
	btnn->setPosition(G_srodek);
	this->addChild(btnn,54);
	btnn->addTouchEventListener([btnn](Ref* reff, Widget::TouchEventType type)
	{
	if (type != Widget::TouchEventType::ENDED) return;
	btnn->setPositionY(2 * G_srodek.y);
	btnn->runAction(EaseBackOut::create(MoveTo::create(0.5f, G_srodek)));
	});*/
	currOpponentsNumber=3;
	currDiffValue=1;
	currModeSelected=0;
	currGatesNumb=7;
	currPlayersNumber = 2;
	m_currOpponentsNumber=1;
	m_currPlayersNumber=2;
	m_currDiffValue=1;
    m_currModeSelected=0;
	m_currGatesNumb=7;
	playerName = G_str("Player");
	m_playersNames[0] = G_str("Player") + std::to_string(1);
	m_playersNames[1] = G_str("Player") + std::to_string(2);
	m_playersNames[2] = G_str("Player") + std::to_string(3);
	m_playersNames[3] = G_str("Player") + std::to_string(4);
	playerboxFileNameIndex = 2;
	m_playersBoxesFileNamesIndexes[0] = 0;
	m_playersBoxesFileNamesIndexes[1] = 1;
	m_playersBoxesFileNamesIndexes[2] = 2;
	m_playersBoxesFileNamesIndexes[3] = 3;
	srodek = Director::getInstance()->getVisibleSize() / 2.0f;
	currMenu = L_MAINMENU;
	//bg//
	Sprite *bg = Sprite::createWithSpriteFrameName(R_tlo);
	bg->setAnchorPoint(Vec2(0, 0));
	G_scaleToFitScreen(bg);
	this->addChild(bg,-3);
	//*layout init*//
	createLayout(L_MAINMENU);
		createLayout(L_PLAYSINGLE);
			createLevelMapUI();
			createLayout(L_FREERUN);
			createLayout(L_CHOOSENAMES);
		createLayout(L_PLAYMULTI);
			createLayout(L_MULTIFREELOCALRUN);
				createLayout(L_M_CHOOSENAMES);
		createLayout(L_OPTIONS);
	//*MAIN MENU BUTTNS*//
	createBtn(R_btnOn[0], R_btnOn[1], "Single_Player", CC_CALLBACK_2(MyMenu::playSingleEvent, this), B_PLAYSINGLE, this->getChildByTag(L_MAINMENU));
	createBtn(R_btnOn[0], R_btnOn[1], "Multi_Player", CC_CALLBACK_2(MyMenu::playMultiEvent, this), B_PLAYMULTI, this->getChildByTag(L_MAINMENU));
	createBtn(R_btnOn[0], R_btnOn[1], "Options", CC_CALLBACK_2(MyMenu::optionsEvent, this), B_OPTIONS, this->getChildByTag(L_MAINMENU));
	//*SINGLE PLAYER BUTTONS*//
	createLabel(G_str("Single_Player"), L_PLAYSINGLE, LAB_SINGLEPLAYER);
	createBtn(R_btnOn[0], R_btnOn[1], "Carrer", CC_CALLBACK_2(MyMenu::playCarrer, this), B_CARRER, this->getChildByTag(L_PLAYSINGLE));
	createBtn(R_btnOn[0], R_btnOn[1], "FreeRun", CC_CALLBACK_2(MyMenu::playCustom, this), B_FREERUN, this->getChildByTag(L_PLAYSINGLE));
	//*CARRER BUTTONS*//
	//TODO
	//*FRE RUN BUTTONS*//
	createLabel(G_str("FreeRun"), L_FREERUN, LAB_FREERUN);
	createPages(G_str("Choose_Mode"), { G_str("Gate_Collector"), G_str("Elimination"),G_str("Endless")}, { R_pageGate, R_pageEndless, R_pageElimination }, currModeSelected, PG_CHOOSEMODE, L_FREERUN, CC_CALLBACK_1(MyMenu::modeChooserPageChanged, this));
	createSpinner(std::to_string(currGatesNumb), G_str("Gates"), currGatesNumb, 24, 3,B_GATESLIDER, L_FREERUN);
	createSpinner(std::to_string(currOpponentsNumber), G_str("Opponents"), currOpponentsNumber, maxOpponentsNumber, 1, B_OPPONENTSSLIDE, L_FREERUN);
	createSpinner("Medium", G_str("Difficulty"), currDiffValue, 2, 0, B_DIFFICULTYSLIDER,L_FREERUN, CC_CALLBACK_1(MyMenu::difficultySpinnerChanged, this));
	createBtn(R_btnOn[0], R_btnOn[1], "Continue", CC_CALLBACK_2(MyMenu::continueToBoxChoose, this), B_CONTINUETOCHOSEBOX, this->getChildByTag(L_FREERUN));
	//SINGLE CHOOSE BOX
	createLabel(G_str("Choose_Name"), L_CHOOSENAMES, LAB_CHOSENAMES);
	auto singletextField = createTextEdit(playerName, G_colors[playerboxFileNameIndex], L_CHOOSENAMES, -1);
	createPages("", { "BLUE", "GREEN", "PURPLE", "RON", "RED", "BLUE" }, { R_Box[0], R_Box[1], R_Box[2], R_Box[3], R_Box[4], R_Box[5] }, playerboxFileNameIndex, PG_PLAYERBOX, L_CHOOSENAMES,
		[singletextField](PageView *pgview)
	{
		singletextField->setColor(G_colors[pgview->getCurPageIndex()]);
	});
	createBtn(R_btnOn[0], R_btnOn[1], "Play", CC_CALLBACK_2(MyMenu::playCustomNow, this), B_FREERUNACCEPTANDPLAY, this->getChildByTag(L_CHOOSENAMES));
	//**LOCAL MULTIPLAYER**//
	createLabel(G_str("Multi_Player"), L_MULTIFREELOCALRUN, LAB_FREERUNMULTI);
	createPages(G_str("Choose_Mode"), { G_str("Gate_Collector"), G_str("Elimination") }, { R_pageGate, R_pageElimination }, m_currModeSelected, PG_MULTICHOSEMODE, L_MULTIFREELOCALRUN, CC_CALLBACK_1(MyMenu::m_ModeChooserPageChanged,this));
	createSpinner(std::to_string(m_currGatesNumb), G_str("Gates"), m_currGatesNumb, 24, 3, B_M_GATESLIDER, L_MULTIFREELOCALRUN);
	createSpinner(std::to_string(m_currPlayersNumber), G_str("Players"), m_currPlayersNumber, 4, 2, B_M_PLAYERSLIDER, L_MULTIFREELOCALRUN);
	createSpinner(std::to_string(m_currOpponentsNumber), G_str("Computers"), m_currOpponentsNumber, 2, 0, B_M_OPPONENTSSLIDER, L_MULTIFREELOCALRUN, CC_CALLBACK_1(MyMenu::m_OpponentsChanged, this));
	createSpinner("Medium", G_str("Difficulty"), m_currDiffValue, 2, 0,B_M_DIFFICULTYSLIDER, L_MULTIFREELOCALRUN, CC_CALLBACK_1(MyMenu::m_difficultySpinnerChanged, this));
	createBtn(R_btnOn[0], "", "Continue", CC_CALLBACK_2(MyMenu::m_continueToBoxChoose, this), B_M_CONTINUETOBOXCHOOSE, this->getChildByTag(L_MULTIFREELOCALRUN));
	//MULTIPLAYER CHOOSE NAMES//
	createLabel(G_str("Choose_Name"),L_M_CHOOSENAMES,LAB_M_CHOSENAMES);
	for (int i = T_PLAYER1NAME, j = PG_PLAYER1BOX, k = 0; k < 4; j++, i++, k++)
	{
		auto textfield = createTextEdit(m_playersNames[k],G_colors[k], L_M_CHOOSENAMES, i);
		auto page = createPages("", { "BLUE", "GREEN", "PURPLE","RON","RED","BLUE" }, { R_Box[0], R_Box[1], R_Box[2], R_Box[3] , R_Box[4] , R_Box[5] }, m_playersBoxesFileNamesIndexes[k], j, L_M_CHOOSENAMES,
			[textfield](PageView *pgview)
		{
			textfield->setColor(G_colors[pgview->getCurPageIndex()]);
		});
	}
    createBtn(R_btnOn[0], "", "Play", CC_CALLBACK_2(MyMenu::playMultiNow, this), B_M_PLAYNOW, this->getChildByTag(L_M_CHOOSENAMES));
	//*GENERAL BUTTONS*//
	createBtn(R_btnBack[0], R_btnBack[1],"", CC_CALLBACK_2(MyMenu::goBack, this), B_BACK, this);
	//tutorial
	//createSinglePlayerTutorialDialog();
	//*MODIFICATION*//
	auto backbtn = this->getChildByTag(B_BACK);
	backbtn->setPosition(2 * srodek.height*0.1f, 2 * srodek.height*0.9f);
	backbtn->setOpacity(0);
	backbtn->setVisible(false);
	this->getChildByTag(L_MAINMENU)->setOpacity(255);
	this->getChildByTag(L_MAINMENU)->setVisible(true);
	if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)this->setScale(0.3f);
	return true;
}
cocos2d::Scene* MyMenu::createScene()
{
	Scene *scena = Scene::create();
	MyMenu *menu = MyMenu::create();
	scena->addChild(menu);
	return scena;
}
//**CREATING**//
void MyMenu::createSpinner(const std::string &defaultText, const std::string &labelText,int &changinVal, int maxVal, int minVal,int tag, int parenttag ,std::function<void(cocos2d::ui::Text*)> additionalFunction)
{
	myLayout *verLayout = myLayout::create();
	verLayout->setType(0);
	myLayout *horLayout = myLayout::create();
	horLayout->setType(1);
	Text *titleText = Text::create(labelText, R_defaultFont, G_wF(25));
	Text *valueText = Text::create(defaultText, R_defaultFont, G_wF(25));
	Button *minusBtn = Button::create(R_minusBtn,"","",TextureResType::PLIST);
	Button *plusbtn = Button::create(R_plusBtn, "", "", TextureResType::PLIST);
	minusBtn->setTitleFontName(R_defaultFont);
	plusbtn->setTitleFontName(R_defaultFont);
	minusBtn->addTouchEventListener([&changinVal, minVal, valueText, additionalFunction](Ref* pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) return;
		if (changinVal-1 >= minVal)
		{
			changinVal = changinVal - 1;
			valueText->setString(String::createWithFormat("%d",changinVal)->getCString());
			if (additionalFunction != nullptr)
				additionalFunction(valueText);
		}
	});
	plusbtn->addTouchEventListener([&changinVal, maxVal, valueText ,additionalFunction](Ref* pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) return;
		if (changinVal + 1 <= maxVal)
		{
			changinVal = changinVal + 1;
			valueText->setString(String::createWithFormat("%d", changinVal)->getCString());
			if (additionalFunction != nullptr)
				additionalFunction(valueText);
		}
	});
	LinearLayoutParameter *magrinparam = LinearLayoutParameter::create();
	magrinparam->setMargin(Margin(G_wF(15), 0, G_wF(15), 0));
	magrinparam->setGravity(LinearGravity::CENTER_VERTICAL);
	LinearLayoutParameter *param = LinearLayoutParameter::create();
	param->setGravity(LinearGravity::CENTER_HORIZONTAL);
	//param->setMargin(Margin(0, G_hF(20), 0, 0));
	valueText->setLayoutParameter(magrinparam);
	verLayout->setLayoutParameter(param);
	horLayout->addWidget(minusBtn);
	horLayout->addWidgetCustomParam(valueText);
	horLayout->addWidget(plusbtn);
	verLayout->addWidget(titleText);
	verLayout->addWidget(horLayout);
	this->getChildByTag(parenttag)->addChild(verLayout,0,tag);
}
cocos2d::ui::TextField* MyMenu::createTextEdit(std::string &text, cocos2d::Color3B textColor, int parenttag, int tag, std::function<void(TextField*)> callback)
{
	Layout *bgLayout = Layout::create();
	bgLayout->setSize(Sprite::createWithSpriteFrameName(R_multiBtn)->getContentSize());
	bgLayout->setClippingType(Layout::ClippingType::SCISSOR);
	bgLayout->setClippingEnabled(true);
	bgLayout->setBackGroundImage(R_multiBtn,TextureResType::PLIST);
	auto textField = TextField::create("",R_defaultFont,G_wF(35));
	textField->setColor(textColor);
	textField->setPlaceHolder(text);
	textField->setMaxLengthEnabled(true);
	textField->setMaxLength(8);
	textField->setTextHorizontalAlignment(TextHAlignment::CENTER);
	textField->setTextVerticalAlignment(TextVAlignment::CENTER);
	textField->setNormalizedPosition(Vec2(0.5, 0.5));
	textField->addEventListener([&text, textField,callback](Ref*, TextField::EventType type)
	{
		text = textField->getStringValue();
		if (callback != nullptr) callback(textField);
	});
	LinearLayoutParameter* par = LinearLayoutParameter::create();
	par->setGravity(LINEAR_GRAVITY_CENTER_HORIZONTAL);
	bgLayout->addChild(textField);
	bgLayout->setLayoutParameter(par);
	this->getChildByTag(parenttag)->addChild(bgLayout, 1, tag);
	return textField;
}
cocos2d::ui::PageView* MyMenu::createPages(const std::string title, const std::vector<const std::string> names, const std::vector<const std::string> filepaths, int &defaultState, const int tag, int parent, std::function<void(PageView *)> callback)
{
	LinearLayoutParameter* par = LinearLayoutParameter::create();
	par->setGravity(LINEAR_GRAVITY_CENTER_HORIZONTAL);
	createLabel(title, parent, 999);
	PageView* pageView = PageView::create();
	pageView->setClippingEnabled(false);
	pageView->setBackGroundColor(Color3B(100, 100, 100));
	pageView->removeAllPages();
	int sizex = 0;
	int sizey = 0;
	int i = 0;
	Vector <Node *> layouts;
	for (auto name : names)
	{
		Layout *layout = Layout::create();
		layout->setLayoutType(LAYOUT_LINEAR_VERTICAL);
		ImageView *imageView = ImageView::create(filepaths.at(i), TextureResType::PLIST);
		Text *text = Text::create(names.at(i), R_defaultFont, G_wF(25));
		text->setLayoutParameter(par);
		imageView->setLayoutParameter(par);
		layout->addChild(text,1);
		layout->addChild(imageView,0);
		pageView->insertPage(layout, i);
		//resize
		if (sizey < imageView->getContentSize().height + text->getContentSize().height)
		{
			sizey = imageView->getContentSize().height + text->getContentSize().height;
		}
		if (sizex < imageView->getContentSize().width)
		{
			sizex = imageView->getContentSize().width;
		}
		layout->ignoreAnchorPointForPosition(true);
		layout->setAnchorPoint(Vec2(0, 0.5f));
		layouts.pushBack(imageView);
		i++;
	}
	pageView->setContentSize(Size(scaleFactor*sizex, scaleFactor*sizey));
	pageView->setLayoutParameter(par);
	PageViewController *controller = PageViewController::create();
	this->getChildByTag(parent)->addChild(pageView, 1, tag);
	controller->setControlledpageView(pageView);
    pageView->addEventListener([this,&defaultState, layouts, pageView,callback](cocos2d::Ref* pSender, cocos2d::ui::PageView::EventType type)
    {
		if (type != PageView::EventType::TURNING) return;
        layouts.at(defaultState)->runAction(ScaleTo::create(scaleTime, 1));
        defaultState = pageView->getCurPageIndex();
        layouts.at(defaultState)->runAction(Sequence::createWithTwoActions(ScaleTo::create(scaleTime, scaleFactor + 0.2f), ScaleTo::create(scaleTime / 2.0f, scaleFactor)));
		if(callback != nullptr)callback(pageView);
	});
   	pageView->scrollToPage(defaultState);
	return pageView;
}
void MyMenu::createLayout(int layoutTag)
{
	cocos2d::ui::Layout *mylater = cocos2d::ui::Layout::create();
	mylater->setPosition(Vec2(srodek.width,srodek.height*2));
	mylater->setLayoutType(LAYOUT_LINEAR_VERTICAL);
	mylater->setVisible(false);
	mylater->setOpacity(0);
	mylater->setBackGroundColorType(LAYOUT_COLOR_SOLID);
	mylater->setBackGroundColor(Color3B(128, 200, 200));
	this->addChild(mylater, 0, layoutTag);
}
void MyMenu::createLabel(const std::string &text, int parenttag, int tag)
{
	auto label = Text::create(text, R_defaultFont,G_wF(35));
	label->setFontSize(35);
	label->setString(text);
	LinearLayoutParameter* par = LinearLayoutParameter::create();
	par->setGravity(LINEAR_GRAVITY_CENTER_HORIZONTAL);
	label->setLayoutParameter(par);
	this->getChildByTag(parenttag)->addChild(label, 1, tag);
}
void MyMenu::createBtn(const std::string &imgOn, const std::string &imgOf, const std::string &btnText, cocos2d::ui::Widget::ccWidgetTouchCallback callback, int typed, cocos2d::Node  *layout)
{
	cocos2d::ui::Button* btn = cocos2d::ui::Button::create(imgOn, imgOf, "", TextureResType::PLIST);
	btn->setTitleFontName(R_defaultFont);
	btn->setTitleFontSize(35);
	btn->setTouchEnabled(true);
	if (btnText != "")
	{
		btn->setTitleText(G_str(btnText));
	}
	btn->addTouchEventListener(callback);
	LinearLayoutParameter* par = LinearLayoutParameter::create();
	par->setGravity(LINEAR_GRAVITY_CENTER_HORIZONTAL);
	btn->setLayoutParameter(par);
	layout->addChild(btn, 15, typed);
}
void MyMenu::createSlider(const char *defaultText, const float defaultval, const float maxVal, int &changingValue, Slider::ccSliderCallback callback, int parenttag, int tag, int labelTag)
{
	Slider* slider = Slider::create();
	slider->loadBarTexture(R_slider[0], TextureResType::PLIST);
	slider->loadSlidBallTextures(R_sliderDot, R_sliderDot, "", TextureResType::PLIST);
	slider->loadProgressBarTexture(R_slider[1], TextureResType::PLIST);
	slider->setCapInsets(Rect(0, 0, 0, 0));
	LinearLayoutParameter* par = LinearLayoutParameter::create();
	par->setGravity(LINEAR_GRAVITY_CENTER_HORIZONTAL);
	slider->setLayoutParameter(par);
	auto txt = Text::create(defaultText, R_defaultFont, G_wF(25));
	txt->setLayoutParameter(par);
	slider->setPercent(defaultval/maxVal * 100);
	slider->addEventListener(callback);
	changingValue = defaultval;
	this->getChildByTag(parenttag)->addChild(txt, 1, labelTag);
	this->getChildByTag(parenttag)->addChild(slider, 1, tag);
}
//**MISC**
void MyMenu::preload()
{
	G_srodek = Director::getInstance()->getVisibleSize() / 2;
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(R_MP3_punch.c_str());
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(R_res1);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(R_bgmusic.c_str());
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(R_bgmusic.c_str(),true);

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
		break;
	default:
		break;
	}
}
//**MAIN MENU EVENTS**//
void MyMenu::playSingleEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;
	show(B_BACK);
	show(L_PLAYSINGLE);
	hide(L_MAINMENU);
}
void MyMenu::playMultiEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	show(B_BACK);
	show(L_MULTIFREELOCALRUN);
	hide(L_MAINMENU);
}
void MyMenu::optionsEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	show(L_OPTIONS);
	show(B_BACK);
	hide(L_MAINMENU);
}
//**SINGLE PLAYER EVENTS**//
void MyMenu::playCarrer(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;
	hide(L_PLAYSINGLE);
	show(L_CARRER);
}
void MyMenu::playCustom(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	show(L_FREERUN);
	hide(L_PLAYSINGLE);
}
//**CUSTOM SINGLE PLAYER EVENTS**//
void MyMenu::playCustomNow(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Scene *scene;
	if (currModeSelected == 0)
	{
		scene = SingleGateWorld::createScene(currOpponentsNumber + 1, currGatesNumb, currDiffValue);
	}
	else if (currModeSelected == 1)
	{
		scene = SingleEliminationWorld::createScene(currOpponentsNumber + 1, currDiffValue);
	}
	else if (currModeSelected == 2)
	{
		scene = EndlessWorld::createScene();
	}
	World *world = (World*)scene->getChildByTag(LAYER_GAMEPLAY);
	world->setSinglePlayer(Player::create(R_Box[playerboxFileNameIndex], playerName, world->getGravitySpace(),G_colors[playerboxFileNameIndex]));
	G_dir()->replaceScene(scene);
}
void MyMenu::modeChooserPageChanged(cocos2d::ui::PageView *pages)
{
	currModeSelected = pages->getCurPageIndex();
	
	if (currModeSelected == 0)
	{
		((myLayout*)this->getChildByTag(L_FREERUN)->getChildByTag(B_GATESLIDER))->enableWidgets();
		((myLayout*)this->getChildByTag(L_FREERUN)->getChildByTag(B_DIFFICULTYSLIDER))->enableWidgets();
		((myLayout*)this->getChildByTag(L_FREERUN)->getChildByTag(B_OPPONENTSSLIDE))->enableWidgets();
	}
	else if (currModeSelected == 1)
	{
		((myLayout*)this->getChildByTag(L_FREERUN)->getChildByTag(B_GATESLIDER))->disableWidgets();
		((myLayout*)this->getChildByTag(L_FREERUN)->getChildByTag(B_DIFFICULTYSLIDER))->enableWidgets();
		((myLayout*)this->getChildByTag(L_FREERUN)->getChildByTag(B_OPPONENTSSLIDE))->enableWidgets();
	}
	else
	{
		((myLayout*)this->getChildByTag(L_FREERUN)->getChildByTag(B_GATESLIDER))->disableWidgets();
		((myLayout*)this->getChildByTag(L_FREERUN)->getChildByTag(B_DIFFICULTYSLIDER))->disableWidgets();
		((myLayout*)this->getChildByTag(L_FREERUN)->getChildByTag(B_OPPONENTSSLIDE))->disableWidgets();
	}
}
void MyMenu::difficultySpinnerChanged(cocos2d::ui::Text *textTochange)
{
	std::string str = "";
	if (currDiffValue == 0) str = "Easy";
	else if (currDiffValue == 1) str = "Medium";
	else if (currDiffValue == 2) str = "Hard";
	textTochange->setString(str);
}
//**CUSTOM MULTI PLAYER EVENTS**//
void MyMenu::m_ModeChooserPageChanged(cocos2d::ui::PageView *pages)
{
	m_currModeSelected = pages->getCurPageIndex();
	if (m_currModeSelected == 0)
	{
		((myLayout*)this->getChildByTag(L_MULTIFREELOCALRUN)->getChildByTag(B_M_GATESLIDER))->enableWidgets();
	}
	else
	{
		((myLayout*)this->getChildByTag(L_MULTIFREELOCALRUN)->getChildByTag(B_M_GATESLIDER))->disableWidgets();
	}
}
void MyMenu::m_continueToBoxChoose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED) return;
	auto choosemenu = this->getChildByTag(L_M_CHOOSENAMES);
	for (int i = T_PLAYER1NAME, j = PG_PLAYER1BOX, k = 1; k <= 4; k++, i++, j++)
	{
		if (k > m_currPlayersNumber)
		{
			//wpierdol na koniec kolejki i ukryj
			Node *child1 = choosemenu->getChildByTag(i);
			child1->setVisible(false);
		//	child1->retain();
		//	child1->removeFromParent();
		//	choosemenu->addChild(child1, 0, i);
		//	child1->release();
			Node *child2 = choosemenu->getChildByTag(j);
			child2->setVisible(false);
		//	child2->retain();
		//	child2->removeFromParent();
		//	choosemenu->addChild(child2, 0, j);
		//	child2->release();
        }
		else
		{
			choosemenu->getChildByTag(i)->setVisible(true);
			choosemenu->getChildByTag(j)->setVisible(true);
		}
        
	}
    show(L_M_CHOOSENAMES);
    hide(L_MULTIFREELOCALRUN);
    
}
void MyMenu::m_difficultySpinnerChanged(cocos2d::ui::Text *textTochange)
{
	std::string str = "";
	if (m_currDiffValue == 0) str = "Easy";
	else if (m_currDiffValue == 1) str = "Medium";
	else if (m_currDiffValue == 2) str = "Hard";
	textTochange->setString(str);
}
//*MUTLI CHOOSE BOXES *//
void MyMenu::playMultiNow(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;
	if (!m_checkPlayersOverlap()) return;
	Vector<Player*> players;
	Vector<AIOpponent*> opponentz;
	Scene *scene;
	if (currModeSelected == 0)
	{
		scene = SingleGateWorld::createScene(m_currOpponentsNumber +m_currPlayersNumber, currGatesNumb, currDiffValue);
	}
	else if (currModeSelected == 1)
	{
		scene = SingleEliminationWorld::createScene(m_currOpponentsNumber + m_currPlayersNumber, currDiffValue);
	}
	World *world = (World*)scene->getChildByTag(LAYER_GAMEPLAY);
	for (int i = 0; i < m_currPlayersNumber; i++)
	{
		players.pushBack(Player::create(R_Box[m_playersBoxesFileNamesIndexes[i]], m_playersNames[i], world->getGravitySpace(), G_colors[m_playersBoxesFileNamesIndexes[i]]));
	}
	world->setMultiplayer(players);
	G_dir()->replaceScene(scene);
}
void MyMenu::m_OpponentsChanged(cocos2d::ui::Text *textToChange)
{
	if (m_currOpponentsNumber == 0)
	{ 
		((myLayout*)this->getChildByTag(L_MULTIFREELOCALRUN)->getChildByTag(B_M_DIFFICULTYSLIDER))->disableWidgets();
	}
	else
	{
		((myLayout*)this->getChildByTag(L_MULTIFREELOCALRUN)->getChildByTag(B_M_DIFFICULTYSLIDER))->enableWidgets();
	}
}
void MyMenu::continueToBoxChoose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;
	if (currMenu == L_FREERUN)
	{
		hide(L_FREERUN);
	}
	else
	{
		hide(L_CARRER);
	}
	show(L_CHOOSENAMES);
}
//FIXES
bool MyMenu::m_checkPlayersOverlap()
{
	bool ok = true;
	for (int i = PG_PLAYER1BOX; i < PG_PLAYER1BOX+m_currPlayersNumber-1; i++)
	{
		for (int j = i + 1; j < PG_PLAYER1BOX + m_currPlayersNumber; j++)
		{
			PageView *pageviewAtI = (PageView*)this->getChildByTag(L_M_CHOOSENAMES)->getChildByTag(i);
			PageView *pageviewAtJ = (PageView*)this->getChildByTag(L_M_CHOOSENAMES)->getChildByTag(j);
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
		Layout *layout = dynamic_cast<Layout*>(this->getChildByTag(E_DIALOGREPAIR));
		if (layout == NULL)
		{
			layout = dynamic_cast<Layout*>(cocostudio::GUIReader::shareReader()->widgetFromJsonFile("Dialog_1.json"));
			m_setupAutoCorrectDialog(layout);
			this->addChild(layout, 1, E_DIALOGREPAIR);
		}
		cocostudio::ActionManagerEx::getInstance()->playActionByName("Dialog_1.json", "Animation0");
	}
	return ok;
}
void MyMenu::m_autocorrectWrongPlayerChoose()
{
	bool zajete[6] = { false, false, false, false, false, false };
	for (int i = PG_PLAYER1BOX; i < PG_PLAYER1BOX + m_currPlayersNumber; i++)
	{
		PageView *pageviewAtI = (PageView*)this->getChildByTag(L_M_CHOOSENAMES)->getChildByTag(i);
		zajete[pageviewAtI->getCurPageIndex()] = true;
	}
	for (int i = PG_PLAYER1BOX; i < PG_PLAYER1BOX + m_currPlayersNumber; i++)
	{
		PageView *pageviewAtI = (PageView*)this->getChildByTag(L_M_CHOOSENAMES)->getChildByTag(i);
		if (pageviewAtI->getColor() == Color3B(255, 255, 254))	//chujowy kolor
		{
			bool moved = false;
			int k = 1;
			const int curpost = pageviewAtI->getCurPageIndex();
			while (moved == false)
			{
				if (curpost + k <= 6 && !zajete[curpost + k])
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
void MyMenu::m_setupAutoCorrectDialog(cocos2d::ui::Layout *layout)
{
	layout->enumerateChildren("//closeBtn", [this, layout](Node *node) -> bool
	{
		Button *btn = (Button*)node;
		btn->addTouchEventListener([this, layout](Ref*, Widget::TouchEventType type)
		{
			if (type != Widget::TouchEventType::ENDED) return;
			auto usun = CallFunc::create([layout](){cocostudio::ActionManagerEx::getInstance()->releaseActions(); layout->removeFromParent(); });
			auto czekaj = DelayTime::create(1);
			cocostudio::ActionManagerEx::getInstance()->playActionByName("Dialog_1.json", "Animation1");
			this->runAction(Sequence::createWithTwoActions(czekaj, usun));
		});
		return false;
	});
	layout->enumerateChildren("//autocorrectBtn", [this, layout](Node *node)
	{
		auto btn = (Button*)node;
		btn->setTitleFontSize(G_wF(20));
		btn->addTouchEventListener([this, layout](Ref*, Widget::TouchEventType type)
		{
			if (type != Widget::TouchEventType::ENDED) return;
			auto usun = CallFunc::create([layout](){cocostudio::ActionManagerEx::getInstance()->releaseActions(); layout->removeFromParent(); });
			auto czekaj = DelayTime::create(1);
			cocostudio::ActionManagerEx::getInstance()->playActionByName("Dialog_1.json", "Animation1");
			this->runAction(Sequence::createWithTwoActions(czekaj, usun));
			this->m_autocorrectWrongPlayerChoose();
		});
		return false;
	});
	layout->enumerateChildren("//dialogText", [layout](Node *node)
	{
		auto border = utils::findChildren(*layout, "//textBorder").at(0)->getContentSize();
		auto labell = ((Text*)node);
		labell->setString(G_str("AutoCorrectDialogText"));
		labell->setTextAreaSize(border);
		labell->enableShadow();
		labell->setFontSize(G_wF(20));
		return false;
	});
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
	Layout *dialogRoot = dynamic_cast<Layout*>(cocostudio::GUIReader::shareReader()->widgetFromJsonFile(R_LevelMapUI.c_str()));
	dialogRoot->setVisible(false);
	dialogRoot->setOpacity(0);
	this->addChild(dialogRoot,2,L_CARRER);
	//listeners
	for (auto childd : utils::findChildren(*dialogRoot, "mainscroll").at(0)->getChildren())
	{
		Button *btn = (Button*)childd;
		int levelNumber = std::stoi(btn->getTitleText());
		//ZABLOKOWANE
		if (!DbReader::getInstance()->isLevelUnlocked(levelNumber))
		{
			btn->setColor(Color3B::GRAY);
			btn->addTouchEventListener([this](Ref *reff, Widget::TouchEventType type)
			{
				if (type != Widget::TouchEventType::ENDED) return;
				if (levelLockedDialog == NULL)
				{
					levelLockedDialog = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("levelLockedDialog.json");
					((Text*)utils::findChildren(*levelLockedDialog, "//DialogText").at(0))->setText(G_str("completePrev"));
					this->addChild(levelLockedDialog);
				}
				cocostudio::ActionManagerEx::getInstance()->playActionByName("levelLockedDialog.json", "AnimIn");
				levelLockedDialog->enumerateChildren("//DialogButton", [](Node *buttn)
				{
					((Button*)buttn)->addTouchEventListener([](Ref *refff, Widget::TouchEventType typee)
					{
						if (typee != Widget::TouchEventType::ENDED) return;
						cocostudio::ActionManagerEx::getInstance()->playActionByName("levelLockedDialog.json", "AnimOut");
					});
					return false;
				});
			});
			continue;
		}
		btn->addTouchEventListener([levelNumber](Ref *reff, Widget::TouchEventType type)
		{
			if (type != Widget::TouchEventType::ENDED) return;
			Scene *scene = NULL;
			switch (levelNumber)
			{
			case 1:
				scene = SingleGateWorld::createScene(4, 5, 1);
				break;
			case 2:
				scene = SingleEliminationWorld::createScene(4, 1);
				break;
			case 3:
				break;
			default:
				break;
			}
			if (scene == NULL) return;
			World *world = (World*)scene->getChildByTag(LAYER_GAMEPLAY);
			world->setSinglePlayer(Player::create(R_Box[0], "kuba", world->getGravitySpace(), G_colors[0]));
			world->setCarrierLevel(levelNumber);
			G_dir()->replaceScene(scene);
		});
	}
	DbReader::getInstance()->flush();
}
