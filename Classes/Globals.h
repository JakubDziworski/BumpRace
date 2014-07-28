#ifndef __GLOBALS_H__
#define __GLOBALS_H__
#include "cocos2d.h"
extern int G_endlessGateNumber;
extern cocos2d::Director *G_director;
extern int G_odlegloscmiedzyBramkami;
extern cocos2d::Vec2 G_srodek;
extern float G_prevXgravity;
const extern float G_radToAngle;
const extern float Globals_radWsp;
extern float G_Currangle;
extern float G_myCos;
extern float G_mySin;
extern int G_maxVelocity;
const extern int G_ALLboxesNumber;
const extern int G_maxVelConstant;
const extern int G_maxVelAddition;
extern void G_scaleToFitScreen(cocos2d::Sprite *spr);
cocos2d::Director *G_dir();
extern void G_setCurrAngle(float angle);
extern float G_wF(float inp);
extern float G_hF(float inp);
extern cocos2d::Vec2 G_wV(cocos2d::Vec2 inp);
extern cocos2d::Vec2 G_hV(cocos2d::Vec2 inp);
extern void G_initLanguage();
extern cocos2d::Dictionary *G_strings;
extern std::string G_str(const std::string& input);
extern cocos2d::Color3B G_getRandomColor();
extern const cocos2d::Color3B G_colors[6];
#endif

