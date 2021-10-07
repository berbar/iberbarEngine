#pragma once

#include <iberbar/Lua/LuaBase.h>
#include <iberbar/Lua/LuaError.h>
#include <iberbar/Utility/Unknown.h>

namespace iberbar
{
	namespace Lua
	{
		struct UClassObjectType
		{
			const char* classFullName;
			int nLuaRef;
			void* data;
		};

		template < typename T >
		int Class_Default_Constructor_New( lua_State* L );

		template < typename T >
		int Class_Default_Distructor_Delete( lua_State* L );

		template < typename T >
		int Class_Unknown_Constructor_New( lua_State* L );

		template < typename T >
		int Class_Unknown_Distructor_Release( lua_State* L );

		template < typename T >
		int Class_Unknown_Dynamic_Cast( lua_State* pLuaState );


		// ��new���췽���л�ȡ��Ӧc++��metatable��c++����
		__iberbarLuaApi__ int Class_Default_Constructor_New_GetMetatable( lua_State* L, const char* (*ppClassFullName), int* metatable );

		// ��new���췽���л�ȡ��Ӧc++��metatable��c++����
		__iberbarLuaApi__ int Class_Unknown_Constructor_New_GetMetatable( lua_State* L, const char* (*ppClassFullName), int* metatable );

		// ��c++����ѹ��luaջ
		__iberbarLuaApi__ void Class_Default_PushObjectWithName( lua_State* L, const char* classFullName, void* instance );

		// ��c++����ѹ��luaջ
		__iberbarLuaApi__ void Class_Default_PushObject( lua_State* L, int metatable, void* instance, const char* strClassFullName = nullptr );

		// ��c++����ѹ��luaջ
		__iberbarLuaApi__ void Class_Ref_PushObjectWithName( lua_State* pLuaState, const char* strClassFullName, CRef* pInstance );

		// ��c++����ѹ��luaջ
		__iberbarLuaApi__ void Class_Ref_PushObject( lua_State* pLuaState, int nMetatable, CRef* pInstance, const char* strClassFullName = nullptr );

		// ��luaջ��ת��Ϊc++����
		template < typename T >
		T* ToClassObject( lua_State* L, int narg );

		// ��luaջ��ת��Ϊc++����
		template < typename T >
		T* ToClassObject( lua_State* pLuaState, int nArg, const char* strClassName );
	}
}



template < typename T >
int iberbar::Lua::Class_Default_Constructor_New( lua_State* L )
{
	int metatable;
	const char* strClassFullName;
	int ret = Class_Default_Constructor_New_GetMetatable( L, &strClassFullName, &metatable );
	if ( ret > 0 )
		return ret;

	// �������
	T* instance = new T();
	iberbar::Lua::Class_Default_PushObject( L, metatable, instance, strClassFullName );

	return 1;
}

template < typename T >
int iberbar::Lua::Class_Default_Distructor_Delete( lua_State* L )
{
	int n = lua_gettop( L );

	if ( lua_isuserdata( L, 1 ) == false )
		return 0;

	UClassObjectType* s = (UClassObjectType*)lua_touserdata( L, 1 );
	if ( s == nullptr )
		return 0;

	if ( s->data != nullptr )
	{
		delete ((T*)(s->data));  // call destructor for T objects
		s->data = nullptr;
	}

	return 0;
}


template < typename T >
int iberbar::Lua::Class_Unknown_Constructor_New( lua_State* L )
{
	int metatable;
	const char* strClassFullName;
	int ret = Class_Default_Constructor_New_GetMetatable( L, &strClassFullName, &metatable );
	if ( ret > 0 )
		return ret;

	// �������
	T* instance = new T();
	iberbar::Lua::Class_Default_PushObject( L, metatable, instance, strClassFullName );

	return 1;
}

template < typename T >
int iberbar::Lua::Class_Unknown_Distructor_Release( lua_State* L )
{
	int n = lua_gettop( L );

	if ( lua_isuserdata( L, 1 ) == false )
		return 0;

	UClassObjectType* s = (UClassObjectType*)lua_touserdata( L, 1 );
	if ( s == nullptr )
		return 0;

	if ( s->data != nullptr )
	{
		((CRef*)(s->data))->Release();  // call destructor for T objects
		s->data = nullptr;
	}

	return 0;
}


template < typename T >
T* iberbar::Lua::ToClassObject( lua_State* L, int narg )
{
	if ( lua_isuserdata( L, narg ) == false )
		return nullptr;
	if ( lua_isnil( L, narg ) == true )
		return nullptr;
	UClassObjectType* s = (UClassObjectType*)lua_touserdata( L, narg );
	if ( s == nullptr )
		return nullptr;
	return (T*)s->data;
}


template < typename T >
T* iberbar::Lua::ToClassObject( lua_State* pLuaState, int nArg, const char* strClassName )
{
	if ( lua_isuserdata( pLuaState, nArg ) == false )
		return nullptr;
	if ( lua_isnil( pLuaState, nArg ) == true )
		return nullptr;
	UClassObjectType* s = (UClassObjectType*)lua_touserdata( pLuaState, nArg );
	if ( s == nullptr )
		return nullptr;
	if ( strcmp( strClassName, s->classFullName ) != 0 )
		return nullptr;
	return (T*)s->data;
}


template < typename T >
int iberbar::Lua::Class_Unknown_Dynamic_Cast( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );


	int nMetatable = 0;
	const char* strClassName = nullptr;
	Class_Unknown_Constructor_New_GetMetatable( pLuaState, &strClassName, &nMetatable );

	// �ж���Ҫת���Ķ����Ƿ�Ϊ�ջ���userdata
	if (lua_isnil( pLuaState, 2 ) == true || lua_isuserdata( pLuaState, 2 ) == false)
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	CRef* pRef = ToClassObject<CRef>( pLuaState, 2 );
	if ( pRef == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

//#ifdef __iberbarUseRtti__
//	T* pReturn = dynamic_cast<T*>(pRef);
//#else
//	T* pReturn = static_cast<T*>( pRef );
//#endif
	T* pReturn = static_cast<T*>(pRef);

	Lua::Class_Ref_PushObject( pLuaState, nMetatable, pReturn, strClassName );

	return 1;
}


inline int iberbar::Lua::Class_Unknown_Constructor_New_GetMetatable( lua_State* L, const char* (*ppClassFullName), int* metatable )
{
	return Class_Default_Constructor_New_GetMetatable( L, ppClassFullName, metatable );
}

