#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "SplashScreen.h"
#include "Globals.h"
#include "GlobalAdManager.h"
#include "MyMenu.h"
#include "screw/screw.h"
USING_NS_CC;

AppDelegate::AppDelegate() 
{
	
}

AppDelegate::~AppDelegate() 
{
	G_strings->release();
}
bool AppDelegate::applicationDidFinishLaunching() {
    GlobalAdManager::sendFlurryEvent("App did Finish Launching");
	screw::facebook::Session::start();
	GlobalAdManager::preloadAds();
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
		director->setOpenGLView(glview);
    }
	srand(time(NULL));
    // turn on display FPS
   // director->setDisplayStats(true);
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    // create a scene. it's an autorelease object
	G_initLanguage();
	GlobalAdManager::checkBought();
	auto screenSize = glview->getFrameSize();
	prepareImageRes(screenSize);
    Scene* scene;
    if(CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    {
    	scene = SplashScreen::createScene();
    }
    else
    {
    	scene = MyMenu::createScene();
    }
    director->runWithScene(scene);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() 
{
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AppDelegate::prepareImageRes(cocos2d::Size scrSize)
{
	std::vector<std::string> resPaths;
	GLView *glView = Director::getInstance()->getOpenGLView();
	if (scrSize.width <= 640)
	{
		GlobalAdManager::sendFlurryEvent("res SD");
		resPaths.push_back("SD");
	}
	else if (scrSize.width <= 1280) //IPAD RES
	{
		GlobalAdManager::sendFlurryEvent("res HD");
		resPaths.push_back("HD");
		Director::getInstance()->setContentScaleFactor(2);
	}
	else 
	{
		GlobalAdManager::sendFlurryEvent("res HDR");
		resPaths.push_back("HDR");
		Director::getInstance()->setContentScaleFactor(4);
	}
	glView->setDesignResolutionSize(512,342, ResolutionPolicy::NO_BORDER);
	cocos2d::FileUtils::getInstance()->setSearchPaths(resPaths);
}
