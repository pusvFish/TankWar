#include "TiledMapMgr.h"
#include "DynamicDt.h"
CTiledMapMgr::CTiledMapMgr()
	:m_nLevelID(0)
{
}

CTiledMapMgr::~CTiledMapMgr()
{
}

bool CTiledMapMgr::init()
{
	if (!Node::init())
	{
		return false;
	}

	m_nLevelID = CDynamicDt::getInstance()->getLevelID();
	this->initMap();
	this->initBirthPos();
	this->initLayers();
	return true;
}

string CTiledMapMgr::checkColliLayer(const Vec2& tile)
{
	unsigned int nGID = m_pBlock->getTileGIDAt(tile);
	if ( nGID > 0 )
	{
		return "block";
	}

	nGID = m_pIronBlock->getTileGIDAt(tile);
	if (nGID > 0)
	{
		return "ironBlock";
	}

	/*nGID = m_pGlass->getTileGIDAt(tile);
	if (nGID > 0)
	{
		return "glass";
	}*/
	
	nGID = m_pWater->getTileGIDAt(tile);
	if (nGID > 0)
	{
		return "water";
	}
	
	nGID = m_pEagle->getTileGIDAt(tile);
	if (nGID > 0)
	{
		return "eagle";
	}

	return "none";
}



void CTiledMapMgr::initMap()
{
	char szName[32] = {};
	sprintf_s(szName, "maps/map%d.tmx", m_nLevelID);
	m_pTiledMap = TMXTiledMap::create(szName);
	this->addChild(m_pTiledMap);
}

void CTiledMapMgr::initLayers()
{
	m_pBlock = m_pTiledMap->getLayer("block");
	m_pWater = m_pTiledMap->getLayer("water");
	m_pEagle = m_pTiledMap->getLayer("eagle");
	m_pIronBlock = m_pTiledMap->getLayer("ironBlock");
	m_pGlass = m_pTiledMap->getLayer("glass");
}

void CTiledMapMgr::initBirthPos()
{
   Vector<TMXObjectGroup*> VecGroup = m_pTiledMap->getObjectGroups();
   for ( TMXObjectGroup* pGroup : VecGroup  )
   {
	   if ("player" == pGroup->getGroupName())
	   {
		   ValueVector objs = pGroup->getObjects();
		   for (cocos2d::Value& obj : objs)
		   {
			   ValueMap valueMap = obj.asValueMap();
			   float fX = valueMap["x"].asInt();
			   float fY = valueMap["y"].asInt();
			   Vec2 pos(fX + 24,fY + 8);
			   m_vecPlayerBirthpos.push_back(pos);
		   }
	   }
	   else
	   {
		   ValueVector objs = pGroup->getObjects();
		   for (cocos2d::Value& obj : objs)
		   {
			   ValueMap valueMap = obj.asValueMap();
			   float fX = valueMap["x"].asInt();
			   float fY = valueMap["y"].asInt();
			   Vec2 pos(fX + 24, fY + 8);
			   m_vecEnemyBirthpos.push_back(pos);
		   }
	   }

	   
   }
}

Vec2 CTiledMapMgr::getTileByPos( Vec2& worldPos)
{
	worldPos -= Vec2(24,8);
	Size sizeTile = m_pTiledMap->getTileSize();
	Size sizeMap = m_pTiledMap->getMapSize();

	int nX = worldPos.x / sizeTile.width;
	int nY = (sizeMap.height * sizeTile.height - worldPos.y) / sizeTile.height;
	return Vec2(nX, nY);
}

Vec2 CTiledMapMgr::getPosByTile(const Vec2& tile)
{
	Size sizeTile = m_pTiledMap->getTileSize();
	Size sizeMap = m_pTiledMap->getMapSize();

	int nX = tile.x * sizeTile.width + sizeTile.width / 2;
	int nY = sizeMap.height * sizeTile.height - (tile.y * sizeTile.height + sizeTile.height / 2);
	return Vec2(nX + 24, nY + 8);
}

void CTiledMapMgr::removeWithPos(Vec2 WorldPos)
{

	Vec2 tile = this->getTileByPos(WorldPos);
	//unsigned int nGID = m_pBlock->getTileGIDAt(tile);
	m_pBlock->removeTileAt(tile);

}

void CTiledMapMgr::eagleDead()
{
	m_pEagle->setVisible(false);
}
