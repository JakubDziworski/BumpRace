#include "cocos2d.h"
#pragma once
class sometest : public cocos2d::Layer
{
public:
	int i = 0;
	float time=0;
	bool init();
	static cocos2d::Scene *createScene();
	void createWithTTF(float dt);
	void createWithBMF(float dt);
	cocos2d::TTFConfig config;
	CREATE_FUNC(sometest);
};

