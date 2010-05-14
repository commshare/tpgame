// luatest1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <complex> //���� 
#include <lua.hpp>
#include <iostream>

#include "Utilities.h"

using namespace std;
#pragma  comment( lib ,"lua5.1.lib")
#pragma  comment( lib ,"lua51.lib")

struct  CCounter 
{
	CCounter() : m_count(0){ }
	int	Count() { return ++m_count; }

private:
	int m_count;
};

///
/// ��ģ��ĺ������� 
///
template <typename T>
struct CallHelper
{
	typedef T (*Func) ();
};

// template <typename T>
// typedef T (*Func) ( ) ;
template < typename T>
int  Call(typename CallHelper<T>::Func fn  , lua_State*  L , int index)
{
	RT ret = (*fn)( );
	Push( L , ret);

	return 1;
}

template < typename RT , typename P1>
int  Call( RT (*Func)(P1) , lua_State* L  ,int  inx )
{
	if (!Match(typetype<P1>(),L,inx))
	return 0;

	RT ret = (*Func)(Get(typetype<P1>,L ,inx ));
	Push( L ,ret );

	return 1;
}

unsigned char* get_pointer(lua_State *L)
{
	return (unsigned char*) lua_touserdata(L, lua_upvalueindex(1));
}

template < typename Func>
int register_proxy( lua_State* L)
{
	unsigned char* buf = get_pointer( L );
	return  Call( *(Func)buf , L , 1 );
}

template< typename Func>
void save_pointer( lua_State* L , Func  fun )
{
	unsigned char* buf = (unsigned char*)lua_newuserdata( L ,sizeof(fun) );
	memcpy( buf , &fun ,sizeof(fun));
}

template < typename Func>
void lua_pushdirectclosure( Func fn ,lua_State* L , int inx)
{
	save_pointer( L ,fn );
	lua_pushcclosure( L ,register_proxy<Func> ,inx+1 );
}

//C���������������㣬����ʵ�����鲿���������ֵ�ͽǶ� 
int calcComplex(lua_State *L) 
{ 
	// ��ջ�ж���ʵ�����鲿 
	double r = luaL_checknumber(L,1); 
	double i = luaL_checknumber(L,2); 
	complex<double> c(r,i); 
	// �������ֵ 
	lua_pushnumber(L,abs(c)); 
	// ����Ƕ� 
	lua_pushnumber(L,arg(c)*180.0/3.14159); 
	// ������� 
	return 2;
} 

int  add( int x , int y )
{
	return 100 + y;
}
int  add_proxy( lua_State *L)
{
	if ( !Match(typetype<int>(), L ,-1 ))
		return 0;
	if ( !Match(typetype<int>(), L ,-2 ))
		return 0;
	
	int x = Get( typetype<int>() , L , -1 );
	int y = Get( typetype<int>() , L , -2 );

	int res =  add( x , y );

	lua_pushnumber(L, res);
	return 1;
}

int main() 
{ 
 	char *szLua_code = 
 		"v,a = CalcComplex(3,4)" 
 		"print(v,a)"; 
 
 	lua_State *L = luaL_newstate(); 
 	luaL_openlibs(L);

// 	//����C���� 
// 	lua_register( L ,"CalcComplex" ,calcComplex);
// 	//ִ�� 
// 	bool err = luaL_loadfile( L , "..//res//l.lua") || lua_pcall(L, 0, 0, 0); 

	lua_register(L ,"add",add_proxy );
	//luaL_loadbuffer()
	bool  err = luaL_loadfile( L , "..//res//add.lua") || lua_pcall(L, 0, 0, 0); 
	if(err) 
	{ 
		cerr << lua_tostring(L, -1); 
		lua_pop(L, 1); 
	} 

	lua_close(L); 
	return 0; 
} 
