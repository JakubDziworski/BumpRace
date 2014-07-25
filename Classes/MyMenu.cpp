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
using namespace cocos2d;
using namespace ui;

bool MyMenu::init()
{
	if (!Layer::init())
	{
		return false;
	}
	preload();
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
	srodek = Director::getInstance()->getVisibleSize() / 2.0f;
	currMenu = L_MAINMENU;
	//*layout init*//
	createLayout(L_MAINMENU);
		createLayout(L_PLAYSINGLE);
			createLayout(L_CARRER);
			createLayout(L_FREERUN);
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
	createPages(G_str("Choose_Mode"), { G_str("Gate_Collector"), G_str("Elimination"),G_str("Endless")}, { R_pageGate, R_pageEndless, R_pageElimination }, currModeSelected, PG_CHOOSEMODE, L_FREERUN, CC_CALLBACK_2(MyMenu::modeChooserPageChanged, this));
	createSpinner(std::to_string(currGatesNumb), G_str("Gates"), currGatesNumb, 24, 3,B_GATESLIDER, L_FREERUN);
	createSpinner(std::to_string(currOpponentsNumber), G_str("Opponents"), currOpponentsNumber, maxOpponentsNumber, 1, B_OPPONENTSSLIDE, L_FREERUN);
	createSpinner("Medium", G_str("Difficulty"), currDiffValue, 2, 0, B_DIFFICULTYSLIDER,L_FREERUN, CC_CALLBACK_1(MyMenu::difficultySpinnerChanged, this));
	createBtn(R_btnOn[0], R_btnOn[1], "Play", CC_CALLBACK_2(MyMenu::playCustomNow, this), B_FREERUNACCEPTANDPLAY, this->getChildByTag(L_FREERUN));
	//**LOCAL MULTIPLAYER**//
	createLabel(G_str("Multi_Player"), L_MULTIFREELOCALRUN, LAB_FREERUNMULTI);
	createPages(G_str("Choose_Mode"), { G_str("Gate_Collector"), G_str("Elimination") }, { R_pageGate, R_pageElimination }, m_currModeSelected, PG_MULTICHOSEMODE, L_MULTIFREELOCALRUN, CC_CALLBACK_2(MyMenu::m_ModeChooserPageChanged, this));
	createSpinner(std::to_string(m_currGatesNumb), G_str("Gates"), m_currGatesNumb, 24, 3, B_M_GATESLIDER, L_MULTIFREELOCALRUN);
	createSpinner(std::to_string(m_currPlayersNumber), G_str("Players"), m_currPlayersNumber, 4, 2, B_M_PLAYERSLIDER, L_MULTIFREELOCALRUN);
	createSpinner(std::to_string(m_currOpponentsNumber), G_str("Computers"), m_currOpponentsNumber, 2, 0, B_M_OPPONENTSSLIDER, L_MULTIFREELOCALRUN, CC_CALLBACK_1(MyMenu::m_OpponentsChanged, this));
	createSpinner("Medium", G_str("Difficulty"), m_currDiffValue, 2, 0,B_M_DIFFICULTYSLIDER, L_MULTIFREELOCALRUN, CC_CALLBACK_1(MyMenu::m_difficultySpinnerChanged, this));
	createBtn(R_btnOn[0], "", "Continue", CC_CALLBACK_2(MyMenu::m_continueToBoxChoose, this), B_M_CONTINUETOBOXCHOOSE, this->getChildByTag(L_MULTIFREELOCALRUN));
	//MULTIPLAYER CHOOSE NAMES//
	createLabel(G_str("Choose_Name"),L_M_CHOOSENAMES,LAB_M_CHOSENAMES);
	createBtn(R_btnOn[0], "", "Play", CC_CALLBACK_2(MyMenu::playMultiNow, this), B_M_PLAYNOW, this->getChildByTag(L_M_CHOOSENAMES));
	for (int i = T_PLAYER1NAME, j = PG_PLAYER1BOX, k = 0; k < 4; j++, i++, k++)
	{
		createTextEdit("Player1", CC_CALLBACK_2(MyMenu::m_textFieldChanged, this), L_M_CHOOSENAMES, i);
		createPages("", { "crazy nigga", "mustache faggot", "regular guy" }, { R_Box[0], R_Box[0], R_Box[0] }, 0, j, L_M_CHOOSENAMES, CC_CALLBACK_2(MyMenu::m_pageBoxChosechanged, this));
	}
	//*GENERAL BUTTONS*//
	createBtn(R_btnBack[0], R_btnBack[1],"", CC_CALLBACK_2(MyMenu::goBack, this), B_BACK, this);
	//*MODIFICATION*//
	auto backbtn = this->getChildByTag(B_BACK);
	backbtn->setPosition(2 * srodek.height*0.1f, 2 * srodek.height*0.9f);
	backbtn->setOpacity(0);
	backbtn->setVisible(false);
	this->getChildByTag(L_MAINMENU)->setOpacity(255);
	this->getChildByTag(L_MAINMENU)->setVisible(true);
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
	Button *minusBtn = Button::create(R_minusBtn);
	Button *plusbtn = Button::create(R_plusBtn);
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
void MyMenu::createTextEdit(const std::string &text,TextField::ccTextFieldCallback callback,int parenttag, int tag)
{
	auto textField = TextField::create();
	textField->setFontSize(35);
	textField->setPlaceHolder("Player 1");
	textField->setTextHorizontalAlignment(TextHAlignment::CENTER);
	textField->setTextVerticalAlignment(TextVAlignment::CENTER);
	textField->addEventListener(callback);
	LinearLayoutParameter* par = LinearLayoutParameter::create();
	par->setGravity(LINEAR_GRAVITY_CENTER_HORIZONTAL);
	textField->setLayoutParameter(par);

	this->getChildByTag(parenttag)->addChild(textField, 1, tag);
}
void MyMenu::createPages(const std::string title,const std::vector<const std::string> names, const std::vector<const std::string> filepaths, int defaultState, const int tag, int parent, PageView::ccPageViewCallback callback)
{
	LinearLayoutParameter* par = LinearLayoutParameter::create();
	par->setGravity(LINEAR_GRAVITY_CENTER_HORIZONTAL);
	createLabel(title, parent,999);
	PageView* pageView = PageView::create();
	pageView->setClippingEnabled(false);
	pageView->setBackGroundColor(Color3B(100, 100, 100));
	auto siz = Sprite::create(filepaths.at(0))->getContentSize();
	pageView->removeAllPages();
	int i = 0;
	for (auto name : names)
	{
		Layout *layout = Layout::create();
		layout->setLayoutType(LAYOUT_LINEAR_VERTICAL);
		ImageView *imageView = ImageView::create(filepaths.at(i));
		Text *text = Text::create(names.at(i),R_defaultFont,G_wF(25));
		text->setLayoutParameter(par);
		imageView->setLayoutParameter(par);
		layout->addChild(text);
		layout->addChild(imageView);
		pageView->insertPage(layout, i);
		pageView->setContentSize(Size(1.1f*siz.width, siz.height + text->getContentSize().height));
		i++;
	}
	pageView->setLayoutParameter(par);
	pageView->addEventListener(callback);
	this->getChildByTag(parent)->addChild(pageView, 1, tag);
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
	cocos2d::ui::Button* btn = cocos2d::ui::Button::create();
	btn->setTitleFontSize(35);
	btn->setTouchEnabled(true);
	btn->loadTextures(imgOn, imgOf, "");
	if (btnText != "")
	{
		btn->setTitleText(G_str(btnText));
	}
	btn->addTouchEventListener(callback);
	LinearLayoutParameter* par = LinearLayoutParameter::create();
	par->setGravity(LINEAR_GRAVITY_CENTER_HORIZONTAL);
	btn->setLayoutParameter(par);
	layout->addChild(btn, 1, typed);
}
void MyMenu::createSlider(const char *defaultText, const float defaultval, const float maxVal, int &changingValue, Slider::ccSliderCallback callback, int parenttag, int tag, int labelTag)
{
	Slider* slider = Slider::create();
	slider->loadBarTexture(R_slider[0]);
	slider->loadSlidBallTextures(R_sliderDot, R_sliderDot, "");
	slider->loadProgressBarTexture(R_slider[1]);
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
}
void MyMenu::hide(int menutypedef)
{
	Node *nodetiHide = this->getChildByTag(menutypedef);
	FiniteTimeAction *fadeout = FadeOut::create(0.5f);
	FiniteTimeAction *disableTouch = CallFunc::create([nodetiHide](){nodetiHide->setVisible(false); });
	nodetiHide->runAction(Sequence::createWithTwoActions(fadeout, disableTouch));
}
void MyMenu::show(int menutypedef)
{
	Node *nodetoShow = this->getChildByTag(menutypedef);
	FiniteTimeAction *fadeIn = FadeIn::create(0.5f);
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

}
void MyMenu::playCustom(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	show(L_FREERUN);
	hide(L_PLAYSINGLE);
}
//**CUSTOM SINGLE PLAYER EVENTS**//
void MyMenu::playCustomNow(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (currModeSelected == 0)
	{
		auto scene = SingleGateWorld::createScene(currOpponentsNumber + 1, currGatesNumb, currDiffValue);
		World *world = (World*)scene->getChildByTag(LAYER_GAMEPLAY);
		world->setSinglePlayer(Player::create(R_Box[0], "Kuba", world->getGravitySpace()));
		G_dir()->replaceScene(scene);
	}
	else if (currModeSelected == 1)
	{
		auto scene = SingleEliminationWorld::createScene(currOpponentsNumber + 1, currDiffValue);
		World *world = (World*)scene->getChildByTag(LAYER_GAMEPLAY);
		world->setSinglePlayer(Player::create(R_Box[0], "Kuba", world->getGravitySpace()));
		G_dir()->replaceScene(scene);
	}
	else if (currModeSelected == 2)
	{
		auto scene = EndlessWorld::createScene();
		World *world = (World*)scene->getChildByTag(LAYER_GAMEPLAY);
		world->setSinglePlayer(Player::create(R_Box[0], "Kuba", world->getGravitySpace()));
		G_dir()->replaceScene(scene);
	}
}
void MyMenu::modeChooserPageChanged(cocos2d::Ref* pSender, cocos2d::ui::PageView::EventType type)
{
	PageView *pages = ((PageView*)this->getChildByTag(L_FREERUN)->getChildByTag(PG_CHOOSEMODE));
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
void MyMenu::m_ModeChooserPageChanged(cocos2d::Ref* pSender, cocos2d::ui::PageView::EventType type)
{
	PageView *pages = ((PageView*)this->getChildByTag(L_MULTIFREELOCALRUN)->getChildByTag(PG_MULTICHOSEMODE));
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
	auto choosemenu = this->getChildByTag(L_M_CHOOSENAMES);
	for (int i = T_PLAYER4NAME, j = PG_PLAYER4BOX, k = 4; k > 0; k--, i--, j--)
	{
		if (k > m_currPlayersNumber)
		{
			choosemenu->getChildByTag(i)->setVisible(false);
			choosemenu->getChildByTag(j)->setVisible(false);
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
void MyMenu::m_textFieldChanged(cocos2d::Ref *psender, cocos2d::ui::TextField::EventType type)
{

}
void MyMenu::m_pageBoxChosechanged(cocos2d::Ref* pSender, cocos2d::ui::PageView::EventType type)
{
}
void MyMenu::playMultiNow(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Vector<Player*> players;
	Vector<AIOpponent*> opponentz;
	if (currModeSelected == 0)
	{
		auto scene = SingleGateWorld::createScene(m_currOpponentsNumber +m_currPlayersNumber, currGatesNumb, currDiffValue);
		World *world = (World*)scene->getChildByTag(LAYER_GAMEPLAY);
		for (int i = 1; i <= m_currPlayersNumber; i++)
		{
			players.pushBack(Player::create(R_Box[0], String::createWithFormat("%s %d", G_str("Player").c_str(), i)->getCString(), world->getGravitySpace(),G_colors[i-1]));
		}
		world->setMultiplayer(players);
		G_dir()->replaceScene(scene);
	}
	else if (currModeSelected == 1)
	{
		auto scene = SingleEliminationWorld::createScene(m_currOpponentsNumber + m_currPlayersNumber, currDiffValue);
		World *world = (World*)scene->getChildByTag(LAYER_GAMEPLAY);
		for (int i = 1; i <= m_currPlayersNumber; i++)
		{
			players.pushBack(Player::create(R_Box[0], String::createWithFormat("%s %d", G_str("Player").c_str(), i)->getCString(), world->getGravitySpace(), G_colors[i - 1]));
		}
		world->setMultiplayer(players);
		G_dir()->replaceScene(scene);
	}
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








