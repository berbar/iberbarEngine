
#include <iberbar/Lua/LuaCppBuilder.h>
#include <iberbar/Lua/StackChecker.h>
#include <iberbar/Lua/LoggingHelper.h>
#include <iberbar/lua/LuaCppCommon.h>
#include <iberbar/Utility/String.h>

using namespace iberbar::Lua;
using namespace iberbar::Lua;


namespace iberbar
{
	namespace Lua
	{
		// 设置为弱表
		// k  表示table.key是weak的，也就是table的keys是能够被垃圾收集器自动回收。
		// v  表示table.value是weak的，也就是table的values能够被垃圾收集器自动回收。
		// kv 是二者的组合，任何情况下只要key和value中的一个被垃圾收集器自动回收，那么kv键值对就被从表中移除。
		void CreateWeakTableForInstanceCache( lua_State* pLuaState, int nMetatable, const char* strCacheName, const char* strCacheMode );

		class CTableScope
		{
		public:
			CTableScope( lua_State* pLuaState );
		public:
			CResult Next( const char* name );
			void Pop();
			int Table() { return m_table; }
		public:
			lua_State* m_pLuaState;
			int m_table;
			int m_depth;
		};

		// 调用前需要插入值，该函数会将key和value调换顺序在执行lua_settable
		void SetTable( lua_State* L, int table_index, const char* key )
		{
			lua_pushstring( L, key );
			lua_insert( L, -2 );  // swap value and key
			lua_settable( L, table_index );
		}
	}
}






void iberbar::Lua::CreateWeakTableForInstanceCache( lua_State* pLuaState, int nMetatable, const char* strCacheName, const char* strCacheMode )
{
	lua_pushstring( pLuaState, strCacheName );
	lua_gettable( pLuaState, nMetatable );
	if ( lua_isnil( pLuaState, -1 ) )
	{
		lua_pop( pLuaState, 1 );
		lua_checkstack( pLuaState, 3 );

		lua_newtable( pLuaState );
		int nCacheTable = lua_gettop( pLuaState );

		// table is its own metatable
		// 缓存table的metatable是自己
		lua_pushvalue( pLuaState, nCacheTable );
		lua_setmetatable( pLuaState, -2 );

		// 关键，缓存自动清理的关键
		lua_pushstring( pLuaState, "__mode" );
		lua_pushstring( pLuaState, strCacheMode );
		lua_settable( pLuaState, -3 );   // metatable.__mode = mode

		lua_pushstring( pLuaState, strCacheName );
		lua_pushvalue( pLuaState, nCacheTable );
		lua_settable( pLuaState, nMetatable );
	}
	lua_pop( pLuaState, 1 );
}








iberbar::Lua::CScopeBuilder::CScopeBuilder( lua_State* pLuaState )
	: m_pLuaState( pLuaState )
	, m_name( "" )
	, m_nLuaModule( 0 )
{
}


iberbar::Lua::CScopeBuilder::CScopeBuilder( lua_State* pLuaState, const char* moduleName, int nLuaModule )
	: m_pLuaState( pLuaState )
	, m_name( moduleName == nullptr ? "" : moduleName )
	, m_nLuaModule( nLuaModule )
{
}


void iberbar::Lua::CScopeBuilder::AddClass( const char* className, std::function<PHowToBuildClass> pHowToAddClass, const char* extends )
{
	AddClass( className, nullptr, pHowToAddClass, extends );
}


