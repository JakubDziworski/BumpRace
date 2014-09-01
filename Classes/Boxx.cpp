#include "Boxx.h"
#include "Globals.h"
#include "Paths.h"
#include "Macros.h"
#include "SimpleAudioEngine.h"
#include "soundManager.h"
using namespace cocos2d;
bool Boxx::myInit(const std::string& filename, std::string ID, cpSpace *space, cocos2d::Color3B boxColorr)
{
	if (filename == R_FACEBOOKFACENAME)
	{
		if (!Sprite::initWithTexture(G_faceBookAvatarTex))
		{
			return false;
		}
	}
	else
	{
		if (!Sprite::initWithSpriteFrameName(filename))
		{
			return false;
		}
	}
	
	positiveSprite = Sprite::createWithSpriteFrameName(String::createWithFormat(R_positiveActionFormat.c_str(), 0)->getCString());
	positiveSprite->setOpacity(0);
	this->addChild(positiveSprite,2);
	positiveSprite->setNormalizedPosition(Vec2(0.5f, 0.5f));
	positiveSprite->setScaleX(this->getContentSize().width / 45.5f);
	positiveSprite->setScaleY(this->getContentSize().height / 42.0f);
	this->boxColor = boxColorr;
	//init variables//
	this->fileName = filename;
	maxVel = 500;
	wind = 0;
	points = 0;
	deactivated = false;
	//create cocos stuff//
	debugL = Label::create("",R_defaultFont,22);
	debugL->setColor(this->boxColor);
	//physics//
	auto bounding = this->getContentSize();
	myBody = cpBodyNew(1.0f,INFINITY);
	cpSpaceAddBody(space, myBody);
	cpVect verts[] = {
		cpv(-bounding.width / 2.0f, -bounding.height / 2.0f),
		cpv(-bounding.width / 2.0f, bounding.height / 2.0f),
		//cpv(-bounding.width / 4.0f, bounding.height / 2.0f),
		//cpv(bounding.width / 4.0f, bounding.height / 2.0f),
		cpv(bounding.width / 2.0f, bounding.height/2.0f ),
		cpv(bounding.width / 2.0f, -bounding.height / 2.0f),
	};
	cpVect topa = cpv(-bounding.width / 2.2f, bounding.height / 2.0f);
	cpVect topb = cpv(bounding.width / 2.2f, bounding.height / 2.0f);
	cpShape* topShape = cpSegmentShapeNew(myBody, topa, topb, bounding.height*0.05f); shapes[0] = topShape;
	topShape->e = 0;
	topShape->u = 0;
	cpVect bota = cpv(-bounding.width / 2.2f, -bounding.height / 2.0f);
	cpVect botb = cpv(bounding.width / 2.2f,-bounding.height / 2.0f);
	cpShape* botShape = cpSegmentShapeNew(myBody, bota, botb, bounding.height*0.05f); shapes[1] = botShape;
	botShape->e = 0;
	botShape->u = 0;
	cpShape* shape = cpPolyShapeNew(myBody, 4, verts, cpvzero); shapes[2] = shape;
	shape->e = 1.1f; shape->u = 0.1f;
	cpSpaceAddShape(space, topShape);
	cpSpaceAddShape(space, shape);
	//tweaks and adchild//
	debugL->setNormalizedPosition(Vec2(0.5f,2));
	G_enableShadow(debugL);
	debugL->setHorizontalAlignment(TextHAlignment::CENTER);
	debugL->setVisible(false);
	myBody->data = this;
	myBody->velocity_func = gravityFunc;
	this->addChild(debugL);
	this->ID = ID;
	for (int i = 0; i < 3; i++)
	{
		shapes[i]->collision_type = COLLISONTYPEBOX; 
		cpShapeSetLayers(shapes[i], CPCOLIDEWITHBOXES);
	}
	rocket = NULL;
	jetpack = NULL;
	ghost = NULL;
	powerUpExecuted = false;
	pwrupType = PowerUp::PowerUpType::NONE;
	//wind
	ped = Sprite::createWithSpriteFrameName("ped.png");
	this->addChild(ped);
	ped->setAnchorPoint(Point(0.9, 0.5));
	defaultPedScaleX = this->getContentSize().width / ped->getContentSize().width*1.25;
    defaultPedScaleY = this->getContentSize().height / ped->getContentSize().height*1.25;
	ped->setScaleX(defaultPedScaleX);
	ped->setScaleY(defaultPedScaleY);
	ped->setNormalizedPosition(Point(1, 0.52));
	ped->setOpacity(0);
	//endWind
	displayDebugInfo();
	return true;
};
Boxx* Boxx::create(const std::string& filename, std::string ID,cpSpace *space, cocos2d::Color3B boxColorr)
{
	Boxx *pRet = new Boxx();
	if (pRet && pRet->myInit(filename, ID, space, boxColorr))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}
