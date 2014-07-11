#include "Boxx.h"
#include "cocos2d.h"
class AIOpponent : public Boxx
{
private:
	Boxx *tylni;
	Boxx *przedni;
	cocos2d::Vector<Boxx*> *orderedOpponents;
	float odleglosc(Boxx *box);
	void setPrzedniTylni();
public:
	void simulate(float dt);
	bool myInit(const std::string& filename,std::string ID, cpSpace *space);
	static AIOpponent* create(const std::string& filename, std::string ID, cpSpace *space);
	void addOrderedOpponents(cocos2d::Vector<Boxx*> &orderedOpponents){this->orderedOpponents = &orderedOpponents; }
	virtual void additionalDebugInfo();

};
