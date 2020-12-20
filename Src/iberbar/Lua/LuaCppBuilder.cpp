
#include <iberbar/Lua/LuaCppBuilder.h>
#include <iberbar/lua/LuaCppCommon.h>
#include <iberbar/Utility/String.h>

using namespace iberbar::LuaCpp;


namespace iberbar
{
	namespace LuaCpp
	{
		// 设置为弱表
		// k  表示table.key是weak的，也就是table的keys是能够被垃圾收集器自动回收。
		// v  表示table.value是weak的，也就是table的values能够被垃圾收集器自动回收。
		// kv 是二者的组合，任何情况下只要key和value中的一个被垃圾收集器自动回收，那么kv键值对就被从表中移除。
		void CreateWeakTableForInstanceCache( lua_State* pLuaState, int nMetatable, const char* strCacheName, const char* strCacheMode );

		class CScopeGlobal
			: public CScopeBuilder
		{
		public:
			CScopeGlobal( lua_State* pLuaState );

		public:
			virtual void AddClass( const char* className, PHowToBuildClass pHowToAddClass, const char* extends = nullptr ) override;
			virtual void AddEnum( const char* strEnumTypeName, PHowToBuildEnum pHow, int nCount ) override;
			virtual void AddFunctionOne( const char* functionName, lua_CFunction pFunction ) override;
			virtual void AddFunctions( const luaL_Reg* pFunctionRegs ) override;
		};

