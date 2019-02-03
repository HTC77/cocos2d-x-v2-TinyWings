#ifndef __Terrain__
#define __Terrain__

#pragma once
#define kMaxHillKeyPoints 1000
#define kHillSegmentWidth 2
#define kMaxHillVertices 4000
#define kMaxBorderVertices 800

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "GLES-Render.h"
#define PTM_RATIO 32.0

USING_NS_CC;
class HelloWorld;
class Terrain : public CCNode
{
public:
	Terrain();
	~Terrain();
	
	bool init();
	static Terrain* createTerrain();
	static Terrain*  createWithWorld(b2World *world);
	bool initWithWorld(b2World *world);
	void setOffsetX(float newOffsetX);
	int _offsetX;
	CCSprite* _stripes;

	CCSize winSize;
	int _fromKeyPointI;
	int _toKeyPointI;

	void resetHillVertices();
	void draw();
	void resetBox2DBody();

	CCPoint _hillKeyPoints[kMaxHillKeyPoints];
	int _nHillVertices;
	CCPoint _hillVertices[kMaxHillVertices];
	CCPoint _hillTexCoords[kMaxHillVertices];
	int _nBorderVertices;
	CCPoint _borderVertices[kMaxBorderVertices];

	void generateHills();
	void setupDebugDraw();

	b2World *_world;
	b2Body *_body;
	GLESDebugDraw * _debugDraw;
	CCSpriteBatchNode * _batchNode;
};	

#endif // __Terrain_H__

