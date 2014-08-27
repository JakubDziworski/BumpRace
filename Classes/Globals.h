#ifndef __GLOBALS_H__
#define __GLOBALS_H__
#include "cocos2d.h"
#include "World.h"
#include "Hud.h"
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
extern int G_maxVelConstant;
extern int G_maxVelAddition;
extern int G_powerUpOdleglos;
extern int G_powerUpOdlegloscVar;
extern int G_powerUpsNumbers;
extern void G_scaleToFitScreen(cocos2d::Node *spr);
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
extern World* G_getWorld();
extern Hud* G_getHud();
extern float G_getFTimeScale(float val);
extern cocos2d::ParticleSystemQuad* G_getParticleFromFile(const std::string &filename, cocos2d::ParticleSystemQuad::PositionType type= cocos2d::ParticleSystemQuad::PositionType::RELATIVE);
extern void G_displayCorrectLevelStarter(int level,cocos2d::Node *parent);
extern void G_enableShadow(cocos2d::Label *lbl);
extern void G_enableShadow(cocos2d::ui::Text *lbl);
#endif

