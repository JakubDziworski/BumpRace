#include "MyMenu.h"
#include "Macros.h"
#include "CarrerWorld.h"
#include "SimpleAudioEngine.h"
#include "Paths.h"
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
	createLayout(L_PLAYMULTI);
	createLayout(L_OPTIONS);
	createLayout(L_FREERUN);
	//*MAIN MENU BUTTNS*//
	createBtn("btnOn.png", "btnOf.png", "Single Player", CC_CALLBACK_2(MyMenu::playSingleEvent, this), B_PLAYSINGLE, this->getChildByTag(L_MAINMENU));
	createBtn("btnOn.png", "btnOf.png", "Multi Player", CC_CALLBACK_2(MyMenu::playMultiEvent, this), B_PLAYMULTI, this->getChildByTag(L_MAINMENU));
	createBtn("btnOn.png", "btnOf.png", "Options", CC_CALLBACK_2(MyMenu::optionsEvent, this), B_BACK, this->getChildByTag(L_MAINMENU));
	//*SINGLE PLAYER BUTTONS*//
	createBtn("btnOn.png", "btnOf.png", "Carrer", CC_CALLBACK_2(MyMenu::playCarrer, this), B_CARRER, this->getChildByTag(L_PLAYSINGLE));
	createBtn("btnOn.png", "btnOf.png", "Elimination", CC_CALLBACK_2(MyMenu::playElimination, this), B_ELIMINATION, this->getChildByTag(L_PLAYSINGLE));
	createBtn("btnOn.png", "btnOf.png", "Endless Run", CC_CALLBACK_2(MyMenu::playEndless, this), B_ENDLESS_RUN, this->getChildByTag(L_PLAYSINGLE));
	createBtn("btnOn.png", "btnOf.png", "FreeRun", CC_CALLBACK_2(MyMenu::playCustom, this), B_FREERUN, this->getChildByTag(L_PLAYSINGLE));
	//*FRE RUN BUTTONS*//
	createSlider(0, 200, L_FREERUN, B_DIFFICULTYSLIDER, B_DIFFICULTYLABELSLIDER);
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
void MyMenu::createSlider(int minvalue, int maxvalue, int parenttag,int tag,int labelTag)
{
	Slider* slider = Slider::create();
	slider->loadBarTexture("slider.png");
	slider->loadSlidBallTextures("sliderDot.png", "sliderDot.png", "");
	slider->loadProgressBarTexture("sliderActive.png");
	slider->setCapInsets(Rect(0, 0, 0, 0));
	LinearLayoutParameter* par = LinearLayoutParameter::create();
	par->setGravity(LINEAR_GRAVITY_CENTER_HORIZONTAL);
	slider->setLayoutParameter(par);
	auto dispValLabel = Label::create();
	dispValLabel->setSystemFontSize(35);
	dispValLabel->setPosition(slider->getContentSize().width / 2.0f, slider->getContentSize().height);
	dispValLabel->setAnchorPoint(Vec2(0.5, 0));
	slider->addEventListenerSlider(this, sliderpercentchangedselector(MyMenu::sliderValueChanged));
	slider->addChild(dispValLabel, 1, labelTag);
	this->getChildByTag(parenttag)->addChild(slider, 1, tag);
}

void MyMenu::optionsEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	/*show(L_OPTIONS);
	hide(L_MAINMENU);*/
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

void MyMenu::sliderValueChanged(cocos2d::Ref* stg, cocos2d::ui::SliderEventType* evnt)
{
	
	Slider *slid = ((Slider*)this->getChildByTag(L_FREERUN)->getChildByTag(B_DIFFICULTYSLIDER));
	Label* lbl = ((Label*)slid->getChildByTag(B_DIFFICULTYLABELSLIDER));
	//
	const float percent = slid->getPercent();
	int nearest = std::round(percent / maxOpponentsNumber);
	slid->setPercent(nearest*maxOpponentsNumber);
	lbl->setString(String::createWithFormat("Opponents:%d", nearest)->getCString());
	currOpponentsNumber = nearest;
}

void MyMenu::playCustomNow(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Director::getInstance()->replaceScene(CarrerWorld::createScene(currOpponentsNumber));
}

void MyMenu::preload()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(R_MP3_punch.c_str());
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Res1.plist");
}







