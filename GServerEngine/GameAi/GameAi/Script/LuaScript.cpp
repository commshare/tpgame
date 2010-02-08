#include "stdafx.h"
#include "LuaScript.h"


namespace tp_script
{

#define  LUA_OUTERRMSG(STR ) \
	fprintf( stderr, STR )


	/// ���캯��
	CLuaScript::CLuaScript()
	{
		m_LuaState  = lua_open( 100 );

		if ( m_LuaState == NULL )
		{
			ScriptError( LUA_CREATE_ERROR );
			m_IsRuning = false;
			return ;
		}

		m_IsRuning = true;
		m_szScriptName[0] = '\0';
	}

	/// ��������
	CLuaScript::~CLuaScript()
	{
		Exit();
	}

	bool CLuaScript::LoadBuffer(unsigned char *pBuffer, size_t dwLen)
	{
		if ( dwLen < 0 )
		{
			ScriptError( LUA_SCRIPT_LEN_ERROR );
			return false;
		}

		//prase_buffer()
		if ( luaL_loadbuffer(m_LuaState , (char*)pBuffer , dwLen , 0) != 0 )
		{
			ScriptError( LUA_SCRIPT_COMPILE_ERROR );
			return false;
		}
		return true;
	}

	bool  CLuaScript::Load(const char *FileName)
	{
		try
		{
			if( !m_LuaState )
				return false;
			if ( !luaL_loadfile(m_LuaState , FileName))
			{
				ScriptError( LUA_SCRIPT_COMPILE_ERROR );
				return false;
			}
		}
		catch( ... )
		{
			return false;
		}

		//if ( !ExecuteCode() )
		//	return false;
		return true;
	}

	/// ִ��
	bool CLuaScript::Execute()
	{
		if( m_IsRuning && m_LuaState )
			return CallFunction("main", 0 , "" );
		
		return false;
	}

	/// ִ��
	bool CLuaScript::ExecuteCode()
	{
		if ( !( m_IsRuning && m_LuaState ))
		{
			ScriptError( LUA_SCRIPT_EXECUTE_ERROR );
			return false;
		}

		int  state ; //lua_execute
		if ( state = lua_pcall (m_LuaState,0,LUA_MULTRET,0) != 0 )
		{
			ScriptError( LUA_SCRIPT_EXECUTE_ERROR , state );
			return false;
		}
		return true;
	}

	/**
	 * ����:	CallFunction
	 * ����:	����Lua�ű��ڵĺ���
	 * ����:	char* cFuncName
	 * ����:	int   nResults
	 * ����:	char* cFormat  ����ʱ��������������
	 *			n:������(double) d:����(int) s:�ַ����� f:C������  
	            n:Nil v:Value p:Point v��ΪLua֧�ֵ�
	            ����Ϊ���ε���index��ָ����index��ָ��ջ�ı�����Ϊ�ú����ĵ��ò�����	

	 *	ע�⣺���ڸú����в���������,�������֣�ϵͳ����ȷ��������double������int
	 *  ���ڣ����ֱ�����ʽ�ǲ�ͬ�ġ������Ҫע�⵱�������������ʱ����ʽ��Ӧ��d
	 *  ��������n,����ǿ�иı�Ϊdouble�Ρ��������ּ���Ĵ���  
	 */