void iberbar::Lua::CScopeBuilder::AddClass( const char* className, const char* strClassNameFull, std::function<PHowToBuildClass> pHowToAddClass, const char* extends )
{
	assert( m_pLuaState );
	assert( className );

	CStackSnapshot StackSS( m_pLuaState );

	lua_State* L = m_pLuaState;
	int metatable;

	std::string strClassNameFullNew;
	if ( m_nLuaModule == 0 )
	{
		strClassNameFullNew = className;
	}
	else
	{
		if ( strClassNameFull == nullptr )
		{
			strClassNameFullNew = m_name;
			strClassNameFullNew += ".";
			strClassNameFullNew += className;
		}
		else
		{
			strClassNameFullNew = strClassNameFull;
		}
	}
	

	luaL_newmetatable( L, strClassNameFullNew.c_str() );
	metatable = lua_gettop( L );

	if ( m_nLuaModule == 0 )
	{
		lua_setglobal( L, className );
	}
	else
	{
		// 将metatable放入模块的table中
		lua_pushstring( L, className );
		lua_pushvalue( L, metatable );
		lua_settable( L, m_nLuaModule );
	}

	// 创建弱表缓存，v模式
	CreateWeakTableForInstanceCache( L, metatable, uWeakTable_ForUserData, "v" );

	// 继承父类
	if ( StringIsNullOrEmpty( extends ) == false )
	{
		lua_pushvalue( m_pLuaState, metatable );
		luaL_getmetatable( m_pLuaState, extends );  // lookup metatable in Lua registry
		if ( lua_isnil( L, -1 ) )
		{
			luaL_error( L, "can't extends because %s is missing", strClassNameFullNew.c_str() );
		}

		lua_pushstring( m_pLuaState, "__tostring" );
		lua_gettable( m_pLuaState, -2 );
		if ( !lua_isnil( m_pLuaState, -1 ) && lua_iscfunction( m_pLuaState, -1 ) )
		{
			lua_pushstring( m_pLuaState, "__tostring" );
			lua_rotate( m_pLuaState, -2, 1 );
			lua_settable( m_pLuaState, metatable );
		}
		else
		{
			lua_pop( m_pLuaState, 1 );
		}
		
		lua_setmetatable( m_pLuaState, -2 );
		lua_pop( m_pLuaState, 1 );
	}

	// 设置metatable的__index域
	lua_pushstring( m_pLuaState, "__index" );
	lua_pushvalue( m_pLuaState, metatable );
	lua_settable( L, metatable );

	// 设置Class的类名
	lua_pushstring( m_pLuaState, uClassReflection_FullName );
	lua_pushstring( L, strClassNameFullNew.c_str() );
	lua_settable( m_pLuaState, metatable );

	// 设置Cpp类型
	lua_pushstring( m_pLuaState, uClassReflection_CType );
	lua_pushinteger( L, (lua_Integer)UClassReflectionCType::Cpp );
	lua_settable( m_pLuaState, metatable );

	// 构建class
	CClassBuilder classBuilder( m_pLuaState, strClassNameFullNew.c_str(), extends, metatable );
	pHowToAddClass( nullptr, &classBuilder );

	// 弹出 metatable 两个table
	lua_pop( L, 1 );

	iberbar_Lua_StackSnapshot_DiffError_Format( StackSS, 0, "when register class<%s>", strClassNameFullNew.c_str() );
}


void iberbar::Lua::CScopeBuilder::AddClass( const UClassDefinition& Definition )
{
	AddClass( Definition.classname, Definition.classnamefull, [&Definition]( const char*, CClassBuilder* pClass )
		{
			int nTestMethodCount = 0;
			while ( true )
			{
				if ( Definition.__methods[nTestMethodCount].name == nullptr || Definition.__methods[nTestMethodCount].func == nullptr )
					break;
				if ( nTestMethodCount >= 100 )
				{
					assert( "too many method to register, just less than 100" );
				}
				pClass->AddMemberMethod( Definition.__methods[nTestMethodCount].name, Definition.__methods[nTestMethodCount].func );
				nTestMethodCount++;
			}
			//pClass->AddStandardMethod( "__tostring", Definition.__tostring );
			//pClass->AddStandardMethod( "__add", Definition.__add );
			//pClass->AddStandardMethod( "__sub", Definition.__sub );
			//pClass->AddStandardMethod( "__mul", Definition.__mul );
			//pClass->AddStandardMethod( "__div", Definition.__div );
			//pClass->AddStandardMethod( "__mod", Definition.__mod );
			//pClass->AddStandardMethod( "__unm", Definition.__unm );
			//pClass->AddStandardMethod( "__concat", Definition.__concat );
			//pClass->AddStandardMethod( "__eq", Definition.__eq );
			//pClass->AddStandardMethod( "__lt", Definition.__lt );
			//pClass->AddStandardMethod( "__le", Definition.__le );
			if ( Definition.__constructor )
				pClass->AddConstructor( Definition.__constructor );
			if ( Definition.__distructor )
				pClass->AddDistructor( Definition.__distructor );
		}, Definition.classname_extends );
}


void iberbar::Lua::CScopeBuilder::AddEnum( const char* strName, PHowToBuildEnum pHow, int nCount )
{
	CEnumBuilder EnumBuilder( m_pLuaState, nCount );
	pHow( &EnumBuilder );
	if ( m_nLuaModule == 0 )
	{
		lua_setglobal( m_pLuaState, strName );
	}
	else
	{
		lua_pushstring( m_pLuaState, strName );
		lua_rotate( m_pLuaState, -2, 1 );
		lua_settable( m_pLuaState, m_nLuaModule );
	}
}


void iberbar::Lua::CScopeBuilder::AddFunctionOne( const char* strName, lua_CFunction pFunction )
{
	if ( m_nLuaModule == 0 )
	{
		lua_register( m_pLuaState, strName, pFunction );
	}
	else
	{
		lua_pushstring( m_pLuaState, strName );
		lua_pushcclosure( m_pLuaState, pFunction, 0 );
		lua_settable( m_pLuaState, m_nLuaModule );
	}
}

