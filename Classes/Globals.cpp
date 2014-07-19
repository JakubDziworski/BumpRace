#include "Globals.h"
#include "cocos2d.h"
int G_odlegloscmiedzyBramkami = 2000;
cocos2d::Director *G_director = NULL;
cocos2d::Vec2 G_srodek = cocos2d::Vec2(0,0);
float G_prevXgravity = 0;
float G_Currangle = 0;
float G_myCos = 0;
float G_mySin = 0;
const float G_radToAngle = 180.0f / M_PI;
const float Globals_radWsp = M_PI / 180.0f;
int G_maxVelocity = 1000;
const int G_maxVelConstant = 500;
const int G_maxVelAddition = 2000;

void G_setCurrAngle(float angle)
{
	G_Currangle = angle;
	G_myCos = cos(G_Currangle*Globals_radWsp);
	G_mySin = sin(G_Currangle*Globals_radWsp);
}
float G_wF(float inp)
{
	return inp *  cocos2d::Director::getInstance()->getVisibleSize().width/960.0f ;
}
float G_hF(float inp)
{
	return inp * cocos2d::Director::getInstance()->getVisibleSize().height /640.0f;
}
cocos2d::Vec2 G_wV(cocos2d::Vec2 inp)
{
	return inp *  cocos2d::Director::getInstance()->getVisibleSize().width / 960.0f;
}
cocos2d::Vec2 G_hV(cocos2d::Vec2 inp)
{
	return inp * cocos2d::Director::getInstance()->getVisibleSize().height / 640.0f;
}

cocos2d::Director * G_dir()
{
	if (G_director == NULL) G_director = cocos2d::Director::getInstance();
	return G_director;
}
