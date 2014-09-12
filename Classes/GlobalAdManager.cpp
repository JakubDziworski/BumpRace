/*
 * GlobalAdManager.cpp
 *
 *  Created on: 06-09-2014
 *      Author: Jakub
 */

#include "GlobalAdManager.h"
#include "cocos2d.h"
#include "Macros.h"
#include "MyMenu.h"
#define COCOS2D_DEBUG 2;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "AdsCPPManager.h"
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "AdsAndroidManager.h"
#endif
void GlobalAdManager::preloadAds() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    {
        AdsAndroidManager::preloadAndroid();
    }
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
        AdsCPPManager::preload();
    }
#endif
}

void GlobalAdManager::showInteristial() {
if(!DbReader::getInstance()->areAdsEnabled()) return;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    {
    	AdsAndroidManager::showInteristialAndroid();
    }
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
        AdsCPPManager::showIntereIstial();
    }
#endif
}

void GlobalAdManager::showMoreGames()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    {
    	AdsAndroidManager::showMoreGamesAndroid();
    }
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
        AdsCPPManager::showMoreApps();
    }
#endif
}
void GlobalAdManager::showBanner()
{
if(!DbReader::getInstance()->areAdsEnabled()) return;
#if  (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    {
    	AdsAndroidManager::showBannerAndroid();
    }
#endif
#if  (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
        AdsCPPManager::showBanner();
    }
#endif
}
void GlobalAdManager::hideBanner()
{
#if  (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    {
    	AdsAndroidManager::hideBannerAndroid();
    }
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
        AdsCPPManager::hideBanner();
    }
#endif
}
void GlobalAdManager::goToLink(const std::string &str)
{
#if  (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    {
    	AdsAndroidManager::goToLinkAndroid(str);
    }
#endif
#if  (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
        AdsCPPManager::goToUrl(str);
    }
#endif
}

void GlobalAdManager::rmvAds()
{
#if  (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    {
    	AdsAndroidManager::rmvAdsAndroid();
    }
#endif
#if  (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
        AdsCPPManager::buyrmvAdsIos();
    }
#endif
}

void GlobalAdManager::unlockLevel(int i)
{
#if  (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    {
    	AdsAndroidManager::unlockLevelAndroid();
    }
#endif
#if  (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
        AdsCPPManager::buyAllLevelsIos();
    }
#endif
}
void GlobalAdManager::checkBought()
{
#if  (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    {
		AdsAndroidManager::checkPurchases();
    }
#endif
#if  (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
       //nothing
    }
#endif
}
void GlobalAdManager::onBoughtLevels()
{
    DbReader::getInstance()->setLevelsEnabledAll(true);
    auto scene = cocos2d::Director::getInstance()->getRunningScene();
    if(!scene) return;
    MyMenu *menu = dynamic_cast<MyMenu*>(scene->getChildByTag(LAYER_HUD));
    if(!menu) return;
    CCLOG("BEFORE TRYING TO ACCES LVEL MAP");
    menu->createLevelMapUI();
    CCLOG("AFTER TRYING TO ACCES LVEL MAP");
}
void GlobalAdManager::onBoughtRemoveAds()
{
    DbReader::getInstance()->setAdsEnabled(false);
}

