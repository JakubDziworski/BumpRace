//
//  AdsCPPManager.h
//  BumpRace
//
//  Created by kuba on 04.09.2014.
//
//

#ifndef __BumpRace__AdsCPPManager__
#define __BumpRace__AdsCPPManager__
#include "cocos2d.h"
class AdsCPPManager
{
public:
    static void preload();
    static void showIntereIstial();
    static void showMoreApps();
    static void showBanner();
    static void hideBanner();
    static void goToUrl(const std::string &str);
    //in app
    static void restorePurchases();
    static void buyrmvAdsIos();
    static void buyAllLevelsIos();
    //flurry
    static void sendFlurryEvent(const std::string &str);
    
};
#endif /* defined(__BumpRace__AdsCPPManager__) */
