#ifndef _DynamicDt_H_
#define _DynamicDt_H_
#include "cocos2d.h"
USING_NS_CC;
class CDynamicDt
{
public:
	
	~CDynamicDt();
	static CDynamicDt* getInstance();
	CC_SYNTHESIZE(int, m_nLevelID,LevelID);
	CC_SYNTHESIZE(int, m_nEnemyCount, EnemyCount);
	CC_SYNTHESIZE(int, m_nLives, Lives);
	void addLevelID();
private:
	CDynamicDt();
	static CDynamicDt* m_pInstance;

};


#endif 

