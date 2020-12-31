#include "DynamicDt.h"
CDynamicDt* CDynamicDt::m_pInstance = nullptr;
CDynamicDt::CDynamicDt()
	:m_nLevelID(1),
	m_nEnemyCount(20),
	m_nLives(3)
{
}

CDynamicDt::~CDynamicDt()
{
}

CDynamicDt* CDynamicDt::getInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CDynamicDt();
		return m_pInstance;
	}
	return m_pInstance;
}

void CDynamicDt::addLevelID()
{
	m_nLevelID++;
}