	bool  CLuaScript::CallFunction(const char* cFuncName , int nResults ,
									char* cFormat, va_list vlist)
	{

		double nNumber ;
		char*  cString =  NULL;
		void*  pPoint  =  NULL;
		
		lua_CFunction   CFunc;
		
		int    i = 0 , nArgNum = 0 , nIndex = 0 , nRetcode = 0;

		if ( ! (m_IsRuning && m_LuaState) )
		{
			ScriptError( LUA_SCRIPT_STATES_IS_NULL  );
			return false;
		}

		lua_getglobal( m_LuaState , cFuncName );

		while ( cFormat[i] != '\0' )
		{
			switch ( cFormat[i] )
			{
				/// double 
			case 'n':
				{
					nNumber = va_arg( vlist ,double);
					lua_pushnumber( m_LuaState , nNumber );
					nArgNum ++ ;
				}
				break;

				///  int 
			case 'd':
				{
					nNumber = (double)(va_arg(vlist, int));
					lua_pushnumber(m_LuaState, (double) nNumber);
					nArgNum ++;
				}
				break;

				///  string
			case 's':
				{
					cString = va_arg(vlist, char *);
					lua_pushstring(m_LuaState, cString);
					nArgNum ++;							
				}
				break;

				/// Null
			case 'N':
				{
					lua_pushnil(m_LuaState);
					nArgNum ++;
				}
				break;

				/// call function
			case 'f':
				{
					CFunc = va_arg(vlist, lua_CFunction);
					lua_pushcfunction(m_LuaState, CFunc) ;
					nArgNum ++;
				}
				break;
				
				/// ������Ƕ�ջ��IndexΪnIndex����������
			case 'v':
				{
					nNumber = va_arg(vlist, int);
					int nIndex1 = (int) nNumber;
					lua_pushvalue(m_LuaState, nIndex1);
					nArgNum ++;
				}
				break;

				/// ����ΪһTable����
			case 't':
				{

				}
				break;

			case 'p':
				{
					pPoint = va_arg(vlist, void *);

					/// Lua_PushUserTag(m_LuaState, pPoint,m_UserTag);
					nArgNum ++;
				}
				break;
			}
			i++;	
		}

		lua_call( m_LuaState , nArgNum , nResults );
		if ( nRetcode != 0 )
		{
			ScriptError( LUA_SCRIPT_EXECUTE_ERROR , nRetcode );
			return false;
		}
		return true;
	}


	bool CLuaScript::CallFunction(const char* cFuncName, int nResults, char* cFormat, ...)
	{
		bool    bResult  = false;
		va_list vlist;
		va_start(vlist, cFormat);
		bResult = CallFunction(cFuncName, nResults, cFormat, vlist);
		va_end(vlist);
		return bResult;
	}

	///
	///  CLuaScript::GetValuesFromStack �Ӷ�ջ�л�ñ���
	/// 
	bool CLuaScript::GetValuesFromStack(const char * cFormat, ...)	
	{
		va_list vlist;
		double* pNumber = NULL;
		const char **   pString ;
		int * pInt = NULL;
		int i = 0;
		int nTopIndex = 0;
		int nIndex = 0;	  

		/// cFormat���ַ����ȣ���ʾ��Ҫȡ�Ĳ�������
		int nValueNum = 0;

		if (! m_LuaState)
			return false;

		/// Lua_GetTopIndex
		nTopIndex = lua_gettop(m_LuaState);	
		nValueNum = strlen(cFormat);
		
		/// ����ջ�������ݻ�ȡ�����Ƿ���false
		if (nTopIndex == 0 || nValueNum == 0)
			return false;

		if (nTopIndex < nValueNum)
			return false;

		nIndex = nTopIndex - nValueNum +1;
		{
			va_start(vlist, cFormat);     
			while (cFormat[i] != '\0')
			{
				switch(cFormat[i])
				{					
					/// ����ֵΪ��ֵ��,Number,��ʱLuaֻ����double�ε�ֵ
				case 'n':
					{
						pNumber = va_arg(vlist, double *);
						if (pNumber == NULL)
							return false;

						if (lua_isnumber(m_LuaState, nIndex ))
						{
							* pNumber = lua_tonumber(m_LuaState, nIndex ++ );
						}
						else
						{
							ScriptError(LUA_SCRIPT_NOT_NUMBER_ERROR);
							return false;
						}
					}
					break;
				case 'd':
					{
						pInt = va_arg(vlist, int *);
						if (pInt == NULL)
							return false;
						if ( lua_isnumber(m_LuaState, nIndex))
						{
							* pInt = (int ) lua_tonumber(m_LuaState, nIndex ++);
						}
						else
						{
							ScriptError(LUA_SCRIPT_NOT_NUMBER_ERROR);
							return false;
						}
					}
					break;
				case 's'://�ַ�����
					{
						pString = va_arg(vlist, const char **);

						if (pString == NULL)
							return false;

						if (lua_isstring(m_LuaState, nIndex))
						{
							(*pString) = (const char *)lua_tostring(m_LuaState, nIndex++);
						}
						else
						{
							ScriptError(LUA_SCRIPT_NOT_STRING_ERROR);
							return false;
						}
					}
					break;
				}

				i ++;	
			}
			va_end(vlist);     		/* Reset variable arguments.      */

		}
		return	true;
	}

