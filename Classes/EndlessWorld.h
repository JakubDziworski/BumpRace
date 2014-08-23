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
	void extendFlat();
public:
	int getMinliczbabramek() const { return minliczbabramek; }
	int getScore() { return score; }
	void setMinGates(int number){ minliczbabramek = number; }
	static cocos2d::Scene *createScene(int oppNum,int aiLevel);
	virtual bool init(int oppNum, int aiLevel);
	static EndlessWorld *create(int oppNum, int aiLevel);
	virtual void restartLevel();
	virtual void checkpointReachedExtended(Boxx *box, int pos);
	virtual void shouldEnableSlowmo(Chcekpoint *chkpt, bool first);
	virtual void customWorldUpdate();
	virtual void modifyGate(Chcekpoint *chkpt);

};
#endif

