// GameAi.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Public/StateSet.h"
#include "Public/EntityManager.h"
#include "Public/StateMachine.h"
#include "Public/Event.h"
#include "Entity/CHumanEntity.h"
#include "Entity/CMonsterEntity.h"

int _tmain(int argc, _TCHAR* argv[])
{
	/// �ű�����
	
	return 0;
}


/** 

GetInstObj(CStateSet).Create();

CHumanEntity		human1(Ent_Player);
CHumanEntity		human2(Ent_Player);
CHumanEntity		human3(Ent_Player);

// 	CMonsterEntity		Monster1(Ent_Monster);
// 	CMonsterEntity		Monster2(Ent_Monster);
// 	CMonsterEntity		Monster3(Ent_Monster);

GetInstObj(CEntityManager).RegEntityManager( &human1 );
GetInstObj(CEntityManager).RegEntityManager( &human2 );
GetInstObj(CEntityManager).RegEntityManager( &human3 );

// 	GetInst(CEntityManager).RegEntityManager( &Monster1 );
// 	GetInst(CEntityManager).RegEntityManager( &Monster2 );
// 	GetInst(CEntityManager).RegEntityManager( &Monster3 );

while ( 1 )
{
/// �������е�ai
GetInstObj(CEntityManager).Update();
/// �������е��¼�
GetInstObj(CEventDispatch).DispacthEevent();

Sleep( 100 );
}
*/