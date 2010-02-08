#include "stdafx.h"
#include "LuaFnRegister.h"
#include "LuaFunc/LuaFnTest.h"
#include "LuaInterface.h"

namespace tp_script
{
	
	struct   reg_luafun  functbl []	=
	{
		{"test1" , func(tp_script::luatest1) } ,
		{"test2" , func(tp_script::luatest2) } ,
	};

	CLuaFnRegister::CLuaFnRegister()
	{
		m_pOwner = NULL;
	}

	CLuaFnRegister::~CLuaFnRegister()
	{
		//pOwner = NULL;
	}

	void  CLuaFnRegister::RegisterFun()
	{
		size_t  size = sizeof(functbl) / sizeof(reg_luafun);
		RegisterFun( functbl , size );
	}

	void  CLuaFnRegister::SetOwner( CLuaInterface* pOwner )
	{
		m_pOwner = pOwner;
	}

	CLuaInterface* CLuaFnRegister::GetOwner() const 
	{
		return m_pOwner;
	}

	///
	/// ����:	����ע��Lua���ڲ�C������������������Ϣ������reg_luafun��������
	/// ����:	reg_luafun *Funcs �����ָ��
	/// ����:	int n ��������������Ϊ�㣬��ϵͳ����õ���
	///
	bool CLuaFnRegister::RegisterFun(reg_luafun Funcs[], size_t n)
	{
		if ( !m_pOwner->m_MainState)	return false;

		if (n == 0)	
			n = sizeof(Funcs) / sizeof(Funcs[0]);

		for (size_t i = 0; i < n; i ++)	
			lua_register(m_pOwner->m_MainState, Funcs[i].name, Funcs[i].func);
		return true;
	}
}