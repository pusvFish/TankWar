#include "GameOver.h"
#include "GameScene.h"
#include "GameMenu.h"
CGameOver::CGameOver()
{
}

CGameOver::~CGameOver()
{
}

bool CGameOver::init()
{
	if (!Scene::init())
	{
	return false;
	}
	//
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	Sprite* pBg = Sprite::create("images/big-gameover.png");
	this->addChild(pBg);
	this->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

	this->scheduleOnce([this](float)
		{
			//CGameScene::getInstance()->destroyAndNext();
			Director::getInstance()->replaceScene(TransitionFade::create(1.0f, CGameMenu::create()));
		},
		3.0f,
		"reStart");
	return true;
}
