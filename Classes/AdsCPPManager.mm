//
//  AdsCPPManager.cpp
//  BumpRace
//
//  Created by kuba on 04.09.2014.
//
//

#include "AdsCPPManager.h"
#include "MyAdsManager.h"
void AdsCPPManager::preload()
{
    [[MyAdsManager getInstance] preloadALL];
}
void AdsCPPManager::showIntereIstial()
{
    [[MyAdsManager getInstance] showInterstitial];
}
void AdsCPPManager::showMoreApps()
{
    [[MyAdsManager getInstance]showMoreGames];
}
void AdsCPPManager::showBanner()
{
    [[MyAdsManager getInstance] showBanner];
}
void AdsCPPManager::hideBanner()
{
    [[MyAdsManager getInstance] hideBanner];
}
void AdsCPPManager::goToUrl(const std::string &str)
{
    NSString* output = [NSString stringWithUTF8String:str.c_str()];
    [[MyAdsManager getInstance] openURLPage : output];
}
void AdsCPPManager::buyrmvAdsIos()
{
    [[MyAdsManager getInstance] rmvAdsIos];
}
void AdsCPPManager::buyAllLevelsIos()
{
    [[MyAdsManager getInstance] unlockLevelIos];
}
void AdsCPPManager::sendFlurryEvent(const std::string &str)
{
    NSString* output = [NSString stringWithUTF8String:str.c_str()];
    [[MyAdsManager getInstance] callFlurryIos : output];
}