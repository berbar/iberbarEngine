
#include <iberbar/Base/Lua/LuaStateRef.h>
#include <iberbar/Base/Lua/LuaTypes.h>

#include <list>



iberbar::PLuaAPIError iberbar::gLuaAPIError = iberbar::LuaAPIErrorDefault;
void iberbar::LuaAPIErrorDefault( const char* sfile, int sline, const char* err )
{
}



iberbar::CLuaCppClass::CLuaCppClass( void )
	: CUnknown()
	, m_pLuaState( NULL )
	, m_strClassName( "" )
	, m_bHasDeclareStarted( 0 )
{
}


void iberbar::CLuaCppClass::declareStart( lua_State* pLuaState, const char* strClassName )
{
	assert( pLuaState );
	assert( strClassName );
	m_pLuaState = pLuaState;
	m_strClassName = strClassName;

	assert( m_bHasDeclareStarted == 0 );
	m_bHasDeclareStarted = 1;

	lua_State* L = m_pLuaState;

	// 创建method table方法表
	lua_newtable( L );
	m_methods = lua_gettop( L );

	// 创建meta元表
	// 说明：luaL_newmetatable函数创建一个新表(此表将被用作metatable), 将新表放
	// 到栈顶并建立表和registry中T::className的联系。
	luaL_newmetatable( L, m_strClassName.c_str() );
	m_metatable = lua_gettop( L );

	// store method table in globals so that
	// scripts can add functions written in Lua.
	// 将method table拷贝压入栈顶，然后设置到全局索引中，名称为T::className
	lua_pushvalue( L, m_methods );

	// set函数的功能：如果set(A,B,C),
	// 那么，将当前栈顶的元素作为值,C作为key，添加到索引为B的表中.
	// 当B为负数时，-1，-2，被值、key占用，-3是压入值之前的元素......
	// 所以这行代码读作：在LUA_GLOBALSINDEX表中添加T::className为key的值。
	//                   值就是那个栈顶元素。
	// =========调用set后，栈回到压入值之前的状态，因为lua_settable弹出值和key。======
	//set( L, LUA_GLOBALSINDEX, m_strClassName.c_str() );
	lua_setglobal( L, m_strClassName.c_str() );

	// hide metatable from Lua getmetatable()
	// 设置meta table的__metatable域
	lua_pushvalue( L, m_methods );
	set( L, m_metatable, "__metatable" );

	// 设置meta table的__index域
	// 说明：__index域可以是一个函数，也可以是一个表
	// 当它是一个函数的时候，Lua将table和缺少的域作为
	// 参数调用这个函数；当它是一个表的时候，Lua将在这
	// 个表中看是否有缺少的域。
	lua_pushvalue( L, m_methods );
	set( L, m_metatable, "__index" );

	lua_pushstring( L, m_strClassName.c_str() );
	set( L, m_methods, "classname" );         // add new_T to method table
}


void iberbar::CLuaCppClass::declareEnd()
{
	assert( m_bHasDeclareStarted == 1 );
	m_bHasDeclareStarted = 0;

	lua_State* L = m_pLuaState;

	lua_pop( L, 2 );  // drop metatable and method table
}


void iberbar::CLuaCppClass::setFunctionConstruct( lua_CFunction func )
{
	assert( m_bHasDeclareStarted == 1 );

	lua_State* L = m_pLuaState;

	lua_newtable( L );                // mt for method table
	lua_pushcfunction( L, func );
	lua_pushvalue( L, -1 );           // dup new_T function
	set( L, m_methods, "new" );         // add new_T to method table
	set( L, -3, "__call" );           // mt.__call = new_T
	lua_setmetatable( L, m_methods );
}


void iberbar::CLuaCppClass::setFunctionImportant( const char* name, lua_CFunction func )
{
	assert( m_bHasDeclareStarted == 1 );

	lua_State* L = m_pLuaState;

	lua_pushcfunction( L, func );
	set( L, m_metatable, name );
}


void iberbar::CLuaCppClass::setFunction( const luaL_Reg* regs )
{
	assert( m_bHasDeclareStarted == 1 );

	lua_State* L = m_pLuaState;

	// fill method table with methods from class T
	for ( const luaL_Reg *l = regs; l->name; l++ )
	{
		lua_pushstring( L, l->name );

		// Pushes a light userdata onto the stack. 
		// Userdata represent C values in Lua. 
		// A light userdata represents a pointer. 
		// It is a value (like a number): 
		// you do not create it, it has no 
		// individual metatable, and it is 
		// not collected (as it was never created). 
		// A light userdata is equal to "any" 
		// light userdata with the same C address. 
		lua_pushlightuserdata( L, (void*)l );
		//lua_pushcfunction( L, l->func );

		// 创建c函数thunk，带有1个upvalue，并从栈上弹出这个upvalue
		// 此处，upvalue是lua_pushlightuserdata(L, (void*)l)压入的l
		lua_pushcclosure( L, l->func, 1 );
		//LuaSet( L, metatable, l->name );

		lua_settable( L, m_methods );
	}
}


