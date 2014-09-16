/*
 * SplashScreen.h
 *
 *  Created on: 15-09-2014
 *      Author: Jakub
 */

#ifndef SPLASHSCREEN_H_
#define SPLASHSCREEN_H_

#include "cocos2d.h"

class SplashScreen: public cocos2d::Layer {
public:
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(SplashScreen);
};

#endif /* SPLASHSCREEN_H_ */