void iberbar::Lua::CScopeBuilder::AddFunctions( const luaL_Reg* pFunctionRegs )
{
	if ( m_nLuaModule == 0 )
	{
		const luaL_Reg* pNode = pFunctionRegs;
		while ( pNode->name != nullptr )
		{
			lua_register( m_pLuaState, pNode->name, pNode->func );
			pNode++;
		}
	}
	else
	{
		const luaL_Reg* pNode = pFunctionRegs;
		while ( pNode->name != nullptr )
		{
			lua_pushstring( m_pLuaState, pNode->name );
			lua_pushcclosure( m_pLuaState, pNode->func, 0 );
			lua_settable( m_pLuaState, m_nLuaModule );
			pNode++;
		}
	}
}











iberbar::Lua::CBuilder::CBuilder( lua_State* pLuaState )
	: m_pLuaState( pLuaState )
{
}

void iberbar::Lua::CBuilder::ResolveScope( PHowToBuildScope pHowToBuildScope, const char* moduleName )
{
	if ( StringIsNullOrEmpty( moduleName ) == true )
	{
		CScopeBuilder Scope( m_pLuaState );
		pHowToBuildScope( &Scope );
	}
	else
	{
		CStringEasySplitHelper<char, 128, 8 > SpiltHelper;
		int nSpiltCount = SpiltHelper.Split( moduleName, '.' );
		CTableScope TableScope( m_pLuaState );
		for ( int i = 0; i < nSpiltCount; i++ )
		{
			if ( SpiltHelper[i][0] == 0 )
			{
				luaL_error( m_pLuaState, "error namespace<%s>", moduleName );
				return;
			}
			TableScope.Next( SpiltHelper[i] );
		}
		CScopeBuilder ScopeBuilder( m_pLuaState, moduleName, TableScope.Table() );
		pHowToBuildScope( &ScopeBuilder );
		TableScope.Pop();
	}
}










iberbar::Lua::CTableScope::CTableScope( lua_State* pLuaState )
	: m_pLuaState( pLuaState )
	, m_table( 0 )
	, m_depth( 0 )
{
}

iberbar::CResult iberbar::Lua::CTableScope::Next( const char* name )
{
	if ( m_depth == 0 )
	{
		lua_getglobal( m_pLuaState, name );
		m_table = lua_gettop( m_pLuaState );
		if ( lua_isnil( m_pLuaState, m_table ) )
		{
			lua_pop( m_pLuaState, 1 );

			lua_newtable( m_pLuaState );
			m_table = lua_gettop( m_pLuaState );
			lua_pushvalue( m_pLuaState, -1 );
			lua_setglobal( m_pLuaState, name );
		}
		else if ( lua_istable( m_pLuaState, -1 ) == false )
		{
			lua_pop( m_pLuaState, 1 );
			return MakeResult( ResultCode::Bad, "%s exist but not module", name );
		}
	}
	else
	{
		int tableParent = m_table;
		lua_pushstring( m_pLuaState, name );
		lua_gettable( m_pLuaState, tableParent );
		m_table = lua_gettop( m_pLuaState );
		if ( lua_isnil( m_pLuaState, m_table ) )
		{
			lua_pop( m_pLuaState, 1 );

			lua_newtable( m_pLuaState );
			m_table = lua_gettop( m_pLuaState );

			lua_pushstring( m_pLuaState, name );
			lua_pushvalue( m_pLuaState, -2 );

			lua_settable( m_pLuaState, tableParent );
		}
		else if ( lua_istable( m_pLuaState, -1 ) == false )
		{
			lua_pop( m_pLuaState, 1 );
			return MakeResult( ResultCode::Bad, "%s exist but not module", name );
		}
	}

	m_depth++;

	return CResult();
}

void iberbar::Lua::CTableScope::Pop()
{
	if ( m_depth > 0 )
	{
		lua_pop( m_pLuaState, m_depth );
	}
}





iberbar::Lua::CClassBuilder::CClassBuilder(
	lua_State* pLuaState,
	const char* classNameFull,
	const char* strClassName_Extends,
	int metatable )
	: m_pLuaState( pLuaState )
	, m_classNameFull( classNameFull )
	, m_strClassName_Extends( strClassName_Extends == nullptr ? "" : strClassName_Extends )
	, m_metatable( metatable )
	, m_methods( metatable )
{
}

