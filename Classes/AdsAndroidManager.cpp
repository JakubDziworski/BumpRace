/*
 * AdsAndroidManager.cpp
 *
 *  Created on: 06-09-2014
 *      Author: Jakub
 */

#include "AdsAndroidManager.h"
#include "platform/android/jni/JniHelper.h"
#define COCOS2D_DEBUG 2
using namespace cocos2d;
extern "C"
{
void AdsAndroidManager::preloadAndroid()
{
	JniMethodInfo t;

		if (JniHelper::getStaticMethodInfo(t,
			"org/cocos2dx/cpp/AppActivity",    // fully qualified class name
			"loadAds",                              // method name
			"()V")) {             // data type of argument
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
		}
}

void AdsAndroidManager::showInteristialAndroid()
{
	JniMethodInfo t;

			if (JniHelper::getStaticMethodInfo(t,
				"org/cocos2dx/cpp/AppActivity",    // fully qualified class name
				"showInteristial",                              // method name
				"()V")) {             // data type of argument
				t.env->CallStaticVoidMethod(t.classID, t.methodID);
			}
}

void AdsAndroidManager::showMoreGamesAndroid()
{
	JniMethodInfo t;

				if (JniHelper::getStaticMethodInfo(t,
					"org/cocos2dx/cpp/AppActivity",    // fully qualified class name
					"showMoreGames",                              // method name
					"()V")) {             // data type of argument
					t.env->CallStaticVoidMethod(t.classID, t.methodID);
				}
}

void AdsAndroidManager::showBannerAndroid()
{
	JniMethodInfo t;

				if (JniHelper::getStaticMethodInfo(t,
					"org/cocos2dx/cpp/AppActivity",    // fully qualified class name
					"showBanner",                              // method name
					"()V")) {             // data type of argument
					t.env->CallStaticVoidMethod(t.classID, t.methodID);
				}
}

void AdsAndroidManager::hideBannerAndroid()
{
	JniMethodInfo t;

					if (JniHelper::getStaticMethodInfo(t,
						"org/cocos2dx/cpp/AppActivity",    // fully qualified class name
						"hideBanner",                              // method name
						"()V")) {             // data type of argument
						t.env->CallStaticVoidMethod(t.classID, t.methodID);
					}
}

void AdsAndroidManager::rmvAdsAndroid()
{
	JniMethodInfo t;

				if (JniHelper::getStaticMethodInfo(t,
					"org/cocos2dx/cpp/AppActivity",    // fully qualified class name
					"removeAd",                              // method name
					"()V")) {             // data type of argument
					t.env->CallStaticVoidMethod(t.classID, t.methodID);
				}
}

void AdsAndroidManager::unlockLevelAndroid()
{
	JniMethodInfo t;

					if (JniHelper::getStaticMethodInfo(t,
						"org/cocos2dx/cpp/AppActivity",    // fully qualified class name
						"unlockLevels",                              // method name
						"()V")) {             // data type of argument
						t.env->CallStaticVoidMethod(t.classID, t.methodID);
					}
}

void AdsAndroidManager::goToLinkAndroid(const std::string &str)
{
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t,
		"org/cocos2dx/cpp/AppActivity",    // fully qualified class name
		"openURL",                              // method name
		"(Ljava/lang/String;)V")) {             // data type of argument

		// jstring - C++ representation of Java String
		jstring stringArg = t.env->NewStringUTF(str.c_str());

		// call the method, with arguments
		t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg);
	}
}
int AdsAndroidManager::checkPurchases()
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t,
			"org/cocos2dx/cpp/AppActivity",    // fully qualified class name
			"inAppUnlocked",                              // method name
			"()I")) {             // data type of argument

			jint i =  (jint)t.env->CallObjectMethod(t.classID,t.methodID);
			return i;
	}
	return 0;
}
JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_purchasedAds(JNIEnv* env, jobject thiz);
JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_purchasedLevels(JNIEnv* env, jobject thiz);
}
JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_purchasedAds(JNIEnv* env, jobject thiz)
{
	CCLOG("BOUGHT ADS");
}
JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_purchasedLevels(JNIEnv* env, jobject thiz)
{
	CCLOG("BOUGHT LEVELS");
}

