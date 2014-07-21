#ifndef ___ENDLESS_HUD_H_
#define ___ENDLESS_HUD_H_
#include "Hud.h"
#include "EndlessWorld.h"
#include "myLayout.h"
class EndlessHud : public Hud
{
private:
	EndlessWorld* world;
	cocos2d::Map<Boxx*, cocos2d::ui::Text*> scoreTable;
	cocos2d::Vector<Boxx*> *orderedBoxes;
	cocos2d::ui::Layout *scoreNode;
	myLayout *gmOverNode;
public:
	virtual bool init(EndlessWorld* worldd);
	static EndlessHud* create(EndlessWorld* worldd);
	virtual void pointsChanged(cocos2d::Vector<Boxx*> *orderedByPointsBoxes);
	virtual void gameIsOver();

};
#endif // !___ENDLESS_HUD_H_
