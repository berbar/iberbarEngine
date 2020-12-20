
#include <iberbar/Lua/LuaCppCommon.h>
#include <iberbar/Lua/LuaCppInstantiate.h>





namespace iberbar
{
	namespace LuaCpp
	{
		void Class_XXX_PushObject(
			lua_State* pLuaState,
			int nMetatable,
			void* pInstanceVoid,
			const char* strClasName,
			CRef* pInstanceRef );
	}
}





FORCEINLINE void iberbar::LuaCpp::Class_XXX_PushObject(
	lua_State* pLuaState,
	int nMetatable,
	void* pInstanceVoid,
	const char* strClasName,
	CRef* pInstanceRef )
{
	assert( pInstanceVoid );

	lua_pushstring( pLuaState, uWeakTable_ForUserData );
	lua_gettable( pLuaState, nMetatable );
	// 检验是否有userdata缓存
	//if ( lua_isnil( pLuaState, -1 ) || lua_istable( pLuaState, -1 ) )
	//{
	//	lua_pop( pLuaState, 1 );
	//	lua_pushnil( pLuaState );
	//	return;
	//}
	int nUserdata = lua_gettop( pLuaState );

	// 先从metatable中缓存中获取对象
	lua_pushlightuserdata( pLuaState, pInstanceVoid );
	lua_gettable( pLuaState, nUserdata );
	if ( lua_isnil( pLuaState, -1 ) )
	{
		lua_pop( pLuaState, 1 );

		UClassObjectType* userdata = (UClassObjectType*)lua_newuserdata( pLuaState, sizeof( UClassObjectType ) );
		if ( userdata == nullptr )
			return;

		//
		// 先禁止将userdata存在lua的metatable里面，否则无法自动GC，有时间再考虑新的结构
		//
		// 将实例的userdata存放到metatable里面
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

		// 将栈顶的对象和下一层的metatable交换位置，然后将metatable弹出
		lua_replace( pLuaState, -2 );
	}
	else
	{
		lua_replace( pLuaState, -2 );
	}
}





void iberbar::LuaCpp::Class_Default_PushObjectWithName( lua_State* pLuaState, const char* strClassFullName, void* pInstance )
{
	// 获取 metatable
	luaL_getmetatable( pLuaState, strClassFullName );
	if ( lua_isnil( pLuaState, -1 ) )
	{
		luaL_error( pLuaState, "%s missing metatable", strClassFullName );
	}
	int nMetatable = lua_gettop( pLuaState );
	Class_XXX_PushObject( pLuaState, nMetatable, pInstance, strClassFullName, nullptr );
	lua_replace( pLuaState, -2 );
}


void iberbar::LuaCpp::Class_Default_PushObject( lua_State* pLuaState, int nMetatable, void* pInstance, const char* strClassFullName )
{
	Class_XXX_PushObject( pLuaState, nMetatable, pInstance, strClassFullName, nullptr );
}


void iberbar::LuaCpp::Class_Ref_PushObjectWithName( lua_State* pLuaState, const char* strClassFullName, CRef* pInstance )
{
	// 获取 metatable
	luaL_getmetatable( pLuaState, strClassFullName );
	if ( lua_isnil( pLuaState, -1 ) )
	{
		luaL_error( pLuaState, "%s missing metatable", strClassFullName );
	}
	int nMetatable = lua_gettop( pLuaState );
	Class_XXX_PushObject( pLuaState, nMetatable, pInstance, strClassFullName, pInstance );
	lua_replace( pLuaState, -2 );
}


void iberbar::LuaCpp::Class_Ref_PushObject( lua_State* pLuaState, int nMetatable, CRef* pInstance, const char* strClassFullName )
{
	Class_XXX_PushObject( pLuaState, nMetatable, pInstance, strClassFullName, pInstance );
}


int iberbar::LuaCpp::Class_Default_Constructor_New_GetMetatable( lua_State* L, const char* (*ppClassFullName), int* pOutMetatable )
{
	*ppClassFullName = nullptr;
	*pOutMetatable = 0;

	// 通过闭包传递的完整类名
	int upvalue_1 = lua_upvalueindex( 1 );
	const char* strClassFullName = lua_tostring( L, upvalue_1 );

	// 获取 metatable
	int metatable;
	
	// 通过类名获取metatable
	//luaL_getmetatable( L, strClassFullName );  // lookup metatable in Lua registry
	//if ( lua_isnil( L, -1 ) )
	//{
	//	luaL_error( L, "%s missing metatable", strClassFullName );
	//}
	//metatable = lua_gettop( L );

	// 直接从第一个参数获取metatable
	metatable = 1;

	*pOutMetatable = metatable;
	*ppClassFullName = strClassFullName;

	return 0;
}

