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
		Sprite* pNumber = Sprite::create("images/black-number.png", rc);//��ͼƬ���Ͻ�(0,0)��ʼ��ȡ��ê����Sprite��ͬ
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

