#pragma once

#include <iberbar/Lua/LuaBase.h>

namespace iberbar
{
	class __iberbarLuaApi__ CLuaTable
	{
	public:
		CLuaTable( lua_State* L, int idx );
		//~CLuaTable();

	public:
		bool IsTable();

		lua_Integer GetInteger_KeyStr( const char* key );
		lua_Number GetNumber_KeyStr( const char* key );
		const char* GetString_KeyStr( const char* key );
		CLuaTable GetTable_KeyStr( const char* key );

		lua_Integer GetInteger_KeyInt( int key );
		lua_Number GetNumber_KeyInt( int key );
		const char* GetString_KeyInt( int key );
		CLuaTable GetTable_KeyInt( int key );

		void SetNil_KeyStr( const char* key );
		void SetBoolean_KeyStr( const char* key, bool value );
		void SetInteger_KeyStr( const char* key, lua_Integer value );
		void SetNumber_KeyStr( const char* key, lua_Number value );
		void SetString_KeyStr( const char* key, const char* value );
		void SetTable_KeyStr( const char* key );

		void SetNil_KeyInt( lua_Integer key );
		void SetBoolean_KeyInt( lua_Integer key, bool value );
		void SetInteger_KeyInt( lua_Integer key, lua_Integer value );
		void SetNumber_KeyInt( lua_Integer key, lua_Number value );
		void SetString_KeyInt( lua_Integer key, const char* value );
		void SetTable_KeyInt( lua_Integer key );

		lua_State* GetLuaState() { return m_pLuaState; }
		int GetIdx() { return m_idx; };

	private:
		lua_State* m_pLuaState;
		int m_idx;
	};
}


inline iberbar::CLuaTable::CLuaTable( lua_State* L, int idx )
	: m_pLuaState( L )
	, m_idx( idx )
{
}

inline bool iberbar::CLuaTable::IsTable()
{
	return lua_istable( m_pLuaState, m_idx );
}

inline lua_Integer iberbar::CLuaTable::GetInteger_KeyStr( const char* key )
{
	lua_pushstring( m_pLuaState, key );
	lua_gettable( m_pLuaState, m_idx );
	lua_Integer value = lua_tointeger( m_pLuaState, -1 );
	lua_pop( m_pLuaState, 1 );
	return value;
}

inline lua_Number iberbar::CLuaTable::GetNumber_KeyStr( const char* key )
{
	lua_pushstring( m_pLuaState, key );
	lua_gettable( m_pLuaState, m_idx );
	lua_Number value = lua_tonumber( m_pLuaState, -1 );
	lua_pop( m_pLuaState, 1 );
	return value;
}

inline const char* iberbar::CLuaTable::GetString_KeyStr( const char* key )
{
	lua_pushstring( m_pLuaState, key );
	lua_gettable( m_pLuaState, m_idx );
	const char* value = lua_tostring( m_pLuaState, -1 );
	lua_pop( m_pLuaState, 1 );
	return value;
}

inline iberbar::CLuaTable iberbar::CLuaTable::GetTable_KeyStr( const char* key )
{
	lua_pushstring( m_pLuaState, key );
	lua_gettable( m_pLuaState, m_idx );
	// 这里不调用lua_pop弹出
	return CLuaTable( m_pLuaState, lua_gettop( m_pLuaState ) );
}

inline lua_Integer iberbar::CLuaTable::GetInteger_KeyInt( int key )
{
	lua_pushinteger( m_pLuaState, key );
	lua_gettable( m_pLuaState, m_idx );
	lua_Integer value = lua_tointeger( m_pLuaState, -1 );
	lua_pop( m_pLuaState, 1 );
	return value;
}

inline lua_Number iberbar::CLuaTable::GetNumber_KeyInt( int key )
{
	lua_pushinteger( m_pLuaState, key );
	lua_gettable( m_pLuaState, m_idx );
	lua_Number value = lua_tonumber( m_pLuaState, -1 );
	lua_pop( m_pLuaState, 1 );
	return value;
}