void iberbar::CLuaCppClass::set( lua_State *L, int table_index, const char *key )
{
	lua_pushstring( L, key );
	lua_insert( L, -2 );  // swap value and key
	lua_settable( L, table_index );
}


int iberbar::CLuaCppClass::PushInstance( lua_State* L, const char* strClassName, void *obj, bool gc, bool* pbHasUserdata )
{

	if ( !obj )
	{
		lua_pushnil( L ); return 0;
	}
	luaL_getmetatable( L, strClassName );  // lookup metatable in Lua registry
	if ( lua_isnil( L, -1 ) )
	{
		luaL_error( L, "%s missing metatable", strClassName );
	}
	int mt = lua_gettop( L );

	// subtable首先在这个表上查找userdata的值
	// 如果表有userdata字段则返回该字段的值（是一个表）
	// 如果表没有userdata字段则新建一个表，设置这个新建表
	// 的meta表的__mode="v"。
	subtable( L, mt, "userdata", "v" );


	// pushuserdata函数会在栈顶放置userdata
	// 这个userdata可能是根据key查找到的，也
	// 可能是新创建的。
	LuaCppClassUsertype* lc_userdata =
		(LuaCppClassUsertype*)( push_userdata( L, obj, sizeof( LuaCppClassUsertype ) ) );
	if ( lc_userdata )
	{
		lc_userdata->data = obj;  // store pointer to object in userdata
		lua_pushvalue( L, mt );
		lua_setmetatable( L, -2 );
		if ( gc == false )
		{
			lua_checkstack( L, 3 );
			subtable( L, mt, "do not trash", "k" );
			lua_pushvalue( L, -2 );
			lua_pushboolean( L, 1 );
			lua_settable( L, -3 );
			lua_pop( L, 1 );
		}
	}
	if ( pbHasUserdata )
	{
		if ( lc_userdata )
			*pbHasUserdata = true;
		else
			*pbHasUserdata = false;
	}
	lua_replace( L, mt );
	lua_settop( L, mt );
	return mt;  // index of userdata containing pointer to T object
}


void* iberbar::CLuaCppClass::push_userdata( lua_State *L, void *key, size_t sz )
{
	void *ud = 0;
	lua_pushlightuserdata( L, key );
	lua_gettable( L, -2 );     // lookup[key]
	if ( lua_isnil( L, -1 ) )
	{
		lua_pop( L, 1 );         // drop nil
		lua_checkstack( L, 3 );
		ud = lua_newuserdata( L, sz );  // create new userdata
		lua_pushlightuserdata( L, key );
		lua_pushvalue( L, -2 );  // dup userdata

		lua_settable( L, -4 );   // lookup[key] = userdata
	}
	return ud;

}


void iberbar::CLuaCppClass::subtable( lua_State *L, int tindex, const char *name, const char *mode )
{
	// 在指定的表上查找name的值
	// 如果没有对应值，则新建一个表，并将这个表作为自身的meta表
	// 这个表在栈顶
	lua_pushstring( L, name );
	lua_gettable( L, tindex );
	if ( lua_isnil( L, -1 ) )
	{
		lua_pop( L, 1 );
		lua_checkstack( L, 3 );
		lua_newtable( L );
		lua_pushvalue( L, -1 );  // table is its own metatable
		lua_setmetatable( L, -2 );
		lua_pushliteral( L, "__mode" );
		lua_pushstring( L, mode );
		lua_settable( L, -3 );   // metatable.__mode = mode
		lua_pushstring( L, name );
		lua_pushvalue( L, -2 );
		lua_settable( L, tindex );
	}
}





iberbar::CLuaUnknownEventDelegateBinder::CLuaUnknownEventDelegateBinder( PTR_CLuaStateRef ptrLua, PTR_CLuaCppClass ptrCppClass, const char* pzLuaFunction )
	: CUnknownObject()
	, m_ptrLuaRef( ptrLua )
	, m_ptrCppClass( ptrCppClass )
	, m_pzLuaFunction( pzLuaFunction )
{
}


void iberbar::CLuaUnknownEventDelegateBinder::execute( PTR_CUnknownObject pEventSender, const UnknownEventParams& refEventParams )
{
	lua_State* lc_pLuaState = m_ptrLuaRef->get();

	int a = 0;
	lua_getglobal( lc_pLuaState, m_pzLuaFunction.c_str() );
	// 暂时不传入CUnknown*对象
	//m_ptrCppClass->PushInstance( lc_pLuaState, m_ptrCppClass->getClassName(), pEventSender, true ); // 1.object
	lua_pushcppunknown( lc_pLuaState, "CUnknownObject", (CUnknownObject*)pEventSender, true );
	lua_pushinteger( lc_pLuaState, refEventParams.nEventId );       // 2. eventid ( integer )
	lua_pushinteger( lc_pLuaState, refEventParams.wParam );         // 3. arg1 ( integer )
	lua_pushinteger( lc_pLuaState, refEventParams.lParam );         // 4. arg2 ( integer )
	lua_pushstring( lc_pLuaState, refEventParams.cParam.c_str() );  // 5. arg3 ( string )
	lua_pcall( lc_pLuaState, 5, 0, 0 );
}






