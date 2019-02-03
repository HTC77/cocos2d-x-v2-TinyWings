#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Terrain.h"
#include "Box2D/Box2D.h"
#include "Hero.h"

#define PTM_RATIO 32.0

USING_NS_CC;
class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
	CCSize visibleSize;
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

	CCSprite* _background;
	CCSprite* _stripes;
	void genBackground();
	ccColor4F randomBrightColor();
	CCSprite* spriteWithColor(ccColor4F bgColor, float textureWidth, float textureHeight);
	CCSprite* stripedSpriteWithColor(ccColor4F color1, ccColor4F color2, float textureWidth, float textureHeight, int nStripes);

	void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
	//adding hill
	Terrain* _terrain;

	b2World * _world;
	void setupWorld();
	void createTestBodyAtPostition(CCPoint position);
	void update(float delta);

	Hero * _hero;

};

#endif // __HELLOWORLD_SCENE_H__
