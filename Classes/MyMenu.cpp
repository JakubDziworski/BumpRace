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
	createBtn("btnOn.png", "btnOf.png", "Single_Player", CC_CALLBACK_2(MyMenu::playSingleEvent, this), B_PLAYSINGLE, this->getChildByTag(L_MAINMENU));
	createBtn("btnOn.png", "btnOf.png", "Multi_Player", CC_CALLBACK_2(MyMenu::playMultiEvent, this), B_PLAYMULTI, this->getChildByTag(L_MAINMENU));
	createBtn("btnOn.png", "btnOf.png", "Options", CC_CALLBACK_2(MyMenu::optionsEvent, this), B_OPTIONS, this->getChildByTag(L_MAINMENU));
	//*SINGLE PLAYER BUTTONS*//
	createLabel(G_str("Single_Player"), L_PLAYSINGLE, LAB_SINGLEPLAYER);
	createBtn("btnOn.png", "btnOf.png", "Carrer", CC_CALLBACK_2(MyMenu::playCarrer, this), B_CARRER, this->getChildByTag(L_PLAYSINGLE));
	createBtn("btnOn.png", "btnOf.png", "FreeRun", CC_CALLBACK_2(MyMenu::playCustom, this), B_FREERUN, this->getChildByTag(L_PLAYSINGLE));
	//*CARRER BUTTONS*//
	//TODO
	//*FRE RUN BUTTONS*//
	createLabel(G_str("FreeRun"), L_FREERUN, LAB_FREERUN);
	createPages(G_str("Choose_Mode"), { G_str("Gate_Collector"), G_str("Elimination"),G_str("Endless")}, { "pageGate.png", "pageEndlessRun.png", "PageElimination.png" }, currModeSelected, PG_CHOOSEMODE, L_FREERUN, CC_CALLBACK_2(MyMenu::modeChooserPageChanged, this));
	createSlider(String::createWithFormat("%s:7",G_str("Gates").c_str())->getCString(), currGatesNumb, 24, currGatesNumb, CC_CALLBACK_2(MyMenu::gatesSliderChanged, this), L_FREERUN, B_GATESLIDER, LAB_GATESNUMBER);
	createSlider(String::createWithFormat("%s:4", G_str("Opponents").c_str())->getCString(), currOpponentsNumber, maxOpponentsNumber - 1, currOpponentsNumber, CC_CALLBACK_2(MyMenu::opponentsSliderChanged, this), L_FREERUN, B_AMOUNTOFOPPONENTSSLIDE, LAB_OPPONENTSNUMBERSLIDER);
	createSlider(String::createWithFormat("%s:Medium", G_str("Difficulty").c_str())->getCString(), currDiffValue, 2, currDiffValue, CC_CALLBACK_2(MyMenu::difficultySliderChanged, this), L_FREERUN, B_DIFFICULTYSLIDER, LAB_DIFFICULTYLABEL);
	createBtn("btnOn.png", "btnOf.png", "Play", CC_CALLBACK_2(MyMenu::playCustomNow, this), B_FREERUNACCEPTANDPLAY, this->getChildByTag(L_FREERUN));
	//**LOCAL MULTIPLAYER**//
	createLabel(G_str("Multi_Player"), L_MULTIFREELOCALRUN, LAB_FREERUNMULTI);
	createPages(G_str("Choose_Mode"), { G_str("Gate_Collector"), G_str("Elimination") }, { "pageGate.png", "pageElimination.png" }, m_currModeSelected, PG_MULTICHOSEMODE, L_MULTIFREELOCALRUN, CC_CALLBACK_2(MyMenu::m_ModeChooserPageChanged, this));
	createSlider(String::createWithFormat("%s:7", G_str("Gates").c_str())->getCString(), m_currGatesNumb, 24, m_currGatesNumb, CC_CALLBACK_2(MyMenu::m_GatesSliderChanged, this), L_MULTIFREELOCALRUN, B_M_GATESLIDER, LAB_M_GATESLIDER);
	createSlider(String::createWithFormat("%s:2", G_str("Players").c_str())->getCString(), m_currPlayersNumber, 4, m_currPlayersNumber, CC_CALLBACK_2(MyMenu::m_PlayerSliderChanged, this), L_MULTIFREELOCALRUN, B_M_PLAYERSLIDER, LAB_M_PLAYERSNUMBER);
	createSlider(String::createWithFormat("%s:1", G_str("Computers").c_str())->getCString(), m_currOpponentsNumber, 4, m_currOpponentsNumber, CC_CALLBACK_2(MyMenu::m_OpponentsSliderChanged, this), L_MULTIFREELOCALRUN, B_M_OPPONENTSSLIDER, LAB_M_OPPONENTSNUMBER);
	createSlider(String::createWithFormat("%s:Medium", G_str("Difficulty").c_str())->getCString(), m_currDiffValue, 4, m_currDiffValue, CC_CALLBACK_2(MyMenu::m_DifficultySliderChanged, this), L_MULTIFREELOCALRUN, B_M_DIFFICULTYSLIDER, LAB_M_DIFFLABELSLIDER);
	createBtn("btnOn.png", "", "Continue", CC_CALLBACK_2(MyMenu::m_continueToBoxChoose, this), B_M_CONTINUETOBOXCHOOSE, this->getChildByTag(L_MULTIFREELOCALRUN));
	//MULTIPLAYER CHOOSE NAMES//
	createLabel(G_str("Choose_Name"),L_M_CHOOSENAMES,LAB_M_CHOSENAMES);
	createBtn("btnOn.png", "", "Play", CC_CALLBACK_2(MyMenu::playMultiNow, this), B_M_PLAYNOW, this->getChildByTag(L_M_CHOOSENAMES));
	for (int i = T_PLAYER1NAME, j = PG_PLAYER1BOX, k = 0; k < 4; j++, i++, k++)
	{
		createTextEdit("Player1", CC_CALLBACK_2(MyMenu::m_textFieldChanged, this), L_M_CHOOSENAMES, i);
		createPages("", { "crazy nigga", "mustache faggot", "regular guy" }, { "box.png", "box.png", "box.png" }, 0, j, L_M_CHOOSENAMES, CC_CALLBACK_2(MyMenu::m_pageBoxChosechanged, this));
	}
	//*GENERAL BUTTONS*//
	createBtn("btnBackOn.png", "btnBackOf.png","", CC_CALLBACK_2(MyMenu::goBack, this), B_BACK, this);
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
	slider->loadBarTexture("slider.png");
	slider->loadSlidBallTextures("sliderDot.png", "sliderDot.png", "");
	slider->loadProgressBarTexture("sliderActive.png");
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
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Res1.plist");
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
		world->setSinglePlayer(Player::create("Boxx.png", "Kuba", world->getGravitySpace()));
		G_dir()->replaceScene(scene);
	}
	else if (currModeSelected == 1)
	{
		auto scene = SingleEliminationWorld::createScene(currOpponentsNumber + 1, currDiffValue);
		World *world = (World*)scene->getChildByTag(LAYER_GAMEPLAY);
		world->setSinglePlayer(Player::create("Boxx.png", "Kuba", world->getGravitySpace()));
		G_dir()->replaceScene(scene);
	}
	else if (currModeSelected == 2)
	{
		auto scene = EndlessWorld::createScene();
		World *world = (World*)scene->getChildByTag(LAYER_GAMEPLAY);
		world->setSinglePlayer(Player::create("Boxx.png", "Kuba", world->getGravitySpace()));
		G_dir()->replaceScene(scene);
	}
}
void MyMenu::opponentsSliderChanged(cocos2d::Ref* stg, Slider::EventType evnt)
{
	Slider *slid = ((Slider*)this->getChildByTag(L_FREERUN)->getChildByTag(B_AMOUNTOFOPPONENTSSLIDE));
	Text* lbl = ((Text*)this->getChildByTag(L_FREERUN)->getChildByTag(LAB_OPPONENTSNUMBERSLIDER));
	//
	const float percent = slid->getPercent();
	int nearest = std::round(percent*(maxOpponentsNumber-1) / 100.0f);
	slid->setPercent(nearest / (maxOpponentsNumber-1) * 100);
	lbl->setString(String::createWithFormat("%s:%d",G_str("Opponents").c_str(), nearest+1)->getCString());
	currOpponentsNumber = nearest+1;
}
void MyMenu::difficultySliderChanged(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type)
{
	Slider *slid = ((Slider*)this->getChildByTag(L_FREERUN)->getChildByTag(B_DIFFICULTYSLIDER));
	Text* lbl = ((Text*)this->getChildByTag(L_FREERUN)->getChildByTag(LAB_DIFFICULTYLABEL));
	
	const float maxdiffLevel = 2;
	const float percent = slid->getPercent();
	int nearest = std::round(percent*maxdiffLevel / 100.0f);
	slid->setPercent(nearest/maxdiffLevel*100);
	std::string poziom = "";
	if (nearest == 0) poziom = "Easy";
	else if (nearest == 1) poziom = "Medium";
	else if (nearest == 2) poziom = "Hard";
	lbl->setString(String::createWithFormat("%s:%s",G_str("Difficulty").c_str(), poziom.c_str())->getCString());
	currDiffValue = nearest;
}
void MyMenu::modeChooserPageChanged(cocos2d::Ref* pSender, cocos2d::ui::PageView::EventType type)
{
	PageView *pages = ((PageView*)this->getChildByTag(L_FREERUN)->getChildByTag(PG_CHOOSEMODE));
	currModeSelected = pages->getCurPageIndex();
}
void MyMenu::gatesSliderChanged(cocos2d::Ref* pSender, cocos2d::ui::Slider::EventType type)
{
	Slider *slid = ((Slider*)this->getChildByTag(L_FREERUN)->getChildByTag(B_GATESLIDER));
	Text* lbl = ((Text*)this->getChildByTag(L_FREERUN)->getChildByTag(LAB_GATESNUMBER));

	const float maxdiffLevel = 24;
	const float percent = slid->getPercent();
	int nearest = std::round(percent*maxdiffLevel / 100.0f);
	slid->setPercent(nearest / maxdiffLevel * 100);
	lbl->setString(String::createWithFormat("%s:%d", G_str("Gates").c_str(), nearest + 1)->getCString());
	currGatesNumb = nearest+1;
}
//**CUSTOM MULTI PLAYER EVENTS**//
void MyMenu::m_PlayerSliderChanged(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type)
{

	Slider *slid = ((Slider*)this->getChildByTag(L_MULTIFREELOCALRUN)->getChildByTag(B_M_PLAYERSLIDER));
	Text* lbl = ((Text*)this->getChildByTag(L_MULTIFREELOCALRUN)->getChildByTag(LAB_M_PLAYERSNUMBER));
	//
	const float percent = slid->getPercent();
	int nearest = std::round(percent*(m_maxPlayersNumber - 2) / 100.0f);
	slid->setPercent(nearest / (m_maxPlayersNumber - 2) * 100);
	lbl->setString(String::createWithFormat("%s:%d",G_str("Players").c_str(), nearest + 2)->getCString());
	m_currPlayersNumber = nearest + 2;
}
void MyMenu::m_OpponentsSliderChanged(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type)
{

	Slider *slid = ((Slider*)this->getChildByTag(L_MULTIFREELOCALRUN)->getChildByTag(B_M_OPPONENTSSLIDER));
	Text* lbl = ((Text*)this->getChildByTag(L_MULTIFREELOCALRUN)->getChildByTag(LAB_M_OPPONENTSNUMBER));
	//
	const float percent = slid->getPercent();
	int nearest = std::round(percent*(m_maxOpponentsNumber) / 100.0f);
	slid->setPercent(nearest / (m_maxOpponentsNumber) * 100);
	lbl->setString(String::createWithFormat("%s:%d",G_str("Computers").c_str(), nearest)->getCString());
	m_currOpponentsNumber = nearest;
}
void MyMenu::m_DifficultySliderChanged(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type)
{
	Slider *slid = ((Slider*)this->getChildByTag(L_MULTIFREELOCALRUN)->getChildByTag(B_M_DIFFICULTYSLIDER));
	Text* lbl = ((Text*)this->getChildByTag(L_MULTIFREELOCALRUN)->getChildByTag(LAB_M_DIFFLABELSLIDER));

	const float maxdiffLevel = 2;
	const float percent = slid->getPercent();
	int nearest = std::round(percent*maxdiffLevel / 100.0f);
	slid->setPercent(nearest / maxdiffLevel * 100);
	std::string poziom = "";
	if (nearest == 0) poziom = "Easy";
	else if (nearest == 1) poziom = "Medium";
	else if (nearest == 2) poziom = "Hard";
	lbl->setString(String::createWithFormat("%s:%s",G_str("Difficulty").c_str(), poziom.c_str())->getCString());
	m_currDiffValue = nearest;
}
void MyMenu::m_ModeChooserPageChanged(cocos2d::Ref* pSender, cocos2d::ui::PageView::EventType type)
{
	PageView *pages = ((PageView*)this->getChildByTag(L_MULTIFREELOCALRUN)->getChildByTag(PG_MULTICHOSEMODE));
	currModeSelected = pages->getCurPageIndex();
}
void MyMenu::m_GatesSliderChanged(cocos2d::Ref* pSender, cocos2d::ui::Slider::EventType type)
{
	Slider *slid = ((Slider*)this->getChildByTag(L_MULTIFREELOCALRUN)->getChildByTag(B_M_GATESLIDER));
	Text* lbl = ((Text*)this->getChildByTag(L_MULTIFREELOCALRUN)->getChildByTag(LAB_M_GATESLIDER));

	const float maxdiffLevel = 24;
	const float percent = slid->getPercent();
	int nearest = std::round(percent*maxdiffLevel / 100.0f);
	slid->setPercent(nearest / maxdiffLevel * 100);
	lbl->setString(String::createWithFormat("%s:%d",G_str("Gates").c_str(), nearest + 1)->getCString());
	m_currGatesNumb = nearest + 1;
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
			players.pushBack(Player::create("Box.png", String::createWithFormat("%s %d",G_str("Player").c_str(), i)->getCString(), world->getGravitySpace()));
		}
		world->setMultiplayer(players);
		G_dir()->replaceScene(scene);
	}
	else if (currModeSelected == 1)
	{
		auto scene = SingleEliminationWorld::createScene(m_currOpponentsNumber + m_currPlayersNumber, currDiffValue);
		World *world = (World*)scene->getChildByTag(LAYER_GAMEPLAY);
		world->setSinglePlayer(Player::create("Boxx.png", "Kuba", world->getGravitySpace()));
		G_dir()->replaceScene(scene);
	}
}









