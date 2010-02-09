/*
*	File  :
*   Brief :
*   Author:  Expter 
*	Creat Date:  [2010/2/4]
*/

#pragma  once

#include "LuaFnRegister.h"

namespace tp_script
{
	
#define STATE_TRS(S)  (long)(S)

	class  CLuaInterface
	{

		typedef  std::map<long,CLuaScript*>   ScriptTable;
		typedef  ScriptTable::iterator		  ScriptIter;

 	public:

		lua_State*		m_MainState;

		ScriptTable		m_Scriptt;

		/// �ű�����
		/// CLuaScript		m_LuaParse;

		/// Luaע����
		//CLuaFnRegister  m_LuaFnReg;

		/// �ű��б�
		//TableList		m_ScriptTable;



		/// ִ�нű���object
		void			*m_pOwner;

	public:

		CLuaInterface();

		~CLuaInterface();

		void  Init();

		CLuaScript*  Create();

		void  Destroy(lua_State* L);

		template< typename owner>
		void  SetOwner(owner * powner) { m_pOwner = powner; }

		template< typename owner>
		owner* GetOwner()			   { return m_pOwner ;  }
	    
		bool  FindSymbol( lua_State* L , char* funcname );

		bool  PrefixCall( lua_State* L , char** funcname );

		int   ExeScript( int  sid , char* funcname );

		int   ExeScript( int  sid , char* funcname , int Param0 );


	private:

		int   ExeFile( char * filename , char* funcanme , bool bload );

	};

}