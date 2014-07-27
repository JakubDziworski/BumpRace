#include "Boxx.h"
#include "cocos2d.h"
class AIOpponent : public Boxx
{
private:
	int smartness; //0-2
	Boxx *tylni;
	Boxx *przedni;
	cocos2d::Vector<Boxx*> *orderedOpponents;
	float odleglosc(Boxx *box);
	void setPrzedniTylni();
	bool stykasie();
public:
	void simulate(float dt);
	bool myInit(const std::string& filename, std::string ID, cpSpace *space, int smartnez, cocos2d::Color3B boxColorr);
	static AIOpponent* create(const std::string& filename, std::string ID,  cpSpace *space, int smartnez, cocos2d::Color3B boxColorr = cocos2d::Color3B::BLACK);
	void addOrderedOpponents(cocos2d::Vector<Boxx*> &orderedOpponents){this->orderedOpponents = &orderedOpponents; }

};
