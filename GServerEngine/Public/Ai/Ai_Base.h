#pragma once

namespace AI_SYSTEM
{
	
	typedef OPERATE_RESULT ORESULT;
	class CBaseAi
	{
	public:
		CBaseAi(void);

		~CBaseAi(void);
		
		virtual	 bool	Init( CObj_MoveShape	* pOwner);

		virtual  void	Term( void );

	public:
	
		/// ��ʱ����װ����
	public:

		/// ��Ϊ���(������Ϊ)
	public:

		/// ������Ϊ
	public:

		/// �¼����
	public:
		

	private:
		CObj_MoveShape		m_pOwner;

	};
}