#include "Globals.h"
#include "cocos2d.h"
#include "Macros.h"
int G_endlessGateNumber = 5;
int G_odlegloscmiedzyBramkami = 7000;
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
const int G_powerUpOdleglos = 5000;
const int G_powerUpOdlegloscVar = 5000;
const int G_powerUpsNumbers = 2;
cocos2d::Dictionary *G_strings;
const int G_ALLboxesNumber = 6;
const cocos2d::Color3B G_colors[6] = { cocos2d::Color3B(115, 207, 231), cocos2d::Color3B(178, 210, 53), cocos2d::Color3B(130, 85, 127), cocos2d::Color3B(244, 191, 26), cocos2d::Color3B(226, 54, 39), cocos2d::Color3B(115, 207, 231) };
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
const char* G_form_str(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	return cocos2d::String::createWithFormat(format, ap)->getCString();
}
void G_initLanguage()
{
	cocos2d::LanguageType currentLanguageType = cocos2d::Application::getInstance()->getCurrentLanguage();
	if (currentLanguageType == cocos2d::LanguageType::POLISH)
	{
		G_strings = cocos2d::Dictionary::createWithContentsOfFile("multilanguage/Polish.plist");
	}
	else
	{
		G_strings = cocos2d::Dictionary::createWithContentsOfFile("multilanguage/English.plist");
	}
	G_strings->retain();
}
std::string G_str(const std::string& input)
{
	std::string str = G_strings->valueForKey(input)->getCString();
	return str;
}
cocos2d::Color3B G_getRandomColor()
{
	int tab[3];
	for (int i = 0; i < 3; i++)
	{
		tab[i] = rand() % 155 + 100;
	}
	return cocos2d::Color3B(tab[0], tab[1], tab[2]);
}
void G_scaleToFitScreen(cocos2d::Node *spr)
{
	spr->setScale(G_dir()->getWinSize().width / spr->getContentSize().width, G_dir()->getWinSize().height / spr->getContentSize().height);
}

World* G_getWorld()
{
	return (World*)G_director->getRunningScene()->getChildByTag(LAYER_GAMEPLAY);
}

Hud* G_getHud()
{
	return (Hud*)cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(LAYER_HUD);
}

float G_getFTimeScale(float val)
{
	return G_director->getScheduler()->getTimeScale()*val;
}


