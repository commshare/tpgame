#pragma once

#include "Ai/States.h"
#include "obj/Region.h"

namespace AI_SYSTEM
{
	typedef OPERATE_RESULT			ORESULT;
	typedef AI_SYSTEM::CRegion		Region;
	class CBaseAi
	{
	public:
		/*
		 *	��ʼ����Ϣ
		 */
		CBaseAi(void);

		~CBaseAi(void);

		virtual	 bool	Init( CObj_MoveShape	* pOwner);

		virtual  void	Term( void );

		/// ��ʱ����װ����
	public:

	
		/// ��Ϊ���(������Ϊ)
	public:
		/*
		 *	������Ϊ��Ӧ,	������Ϊ��Ӧ�� ��ͬ״̬����Ϊ��ƽ��ս�������ܣ�����������
		 */
		virtual		ORESULT		OnActive(void);
		virtual		ORESULT		OnPeaceState();
		virtual		ORESULT		OnFightState();
		virtual		ORESULT		OnEscapeState();
		virtual		ORESULT		OnDeadState();
		virtual		ORESULT		OnHoldOnState();
		
		/// ��Ϊ���(������Ϊ)
	public:
		/*
		 *	 ����������ʱ��
		 */
		virtual		ORESULT		WhenBeenHurted();
		virtual		ORESULT		WhenBeenKilled()

		/// ������Ϊ
	public:

		virtual		ORESULT		Run( void );
		virtual		ORESULT		Jump( void );
		virtual		ORESULT		Stop( void );
		virtual		ORESULT		Stall( void );
		virtual		ORESULT		Move ( const Region *pTar );
		virtual		ORESULT		MoveTo( long lDesX , long  lDesY );
		virtual		ORESULT		RandomRun( void );
		virtual		ORESULT		RunByRoad( void );
		virtual		ORESULT		Obj_Move(  const Region *pTar );

		//	virtual		ORESULT	Obj_UseSkill( long  SkillID ,..);


		/// �¼����
	public:

		/*
		 *	ȫ��AI�߼�
		 */
	public:
		virtual		bool	Logic()		{	return	m_Aistates.Logic(); }
		//virtual		

	private:
		CObj_MoveShape		m_pOwner;
		CStates				m_Aistates;
	};
}