CClassBuilder* iberbar::Lua::CClassBuilder::AddConstructor( lua_CFunction func )
{
	//lua_State* L = m_pLuaState;

	//lua_newtable( L );                // mt for method table
	//lua_pushcfunction( L, func );
	//lua_pushvalue( L, -1 );           // dup new_T function
	//SetTable( L, m_methods, "new" );         // add new_T to method table
	//SetTable( L, -3, "__call" );           // mt.__call = new_T
	//lua_setmetatable( L, m_methods );

	//lua_newtable( m_pLuaState );
	lua_pushstring( m_pLuaState, "new" );
	//lua_pushvalue( m_pLuaState, m_methods );
	lua_pushstring( m_pLuaState, m_classNameFull.c_str() );
	lua_pushcclosure( m_pLuaState, func, 1 );
	lua_settable( m_pLuaState, m_methods );

	return this;
}

CClassBuilder* iberbar::Lua::CClassBuilder::AddDistructor( lua_CFunction func )
{
	AddStandardMethod( "__gc", func );

	return this;
}

CClassBuilder* iberbar::Lua::CClassBuilder::AddMemberMethod( const char* name, lua_CFunction func, UpValue* upvalues, int upvaluesCount )
{
	assert( upvaluesCount >= 0 );

	lua_pushstring( m_pLuaState, name );

	// 闭包的局部变量
	if ( upvaluesCount > 0 )
	{
		for ( int i = 0; i < upvaluesCount; i++ )
		{
			if ( upvalues->t == UpValue_Boolean )
			{
				lua_pushboolean( m_pLuaState, upvalues->v.v_b );
			}
			else if ( upvalues->t == UpValue_Integer )
			{
				lua_pushinteger( m_pLuaState, upvalues->v.v_i );
			}
			else if ( upvalues->t == UpValue_Number )
			{
				lua_pushnumber( m_pLuaState, upvalues->v.v_n );
			}
			else if ( upvalues->t == UpValue_String )
			{
				lua_pushstring( m_pLuaState, upvalues->v.v_s );
			}
			else if ( upvalues->t == UpValue_Pointer )
			{
				lua_pushlightuserdata( m_pLuaState, upvalues->v.v_p );
			}
			else
			{
				lua_pushnil( m_pLuaState );
			}
			upvalues++;
		}
	}

	// 创建c函数thunk，带有1个upvalue，并从栈上弹出这个upvalue
	// 此处，upvalue是lua_pushlightuserdata(L, (void*)l)压入的l
	lua_pushcclosure( m_pLuaState, func, upvaluesCount );

	lua_settable( m_pLuaState, m_methods );

	return this;
}

CClassBuilder* iberbar::Lua::CClassBuilder::AddStaticMethod( const char* name, lua_CFunction func )
{
	lua_pushstring( m_pLuaState, name );
	//lua_pushstring( m_pLuaState, m_classNameFull.c_str() );
	//lua_pushcclosure( m_pLuaState, func, 1 );
	lua_pushcclosure( m_pLuaState, func, 0 );
	lua_settable( m_pLuaState, m_methods );
	//SetTable( m_pLuaState, m_metatable, name );

	return this;
}

CClassBuilder* iberbar::Lua::CClassBuilder::AddStandardMethod( const char* name, lua_CFunction func )
{
	if ( StringIsNullOrEmpty( name ) || func == nullptr )
		return this;

	lua_pushcfunction( m_pLuaState, func );
	SetTable( m_pLuaState, m_metatable, name );

	//if ( func == nullptr )
	//{
	//	luaL_getmetatable( m_pLuaState, m_strClassName_Extends.c_str() );
	//	if ( !lua_isnil( m_pLuaState, -1 ) )
	//	{
	//		lua_pushstring( m_pLuaState, name );
	//		lua_gettable( m_pLuaState, -2 );
	//		if ( !lua_isnil( m_pLuaState, -1 ) && lua_isfunction( m_pLuaState, -1 ) )
	//		{
	//			SetTable( m_pLuaState, m_metatable, name );
	//		}
	//		else
	//		{
	//			lua_pop( m_pLuaState, 1 );
	//		}
	//	}
	//	lua_pop( m_pLuaState, 1 );
	//}
	//else
	//{

	//}

	return this;
}








iberbar::Lua::CEnumBuilder::CEnumBuilder( lua_State* pLuaState, int nValueCount )
	: m_pLuaState( pLuaState )
	, m_nTable( 0 )
{
	lua_createtable( m_pLuaState, 0, nValueCount > 0 ? nValueCount : 0 );
	m_nTable = lua_gettop( m_pLuaState );
}


void iberbar::Lua::CEnumBuilder::AddValueInt( const char* strKey, lua_Integer Value )
{
	lua_pushstring( m_pLuaState, strKey );
	lua_pushinteger( m_pLuaState, Value );
	lua_rawset( m_pLuaState, m_nTable );
}


void iberbar::Lua::CEnumBuilder::AddValueString( const char* strKey, const char* Value )
{
	lua_pushstring( m_pLuaState, strKey );
	lua_pushstring( m_pLuaState, Value );
	lua_rawset( m_pLuaState, m_nTable );
}

