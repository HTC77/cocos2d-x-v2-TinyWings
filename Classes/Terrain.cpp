#include "Terrain.h"

Terrain::Terrain()
{

}
Terrain::~Terrain()
{

}

Terrain* Terrain::createTerrain()
{
	Terrain* obj = new Terrain();
	if (obj && obj->init())
	{
		obj->autorelease();
		return obj;
	}

	CC_SAFE_DELETE(obj);
	return NULL;
}

void Terrain::setOffsetX(float newOffsetX)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	_offsetX = newOffsetX;
	this->setPosition(CCPointMake(winSize.width / 8 - _offsetX * this->getScale(), 0));
	this->resetHillVertices();
}

void Terrain::resetHillVertices()
{
	// key points interval for drawing
	while (_hillKeyPoints[_fromKeyPointI + 1].x < _offsetX - winSize.width / 8 / this->getScale())
	{
		_fromKeyPointI++;
	}
	while (_hillKeyPoints[_toKeyPointI].x < _offsetX + winSize.width * 12 / 8 / this->getScale())
	{
		_toKeyPointI++;
	}
}

void Terrain::draw()
{
	this->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));

	CC_NODE_DRAW_SETUP();
	for (int i = MAX(_fromKeyPointI, 1); i <= _toKeyPointI; ++i) {
		ccDrawColor4F(1.0, 0, 0, 1.0);
		ccDrawLine(_hillKeyPoints[i - 1], _hillKeyPoints[i]);   //# 1 - real line (RED LINE)

		// Smooth Slopes
		ccDrawColor4F(1.0, 1.0, 1.0, 1.0);
		CCPoint p0 = _hillKeyPoints[i - 1];
		CCPoint p1 = _hillKeyPoints[i];
		int hSegments = floorf((p1.x - p0.x) / kHillSegmentWidth);
		float dx = (p1.x - p0.x) / hSegments;
		float da = M_PI / hSegments;
		float ymid = (p0.y + p1.y) / 2;
		float ampl = (p0.y - p1.y) / 2;

		CCPoint pt0, pt1;
		pt0 = p0;
		for (int j = 0; j < hSegments + 1; ++j) {

			pt1.x = p0.x + j*dx;
			pt1.y = ymid + ampl * cosf(da*j);

			ccDrawLine(pt0, pt1);  //# 2 - smooth line (WHITE LINE)

			pt0 = pt1;

		}
	}
}

void Terrain::generateHills()
{
	//optimized generate
	float minDX = 160;
	float minDY = 60;
	int rangeDX = 80;
	int rangeDY = 40;

	float x = -minDX;
	float y = winSize.height / 2;

	float dy, ny;
	float sign = 1; // +1 - going up, -1 - going  down
	float paddingTop = 20;
	float paddingBottom = 20;

	for (int i = 0; i<kMaxHillKeyPoints; i++) {
		_hillKeyPoints[i] = CCPointMake(x, y);
		if (i == 0) {
			x = 0;
			y = winSize.height / 2;
		}
		else {
			x += rand() % rangeDX + minDX;
			while (true) {
				dy = rand() % rangeDY + minDY;
				ny = y + dy*sign;
				if (ny < winSize.height - paddingTop && ny > paddingBottom) {
					break;
				}
			}
			y = ny;
		}
		sign *= -1;
	}
}

bool Terrain::init()
{
	if (!CCNode::init())
	{
		return false;
	}

	_fromKeyPointI = 0;
	_toKeyPointI = 0;
	_offsetX = 0;
	winSize = CCDirector::sharedDirector()->getWinSize();
	this->generateHills();
	this->resetHillVertices();
	return true;
}