		class CScopeModule
			: public CScopeBuilder
		{
		public:
			CScopeModule( lua_State* pLuaState, const char* moduleName );

		public:
			virtual void AddClass( const char* className, PHowToBuildClass pHowToAddClass, const char* extends = nullptr ) override;
			virtual void AddEnum( const char* strEnumTypeName, PHowToBuildEnum pHow, int nCount ) override;
			virtual void AddFunctionOne( const char* functionName, lua_CFunction pFunction ) override;
			virtual void AddFunctions( const luaL_Reg* pFunctionRegs ) override;

		public:
			CResult Build( PHowToBuildScope pHowToBuild );

		private:
			int m_table;
		};

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






void iberbar::LuaCpp::CreateWeakTableForInstanceCache( lua_State* pLuaState, int nMetatable, const char* strCacheName, const char* strCacheMode )
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






iberbar::LuaCpp::CScopeBuilder::CScopeBuilder( lua_State* pLuaState, const char* moduleName )
	: m_pLuaState( pLuaState )
	, m_name( moduleName )
{
}


iberbar::LuaCpp::CBuilder::CBuilder( lua_State* pLuaState )
	: m_pLuaState( pLuaState )
{
}

void iberbar::LuaCpp::CBuilder::ResolveScope( PHowToBuildScope pHowToBuildScope, const char* moduleName )
{
	if ( moduleName == nullptr )
	{
		CScopeGlobal scope( m_pLuaState );
		pHowToBuildScope( &scope );
	}
	else
	{
		CScopeModule scope( m_pLuaState, moduleName );
		scope.Build( pHowToBuildScope );
	}
}



iberbar::LuaCpp::CScopeGlobal::CScopeGlobal( lua_State* pLuaState )
	: CScopeBuilder( pLuaState, "" )
{
}

void iberbar::LuaCpp::CScopeGlobal::AddClass( const char* className, PHowToBuildClass pHowToAddClass, const char* extends )
{
	assert( m_pLuaState );
	assert( className );

	lua_State* L = m_pLuaState;
	int methods;
	int metatable;

	// 创建method table方法表
	lua_newtable( L );
	methods = lua_gettop( L );

	// 创建meta元表
	// 说明：luaL_newmetatable函数创建一个新表(此表将被用作metatable), 将新表放
	// 到栈顶并建立表和registry中T::className的联系。
	luaL_newmetatable( L, className );
	metatable = lua_gettop( L );

	// store method table in globals so that
	// scripts can add functions written in Lua.
	// 将method table拷贝压入栈顶，然后设置到全局索引中，名称为T::className
	lua_pushvalue( L, methods );

	// set函数的功能：如果set(A,B,C),
	// 那么，将当前栈顶的元素作为值,C作为key，添加到索引为B的表中.
	// 当B为负数时，-1，-2，被值、key占用，-3是压入值之前的元素......
	// 所以这行代码读作：在LUA_GLOBALSINDEX表中添加T::className为key的值。
	//                   值就是那个栈顶元素。
	// =========调用set后，栈回到压入值之前的状态，因为lua_settable弹出值和key。======
	//set( L, LUA_GLOBALSINDEX, m_strClassName.c_str() );
	lua_setglobal( L, className );

	// hide metatable from Lua getmetatable()
	// 设置meta table的__metatable域
	lua_pushvalue( L, methods );
	SetTable( L, metatable, "__metatable" );

	// 创建弱表缓存，v模式
	CreateWeakTableForInstanceCache( L, metatable, uWeakTable_ForUserData, "v" );

	//
	// 继承父类
	if ( StringIsNullOrEmpty( extends ) == false )
	{
		lua_pushvalue( L, methods );
		luaL_getmetatable( L, extends );  // lookup metatable in Lua registry
		//if ( lua_isnil( L, -1 ) )
		//{
		//	luaL_error( L, "%s missing metatable", classFullName );
		//}
		lua_setmetatable( L, -2 );
		lua_pop( L, 1 );
	}

	// 设置meta table的__index域
	// 说明：__index域可以是一个函数，也可以是一个表
	// 当它是一个函数的时候，Lua将table和缺少的域作为
	// 参数调用这个函数；当它是一个表的时候，Lua将在这
	// 个表中看是否有缺少的域。
	lua_pushvalue( L, methods );
	SetTable( L, metatable, "__index" );

	// 反射Class的模块名和类名
	lua_pushstring( L, className );
	SetTable( L, methods, uClassReflection_ClassName );
	lua_pushstring( L, "" );
	SetTable( L, methods, uClassReflection_ModuleName );
	lua_pushstring( L, className );
	SetTable( L, methods, uClassReflection_FullName );
	lua_pushinteger( L, (lua_Integer)UClassReflectionCType::Cpp );
	SetTable( L, methods, uClassReflection_CType );

	// 构建class
	CClassBuilder classBuilder( m_pLuaState, className, className, metatable, methods );
	pHowToAddClass( nullptr, &classBuilder );

	// 弹出 metatable and method 两个table
	lua_pop( L, 2 );
}


void iberbar::LuaCpp::CScopeGlobal::AddEnum( const char* strName, PHowToBuildEnum pHow, int nCount )
{
	CEnumBuilder EnumBuilder( m_pLuaState, nCount );
	pHow( &EnumBuilder );
	lua_setglobal( m_pLuaState, strName );
}


void iberbar::LuaCpp::CScopeGlobal::AddFunctionOne( const char* functionName, lua_CFunction pFunction )
{
	lua_register( m_pLuaState, functionName, pFunction );
}

void iberbar::LuaCpp::CScopeGlobal::AddFunctions( const luaL_Reg* pFunctionRegs )
{
	const luaL_Reg* pNode = pFunctionRegs;
	while ( pNode->name != nullptr )
	{
		lua_register( m_pLuaState, pNode->name, pNode->func );
		pNode ++;
	}
}



iberbar::LuaCpp::CScopeModule::CScopeModule( lua_State* pLuaState, const char* moduleName )
	: CScopeBuilder( pLuaState, moduleName )
	, m_table( 0 )
{
}

void iberbar::LuaCpp::CScopeModule::AddClass( const char* className, PHowToBuildClass pHowToAddClass, const char* extends )
{
	assert( m_pLuaState );
	assert( className );

	lua_State* L = m_pLuaState;
	int methods;
	int metatable;

	// 创建method table方法表
	lua_newtable( L );
	methods = lua_gettop( L );

	std::string classNameFull = m_name + "." + className;

	// 创建meta元表
	// 说明：luaL_newmetatable函数创建一个新表(此表将被用作metatable), 将新表放
	// 到栈顶并建立表和registry中T::className的联系。
	luaL_newmetatable( L, classNameFull.c_str() );
	metatable = lua_gettop( L );

	// 将methods放入模块的table中
	lua_pushstring( L, className );
	lua_pushvalue( L, metatable );
	lua_settable( L, m_table );

	// hide metatable from Lua getmetatable()
	// 设置meta table的__metatable域
	//lua_pushvalue( L, methods );
	//SetTable( L, metatable, "__metatable" );

	// 创建弱表缓存，v模式
	CreateWeakTableForInstanceCache( L, metatable, uWeakTable_ForUserData, "v" );

	
	//
	// 继承父类
	if ( StringIsNullOrEmpty( extends ) == false )
	{
		lua_pushvalue( L, metatable );
		luaL_getmetatable( L, extends );  // lookup metatable in Lua registry
		//if ( lua_isnil( L, -1 ) )
		//{
		//	luaL_error( L, "%s missing metatable", classFullName );
		//}
		lua_setmetatable( L, -2 );
		lua_pop( L, 1 );
	}

	// 设置meta table的__index域
	// 说明：__index域可以是一个函数，也可以是一个表
	// 当它是一个函数的时候，Lua将table和缺少的域作为
	// 参数调用这个函数；当它是一个表的时候，Lua将在这
	// 个表中看是否有缺少的域。
	lua_pushvalue( L, metatable );
	SetTable( L, metatable, "__index" );

	// 反射Class的模块名和类名
	lua_pushstring( L, className );
	SetTable( L, metatable, uClassReflection_ClassName );
	lua_pushstring( L, m_name.c_str() );
	SetTable( L, metatable, uClassReflection_ModuleName );
	lua_pushstring( L, classNameFull.c_str() );
	SetTable( L, metatable, uClassReflection_FullName );
	lua_pushinteger( L, (lua_Integer)UClassReflectionCType::Cpp );
	SetTable( L, metatable, uClassReflection_CType );

	// 构建class
	CClassBuilder classBuilder( m_pLuaState, classNameFull.c_str(), className, metatable, metatable );
	pHowToAddClass( m_name.c_str(), &classBuilder );

	// 弹出 metatable and method 两个table
	lua_pop( L, 2 );
}


void iberbar::LuaCpp::CScopeModule::AddEnum( const char* strName, PHowToBuildEnum pHow, int nCount )
{
	lua_pushstring( m_pLuaState, strName );
	CEnumBuilder EnumBuilder( m_pLuaState, nCount );
	pHow( &EnumBuilder );
	lua_rawset( m_pLuaState, m_table );
}


void iberbar::LuaCpp::CScopeModule::AddFunctionOne( const char* functionName, lua_CFunction pFunction )
{
	lua_pushstring( m_pLuaState, functionName );
	lua_pushcclosure( m_pLuaState, pFunction, 0 );
	lua_settable( m_pLuaState, m_table );
}

void iberbar::LuaCpp::CScopeModule::AddFunctions( const luaL_Reg* pFunctionRegs )
{
	const luaL_Reg* pNode = pFunctionRegs;
	while ( pNode->name != nullptr )
	{
		lua_pushstring( m_pLuaState, pNode->name );
		lua_pushcclosure( m_pLuaState, pNode->func, 0 );
		lua_settable( m_pLuaState, m_table );
		pNode++;
	}
}

iberbar::CResult iberbar::LuaCpp::CScopeModule::Build( PHowToBuildScope pHowToBuildScope )
{
	std::vector<std::string> namespaces;
	static const int s_maxNamespaceLength = 127;
	char name[s_maxNamespaceLength + 1];
	int n = 0;
	const char* ptr_iter = m_name.c_str();
	const char* ptr_last = m_name.c_str();
	while ( true )
	{
		if ( ptr_iter[0] == '.' )
		{
			if ( n == 0 )
				return MakeResult( ResultCode::Bad, "unexpect namesapce<%s>", m_name.c_str() );

			if ( n > s_maxNamespaceLength )
				return MakeResult( ResultCode::Bad, "namesapce<%s> is too long, must be %d", m_name.c_str(), s_maxNamespaceLength );

			memcpy_s( name, s_maxNamespaceLength, ptr_last, n );
			name[n] = 0;

			namespaces.push_back( name );

			n = 0;
			ptr_last = ptr_iter + 1;
		}
		else if ( ptr_iter[0] == 0 )
		{
			if ( n == 0 )
				return MakeResult( ResultCode::Bad, "unexpect namesapce<%s>", m_name.c_str() );

			if ( n > s_maxNamespaceLength )
				return MakeResult( ResultCode::Bad, "namesapce<%s> is too long, must be %d", m_name.c_str(), s_maxNamespaceLength );

			memcpy_s( name, s_maxNamespaceLength, ptr_last, n );
			name[n] = 0;
			namespaces.push_back( name );

			break;
		}
		ptr_iter++;
		n++;
	}

	int top = lua_gettop( m_pLuaState );
	CTableScope tableScope( m_pLuaState );
	auto namespaceIter = namespaces.begin();
	auto namespaceEnd = namespaces.end();
	for ( ; namespaceIter != namespaceEnd; namespaceIter++ )
	{
		tableScope.Next( namespaceIter->c_str() );
	}
	m_table = tableScope.Table();
	top = lua_gettop( m_pLuaState );

	pHowToBuildScope( this );

	top = lua_gettop( m_pLuaState );

	tableScope.Pop();
	top = lua_gettop( m_pLuaState );

	return CResult();
}


iberbar::LuaCpp::CTableScope::CTableScope( lua_State* pLuaState )
	: m_pLuaState( pLuaState )
	, m_table( 0 )
	, m_depth( 0 )
{
}

iberbar::CResult iberbar::LuaCpp::CTableScope::Next( const char* name )
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

void iberbar::LuaCpp::CTableScope::Pop()
{
	if ( m_depth > 0 )
	{
		lua_pop( m_pLuaState, m_depth );
	}
}





iberbar::LuaCpp::CClassBuilder::CClassBuilder( lua_State* pLuaState, const char* classNameFull, const char* className, int metatable, int methods )
	: m_pLuaState( pLuaState )
	, m_classNameFull( classNameFull )
	, m_className( className )
	, m_metatable( metatable )
	, m_methods( methods )
{
}

CClassBuilder* iberbar::LuaCpp::CClassBuilder::AddConstructor( lua_CFunction func )
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

CClassBuilder* iberbar::LuaCpp::CClassBuilder::AddDistructor( lua_CFunction func )
{
	AddStandardMethod( "__gc", func );

	return this;
}

CClassBuilder* iberbar::LuaCpp::CClassBuilder::AddMemberMethod( const char* name, lua_CFunction func, UpValue* upvalues, int upvaluesCount )
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

CClassBuilder* iberbar::LuaCpp::CClassBuilder::AddStaticMethod( const char* name, lua_CFunction func )
{
	lua_pushstring( m_pLuaState, name );
	lua_pushstring( m_pLuaState, m_classNameFull.c_str() );
	lua_pushcclosure( m_pLuaState, func, 1 );
	lua_settable( m_pLuaState, m_methods );
	//SetTable( m_pLuaState, m_metatable, name );

	return this;
}

CClassBuilder* iberbar::LuaCpp::CClassBuilder::AddStandardMethod( const char* name, lua_CFunction func )
{
	lua_pushcfunction( m_pLuaState, func );
	SetTable( m_pLuaState, m_metatable, name );

	return this;
}








iberbar::LuaCpp::CEnumBuilder::CEnumBuilder( lua_State* pLuaState, int nValueCount )
	: m_pLuaState( pLuaState )
	, m_nTable( 0 )
{
	lua_createtable( m_pLuaState, 0, nValueCount > 0 ? nValueCount : 0 );
	m_nTable = lua_gettop( m_pLuaState );
}


void iberbar::LuaCpp::CEnumBuilder::AddValueInt( const char* strKey, lua_Integer Value )
{
	lua_pushstring( m_pLuaState, strKey );
	lua_pushinteger( m_pLuaState, Value );
	lua_rawset( m_pLuaState, m_nTable );
}


void iberbar::LuaCpp::CEnumBuilder::AddValueString( const char* strKey, const char* Value )
{
	lua_pushstring( m_pLuaState, strKey );
	lua_pushstring( m_pLuaState, Value );
	lua_rawset( m_pLuaState, m_nTable );
}

