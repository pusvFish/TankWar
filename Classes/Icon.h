#ifndef _Icon_H_
#define _Icon_H_
#include "cocos2d.h"
#include "AudioEngine.h"
USING_NS_CC;
using namespace std;
class CIcon : public Node
{
public:
	CIcon();
	~CIcon();
	virtual bool init();
	CREATE_FUNC(CIcon);
	CC_SYNTHESIZE_READONLY(Sprite*, m_pImg, Img);
private:

};


#endif 