	/// ��ʼ���ű�����ע��ϵͳ��׼������
	bool CLuaScript::Init( lua_State* LuaMain)
	{
		if (! m_LuaState)
		{
			m_LuaState				= lua_newthread( LuaMain );

			if (m_LuaState == NULL)
			{
				ScriptError(LUA_CREATE_ERROR);
				m_IsRuning			= false;
				return false;
			}

			m_IsRuning				= true;
			m_szScriptName[0]		= '\0';
			//m_UserTag = lua_newtag(m_LuaState)	;
		}

		RegisterStandardFunctions();
		return	true;
	}

	//---------------------------------------------------------------------------
	// ����:	CLuaScript::RegisterFunction
	// ����:	ע��ĳ�ڲ�C�������ű���
	// ����:	char* FuncName  �ڽű���ʹ�õĺ�����
	// ����:	void* Func    ʵ����Ӧ��C����ָ��
	// ����:	int Args = 0 //��KScript�ӿ����ݣ�����
	// ����:	int Flag = 0 //��KScript�ӿ�����, ����
	//---------------------------------------------------------------------------
	bool CLuaScript::RegFunction(const char* FuncName , void* Func)
	{
		if (! m_LuaState)
			return false;
		lua_register(m_LuaState, FuncName, (lua_CFunction)Func);
		return true;
	}

	bool CLuaScript::Compile(const char *filename)
	{
		Load(filename);
		return true;
	}

	// ����:	����ע��Lua���ڲ�C������������������Ϣ������reg_luafun��������
	// ����:	reg_luafun *Funcs �����ָ��
	// ����:	int n ��������������Ϊ�㣬��ϵͳ����õ���
	bool CLuaScript::RegisterFun(reg_luafun Funcs[], int n)
	{
		if (! m_LuaState)	return false;
		if (n == 0)	n = sizeof(Funcs) / sizeof(Funcs[0]);
		for (int i = 0; i < n; i ++)	lua_register(m_LuaState, Funcs[i].name, Funcs[i].func);
		return true;
	}

	/// ע��Luaϵͳ��׼�ĺ�����
	void CLuaScript::RegisterStandardFunctions()
	{
		if (! m_LuaState)		return ;
		luaL_openlibs( m_LuaState );  

		/*
// 		lua_baselibopen(m_LuaState);//Lua������
// 		Lua_OpenIOLib(m_LuaState);//���������
// 		Lua_OpenStrLib(m_LuaState);//�ַ��������
// 		Lua_OpenMathLib(m_LuaState);//��ֵ�����
// 		//Lua_OpenDBLib(m_LuaState);//���Կ�
*/
		return;	
	}


	//---------------------------------------------------------------------------
	// ����:	CLuaScript::ReleaseScript
	// ����:	�ͷŸýű���Դ��
	// ����:	bool 
	//---------------------------------------------------------------------------
	void CLuaScript::Exit()
	{
		if (! m_LuaState)		return ;
		lua_close(m_LuaState);
		m_LuaState = NULL;
		m_IsRuning = false;
	}

	template < typename  type>
	type CLuaScript::PopLuaNumber( const char* vPar)
	{
		lua_settop( m_LuaState , 0 );

		lua_getglobal( m_LuaState, vPar);

		if ( !lua_isnumber(m_LuaState ,1) )
		{
			ScriptError(LUA_SCRIPT_NOT_NUMBER_ERROR);
			return 0;
		}

		type val = (type)lua_tonumber(  m_LuaState , 1 );

		lua_pop(m_LuaState , 1 );

		return val;
	}

	std::string CLuaScript::PopLuaString( const char* vPar)
	{
		lua_settop( m_LuaState , 0 );

		lua_getglobal( m_LuaState, vPar);

		if ( !lua_isstring(m_LuaState ,1) )
		{
			ScriptError(LUA_SCRIPT_NOT_STRING_ERROR);
			return 0;
		}

		std::string str = lua_tostring(  m_LuaState , 1 );

		lua_pop(m_LuaState , 1 );

		return str;
	}

