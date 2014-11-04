/*
 * AdsAndroidManager.h
 *
 *  Created on: 06-09-2014
 *      Author: Jakub
 */

#ifndef ADSANDROIDMANAGER_H_
#define ADSANDROIDMANAGER_H_
#include "cocos2d.h"
extern "C"
{
class AdsAndroidManager {
public:
	static void preloadAndroid();
	static void showInteristialAndroid();
	static void showMoreGamesAndroid();
	static void showBannerAndroid();
	static void hideBannerAndroid();
	static void showExitAd();
	//IN APP
	static void rmvAdsAndroid();
	static void unlockLevelAndroid();
	static void checkPurchases();
	static void callFluryAndroid(const std::string &str);
	//GO TO
	static void goToLinkAndroid(const std::string &str);
};
}
#endif /* ADSANDROIDMANAGER_H_ */