void Boxx::updateBox()
{
	updatePhysPos();
	updateTransform();
	updatePowerUp();
	//displayDebugInfo();
}
void Boxx::jump()
{
	if (isJumping()) return;
	this->runAction(Sequence::createWithTwoActions(ScaleTo::create(0.2f,0.9f, 1.1f), ScaleTo::create(0.2f, 1, 1.0f)));
	cpBodyApplyImpulse(myBody, cpv(0, 250), cpv(0, 0));
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(R_jump.c_str());
}
void Boxx::gravityFunc(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt)
{
	Boxx *box = ((Boxx*)body->data);
	if (body->v.x > box->maxVel)
	{
		gravity.x = -0.4f*gravity.x;
	}
	else
	{
		gravity.x = gravity.x * box->getWind();
	}
	cpBodyUpdateVelocity(body, gravity, damping, dt);
}
bool Boxx::isJumping()
{
	if (isOnFlat()) return false;
	for (auto boxx : *G_getWorld()->getOrderedBoxes())
	{
		if (boxx == this) continue;
		const float odlegloscY = this->getBoundingBox().getMinY() - boxx->getBoundingBox().getMaxY();
		const float margin = this->getContentSize().height*0.05f;
		if ((boxx->getBoundingBox().getMaxX() > this->getBoundingBox().getMinX()) && boxx->getBoundingBox().getMinX() < this->getBoundingBox().getMaxX() && odlegloscY < margin && odlegloscY >= 0)
		{
			return false;
		}
	}
	//if (abs(cpBodyGetVel(myBody).y) > 5.0f)
	//return true;
	return true;
}
void Boxx::updateTransform()
{
	cpVect cpPos = cpBodyGetPos(myBody);
	this->setPosition(Vec2(cpPos.x, cpPos.y));
	this->setRotation(-cpBodyGetAngle(myBody)*G_radToAngle);
}
void Boxx::setBodyPosition(const cocos2d::Vec2 pos)
{
	myBody->p = cpv(pos.x, pos.y);
	updateTransform();
}
bool Boxx::isOnFlat()
{
	if (this->getBoundingBox().getMinY() <  2) return true;
	return false;
}
void Boxx::updatePhysPos()
{
	const float prevWind = wind;
	if (powerUpExecuted == true && pwrupType == PowerUp::PowerUpType::GHOST)
	{
		maxVel = G_maxVelocity * wind;
		return;
	}
	if (deactivated)
	{
		wind = 0;
		maxVel = G_maxVelocity * wind;
		return;
	}
	switch (physPos)
	{
	case 1:
		wind = 1;
		maxVel = G_maxVelocity * wind;
		break;
	case 2:
		wind = 1.2f;
		maxVel = G_maxVelocity*wind;
		break;
	case 3:
		wind = 1.4f;
		maxVel = G_maxVelocity*wind;
		break;
	case 4:
		wind = 1.6f;
		maxVel = G_maxVelocity*wind;
		break;
	case 5:
		wind = 1.8f;
		maxVel = G_maxVelocity*wind;
		break;
	case 6:
		wind = 2.0f;
		maxVel = G_maxVelocity*wind;
		break;
	case 7:
		wind = 2.45f;
		maxVel = G_maxVelocity*wind;
		break;
	case 8:
		wind = 2.55f;
		maxVel = G_maxVelocity*wind;
		break;
	case 9:
		wind = 3.05f;
		maxVel = G_maxVelocity*wind;
		break;
	case 10:
		wind = 3.15f;
		maxVel = G_maxVelocity*wind;
		break;
	case 11:
		wind = 3.3f;
		maxVel = G_maxVelocity*wind;
		break;
	}
	if (getVelocityX() < 150)
	{
		ped->stopAllActions();
		ped->runAction(FadeOut::create(0.2f));
		ped->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(0.1f, defaultPedScaleX + 0.2f, defaultPedScaleY), ScaleTo::create(0.1f, defaultPedScaleX, defaultPedScaleY))));
		return;
	}
	if (prevWind != wind)
	{
		ped->stopAllActions();
		float fade = 0;;
		float scale = 0.1f;
		if (wind == 1.0f){ fade = 150.0f; scale = 0.4f; }
		else if (wind == 1.2f){ fade = 90.0f; scale = 0.3f; }
		else if (wind == 1.4f) { fade = 50.0f; scale = 0.2f; }

		ped->runAction(FadeTo::create(0.2f, fade));
		ped->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(0.1f, defaultPedScaleX + scale, defaultPedScaleY), ScaleTo::create(0.1f, defaultPedScaleX, defaultPedScaleY))));
	}
}
void Boxx::displayDebugInfo()
{
	additionalDebugInfo();
	//if (!isOnFlat()) debugL->setString(debugL->getString()+"JUMPINg \n");
	debugL->setString(debugL->getString() + CCString::createWithFormat(" racePOS: %d", racePos)->getCString());
	debugL->setString(debugL->getString() + CCString::createWithFormat(" physPOS:%d", physPos)->getCString());
	debugL->setString(debugL->getString() + CCString::createWithFormat("\n velx:%.1f", myBody->v.x)->getCString());
	//debugL->setString(debugL->getString() + CCString::createWithFormat("\n v_limit:%.1f", myBody->v_limit)->getCString());
	debugL->setString(debugL->getString() + CCString::createWithFormat("\n wind:%.1f", wind)->getCString());
	debugL->setString(debugL->getString() + CCString::createWithFormat("\n maxVel:%.1f", maxVel)->getCString());
}
void Boxx::additionalDebugInfo()
{
	debugL->setString(ID);
	debugL->setString(debugL->getString() + CCString::createWithFormat("\n points:%d", points)->getCString());
	if (deactivated) 
		debugL->setString(debugL->getString() + "\ndeactivated");
}
void Boxx::addPoint()
{
	if (deactivated) return;
	points++;
	const float offset = this->getContentSize().height;
	Label *plus1 = Label::create("+1",R_defaultFont,50);
	this->addChild(plus1,25);
	plus1->setAnchorPoint(Vec2(0.5f,0.0f));
	plus1->setNormalizedPosition(Vec2(0.5f,1.0f));
	plus1->setOpacity(0);
	G_enableShadow(plus1);
	plus1->setColor(boxColor);
	auto goUp = MoveBy::create(2,Vec2(0,offset));
	auto fadeOut = FadeOut::create(0.4f);
	auto fadeIn = FadeIn::create(0.4f);
	auto idle = DelayTime::create(2.0f-0.8f);
	auto destroy = CallFunc::create([plus1](){plus1->removeFromParent();});
	plus1->runAction(Sequence::create(fadeIn,idle,fadeOut,destroy,NULL));
	plus1->runAction(Sequence::createWithTwoActions(goUp,destroy));
	displayDebugInfo();
}
void Boxx::deactivate()
{
	displayDebugInfo();
	auto fireRED = G_getParticleFromFile(R_boxDeactivatedRED);
	auto smokeYELLOW = G_getParticleFromFile(R_boxDeactivatedYELLOW);
	smokeYELLOW->setPosition(Vec2(this->getContentSize().width*0.5f, this->getContentSize().height*0.7f));
	fireRED->setPosition(Vec2(this->getContentSize().width*0.5f, this->getContentSize().height*0.7f));
	this->addChild(fireRED,1);
	this->addChild(smokeYELLOW,0);
	deactivated = true;
}
bool Boxx::collectedPowerUp(PowerUp::PowerUpType pwruptype)
{
	if (powerUpExecuted) return false;
	if (deactivated) return false;
	switch (pwrupType)
	{
	case PowerUp::PowerUpType::SPEED:
		jetpack->removeFromParent();
		jetpack = NULL;
		break;
	case PowerUp::PowerUpType::GHOST:
		ghost->removeFromParent();
		ghost = NULL;
		break;
	case PowerUp::PowerUpType::THUNDER:
		rocket->removeFromParent();
		rocket = NULL;
		break;
	case PowerUp::PowerUpType::NONE:
		break;
	default:
		break;
	}
	this->pwrupType = pwruptype;
	switch (pwruptype)
	{
	case PowerUp::PowerUpType::SPEED:
		jetpack = Sprite::createWithSpriteFrameName(R_jetpack);
		jetpack->setNormalizedPosition(Vec2(0, 0.5f));
		this->addChild(jetpack);
		break;
	case PowerUp::PowerUpType::GHOST:
		ghost = Sprite::createWithSpriteFrameName(String::createWithFormat("%s1.png",R_ghostPrefix.c_str())->getCString());
		ghost->setNormalizedPosition(Vec2(0.25f, 1.05f));
		ghost->setOpacity(255);
		this->addChild(ghost);
		break;
	case PowerUp::PowerUpType::THUNDER:
		rocket = Sprite::createWithSpriteFrameName(R_rocket);
		rocket->setNormalizedPosition(Vec2(0, 0.5f));
		this->addChild(rocket);
		break;

	default:
		break;
	}
	return true;
}
bool Boxx::activatePowerUp()
{
	if (powerUpExecuted) return false;
	powerUpExecuted = true;
	switch (pwrupType)
	{
	case PowerUp::PowerUpType::SPEED:
	{
										cpBodyApplyImpulse(myBody, cpv(250, 0), cpv(0, 0));
										auto jetpackFireRED = G_getParticleFromFile(R_jetpackFireRED);
										jetpackFireRED->setPosition(0,this->getContentSize().height/2.0f);
										auto jetpackSmokeYELLOW = G_getParticleFromFile(R_jetpackFireYELLOW);
										jetpackSmokeYELLOW->setPosition(0, this->getContentSize().height / 2.0f);
										jetpack->addChild(jetpackFireRED,1);
										jetpack->addChild(jetpackSmokeYELLOW, 2);
										auto rmvjetpack = CallFunc::create([this](){jetpack->removeFromParent(); jetpack = NULL; pwrupType = PowerUp::PowerUpType::NONE; powerUpExecuted = false; });
										auto wait = DelayTime::create(jetpackFireRED->getDuration() + jetpackFireRED->getLife());
										auto fadeOut = FadeOut::create(0.2f);
										jetpack->runAction(Sequence::create(wait, fadeOut, rmvjetpack, NULL));
										break;
	}
	case PowerUp::PowerUpType::GHOST:
	{
										cocos2d::Vector<SpriteFrame*> animFrames(9);
										for (int i = 1; i <= 9; i++)
										{
											animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("%s%d.png", R_ghostPrefix.c_str(), i)->getCString()));
										}
										auto animate = Animate::create(Animation::createWithSpriteFrames(animFrames, 0.04f));
										auto fadeout = FadeTo::create(0.3f, 200);
										auto fadeout2 = FadeTo::create(0.3f, 100);
										auto fadein = FadeTo::create(0.3f, 255);
										auto recolide = CallFunc::create([this](){for (int i = 0; i < 3; i++) cpShapeSetLayers(shapes[i], CPCOLIDEWITHBOXES); ghost->removeFromParent(); ghost = NULL; pwrupType = PowerUp::PowerUpType::NONE; powerUpExecuted = false; updatePhysPos(); });
										auto idle = DelayTime::create(3);
										auto totallyFade = FadeTo::create(0.3f, 0);
										//ghost->runAction(animate);
										auto idle2 = DelayTime::create(animate->getDuration());
										ghost->runAction(Sequence::create(animate,fadeout, idle, totallyFade, NULL));
										this->runAction(Sequence::create(idle2,fadeout2, idle, fadein, recolide, NULL));
										for (int i = 0; i < 3; i++) cpShapeSetLayers(shapes[i], CPUNCOLIDEWITHBOXES);
										break;
	}
	case PowerUp::PowerUpType::THUNDER:
	{
										  if (G_getWorld()->getOstaniActive() == this)
										  {
											  powerUpExecuted = false;
											  return false;
										  }
										  Boxx *target = G_getWorld()->getOrderedBoxes()->at(G_getWorld()->getOrderedBoxes()->getIndex(this) + 1);
										  auto timeToFly = 0.3f;//(0.3f / (2 * target->getContentSize().width))*(this->getPositionX() - target->getPositionX());
										  auto p1 = Vec2(target->getBoundingBox().getMaxX() + target->getVelocityX()*timeToFly, target->getPositionY() + target->getVelocityY()*timeToFly);
										  auto p2 = this->getPosition();
										  float angle = G_radToAngle*atan2(p1.x - p2.x, p1.y - p2.y);
										  if (p1.x > p2.x) angle = G_radToAngle*atan2(target->getBoundingBox().getMaxX()-p2.x, p1.y - p2.y);
										  //rocket
										  auto rocketNew = Sprite::createWithSpriteFrameName(R_rocket);
										  auto particleFireRED = G_getParticleFromFile(R_rocketFireRED);
										  auto particleFireYELLOW = G_getParticleFromFile(R_rocketFireYELLOW);
										  particleFireRED->setDuration(timeToFly);
										  particleFireYELLOW->setDuration(timeToFly);
										  rocketNew->addChild(particleFireRED,2);
										  rocketNew->addChild(particleFireYELLOW,1);
										  particleFireRED->setPosition(Vec2(this->getContentSize().width*0.5f, this->getContentSize().height* -0.3f));
										  particleFireYELLOW->setPosition(Vec2(this->getContentSize().width*0.5f, this->getContentSize().height* -0.3f));
										  auto explosion1 = G_getParticleFromFile(R_explosParticleRED,ParticleSystemQuad::PositionType::FREE);
										  rocketNew->setPosition(this->getPositionX()-this->getContentSize().width/2,this->getPositionY());
										  rocketNew->setRotation(angle);
										  //actions
										  auto fadeOutRocket = FadeOut::create(0.1f);
										  auto fly = MoveTo::create(timeToFly, p1);
										  DelayTime *wait = DelayTime::create(explosion1->getDuration() + explosion1->getLife());
										  auto explode = CallFunc::create([rocketNew, target, particleFireRED,particleFireYELLOW]() mutable
										  {
											  auto explosion1 = G_getParticleFromFile(R_explosParticleRED, ParticleSystemQuad::PositionType::FREE);
											  auto explosion2 = G_getParticleFromFile(R_explosParticleYELLOW, ParticleSystemQuad::PositionType::FREE);
											  SoundManager::getInstance()->playEffect(R_explosion);
											  cpBodyApplyImpulse(target->getBody(), cpv(-500,200), cpv(0, 0));
											  explosion1->setPosition(rocketNew->getContentSize().width / 2.0f, rocketNew->getContentSize().height / 2.0f);
											  explosion2->setPosition(rocketNew->getContentSize().width / 2.0f, rocketNew->getContentSize().height / 2.0f);
											  rocketNew->addChild(explosion1);
											  rocketNew->addChild(explosion2);
											  particleFireRED->stopSystem();
											  particleFireYELLOW->stopSystem();
										  });
										  auto remove = CallFunc::create([rocketNew, this](){rocketNew->removeFromParent(); pwrupType = PowerUp::PowerUpType::NONE; powerUpExecuted = false; });
										  SoundManager::getInstance()->playEffect(R_rocketLaunch);
										  CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(R_rocketLaunch.c_str());
										  rocketNew->runAction(Sequence::create(fly, explode, fadeOutRocket, wait, remove, NULL));
										  rocket->removeFromParent();
										  rocket = NULL;
										  this->getParent()->addChild(rocketNew);
										  break;
	}
	}
	return true;
}
void Boxx::updatePowerUp()
{
	if (deactivated) return;
	if (pwrupType == PowerUp::PowerUpType::THUNDER && rocket != NULL)
	{
		if (G_getWorld()->getOstaniActive() == this) return;
		Boxx *target = G_getWorld()->getOrderedBoxes()->at(G_getWorld()->getOrderedBoxes()->getIndex(this) + 1);
		auto timeToFly = 0.3f;
		auto p1 = target->getPosition(); (target->getBoundingBox().getMaxX() + target->getVelocityX()*timeToFly, target->getPositionY() + target->getVelocityY()*timeToFly);
		auto p2 = this->getPosition();
		const  float angle = G_radToAngle*atan2(p1.x - p2.x, p1.y - p2.y);
		rocket->setRotation(angle);
	}
}
void Boxx::positiveGateAction()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(R_MP3_punch.c_str(), false, Director::getInstance()->getScheduler()->getTimeScale());
	cocos2d::Vector<SpriteFrame*> animFrames(16);
	for (int i = 0; i <= 15; i++)
	{
		animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat(R_positiveActionFormat.c_str(), i)->getCString()));
	}
	auto animate = Animate::create(Animation::createWithSpriteFrames(animFrames, 0.03f));
	auto fadeout = FadeOut::create(1);
	auto fadein = FadeTo::create(0.4f,200);
	auto usun = CallFunc::create([this](){});
	auto idle = DelayTime::create(2);
	positiveSprite->stopAllActions();
	positiveSprite->runAction(fadein);
	positiveSprite->runAction(Sequence::createWithTwoActions(Repeat::create(animate, 10),fadeout));
}