	bool CLuaScript::PopLuaBoolean( const char* vPar)
	{
		lua_settop( m_LuaState , 0 );

		lua_getglobal( m_LuaState, vPar);

		if ( !lua_isboolean(m_LuaState ,1) )
		{
			ScriptError(LUA_SCRIPT_NOT_BOOLEN_ERROR);
			return 0;
		}

		bool val = (bool)lua_toboolean( m_LuaState , 1 );

		lua_pop(m_LuaState , 1 );

		return val;
	}

	void CLuaScript::ScriptError(int Error)
	{
		char lszErrMsg[200];
		sprintf(lszErrMsg, "ScriptError %d. (%s) \n", Error, m_szScriptName);
		LUA_OUTERRMSG(lszErrMsg);
		return;
	}

	void CLuaScript::ScriptError(int Error1 ,int Error2)
	{
		char lszErrMsg[200];
		sprintf(lszErrMsg, "ScriptError %d:[%d] (%s) \n", Error1, Error2, m_szScriptName);
		LUA_OUTERRMSG(lszErrMsg);
		return;
	}

	bool CLuaScript::RunLuaSrcipt(const char* FileName)
	{
		if (int error = luaL_dofile(m_LuaState, FileName) != 0)
		{
			//throw std::runtime_error("ERROR(" + ttos(error) + "): Problem with lua script file " + FileName);
			char lszErrMsg[200];
			sprintf(lszErrMsg, "runtime_error %d (%s) \n", error,  FileName);
			LUA_OUTERRMSG(lszErrMsg);
			return false;
		}
		return true;
	}

	//---------------------------------------------------------------------------
	// ����:	CLuaScript::SafeCallBegin
	// SafeCallBegin��SafeCallEnd������Ӧ����ʹ�ã��Է�ֹ�ڵ���Lua���ⲿ����֮��
	// �ж��������ڶ�ջ��δ��������ﵽ����ǰ����ú��ջ��ռ�ô�С���䡣
	// �������ֻ�����ڵ����ⲿ����ʱ���ڲ�����������˴���
	void CLuaScript::SafeCallBegin(int * pIndex)
	{
		if (! m_LuaState)		return ;
		//lua_gettopindex(m_LuaState, pIndex);
	}

	void CLuaScript::SafeCallEnd(int nIndex)
	{
		if (! m_LuaState)	return;
		//Lua_SafeEnd(m_LuaState, nIndex);
	}

	bool CLuaScript::Stop(void)
	{
		if (! m_IsRuning)		return true;
		if (! m_LuaState)		return false;
		m_IsRuning =  false;
		return true;
	}

	// ����:	CLuaScript::ResumeScript
	// ����:	�ָ�����ֹ�Ľű�
	bool CLuaScript::Resume(void)
	{
// 		if ((! m_IsRuning) && (m_LuaState))
// 		{
// 			m_IsRuning = true;
// 			return true;
// 		}
// 		return false;

		return lua_resume( m_LuaState, 0 );
	}


	// ����:	����һ��Lua��Table���ڵ��øú���������Table������Ա֮�󣬱������
	//			SetGlobalName()�������Tableָ��һ�����֡�
	// ����:	size_t 
	size_t CLuaScript::CreateTable()
	{
		int nIndex = 0;

		nIndex = lua_gettop(m_LuaState) ;
		lua_newtable(m_LuaState);
		if (lua_gettop(m_LuaState) != ++nIndex ) 
			return -1;

		return nIndex;
	}

	void CLuaScript::SetGlobalName(const char* szName)
	{
		if (!szName) return ;
		lua_setglobal(m_LuaState, szName);
	}

	size_t CLuaScript::ModifyTable(const char* szTableName) 
	{
		if (! szTableName[0])		return -1;

		int nIndex = lua_gettop(m_LuaState);

		lua_gettable(m_LuaState, nIndex);

		if (lua_gettop(m_LuaState) != ++nIndex)		return -1;

		return nIndex;
	}

}