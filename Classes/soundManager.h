#ifndef __SOUBD_MMANAGEr__
#define __SOUBD_MMANAGEr__
#include "cocos2d.h"
namespace CocosDenshion{class SimpleAudioEngine;}
class SoundManager : public cocos2d::Node,public cocos2d::ActionTweenDelegate
{
private:
	bool slowMoEnabled;
	static cocos2d::Scene *curScene;
	cocos2d::Node *mainater;
	int rampDownEffect;
	int slowEffect;
	int normalSlideEffect=NULL;
	const float slowLenght = 0.3f;
	const float rampDownLength = 0.1f;
	const float rampUpLength = 0.1f;
	SoundManager();
	static SoundManager *me;
	cocos2d::Director* director;
	CocosDenshion::SimpleAudioEngine *audioEngine;
	float fadeOutTime;
	float fadeInTime;
	float bgVolume;
	const float constfadeTime=0.3f;
	void fadeInMusicScheduler(float dt);
	void fadeOutMusicScheduler(float dt);
	void waitForRampDown(float dt);
public:
	static SoundManager *getInstance();
	void enableSlowMo();
	void disableSlowMo();
	void preloadSounds();
	void playBgMusicGameplay(); 
	void playBgMusicMenu();
	void stopBgMusic();
	void fadeOutMusic();
	void fadeInMusic();
	void playBgmusic(const std::string &inp);
	void playSlideEffect(float speedVal);
	void stopSlideEffect();
	void gameIsOver(bool win);
	void fadeOutEffect(int effect);
	void pauseEffects();
	void playEffect(const std::string &fileName);
	void stopEffect(const int);
	void playBtnEffect();
    void stopAllEffects();
	virtual void updateTweenAction(float value, const std::string& key);

};
#endif // !__SOUBD_MMANAGEr__