iberbar::CLuaUnknownEventDelegateBinderArray::~CLuaUnknownEventDelegateBinderArray()
{
	if ( m_ArrayBinders.empty() == false )
	{
		std::vector< CLuaUnknownEventDelegateBinder* >::iterator lc_iter = m_ArrayBinders.begin();
		std::vector< CLuaUnknownEventDelegateBinder* >::iterator lc_end  = m_ArrayBinders.end();
		for ( ; lc_iter != lc_end; lc_iter ++ )
			UNKNOWN_SAFE_RELEASE_NULL( *lc_iter );
	}
}




//----------------------------------------------------------------------------------------------------------------
iberbar::CLuaCFunctionHelper::CLuaCFunctionHelper( lua_State* pLuaState, const char* pzFunctionName, int nReturns )
	: m_pLuaState( nullptr )
	, m_pzFunctionName( pzFunctionName )
	, m_nReturns( nReturns )
	, m_nReturnsPushed( 0 )
{
	assert( m_pLuaState );
	assert( m_pzFunctionName );
	assert( m_nReturns >= 0 );
}


//----------------------------------------------------------------------------------------------------------------
iberbar::CLuaCFunctionHelper::~CLuaCFunctionHelper()
{
	//GetLogFile()->logva(
	//	PRIORITY_ERROR,
	//	"CLuaCFunctionHelper",
	//	"In cfunction <%s>, it expects to return %d params, but it has just returned %d. Are you forget to push some params to return to lua?",
	//	m_pzFunctionName,
	//	m_nReturns,
	//	m_nReturnsPushed );
	assert( m_nReturnsPushed == m_nReturns );
}


//----------------------------------------------------------------------------------------------------------------
bool iberbar::CLuaCFunctionHelper::isArgBoolean( int nArg )
{
	bool lc_ret = lua_isboolean( m_pLuaState, nArg );
	if ( lc_ret == false )
	{
		//GetLogFile()->logva(
		//	PRIORITY_ERROR,
		//	"CLuaCFunctionHelper",
		//	"In cfunction <%s>, the type of number%d argument is not boolean.",
		//	m_pzFunctionName,
		//	nArg );
	}
	return lc_ret;
}


//----------------------------------------------------------------------------------------------------------------
bool iberbar::CLuaCFunctionHelper::isArgNumber( int nArg )
{
	bool lc_ret = ( lua_isnumber( m_pLuaState, nArg ) == 0 ) ? false : true;
	if ( lc_ret == false )
	{
		//GetLogFile()->logva(
		//	PRIORITY_ERROR,
		//	"CLuaCFunctionHelper",
		//	"In cfunction <%s>, the type of number%d argument is not number.",
		//	m_pzFunctionName,
		//	nArg );
	}
	return lc_ret;
}


//----------------------------------------------------------------------------------------------------------------
bool iberbar::CLuaCFunctionHelper::isArgInteger( int nArg )
{
	bool lc_ret = ( lua_isnumber( m_pLuaState, nArg ) == 0 ) ? false : true;
	if ( lc_ret == false )
	{
		//GetLogFile()->logva(
		//	PRIORITY_ERROR,
		//	"CLuaCFunctionHelper",
		//	"In cfunction <%s>, the type of number%d argument is not integer.",
		//	m_pzFunctionName,
		//	nArg );
	}
	return lc_ret;
}


//----------------------------------------------------------------------------------------------------------------
bool iberbar::CLuaCFunctionHelper::isArgString( int nArg )
{
	bool lc_ret = ( lua_isstring( m_pLuaState, nArg ) == 0 ) ? false : true;
	if ( lc_ret == false )
	{
		//GetLogFile()->logva(
		//	PRIORITY_ERROR,
		//	"CLuaCFunctionHelper",
		//	"In cfunction <%s>, the type of number%d argument is not string.",
		//	m_pzFunctionName,
		//	nArg );
	}
	return lc_ret;
}


//----------------------------------------------------------------------------------------------------------------
bool iberbar::CLuaCFunctionHelper::isArgUserdata( int nArg )
{
	bool lc_ret = ( lua_isuserdata( m_pLuaState, nArg ) == 0 ) ? false : true;
	if ( lc_ret == false )
	{
		//GetLogFile()->logva(
		//	PRIORITY_ERROR,
		//	"CLuaCFunctionHelper",
		//	"In cfunction <%s>, the type of number%d argument is not userdata.",
		//	m_pzFunctionName,
		//	nArg );
	}
	return lc_ret;
}








//----------------------------------------------------------------------------------------------------------------
void iberbar::LuaSetConstVariable_Integer( PTR_CLuaStateRef pLuaRef, const char* strName, lua_Integer value )
{
	lua_State* lc_pLuaState = pLuaRef->get();
	lua_Integer* lc_pointer = (lua_Integer*)lua_newuserdata( lc_pLuaState, sizeof( lua_Integer ) );
	*lc_pointer = value;
	//lua_pushvalue( lc_pLuaState, -1 );
	lua_setglobal( lc_pLuaState, strName );
}








