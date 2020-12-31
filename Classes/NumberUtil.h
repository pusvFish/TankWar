#ifndef _NumberUtil_H_
#define _NumberUtil_H_
#include "cocos2d.h"
USING_NS_CC;
class CNumberUtil
{
public:
	static Sprite* getBalckNumber(int nNumber)
	{
		Rect rc(7.10f * nNumber, 0, 7.0f, 7.0f);
		Sprite* pNumber = Sprite::create("images/black-number.png", rc);//从图片左上角(0,0)开始截取，锚点与Sprite相同
		pNumber->getTexture()->setAliasTexParameters();
		return pNumber;
	}

	static Sprite* getWhiteNumber(int nNumber)
	{
		Rect rc(7.10f * nNumber, 0, 7.0f, 7.0f);
		Sprite* pNumber = Sprite::create("images/white-number.png", rc);
		pNumber->getTexture()->setAliasTexParameters();
		return pNumber;
	}

	static Sprite* getYellowNumber(int nNumber)
	{
		Rect rc(7.10f * nNumber, 0, 7.0f, 7.0f);
		Sprite* pNumber = Sprite::create("images/yellow-number.png", rc);
		pNumber->getTexture()->setAliasTexParameters();
		return pNumber;
	}

};


#endif // !_NumberUtil_H_

