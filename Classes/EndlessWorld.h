#ifndef ___ENDLESS_WORLD_H__
#define ___ENDLESS_WORLD_H__
#include "World.h"
class EndlessWorld : public World
{
private:
	int koniec;
	int iterations;
	int dodatek;
	int minliczbabramek;
	int score;
	bool endless;
	void extendFlat();
public:
	void setEndless(bool val) { endless = val; }
	bool isEndless() const { return endless; }
	int getMinliczbabramek() const { return minliczbabramek; }
	int getScore() { return score; }
	void setMinGates(int number);
	static cocos2d::Scene *createScene(int oppNum,int aiLevel,bool bestScore);
	virtual bool init(int oppNum, int aiLevel, bool bestScore);
	static EndlessWorld *create(int oppNum, int aiLevel, bool bestScore);
	virtual void restartLevel();
	virtual void checkpointReachedExtended(Boxx *box, int pos);
	virtual void shouldEnableSlowmo(Chcekpoint *chkpt, bool first);
	virtual void customWorldUpdate();
	virtual void modifyGate(Chcekpoint *chkpt);
    virtual void replaceSceneAdditional(cocos2d::Scene *scene,World *world);
	virtual void rozmiescCheckpointy();

	virtual void onDisplayModeTutorial();

};
#endif

