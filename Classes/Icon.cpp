#include "Icon.h"

CIcon::CIcon()
{
}

CIcon::~CIcon()
{
}

bool CIcon::init()
{
	if (!Node::init())
	{
		return false;
	}

	m_pImg = Sprite::createWithSpriteFrameName("icon");
	m_pImg->getTexture()->setAliasTexParameters();
	this->addChild(m_pImg);
	return true;
}
