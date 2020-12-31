#ifndef _LoadScene_H_
#define _LoadScene_H_
#include "cocos2d.h"
USING_NS_CC;

class CLoadScene : public Scene
{
public:
	CLoadScene();
	~CLoadScene();
	virtual bool init();
	CREATE_FUNC(CLoadScene);
private:
	void loadAnimation();
	void levelID();
private:
	Vec2 origin;
	Size visibleSize;

	Sprite* m_pState;
	Sprite* m_pLevelID;

	int m_nLevelID;
};




#endif // !_GameMenu_H_

