#ifndef __Hero__
#define __Hero__

#pragma once

#include "cocos2d.h"
#include <Box2D\Box2D.h>
#define NUM_PREV_VELS 5

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

	void wake();
	void dive();
	void limitVelocity();

	b2Vec2 _prevVels[NUM_PREV_VELS];
	int _nextVel;

	CCAnimation* _normalAnim;
	CCAction* _normalAnimate;
	void nodive();
	void runForceAnimation();
	void runNormalAnimation();

};

#endif