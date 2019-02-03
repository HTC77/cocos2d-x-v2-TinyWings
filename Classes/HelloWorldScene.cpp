#include "HelloWorldScene.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
	visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	_background = new CCSprite();
	_stripes = new CCSprite();	
	this->setupWorld();
	_terrain = Terrain::createWithWorld(_world);
	this->addChild(_terrain,1);
	this->genBackground();
	
	this->setTouchEnabled(true);
	this->scheduleUpdate();

	_hero = Hero::createWithWorld(_world);
	_terrain->_batchNode->addChild(_hero);
    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

void HelloWorld::genBackground()
{
	_background->removeFromParentAndCleanup(true);

	ccColor4F bgColor = this->randomBrightColor();

	int stripes = (rand() % 4 + 1) * 2;
	_background = this->spriteWithColor(bgColor, 512, 512);
	_background->setPosition(visibleSize / 2);

	cocos2d::ccTexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
	_background->getTexture()->setTexParameters(&tp);
	this->addChild(_background);

	_stripes->removeFromParentAndCleanup(true);

	ccColor4F color3 = this->randomBrightColor();
	ccColor4F color4 = this->randomBrightColor();
	_stripes = this->stripedSpriteWithColor(color3, color4, 512, 512, 4);
	cocos2d::ccTexParams tp2 = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE };
	_stripes->getTexture()->setTexParameters(&tp2);
	_stripes->setPosition(ccp(visibleSize.width / 2, -visibleSize.height));

	_terrain->_stripes = _stripes;
	this->addChild(_stripes);
}

ccColor4F HelloWorld::randomBrightColor()
{
	while (true)
	{
		int requiredBrightness = 192;
		ccColor4B randomColor =
			ccc4(rand() % 255, rand() % 255, rand() % 255, 255);
		if (randomColor.r > requiredBrightness ||
			randomColor.g > requiredBrightness ||
			randomColor.b > requiredBrightness)
		{
			return ccc4FFromccc4B(randomColor);
		}
	}
}

CCSprite* HelloWorld::spriteWithColor(ccColor4F bgColor, float textureWidth, float textureHeight)
{
	// 1: Create CCRenderTexture
	CCRenderTexture *rt = CCRenderTexture::create(textureWidth, textureHeight);

	// 2: Call CCRenderTexture:begin
	rt->beginWithClear(bgColor.r, bgColor.g, bgColor.b, bgColor.a);

	this->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor));
	CC_NODE_DRAW_SETUP();

	// 3: Draw into texture
	float gradiantAlpha = 0.7f;
	CCPoint vertices[4];
	ccColor4F colors[4];
	int nVertices = 0;

	vertices[nVertices] = CCPointMake(0, 0);
	colors[nVertices++] = ccColor4F(ccc4f(0, 0, 0, 0));
	vertices[nVertices] = CCPointMake(textureWidth, 0);
	colors[nVertices++] = ccColor4F(ccc4f(0, 0, 0, 0));
	vertices[nVertices] = CCPointMake(0, textureHeight);
	colors[nVertices++] = ccColor4F(ccc4f(0, 0, 0, gradiantAlpha));
	vertices[nVertices] = CCPointMake(textureWidth, textureHeight);
	colors[nVertices++] = ccColor4F(ccc4f(0, 0, 0, gradiantAlpha));


	ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color);
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, FALSE, 0, colors);
	glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nVertices);

	// Create Noise
	CCSprite* noise = CCSprite::create("Noise2/Noise.png");
	ccBlendFunc blendFunc;
	blendFunc.dst = GL_ZERO;
	blendFunc.src = GL_DST_COLOR;
	noise->setBlendFunc(blendFunc);
	noise->setPosition(ccp(textureWidth / 2, textureHeight / 2));
	noise->visit();

	// 4: Call CCRenderTexture:end
	rt->end();

	// 5: create new sprite from the texture
	return CCSprite::createWithTexture(rt->getSprite()->getTexture());
}

