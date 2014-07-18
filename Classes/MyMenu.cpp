#include "MyMenu.h"
#include "Macros.h"
#include "SingleGateWorld.h"
#include "SimpleAudioEngine.h"
#include "Paths.h"
#include "Globals.h"
using namespace cocos2d;
using namespace ui;

bool MyMenu::init()
{
	if (!Layer::init())
	{
		return false;
	}
	preload();
	srodek = Director::getInstance()->getVisibleSize() / 2.0f;
	currMenu = L_MAINMENU;
	//*layout init*//
	createLayout(L_MAINMENU);
		createLayout(L_PLAYSINGLE);
			createLayout(L_CARRER);
			createLayout(L_FREERUN);
		createLayout(L_PLAYMULTI);
			createLayout(L_MULTIFREELOCALRUN);
		createLayout(L_OPTIONS);
	//*MAIN MENU BUTTNS*//
	createBtn("btnOn.png", "btnOf.png", "Single Player", CC_CALLBACK_2(MyMenu::playSingleEvent, this), B_PLAYSINGLE, this->getChildByTag(L_MAINMENU));
	createBtn("btnOn.png", "btnOf.png", "Multi Player", CC_CALLBACK_2(MyMenu::playMultiEvent, this), B_PLAYMULTI, this->getChildByTag(L_MAINMENU));
	createBtn("btnOn.png", "btnOf.png", "Options", CC_CALLBACK_2(MyMenu::optionsEvent, this), B_OPTIONS, this->getChildByTag(L_MAINMENU));
	//*SINGLE PLAYER BUTTONS*//
	createLabel("SINGLE PLAYER MODE", L_PLAYSINGLE, LAB_SINGLEPLAYER);
	createBtn("btnOn.png", "btnOf.png", "Carrer", CC_CALLBACK_2(MyMenu::playCarrer, this), B_CARRER, this->getChildByTag(L_PLAYSINGLE));
	createBtn("btnOn.png", "btnOf.png", "FreeRun", CC_CALLBACK_2(MyMenu::playCustom, this), B_FREERUN, this->getChildByTag(L_PLAYSINGLE));
	//*CARRER BUTTONS*//
	//TODO
	//*FRE RUN BUTTONS*//
	createLabel("Free run:", L_FREERUN, LAB_FREERUN);
	createPages("Choose Mode",{ "Gate Collector", "Elimination", "Endless Run" }, { "pageGate.png", "pageEndlessRun.png", "PageElimination.png" }, 0, PG_CHOOSEMODE, L_FREERUN, CC_CALLBACK_2(MyMenu::modeChooserPageChanged, this));
	createSlider("Difficulty:Medium", 1, 2, currDiffValue, CC_CALLBACK_2(MyMenu::difficultySliderChanged, this), L_FREERUN, B_DIFFICULTYSLIDER, LAB_DIFFICULTYLABEL);
	createSlider("Opponents:3", 3, maxOpponentsNumber-1,currOpponentsNumber , CC_CALLBACK_2(MyMenu::opponentsSliderChanged, this), L_FREERUN, B_AMOUNTOFOPPONENTSSLIDE, LAB_OPPONENTSNUMBERSLIDER);
	createSlider("Gates:7", 7, 24, currGatesNumb, CC_CALLBACK_2(MyMenu::gatesSliderChanged, this), L_FREERUN, B_GATESLIDER, LAB_GATESNUMBER);
	createBtn("btnOn.png", "btnOf.png", "Play!", CC_CALLBACK_2(MyMenu::playCustomNow, this), B_FREERUNACCEPTANDPLAY, this->getChildByTag(L_FREERUN));
	//*GENERAL BUTTONS*//
	createBtn("btnBackOn.png", "btnBackOf.png", "", CC_CALLBACK_2(MyMenu::goBack, this), B_BACK, this);
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
void MyMenu::createPages(const std::string title,const std::vector<const std::string> names, const std::vector<const std::string> filepaths, int defaultState, const int tag, int parent, PageView::ccPageViewCallback callback)
{
	LinearLayoutParameter* par = LinearLayoutParameter::create();
	par->setGravity(LINEAR_GRAVITY_CENTER_HORIZONTAL);
	createLabel("Choose Mode:", L_FREERUN, LAB_CHOOSEMODE);
	((Text*)this->getChildByTag(L_FREERUN)->getChildByTag(LAB_CHOOSEMODE))->setFontSize(25);
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
		Text *text = Text::create();
		text->setString(names.at(i));
		text->setFontSize(25);
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
	auto label = Text::create();
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
	btn->setTitleText(btnText);
	btn->addTouchEventListener(callback);
	LinearLayoutParameter* par = LinearLayoutParameter::create();
	par->setGravity(LINEAR_GRAVITY_CENTER_HORIZONTAL);
	btn->setLayoutParameter(par);
	layout->addChild(btn, 1, typed);
}
void MyMenu::createSlider(const std::string &defaultText, const float defaultval, const float maxVal, int &changingValue, Slider::ccSliderCallback callback, int parenttag, int tag, int labelTag)
{
	Slider* slider = Slider::create();
	slider->loadBarTexture("slider.png");
	slider->loadSlidBallTextures("sliderDot.png", "sliderDot.png", "");
	slider->loadProgressBarTexture("sliderActive.png");
	slider->setCapInsets(Rect(0, 0, 0, 0));
	LinearLayoutParameter* par = LinearLayoutParameter::create();
	par->setGravity(LINEAR_GRAVITY_CENTER_HORIZONTAL);
	slider->setLayoutParameter(par);
	auto txt = Text::create();
	txt->setLayoutParameter(par);
	txt->setFontSize(25);
	slider->setPercent(defaultval/maxVal * 100);
	txt->setString(defaultText);
	slider->addEventListener(callback);
	changingValue = defaultval;
	this->getChildByTag(parenttag)->addChild(txt, 1, labelTag);
	this->getChildByTag(parenttag)->addChild(slider, 1, tag);
}

void MyMenu::optionsEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	show(L_OPTIONS);
	show(B_BACK);
	hide(L_MAINMENU);
}

