#ifndef __GameScore__
#define __GameScore__

#include "cocos2d.h"
USING_NS_CC;

class GameScore : public Layer
{
public:
	static GameScore* createScore();
	void initScore();
	std::function<void()> callBackFun;
	void setScore(int score);
	void onReset();

private:
	Sprite* _blood;
	int _score;
	float _speed;
	int _addPre;
	int _levelNum;
	Node* _level;
	Label* _scoreLabel;
	ProgressTimer* progress;

	void callBack();
	void setLevel();
};

#endif