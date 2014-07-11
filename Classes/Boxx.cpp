#include "Boxx.h"
#include "Globals.h"
using namespace cocos2d;
bool Boxx::myInit(const std::string& filename, cpSpace *space)
{
	if (!Sprite::initWithSpriteFrameName("BOX.png"))
	{
		return false;
	}
	//init variables//
	maxVel = 1000;
	wind = 0;
	//create cocos stuff//
	debugL = Label::create();
	debugL->setSystemFontSize(25);
	//physics//
	auto bounding = this->getContentSize();
	myBody = cpBodyNew(1.0f,INFINITY);
	cpSpaceAddBody(space, myBody);
	cpVect verts[] = {
		cpv(-bounding.width / 2.0f, -bounding.height / 2.0f),
		cpv(-bounding.width / 2.0f, bounding.height / 2.0f),
		cpv(bounding.width / 2.0f, bounding.height/2.0f ),
		cpv(bounding.width / 2.0f, -bounding.height / 2.0f),
	};
	cpVect topa = cpv(-bounding.width / 2.2f, bounding.height / 2.0f);
	cpVect topb = cpv(bounding.width / 2.2f, bounding.height / 2.0f);
	cpShape* topShape = cpSegmentShapeNew(myBody, topa, topb, bounding.height*0.05f);
	topShape->e = 0;
	topShape->u = 0;
	cpShape* shape = cpPolyShapeNew(myBody, 4, verts, cpvzero);
	shape->e = 0.8f; shape->u = 0.1f;
	cpSpaceAddShape(space, topShape);
	cpSpaceAddShape(space, shape);
	//tweaks and adchild//
	debugL->setPosition(bounding.width / 2.0f, bounding.height*1.5f);
	myBody->data = this;
	myBody->velocity_func = gravityFunc;
	this->addChild(debugL);
	return true;
};
Boxx* Boxx::create(const std::string& filename, std::string ID, cpSpace *space)
{
	Boxx *pRet = new Boxx();
	if (pRet && pRet->myInit(filename,space))
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
	//displayDebugInfo();
}

void Boxx::jump()
{
	if (isJumping()) return;
	cpBodyApplyImpulse(myBody, cpv(0, 1000),cpv(0, 0));
}

void Boxx::gravityFunc(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt)
{
	Boxx *box = ((Boxx*)body->data);
	if (body->v.x > box->maxVel)
	{
		gravity.x = -1.3f*gravity.x;
	}
	else
	{
		gravity.x = gravity.x * box->getWind();
	}
	box->debugL->setString(CCString::createWithFormat(" gravx:%f\n", gravity.x)->getCString());
	cpBodyUpdateVelocity(body, gravity, damping, dt);
}

bool Boxx::isJumping()
{
	if (abs(cpBodyGetVel(myBody).y) > 10.0f)
	return true;
	return false;
}

void Boxx::updateTransform()
{
	cpVect cpPos = cpBodyGetPos(myBody);
	this->setPosition(Vec2(cpPos.x, cpPos.y));
	this->setRotation(-cpBodyGetAngle(myBody)*G_radToAngle);
}

void Boxx::setBodyPosition(const Vec2 pos)
{
	myBody->p = cpv(pos.x, pos.y);
	updateTransform();
}

bool Boxx::isOnFlat()
{
	if (this->getPositionY() > 63 && this->getPositionY() < 120) return true;
	return false;
}


void Boxx::updatePhysPos()
{
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
		wind = 1.7f;
		maxVel = G_maxVelocity*wind;
		break;
	case 4:
		wind = 2.1f;
		maxVel = G_maxVelocity*wind;
		break;
	}
}

void Boxx::displayDebugInfo()
{
	additionalDebugInfo();
	if (!isOnFlat()) debugL->setString(debugL->getString()+"JUMPINg \n");
	debugL->setString(debugL->getString() + CCString::createWithFormat(" racePOS: %d", racePos)->getCString());
	debugL->setString(debugL->getString() + CCString::createWithFormat(" physPOS:%d", physPos)->getCString());
	debugL->setString(debugL->getString() + CCString::createWithFormat("\n velx:%.1f", myBody->v.x)->getCString());
	debugL->setString(debugL->getString() + CCString::createWithFormat("\n v_limit:%.1f", myBody->v_limit)->getCString());
	debugL->setString(debugL->getString() + CCString::createWithFormat("\n wind:%.1f", wind)->getCString());
	debugL->setString(debugL->getString() + CCString::createWithFormat("\n maxVel:%.1f", maxVel)->getCString());
}

void Boxx::additionalDebugInfo()
{
	debugL->setString("");
}













