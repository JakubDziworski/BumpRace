#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "MyMenu.h"
#include "Globals.h"
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
	//screw::facebook::Session::start();
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
		director->setOpenGLView(glview);
    }
	srand(time(NULL));
    // turn on display FPS
    director->setDisplayStats(true);
	//if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) glview->setFrameSize(1280, 720);
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    // create a scene. it's an autorelease object
	G_initLanguage();
	auto screenSize = glview->getFrameSize();
	prepareImageRes(screenSize);
    auto scene = MyMenu::createScene();
    director->runWithScene(scene);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AppDelegate::prepareImageRes(cocos2d::Size scrSize)
{
	std::vector<std::string> resPaths;
	GLView *glView = Director::getInstance()->getOpenGLView();
	if (scrSize.width <= 512) 
	{
		resPaths.push_back("SD");
	}
	else if (scrSize.width <= 1024) //IPAD RES
	{
		resPaths.push_back("HD");
		Director::getInstance()->setContentScaleFactor(2);
	}
	else 
	{
		resPaths.push_back("HD");
		Director::getInstance()->setContentScaleFactor(2);
	}
	glView->setDesignResolutionSize(512,342, ResolutionPolicy::NO_BORDER);
	cocos2d::FileUtils::getInstance()->setSearchPaths(resPaths);
}