inline const char* iberbar::CLuaTable::GetString_KeyInt( int key )
{
	lua_pushinteger( m_pLuaState, key );
	lua_gettable( m_pLuaState, m_idx );
	const char* value = lua_tostring( m_pLuaState, -1 );
	lua_pop( m_pLuaState, 1 );
	return value;
}

inline iberbar::CLuaTable iberbar::CLuaTable::GetTable_KeyInt( int key )
{
	lua_pushinteger( m_pLuaState, key );
	lua_gettable( m_pLuaState, m_idx );
	// 这里不调用lua_pop弹出
	return CLuaTable( m_pLuaState, lua_gettop( m_pLuaState ) );
}

inline void iberbar::CLuaTable::SetNil_KeyStr( const char* key )
{
	lua_pushstring( m_pLuaState, key );
	lua_pushnil( m_pLuaState );
	lua_settable( m_pLuaState, m_idx );
}

inline void iberbar::CLuaTable::SetBoolean_KeyStr( const char* key, bool value )
{
	lua_pushstring( m_pLuaState, key );
	lua_pushboolean( m_pLuaState, value == true ? 1 : 0 );
	lua_settable( m_pLuaState, m_idx );
}

inline void iberbar::CLuaTable::SetInteger_KeyStr( const char* key, lua_Integer value )
{
	lua_pushstring( m_pLuaState, key );
	lua_pushinteger( m_pLuaState, value );
	lua_settable( m_pLuaState, m_idx );
}

inline void iberbar::CLuaTable::SetNumber_KeyStr( const char* key, lua_Number value )
{
	lua_pushstring( m_pLuaState, key );
	lua_pushnumber( m_pLuaState, value );
	lua_settable( m_pLuaState, m_idx );
}

inline void iberbar::CLuaTable::SetString_KeyStr( const char* key, const char* value )
{
	lua_pushstring( m_pLuaState, key );
	lua_pushstring( m_pLuaState, value );
	lua_settable( m_pLuaState, m_idx );
}

inline void iberbar::CLuaTable::SetTable_KeyStr( const char* key )
{
	lua_pushstring( m_pLuaState, key );
	lua_newtable( m_pLuaState );
	lua_settable( m_pLuaState, m_idx );
}


inline void iberbar::CLuaTable::SetNil_KeyInt( lua_Integer key )
{
	lua_pushinteger( m_pLuaState, key );
	lua_pushnil( m_pLuaState );
	lua_settable( m_pLuaState, m_idx );
}

inline void iberbar::CLuaTable::SetBoolean_KeyInt( lua_Integer key, bool value )
{
	lua_pushinteger( m_pLuaState, key );
	lua_pushboolean( m_pLuaState, value == true ? 1 : 0 );
	lua_settable( m_pLuaState, m_idx );
}

inline void iberbar::CLuaTable::SetInteger_KeyInt( lua_Integer key, lua_Integer value )
{
	lua_pushinteger( m_pLuaState, key );
	lua_pushinteger( m_pLuaState, value );
	lua_settable( m_pLuaState, m_idx );
}

inline void iberbar::CLuaTable::SetNumber_KeyInt( lua_Integer key, lua_Number value )
{
	lua_pushinteger( m_pLuaState, key );
	lua_pushnumber( m_pLuaState, value );
	lua_settable( m_pLuaState, m_idx );
}

inline void iberbar::CLuaTable::SetString_KeyInt( lua_Integer key, const char* value )
{
	lua_pushinteger( m_pLuaState, key );
	lua_pushstring( m_pLuaState, value );
	lua_settable( m_pLuaState, m_idx );
}

inline void iberbar::CLuaTable::SetTable_KeyInt( lua_Integer key )
{
	lua_pushinteger( m_pLuaState, key );
	lua_newtable( m_pLuaState );
	lua_settable( m_pLuaState, m_idx );
}
