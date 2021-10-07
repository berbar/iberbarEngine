
#include <iberbar/LuaCppApis/Json.h>
#include <iberbar/Lua/LuaCppBuilder.h>
#include <iberbar/Lua/LuaError.h>
#include <nlohmann/json.hpp>


namespace iberbar
{
	namespace Json
	{
		int LuaCFunction_JsonDeserializeObject( lua_State* L );
		int LuaCFunction_JsonSerializeObject( lua_State* L );

		//void LuaTable_FromJson_Inner( lua_State* pLuaState, int nTable, const nlohmann::json& json );
		void LuaTable_FromJson_Inner_Object( lua_State* pLuaState, const nlohmann::json& json );
		void LuaTable_FromJson_Inner_Array( lua_State* pLuaState, const nlohmann::json& json );

		void Lua_Json_Pack( lua_State* pLuaState, nlohmann::json& json, int nTable );
		template < typename TKey >
		void Lua_Json_PackNode( lua_State* pLuaState, nlohmann::json& json, int nArg, TKey nKey );
	}

	//void Lua_Json_PackNode( lua_State* pLuaState, nlohmann::json& json, int nArg, const char* strKey );
}




void iberbar::Json::RegisterLuaCpp( lua_State* pLuaState )
{
	Lua::CBuilder builder( pLuaState );
	builder.ResolveScope(
		[]( Lua::CScopeBuilder* pScope )
		{
			pScope->AddFunctionOne( "Serialize", LuaCFunction_JsonSerializeObject );
			pScope->AddFunctionOne( "Deserialize", LuaCFunction_JsonDeserializeObject );
		}, "iberbar.Json"
	);
}



//inline void iberbar::Json::LuaTable_FromJson_Inner( lua_State* pLuaState, int nTable, const nlohmann::json& json )
//{
//	if ( json.is_object() == true )
//	{
//		LuaTable_FromJson_Inner_Object( pLuaState, nTable, json );
//	}
//	else if ( json.is_array() == true )
//	{
//		LuaTable_FromJson_Inner_Array( pLuaState, nTable, json );
//	}
//}

//bool iberbar::Json::LuaTable_FromJson( CLuaTable& table, const char* jsonstr, std::string& err )
//{
//	nlohmann::json json;
//	bool ret = true;
//	try
//	{
//		json = nlohmann::json::parse( jsonstr );
//	}
//	catch ( nlohmann::detail::exception e )
//	{
//		err = e.what();
//		ret = false;
//	}
//	catch ( std::exception e )
//	{
//		err = e.what();
//		ret = false;
//	}
//	if ( ret == true )
//	{
//		LuaTable_FromJson_Inner( table, json );
//	}
//	return ret;
//}


void iberbar::Json::LuaTable_FromJson_Inner_Object( lua_State* pLuaState, const nlohmann::json& json )
{
	auto iter = json.begin();
	auto iterEnd = json.end();
	nlohmann::json::value_t valueType;
	const char* key = nullptr;
	for ( ; iter != iterEnd; iter++ )
	{
		valueType = iter->type();
		key = iter.key().c_str();
		switch ( valueType )
		{
		case nlohmann::json::value_t::null:
			break;
		case nlohmann::json::value_t::array:
		{
			lua_pushstring( pLuaState, key );
			lua_createtable( pLuaState, (int)iter->size(), 0 );
			LuaTable_FromJson_Inner_Array( pLuaState, *iter );
			lua_settable( pLuaState, -3 );
			break;
		}
		case nlohmann::json::value_t::object:
		{
			lua_pushstring( pLuaState, key );
			lua_createtable( pLuaState, 0, (int)iter->size() );
			LuaTable_FromJson_Inner_Object( pLuaState, *iter );
			lua_settable( pLuaState, -3 );
			break;
		}
		case nlohmann::json::value_t::string:
			lua_pushstring( pLuaState, key );
			lua_pushstring( pLuaState, iter->get<std::string>().c_str() );
			lua_settable( pLuaState, -3 );
			break;
		case nlohmann::json::value_t::boolean:
			lua_pushstring( pLuaState, key );
			lua_pushboolean( pLuaState, iter->get<bool>() );
			lua_settable( pLuaState, -3 );
			break;
		case nlohmann::json::value_t::number_integer:
			lua_pushstring( pLuaState, key );
			lua_pushinteger( pLuaState, iter->get<int64_t>() );
			lua_settable( pLuaState, -3 );
			break;
		case nlohmann::json::value_t::number_unsigned:
			lua_pushstring( pLuaState, key );
			lua_pushinteger( pLuaState, iter->get<uint64_t>() );
			lua_settable( pLuaState, -3 );
			break;
		case nlohmann::json::value_t::number_float:
			lua_pushstring( pLuaState, key );
			lua_pushnumber( pLuaState, iter->get<float>() );
			lua_settable( pLuaState, -3 );
			break;
		default:
			break;
		}
	}
}

