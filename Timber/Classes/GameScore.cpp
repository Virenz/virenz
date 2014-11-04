#include "GameScore.h"

GameScore* GameScore::createScore()
{
	auto timber = new GameScore();
	timber->initScore();
	timber->setScale(0.5f);
	return timber;
}

void GameScore::initScore()
{
	auto bg = Sprite::create("blood_bg.png");
	addChild(bg);

	_score = 0;
	_speed = 0.02f;
	_addPre = 20;
	_levelNum = 0;

	progress = ProgressTimer::create(Sprite::create("blood.png"));
	progress->setType(ProgressTimer::Type::BAR);
	progress->setPercentage(100);
	progress->setMidpoint(Vec2(0, 0));
	progress->setBarChangeRate(Vec2(1, 0));
	addChild(progress);

	_scoreLabel = Label::createWithBMFont("fonts/font-issue1343.fnt", "0");
	this->addChild(_scoreLabel);

	_level = Node::create();
	auto lev = Sprite::create("level.png");
	_level->addChild(lev, 0);
	auto lab = Label::createWithBMFont("fonts/font-issue1343.fnt", "0");
	lab->setPosition(lev->getContentSize().width / 2 + 50, 10);
	lab->setScale(1.5f);
	_level->addChild(lab, 1, "level");
	this->addChild(_level);
	_level->setVisible(false);
}

void GameScore::callBack()
{
	if (callBackFun)
	{
		callBackFun();
	}
}

void GameScore::setScore(int score)
{
	_score = score;
	int num = _score / 20;
	if (num > 20)
	{
		num = 20;
	}
	progress->stopAllActions();
	int pro = progress->getPercentage() + _addPre - num*0.5;
	if (pro > 100)
	{
		pro = 100;
	}
	char string[12] = { 0 };
	sprintf(string, "%d", score);

	_scoreLabel->setString(string);
	progress->setPercentage(pro);

	auto to1 = Sequence::create(
		ProgressTo::create(_speed*pro, 0), 
		CallFunc::create(CC_CALLBACK_0(GameScore::callBack, this)), 
		nullptr);
	progress->runAction(to1);
	setLevel();
}

void GameScore::setLevel()
{
	int num = _score / 20;
	if (num > _levelNum)
	{
		_levelNum = _score / 20;
		auto lab = static_cast<Label*>(_level->getChildByName("level"));
		char string[12] = { 0 };
		sprintf(string, "%d", _levelNum);
		lab->setString(string);
		_level->setPosition(Vec2(0, 0));
		_level->setVisible(true);
		_level->runAction(Sequence::create(MoveBy::create(0.7, Vec2(0, 200)), Hide::create(), nullptr));
	}
}

void GameScore::onReset()
{
	_score = 0;
	_speed = 0.02f;
	_addPre = 20;
	_levelNum = 0;

	progress->stopAllActions();
	progress->setPercentage(100);
	auto to1 = Sequence::create(ProgressTo::create(_speed * 100, 0), CallFunc::create(CC_CALLBACK_0(GameScore::callBack, this)), nullptr);
	progress->runAction(to1);
	char string[12] = { 0 };
	sprintf(string, "%d", 0);
	_scoreLabel->setString(string);
}