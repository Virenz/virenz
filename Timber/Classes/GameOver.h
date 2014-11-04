#ifndef __GameOver__
#define __GameOver__
#include "cocos2d.h"
USING_NS_CC;
using namespace std;
class GameOver : public Layer
{
public:
	static GameOver* getInstance();
	void setScore(int score);
	std::function<void()> callBackFun;
private:
	void onCallBack();
	void initLayer();

	Sprite* newScore;
	Label* _newScore;
	Label* _highestScore;
};

#endif