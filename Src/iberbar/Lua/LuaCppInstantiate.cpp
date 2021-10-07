
#include <iberbar/Lua/LuaCppCommon.h>
#include <iberbar/Lua/LuaCppInstantiate.h>





namespace iberbar
{
	namespace Lua
	{
		void Class_XXX_PushObject(
			lua_State* pLuaState,
			int nMetatable,
			void* pInstanceVoid,
			const char* strClasName,
			CRef* pInstanceRef );
	}
}





FORCEINLINE void iberbar::Lua::Class_XXX_PushObject(
	lua_State* pLuaState,
	int nMetatable,
	void* pInstanceVoid,
	const char* strClasName,
	CRef* pInstanceRef )
{
	assert( pInstanceVoid );

	lua_pushstring( pLuaState, uWeakTable_ForUserData );
	lua_gettable( pLuaState, nMetatable );
	// �����Ƿ���userdata����
	//if ( lua_isnil( pLuaState, -1 ) || lua_istable( pLuaState, -1 ) )
	//{
	//	lua_pop( pLuaState, 1 );
	//	lua_pushnil( pLuaState );
	//	return;
	//}
	int nUserdata = lua_gettop( pLuaState );

	// �ȴ�metatable�л����л�ȡ����
	lua_pushlightuserdata( pLuaState, pInstanceVoid );
	lua_gettable( pLuaState, nUserdata );
	if ( lua_isnil( pLuaState, -1 ) )
	{
		lua_pop( pLuaState, 1 );

		UClassObjectType* userdata = (UClassObjectType*)lua_newuserdata( pLuaState, sizeof( UClassObjectType ) );
		if ( userdata == nullptr )
			return;

		//
		// �Ƚ�ֹ��userdata����lua��metatable���棬�����޷��Զ�GC����ʱ���ٿ����µĽṹ
		//
		// ��ʵ����userdata��ŵ�metatable����
		lua_pushlightuserdata( pLuaState, pInstanceVoid );
		lua_pushvalue( pLuaState, -2 );  // dup userdata
		lua_settable( pLuaState, nUserdata );

		if ( userdata )
		{
			userdata->classFullName = strClasName;
			userdata->data = pInstanceVoid;
			lua_pushvalue( pLuaState, nMetatable );
			lua_setmetatable( pLuaState, -2 );
		}

		if ( pInstanceRef != nullptr )
		{
			pInstanceRef->AddRef();
		}

		// ��ջ���Ķ������һ���metatable����λ�ã�Ȼ��metatable����
		lua_replace( pLuaState, -2 );
	}
	else
	{
		lua_replace( pLuaState, -2 );
	}
}





void iberbar::Lua::Class_Default_PushObjectWithName( lua_State* pLuaState, const char* strClassFullName, void* pInstance )
{
	// ��ȡ metatable
	luaL_getmetatable( pLuaState, strClassFullName );
	if ( lua_isnil( pLuaState, -1 ) )
	{
		luaL_error( pLuaState, "%s missing metatable", strClassFullName );
	}
	int nMetatable = lua_gettop( pLuaState );
	Class_XXX_PushObject( pLuaState, nMetatable, pInstance, strClassFullName, nullptr );
	lua_replace( pLuaState, -2 );
}


void iberbar::Lua::Class_Default_PushObject( lua_State* pLuaState, int nMetatable, void* pInstance, const char* strClassFullName )
{
	Class_XXX_PushObject( pLuaState, nMetatable, pInstance, strClassFullName, nullptr );
}


void iberbar::Lua::Class_Ref_PushObjectWithName( lua_State* pLuaState, const char* strClassFullName, CRef* pInstance )
{
	// ��ȡ metatable
	luaL_getmetatable( pLuaState, strClassFullName );
	if ( lua_isnil( pLuaState, -1 ) )
	{
		luaL_error( pLuaState, "%s missing metatable", strClassFullName );
	}
	int nMetatable = lua_gettop( pLuaState );
	Class_XXX_PushObject( pLuaState, nMetatable, pInstance, strClassFullName, pInstance );
	lua_replace( pLuaState, -2 );
}


void iberbar::Lua::Class_Ref_PushObject( lua_State* pLuaState, int nMetatable, CRef* pInstance, const char* strClassFullName )
{
	Class_XXX_PushObject( pLuaState, nMetatable, pInstance, strClassFullName, pInstance );
}


int iberbar::Lua::Class_Default_Constructor_New_GetMetatable( lua_State* L, const char* (*ppClassFullName), int* pOutMetatable )
{
	*ppClassFullName = nullptr;
	*pOutMetatable = 0;

	// ͨ���հ����ݵ���������
	int upvalue_1 = lua_upvalueindex( 1 );
	const char* strClassFullName = lua_tostring( L, upvalue_1 );

	// ��ȡ metatable
	int metatable;
	
	// ͨ��������ȡmetatable
	//luaL_getmetatable( L, strClassFullName );  // lookup metatable in Lua registry
	//if ( lua_isnil( L, -1 ) )
	//{
	//	luaL_error( L, "%s missing metatable", strClassFullName );
	//}
	//metatable = lua_gettop( L );

	// ֱ�Ӵӵ�һ��������ȡmetatable
	metatable = 1;

	*pOutMetatable = metatable;
	*ppClassFullName = strClassFullName;

	return 0;
}

