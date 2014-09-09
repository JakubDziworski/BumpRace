/*
 * GlobalAdManager.cpp
 *
 *  Created on: 06-09-2014
 *      Author: Jakub
 */

#include "GlobalAdManager.h"
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

}

void GlobalAdManager::unlockLevel(int i)
{

}