CCSprite* HelloWorld::stripedSpriteWithColor(ccColor4F color1, ccColor4F color2, float textureWidth, float textureHeight,
	int nStripes)
{
	// 1. create render texture
	CCRenderTexture* rt = CCRenderTexture::create(textureWidth, textureHeight);

	// 2. begin render texture
	rt->beginWithClear(color1.r, color1.g, color1.b, color1.a);

	// 3. draw

	// Layer 1: Noise
	CCSprite *noise = CCSprite::create("Noise2/Noise.png");
	ccBlendFunc blendFunc;
	blendFunc.dst = GL_ZERO;
	blendFunc.src = GL_DST_COLOR;
	noise->setBlendFunc(blendFunc);
	noise->setPosition(ccp(textureWidth / 2, textureHeight / 2));
	noise->visit();

	// Layer 2: Stripes
	CCPoint vertices[48];
	ccColor4F colors[48];
	int nVertices = 0;

	float x1 = -textureHeight;
	float x2;
	float y1 = textureHeight;
	float y2 = 0;
	float dx = textureWidth / nStripes * 2;
	float stripeWidth = dx / 2;
	for (int i = 0; i < nStripes; i++) {
		x2 = x1 + textureHeight;

		vertices[nVertices] = CCPointMake(x1, y1);
		colors[nVertices++] = color2;

		vertices[nVertices] = CCPointMake(x1 + stripeWidth, y1);
		colors[nVertices++] = ccColor4F(ccc4f(0, 0, 0, 0));

		vertices[nVertices] = CCPointMake(x2, y2);
		colors[nVertices++] = color2;

		vertices[nVertices] = vertices[nVertices - 2];
		colors[nVertices++] = color2;

		vertices[nVertices] = vertices[nVertices - 2];
		colors[nVertices++] = color2;

		vertices[nVertices] = CCPointMake(x2 + stripeWidth, y2);
		colors[nVertices++] = color2;

		x1 += dx;
	}

	this->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor));

	CC_NODE_DRAW_SETUP();
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, 0, colors);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)nVertices);

	// layer 3: gradiant alpha
	float gradiantAlpha = 0.7f;
	nVertices = 0;

	vertices[nVertices] = CCPointMake(0, 0);
	colors[nVertices++] = ccColor4F(ccc4f(0, 0, 0, 0));
	vertices[nVertices] = CCPointMake(textureWidth, 0);
	colors[nVertices++] = ccColor4F(ccc4f(0, 0, 0, 0));
	vertices[nVertices] = CCPointMake(0, textureHeight);
	colors[nVertices++] = ccColor4F(ccc4f(0, 0, 0, gradiantAlpha));
	vertices[nVertices] = CCPointMake(textureWidth, textureHeight);
	colors[nVertices++] = ccColor4F(ccc4f(0, 0, 0, gradiantAlpha));

	ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color);
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, FALSE, 0, colors);
	glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nVertices);

	// 4. end render texture
	rt->end();

	// 5. create new sprite from the texture
	return CCSprite::createWithTexture(rt->getSprite()->getTexture());
}

void HelloWorld::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent)
{
	_tapDown = true;
	this->genBackground();
}

void HelloWorld::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent)
{
	_tapDown = false;
}

void HelloWorld::ccTouchesCancelled(CCSet* pTouches, CCEvent* pEvent)
{
	_tapDown = false;
}

void HelloWorld::setupWorld()
{
	b2Vec2 gravity = b2Vec2(0.0f, -7.0f);
	bool doSleep = true;
	_world = new b2World(gravity);
	_world->SetAllowSleeping(doSleep);
}

void HelloWorld::createTestBodyAtPostition(CCPoint position)
{

	b2BodyDef testBodyDef;
	testBodyDef.type = b2_dynamicBody;
	testBodyDef.position.Set(position.x / PTM_RATIO, position.y / PTM_RATIO);
	b2Body * testBody = _world->CreateBody(&testBodyDef);

	b2CircleShape testBodyShape;
	b2FixtureDef testFixtureDef;
	testBodyShape.m_radius = 25.0 / PTM_RATIO;
	testFixtureDef.shape = &testBodyShape;
	testFixtureDef.density = 1.0;
	testFixtureDef.friction = 0.2f;
	testFixtureDef.restitution = 0.5;
	testBody->CreateFixture(&testFixtureDef);

}

void HelloWorld::update(float delta)
{
	if (_tapDown) {
		if (!_hero->awake) {
			_hero->wake();
			_tapDown = false;
		}
		else {
			_hero->dive();
		}
	}
	
	_hero->limitVelocity();

	static double UPDATE_INTERVAL = 1.0f / 60.0f;
	static double MAX_CYCLES_PER_FRAME = 5;
	static double timeAccumulator = 0;

	timeAccumulator += delta;
	if (timeAccumulator > (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL)) {
		timeAccumulator = UPDATE_INTERVAL;
	}

	int32 velocityIterations = 3;
	int32 positionIterations = 2;
	while (timeAccumulator >= UPDATE_INTERVAL) {
		timeAccumulator -= UPDATE_INTERVAL;
		_world->Step(UPDATE_INTERVAL, velocityIterations, positionIterations);
		_world->ClearForces();
	}

	_hero->update();
	float offset = _hero->getPositionX();
	CCSize textureSize = _background->getTextureRect().size;
	_background->setTextureRect(CCRectMake(offset, 0, textureSize.width, textureSize.height));
	_terrain->setOffsetX(offset);
}
