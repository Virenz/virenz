#include "HelloWorldScene.h"

#include "TreeModel.h"
#include "GameOver.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

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
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
	_score = 0;

	bg_1 = Sprite::create("bg1.jpg");
	this->addChild(bg_1);
	bg_1->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	bg_2 = Sprite::create("bg2.jpg");
	this->addChild(bg_2);
	bg_2->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	bg_2->setVisible(false);

	auto model = TreeModel::getInstance();
	model->setPosition(visibleSize.width / 2, 100);
	this->addChild(model);

	timber = Timber::createTimber();
	this->addChild(timber);
	timber->setPosition(visibleSize.width / 2 - timber->getContentSize().width / 2 - 20, 150);

	list = EventListenerTouchOneByOne::create();
	list->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(list, this);
	list->setEnabled(false);	//	set the touch isn't response
    
	score = GameScore::createScore();
	this->addChild(score);
	score->setPosition(visibleSize / 2);
	score->callBackFun = CC_CALLBACK_0(HelloWorld::menuCloseCallback, this);

	auto leftIcon = Sprite::create("left.png");
	auto rightIcon = Sprite::create("right.png");
	leftIcon->setScale(0.5f);
	rightIcon->setScale(0.5f);
	leftIcon->setPosition(leftIcon->getContentSize().width / 2, 50);
	rightIcon->setPosition(visibleSize.width - rightIcon->getContentSize().width / 2, 50);
	this->addChild(leftIcon);
	this->addChild(rightIcon);

	auto gameOver = GameOver::getInstance();
	this->addChild(gameOver);
	gameOver->callBackFun = CC_CALLBACK_0(HelloWorld::onRest, this);
	gameOver->setAnchorPoint(Vec2(0, 0.5));
	gameOver->setVisible(false);
	gameOver->setPosition(visibleSize.width / 2, visibleSize.height + gameOver->getContentSize().height / 2);

	auto play = MenuItemImage::create(
		"play.png",
		"play.png",
		CC_CALLBACK_0(HelloWorld::playGame, this));
	play->setScale(0.5);
	menu = Menu::create(play, nullptr);
	this->addChild(menu);
	menu->setPosition(visibleSize.width / 2, play->getContentSize().height * 2);

	logo = Sprite::create("logo.png");
	logo->setPosition(visibleSize.width / 2, visibleSize.height / 2 + logo->getContentSize().height);
	this->addChild(logo);

	int audioId = cocos2d::experimental::AudioEngine::play2d("music/bgmusic.mp3", true);
	cocos2d::experimental::AudioEngine::setVolume(audioId, 0.5f);

	ParticleSystem* pl = ParticleSnow::create();
	pl->setTexture(Director::getInstance()->getTextureCache()->addImage("particle.png"));
	pl->setPosition(visibleSize.width / 2, visibleSize.height);
	this->addChild(pl, 10);
	
	return true;
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *event)
{
	auto pos = touch->getLocation();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto model = TreeModel::getInstance();

	auto isRight = pos.x > visibleSize.width / 2;
	timber->playAction(isRight ? RIGHT : LEFT);
	if (isRight)
	{
		timber->setPosition(visibleSize.width / 2 + timber->getContentSize().width / 2 + 20, 150);
	}
	else
	{
		timber->setPosition(visibleSize.width / 2 - timber->getContentSize().width / 2 - 20, 150);
	}

	if (getIsOver())
	{
		timber->setTimberDie();
		gameOver();
		return false;
	}

	auto dic = visibleSize.width * 2;
	auto time = 0.5;
	auto tree = model->deleteTreeHeadNode();
	if (isRight)
	{
		tree->runAction(Spawn::create(RotateBy::create(time, -180), MoveBy::create(time, Vec2(-dic, 0)), nullptr));
	}
	else
	{
		tree->runAction(Spawn::create(RotateBy::create(time, 180), MoveBy::create(time, Vec2(dic, 0)), nullptr));
	}

	_score++;
	score->setScore(_score);
	if (getIsOver())
	{
		timber->setTimberDie();
		gameOver();
	}

	return true;
}

bool HelloWorld::getIsOver()
{
	auto model = TreeModel::getInstance();

	if (model->getFirstBranch() == timber->getTimberDir()) return true;
	return false;
}


void HelloWorld::menuCloseCallback()
{
	if (!GameOver::getInstance()->isVisible())
	{
		gameOver();
	}
}

void HelloWorld::onRest()
{
	_score = 0;
	TreeModel::getInstance()->onReset();
	score->onReset();
	timber->onReset();
	list->setEnabled(true);
	auto isBgShow = (CCRANDOM_0_1() * 10 < 5);
	bg_1->setVisible(isBgShow);
	bg_2->setVisible(!isBgShow);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	timber->setPosition(visibleSize.width / 2 - timber->getContentSize().width / 2 - 20, 150);
}

void HelloWorld::gameOver()
{
	list->setEnabled(false);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto gameOver = GameOver::getInstance();
	gameOver->setScore(_score);
	gameOver->setPositionY(visibleSize.height + gameOver->getContentSize().height / 2);
	gameOver->setVisible(true);
	gameOver->runAction(MoveBy::create(0.5, Vec2(0, -gameOver->getContentSize().height)));
}

void HelloWorld::playGame()
{
	menu->removeFromParent();
	logo->removeFromParent();
	list->setEnabled(true);
	score->onReset();
}