#include "soundManager.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Paths.h"
USING_NS_CC;
//variables

SoundManager *SoundManager::me = NULL;
Scene * SoundManager::curScene = NULL;
//functions
SoundManager::SoundManager()
{
	audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
	director = Director::getInstance();
	bgVolume = 1;
	fadeInTime = 0.0f;
	fadeOutTime = 0.0f;
	slowMoEnabled = false;
}
SoundManager * SoundManager::getInstance()
{
	if (me == NULL)
	{
		me = new SoundManager();
	}
	if (curScene == NULL)
	{
		curScene = Director::getInstance()->getRunningScene();
		if (me->getParent() != nullptr)
			me->removeFromParent();
		if(curScene) me->addChild(curScene);
	}
	return me;
}
void SoundManager::preloadSounds()
{
	//effects
	audioEngine->preloadEffect(R_MP3_punch.c_str());
	audioEngine->preloadEffect(R_slide.c_str());
	audioEngine->preloadEffect(R_slideRampDown.c_str());
	audioEngine->preloadEffect(R_slideSlow.c_str());
	audioEngine->preloadEffect(R_slideRampUp.c_str());
	//music
	audioEngine->preloadBackgroundMusic(R_bgmusicMenu.c_str());
}
void SoundManager::disableSlowMo()
{
	if (!slowMoEnabled) return;
	slowMoEnabled = false;
	fadeInTime = 0.0f;
	audioEngine->stopEffect(slowEffect);
	audioEngine->playEffect(R_slideRampUp.c_str());
	auto delay = DelayTime::create(0.65f*director->getScheduler()->getTimeScale());
	auto volumeUp = CallFunc::create([this]
	{
		director->getScheduler()->scheduleSelector(schedule_selector(SoundManager::fadeInMusicScheduler), me, 0, false);
	});
	director->getActionManager()->addAction(Sequence::createWithTwoActions(delay, volumeUp),me,false);
}
void SoundManager::enableSlowMo()
{
	slowMoEnabled = true;
	fadeOutTime = 0.0f;
	rampDownEffect = audioEngine->playEffect(R_slideRampDown.c_str());
	auto delay = DelayTime::create(0.65f*director->getScheduler()->getTimeScale());
	auto triggerNormal = CallFunc::create([this]()
	{
		slowEffect = audioEngine->playEffect(R_slideSlow.c_str());
	});
	director->getActionManager()->addAction(Sequence::createWithTwoActions(delay, triggerNormal), me, false);
	director->getScheduler()->scheduleSelector(schedule_selector(SoundManager::fadeOutMusicScheduler), me, 0, false);
}
void SoundManager::fadeOutMusicScheduler(float dt)
{
	if (fadeInTime != 0.0f)
	{
		director->getScheduler()->unscheduleSelector(schedule_selector(SoundManager::fadeOutMusicScheduler), me);
		return;
	}
	if (fadeOutTime < constfadeTime)
	{
		fadeOutTime += dt/director->getScheduler()->getTimeScale();
		bgVolume = 1.0f - fadeOutTime/ constfadeTime;
	}
	else
	{
		bgVolume = 0.0f;
		fadeOutTime = 0.0f;
		director->getScheduler()->unscheduleSelector(schedule_selector(SoundManager::fadeOutMusicScheduler),me);
	}
	audioEngine->setBackgroundMusicVolume(bgVolume);
}
void SoundManager::fadeInMusicScheduler(float dt)
{
	if (fadeInTime < constfadeTime)
	{
		fadeInTime += dt/director->getScheduler()->getTimeScale();
		bgVolume = fadeInTime / constfadeTime;
	}
	else
	{
		bgVolume = 1.0f;
		fadeInTime = 0.0f;
		director->getScheduler()->unscheduleSelector(schedule_selector(SoundManager::fadeInMusicScheduler), me);
	}
	audioEngine->setBackgroundMusicVolume(bgVolume);
}
void SoundManager::waitForRampDown(float dt)
{
	/*if (!audioEngine->isEffectPlaying(rampDownEffect))
	{
		audioEngine->stopEffect(rampDownEffect);
		slowEffect = audioEngine->playEffect(R_slideSlow.c_str(), true);
		me->unschedule(schedule_selector(SoundManager::waitForRampDown));
	}*/
}
void SoundManager::playBgMusicGameplay()
{
	audioEngine->playBackgroundMusic(R_bgmusicGameplay.c_str(),true);
}
void SoundManager::playBgMusicMenu()
{
	audioEngine->playBackgroundMusic(R_bgmusicMenu.c_str(),true);
}
void SoundManager::fadeOutMusic()
{
	fadeOutTime = 0;
	director->getScheduler()->scheduleSelector(schedule_selector(SoundManager::fadeOutMusicScheduler), me, 0, false);
}
void SoundManager::fadeInMusic()
{
	fadeInTime = 0;
	director->getScheduler()->scheduleSelector(schedule_selector(SoundManager::fadeInMusicScheduler), me, 0, false);
}

