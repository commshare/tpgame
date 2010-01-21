#pragma once

#include "BaseDef.h"
#include "BaseGameEntity.h"

class   CEntityManager
{
	typedef std::map< eEntityType , CBaseGameEntity* >  EntityManager;
	typedef EntityManager::iterator						EntityIter;

private:
	// ʵ�������
	EntityManager		m_EManager;
	
	// �������� ����˽�к���
private:
	CEntityManager();
	~CEntityManager();
	CEntityManager(const CEntityManager&);
	CEntityManager& operator=(const CEntityManager&);

	// ��Ա����
public:

	void	RegEntityManager(CBaseGameEntity* Entity);

	CBaseGameEntity*	GetEntity( eEntityType& e );

	void	RemoveEntity( CBaseGameEntity*    Entity);
};