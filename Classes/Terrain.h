#ifndef __Terrain__
#define __Terrain__

#pragma once
#define kMaxHillKeyPoints 1000
#define kHillSegmentWidth 10

#include "cocos2d.h"

USING_NS_CC;
class HelloWorld;
class Terrain : public CCNode
{
public:
	Terrain();
	~Terrain();
	
	bool init();
	static Terrain* createTerrain();
	void setOffsetX(float newOffsetX);
	int _offsetX;

	CCSize winSize;
	int _fromKeyPointI;
	int _toKeyPointI;

	void resetHillVertices();
	void draw();

	CCPoint _hillKeyPoints[kMaxHillKeyPoints];
	
	void generateHills();	
};	

#endif // __Terrain_H__

