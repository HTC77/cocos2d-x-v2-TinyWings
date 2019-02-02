#ifndef __Terrain__
#define __Terrain__

#pragma once
#define kMaxHillKeyPoints 1000
#define kHillSegmentWidth 2
#define kMaxHillVertices 4000
#define kMaxBorderVertices 800

#include "cocos2d.h"

USING_NS_CC;
class Terrain : public CCNode
{
public:
	Terrain();
	~Terrain();
	
	bool init();
	static Terrain* createTerrain();
	void setOffsetX(float newOffsetX);
	int _offsetX;
	CCSprite* _stripes;

	CCSize winSize;
	int _fromKeyPointI;
	int _toKeyPointI;

	void resetHillVertices();
	void draw();

	CCPoint _hillKeyPoints[kMaxHillKeyPoints];
	int _nHillVertices;
	CCPoint _hillVertices[kMaxHillVertices];
	CCPoint _hillTexCoords[kMaxHillVertices];
	int _nBorderVertices;
	CCPoint _borderVertices[kMaxBorderVertices];

	void generateHills();	
};	

#endif // __Terrain_H__

