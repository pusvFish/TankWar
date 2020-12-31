#ifndef _GameOver_H_
#define _GameOver_H_
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class CGameOver : public Scene
{
public:
	CGameOver();
	~CGameOver();

	virtual bool init();
	CREATE_FUNC(CGameOver);
private:
	Size visibleSize;
	Vec2 origin;
};




#endif // !_GameMenu_H_