void iberbar::Json::LuaTable_FromJson_Inner_Array( lua_State* pLuaState, const nlohmann::json& json )
{
	size_t c = json.size();
	if ( c > 0 )
	{
		nlohmann::json::value_t valueType;
		lua_Integer key;
		for ( int i = 0; i < c; i++ )
		{
			nlohmann::json::const_reference node = json.at( i );
			valueType = node.type();
			key = (lua_Integer)i + 1;
			switch ( valueType )
			{
			case nlohmann::json::value_t::null:
				lua_pushnil( pLuaState );
				lua_rawseti( pLuaState, -2, key );
				break;
			case nlohmann::json::value_t::array:
			{
				lua_createtable( pLuaState, (int)node.size(), 0 );
				LuaTable_FromJson_Inner_Array( pLuaState, node );
				lua_rawseti( pLuaState, -2, key );
				break;
			}
			case nlohmann::json::value_t::object:
			{
				lua_createtable( pLuaState, 0, (int)node.size() );
				LuaTable_FromJson_Inner_Object( pLuaState, node );
				lua_rawseti( pLuaState, -2, key );
				break;
			}
			case nlohmann::json::value_t::string:
				lua_pushstring( pLuaState, node.get<std::string>().c_str() );
				lua_rawseti( pLuaState, -2, key );
				break;
			case nlohmann::json::value_t::boolean:
				lua_pushboolean( pLuaState, node.get<bool>() );
				lua_rawseti( pLuaState, -2, key );
				break;
			case nlohmann::json::value_t::number_integer:
				lua_pushinteger( pLuaState, node.get<int64_t>() );
				lua_rawseti( pLuaState, -2, key );
				break;
			case nlohmann::json::value_t::number_unsigned:
				lua_pushinteger( pLuaState, node.get<uint64_t>() );
				lua_rawseti( pLuaState, -2, key );
				break;
			case nlohmann::json::value_t::number_float:
				lua_pushnumber( pLuaState, node.get<float>() );
				lua_rawseti( pLuaState, -2, key );
				break;
			default:
				break;
			}
		}
	}
}


int iberbar::Json::LuaCFunction_JsonDeserializeObject( lua_State * pLuaState )
{
	if ( lua_isstring( pLuaState, 1 ) == false )
		return luaL_error( pLuaState, "argument(1) must be string." );

	const char* jsonstr = lua_tostring( pLuaState, 1 );
	nlohmann::json json;
	bool ret = true;
	std::string err;
	try
	{
		json = nlohmann::json::parse( jsonstr );
	}
	catch ( nlohmann::detail::exception e )
	{
		err = e.what();
		ret = false;
	}
	catch ( std::exception e )
	{
		err = e.what();
		ret = false;
	}
	if ( ret == true )
	{
		if ( json.is_array() )
		{
			lua_createtable( pLuaState, (int)json.size(), 0 );;
			LuaTable_FromJson_Inner_Array( pLuaState, json );
		}
		else if ( json.is_object() )
		{
			lua_createtable( pLuaState, (int)json.size(), 0 );;
			LuaTable_FromJson_Inner_Object( pLuaState, json );
		}
		else
		{
			lua_pushnil( pLuaState );
		}
	}
	else
	{
		lua_pushnil( pLuaState );
	}
	return 1;
}



