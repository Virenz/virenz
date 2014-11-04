#include "GameOver.h"

static GameOver* _layer = nullptr;
GameOver* GameOver::getInstance()
{
	if(!_layer)
	{
		_layer = new GameOver();
		_layer->initLayer();
		_layer->setScale(0.5);
	}
	return _layer;
}

void GameOver::initLayer()
{
	auto bg = Sprite::create("over.png");
	this->addChild(bg);
	this->setContentSize(Size(bg->getContentSize().width
		,bg->getContentSize().height));

	_newScore = Label::createWithBMFont("fonts/font-issue1343.fnt", "99999");
	_highestScore = Label::createWithBMFont("fonts/font-issue1343.fnt", "99999");
	this->addChild(_newScore);
	this->addChild(_highestScore );

	_highestScore->setPositionY(-100);
	
	auto play = MenuItemImage::create(
		"play.png",
		"play.png",
		CC_CALLBACK_0(GameOver::onCallBack, this));
	auto menu = Menu::create(play, NULL);
	this->addChild(menu);
	menu->setPosition(0,play->getContentSize().height-bg->getContentSize().height/2);

	newScore = Sprite::create("newScore.png");
	this->addChild(newScore);
	newScore->setPositionY(newScore->getContentSize().width/3);
}

void GameOver::onCallBack()
{
	if(callBackFun)
		callBackFun();
	this->setVisible(false);
}

void GameOver::setScore(int score)
{
	int maxScore = score;
	char string[50] = {0};
	sprintf(string, "Score %d", score);
	_newScore->setString(string);

	maxScore = UserDefault::getInstance()->getIntegerForKey("maxScore");
	if( maxScore < score )
	{
		UserDefault::getInstance()->setIntegerForKey("maxScore",score);
	}
	newScore->setVisible(( maxScore < score ));
	char str2[50] = {0};
	sprintf(str2, "Max Score %d", ( maxScore < score ) ? score : maxScore);
	_highestScore->setString(str2);

	UserDefault::getInstance()->flush();
}