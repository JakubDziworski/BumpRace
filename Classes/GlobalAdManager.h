/*
 * GlobalAdManager.h
 *
 *  Created on: 06-09-2014
 *      Author: Jakub
 */

#ifndef GLOBALADMANAGER_H_
#define GLOBALADMANAGER_H_
#include "cocos2d.h"
class GlobalAdManager {
public:
    //ADS
	static void preloadAds();
	static void showInteristial();
	static void showMoreGames();
    static void showBanner();
    static void hideBanner();
    //IN APP
	static void rmvAds();
	static void unlockLevel(int i);
	static void checkBought();
    //GO TO
	static void goToLink(const std::string &str);
};

#endif /* GLOBALADMANAGER_H_ */
