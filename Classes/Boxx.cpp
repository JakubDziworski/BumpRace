#include "Boxx.h"
#include "Globals.h"
using namespace cocos2d;
bool Boxx::myInit(const std::string& filename, std::string ID, cpSpace *space)
{
	if (!Sprite::initWithSpriteFrameName("BOX.png"))
	{
		return false;
	}
	//init variables//
	this->fileName = filename;
	maxVel = 1000;
	wind = 0;
	points = 0;
	deactivated = false;
	//create cocos stuff//
	debugL = Label::create();
	debugL->setSystemFontSize(35);
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
	cpVect bota = cpv(-bounding.width / 2.2f, -bounding.height / 2.0f);
	cpVect botb = cpv(bounding.width / 2.2f,-bounding.height / 2.0f);
	cpShape* botShape = cpSegmentShapeNew(myBody, bota, botb, bounding.height*0.05f);
	botShape->e = 0;
	botShape->u = 0;
	cpShape* shape = cpPolyShapeNew(myBody, 4, verts, cpvzero);
	shape->e = 1.1f; shape->u = 0.1f;
	cpSpaceAddShape(space, topShape);
	cpSpaceAddShape(space, shape);
	//tweaks and adchild//
	debugL->setPosition(bounding.width / 2.0f, bounding.height*1.5f);
	myBody->data = this;
	myBody->velocity_func = gravityFunc;
	this->addChild(debugL);
	this->ID = ID;
	additionalDebugInfo();
	return true;
};
Boxx* Boxx::create(const std::string& filename, std::string ID, cpSpace *space)
{
	Boxx *pRet = new Boxx();
	if (pRet && pRet->myInit(filename,ID,space))
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
	displayDebugInfo();
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
		wind = 1.7f;
		maxVel = G_maxVelocity*wind;
		break;
	case 4:
		wind = 2.1f;
		maxVel = G_maxVelocity*wind;
		break;
	case 5:
		wind = 2.3f;
		maxVel = G_maxVelocity*wind;
		break;
	case 6:
		wind = 2.5f;
		maxVel = G_maxVelocity*wind;
		break;
	case 7:
		wind = 2.7f;
		maxVel = G_maxVelocity*wind;
		break;
	case 8:
		wind = 2.9f;
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
}

void Boxx::displayDebugInfo()
{
	additionalDebugInfo();
	//if (!isOnFlat()) debugL->setString(debugL->getString()+"JUMPINg \n");
	//debugL->setString(debugL->getString() + CCString::createWithFormat(" racePOS: %d", racePos)->getCString());
	//debugL->setString(debugL->getString() + CCString::createWithFormat(" physPOS:%d", physPos)->getCString());
	debugL->setString(debugL->getString() + CCString::createWithFormat("\n velx:%.1f", myBody->v.x)->getCString());
	//debugL->setString(debugL->getString() + CCString::createWithFormat("\n v_limit:%.1f", myBody->v_limit)->getCString());
	//debugL->setString(debugL->getString() + CCString::createWithFormat("\n wind:%.1f", wind)->getCString());
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
	points++;
	additionalDebugInfo();
}

void Boxx::deactivate()
{
	deactivated = true;
}