void iberbar::Json::Lua_Json_Pack( lua_State* pLuaState, nlohmann::json& json, int nTable )
{
	if ( nTable < 0 )
	{
		nTable = lua_gettop( pLuaState ) + nTable + 1;
	}

	lua_len( pLuaState, nTable );
	int nLen = lua_tointeger( pLuaState, -1 );
	lua_pop( pLuaState, 1 );

	if ( nLen > 0 )
	{
		for ( int i = 1; i <= nLen; i++ )
		{
			lua_rawgeti( pLuaState, nTable, i );
			Lua_Json_PackNode( pLuaState, json, -1, i - 1 );
			lua_pop( pLuaState, 1 );
		}
	}
	else
	{
		lua_pushnil( pLuaState );
		const char* strKey;
		size_t nKeyLen;
		while ( lua_next( pLuaState, nTable ) )
		{

			// 现在的栈：-1 => value; -2 => key; index => table
			// 拷贝一份 key 到栈顶，然后对它做 lua_tostring 就不会改变原始的 key 值了
			lua_pushvalue( pLuaState, -2 );

			// 现在的栈：-1 => key; -2 => value; -3 => key; index => table
			strKey = lua_tolstring( pLuaState, -1, &nKeyLen );

			// 打包
			Lua_Json_PackNode( pLuaState, json, -2, strKey );

			// 弹出 value 和拷贝的 key，留下原始的 key 作为下一次 lua_next 的参数
			lua_pop( pLuaState, 2 );

			// 现在的栈：-1 => key; index => table
		}
	}
}


template < typename TKey >
void iberbar::Json::Lua_Json_PackNode( lua_State* pLuaState, nlohmann::json& json, int nArg, TKey Key )
{
	int nLuaType = lua_type( pLuaState, nArg );
	switch ( nLuaType )
	{
		case LUA_TNUMBER:
		{
			if ( lua_isinteger( pLuaState, nArg ) == 1 )
			{
				json[ Key ] = (int)lua_tointeger( pLuaState, nArg );
			}
			else
			{
				json[ Key ] = (float)lua_tonumber( pLuaState, nArg );
			}
			break;
		}

		case LUA_TBOOLEAN:
		{
			json[ Key ] = (lua_toboolean( pLuaState, nArg ) == 0) ? false : true;
			break;
		}

		case LUA_TSTRING:
		{
			json[ Key ] = lua_tostring( pLuaState, nArg );
			break;
		}

		case LUA_TTABLE:
		{
			json[ Key ] = nlohmann::json();
			Lua_Json_Pack( pLuaState, json[ Key ], nArg );

			//if ( nArg < 0 )
			//{
			//	nArg = lua_gettop( pLuaState ) + nArg + 1;
			//}

			//lua_len( pLuaState, nArg );
			//int nLen = lua_tointeger( pLuaState, -1 );
			//lua_pop( pLuaState, 1 );
			//if ( nLen > 0 )
			//{
			//	json[ Key ] = nlohmann::json();
			//	Lua_Json_Pack( pLuaState, json[ Key ], nArg, nLen );
			//}
			//else
			//{
			//	json[ Key ] = nlohmann::json();
			//	Lua_Json_Pack( pLuaState, json[ Key ], nArg, 0 );
			//}
			break;
		}

		default:
			break;
	}
}


int iberbar::Json::LuaCFunction_JsonSerializeObject( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 1 );

	if ( lua_istable( pLuaState, 1 ) == false )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	nlohmann::json json;
	Lua_Json_Pack( pLuaState, json, 1 );
	std::string strJsonText = json.dump();

	lua_pushstring( pLuaState, strJsonText.c_str() );

	return 1;
}


