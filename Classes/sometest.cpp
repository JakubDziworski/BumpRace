#include "sometest.h"
#define COCOS2D_DEBUG 2
USING_NS_CC;

cocos2d::Scene * sometest::createScene()
{
	Scene *scene = Scene::create();
	scene->addChild(sometest::create());
	return scene;
}
bool sometest::init()
{
	if (!Layer::create())
	{
		return false;
	}
	for (int i = 0; i < 5000; i++)
	{
		CCLOG("creating %d ", i);
		//auto lbl = Label::create("SOMETHING", "Aleo-BoldItalic.ttf", 25);
		//lbl->enableShadow();
		auto lbl = Label::createWithBMFont("aleoBoldItalic.fnt", "SOMETHING");
		lbl->setFontSize(15);
		lbl->setNormalizedPosition(Vec2(0.5f, 0.5f));
		this->addChild(lbl);
	}
	
	//this->schedule(schedule_selector(sometest::createWithBMF));
	return true;
}
void sometest::createWithTTF(float dt)
{
	time += dt;
	if (i == 400)
	{
		
		time = 0;
		this->unschedule(schedule_selector(sometest::createWithTTF));
	}
	auto lbl = Label::create("SOMETHING", "Aleo-BoldItalic.ttf",15);
	lbl->enableShadow();
	this->addChild(lbl);
	i++;
}
void sometest::createWithBMF(float dt)
{
	time += dt;
	if (i == 400)
	{
		CCLOG("time to create 400 BMFS = %.2f", time);
		time = 0; i = 0;
		this->unschedule(schedule_selector(sometest::createWithBMF));
	}
	auto lbl = Label::createWithBMFont("aleoBoldItalic.fnt", "SOMETHING");
	lbl->setFontSize(15);
	this->addChild(lbl);
	i++;
}

