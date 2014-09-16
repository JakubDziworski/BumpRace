#include "SplashScreen.h"
#include "VisibleRect.h"
#include "MyMenu.h"
USING_NS_CC;

Scene* SplashScreen::createScene()
{
    auto scene = Scene::create();
    auto layer = SplashScreen::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScreen::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    auto sprite = Sprite::create("splash.png");
    if(VR::width() < 512.0f)
    {
        sprite->setScale(512.0f/VR::width());
    }
    else
    {
        sprite->setScale(342.0f/VR::height());
    }
    sprite->setPosition(VR::center());
    auto lateCallFunc = CallFunc::create([]()
    	{
            auto menu = MyMenu::createScene();
    		Director::getInstance()->replaceScene(cocos2d::TransitionCrossFade::create(0.3, menu));
    	});
    auto delay = DelayTime::create(0.3f);
    this->addChild(sprite, 1);
    this->runAction(Sequence::createWithTwoActions(delay,lateCallFunc));
    return true;
}
