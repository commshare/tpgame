/*
*	File  :
*   Brief :
*   Author:  Expter 
*	Creat Date:  [2010/2/4]
*/

#pragma  once

#include "LuaFnRegister.h"
#include "itable.h"

namespace tp_script
{
	
	class  CLuaFnRegister; 

	class  CLuaInterface
	{
	private:

		/// �ű�����
		CLuaScript		m_LuaParse;

		/// Luaע����
		CLuaFnRegister  m_LuaFnReg;

		/// �ű��б�
		TableList		m_ScriptTable;

	public:

	};

}