void MyMenu::playSingleEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	show(B_BACK);
	show(L_PLAYSINGLE);
	hide(L_MAINMENU);
}

void MyMenu::playMultiEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{

}

void MyMenu::playCarrer(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{

}

void MyMenu::playElimination(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{

}

void MyMenu::playEndless(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{

}

void MyMenu::playCustom(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	show(L_FREERUN);
	hide(L_PLAYSINGLE);
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
		hide(L_PLAYMULTI);
		break;
	default:
		break;
	}
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
	nodetoShow->runAction(Sequence::create(doNothung,enableTouch, fadeIn,NULL));
	currMenu = menutypedef;
}

void MyMenu::playCustomNow(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Director::getInstance()->replaceScene(SingleGateWorld::createScene(currOpponentsNumber + 1, currGatesNumb, currDiffValue));
}

void MyMenu::preload()
{
	G_srodek = Director::getInstance()->getVisibleSize() / 2;
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(R_MP3_punch.c_str());
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Res1.plist");
}

void MyMenu::opponentsSliderChanged(cocos2d::Ref* stg, Slider::EventType evnt)
{
	Slider *slid = ((Slider*)this->getChildByTag(L_FREERUN)->getChildByTag(B_AMOUNTOFOPPONENTSSLIDE));
	Text* lbl = ((Text*)this->getChildByTag(L_FREERUN)->getChildByTag(LAB_OPPONENTSNUMBERSLIDER));
	//
	const float percent = slid->getPercent();
	int nearest = std::round(percent*(maxOpponentsNumber-1) / 100.0f);
	slid->setPercent(nearest / (maxOpponentsNumber-1) * 100);
	lbl->setString(String::createWithFormat("Opponents:%d", nearest+1)->getCString());
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
	lbl->setString(String::createWithFormat("Difficulty:%s", poziom.c_str())->getCString());
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
	lbl->setString(String::createWithFormat("Gates:%d", nearest+1)->getCString());
	currGatesNumb = nearest+1;
}












