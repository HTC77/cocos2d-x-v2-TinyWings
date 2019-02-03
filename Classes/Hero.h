#ifndef __Hero__
#define __Hero__

#pragma once

#include "cocos2d.h"
#include <Box2D\Box2D.h>

USING_NS_CC;

class Hero : public CCSprite
{
public:
	Hero();
	~Hero();
	static Hero* createWithWorld(b2World *world);
	bool initWithWorld(b2World *world);
	void update();
	void createBody();

	b2World *_world;
	b2Body *_body;

	bool _awake;

	bool awake;
	void wake();
	void dive();
	void limitVelocity();

};

#endif