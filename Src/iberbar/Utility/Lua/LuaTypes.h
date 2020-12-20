

#pragma once

#include <iberbar/Base/Unknown.h>
#include <iberbar/Base/UnknownObject.h>
#include <iberbar/Base/Lua/LuaStateRef.h>
#include <iberbar/Base/Result.h>


namespace iberbar
{

	typedef void (*PLuaAPIError)( const char*, int, const char* );
	extern PLuaAPIError gLuaAPIError;
#define SendLuaAPIError( err ) { assert( gLuaAPIError ); gLuaAPIError( __FILE__, __LINE__, err ); }
	//inline void SendLuaAPIError( const char* sfile, int sline, const char* err )
	//{
	//	assert( gLuaAPIError );
	//	gLuaAPIError( sfile, sline, err );
	//}
	static void LuaAPIErrorDefault( const char* sfile, int sline, const char* err );


	struct LuaCppClassUsertype
	{
		void* data;
	};



	//==========================================================================
	//  1.通过declareStart和declareEnd来在lua定义一个c++类
	//    在之间添加成员函数和构造函数、析构函数
	//==========================================================================
	class CLuaCppClass
		: public CUnknown
	{
		IBERBAR_UNKNOWN_CLONE_DISABLED( CLuaCppClass );

	public:
		CLuaCppClass( void );

	public:
		// 开始定义类
		void declareStart( lua_State* pLuaState, const char* strClassName );
		// 结束定义类
		void declareEnd();

		void setFunctionConstruct( lua_CFunction func ); // new
		void setFunctionImportant( const char* name, lua_CFunction func ); // __tostring, __gc, etc
		void setFunction( const luaL_Reg* regs ); // member function
		void setFunctionStatic(); // static member function
		void setVariable(); // member variable

		static int  PushInstance( lua_State* L, const char* strClassName, void *obj, bool gc, bool* pbHasUserdata );
		void check();


		const char* getClassName() { return m_strClassName.c_str(); }


	private:
		void set( lua_State *L, int table_index, const char *key );
		static void* push_userdata( lua_State *L, void *key, size_t sz );
		static void subtable( lua_State *L, int tindex, const char *name, const char *mode );


	private:
		lua_State* m_pLuaState;
		std::string m_strClassName;
		int m_bHasDeclareStarted;

		int m_methods;
		int m_metatable;
	};

	IBERBAR_UNKNOWN_PTR_DECLARE( CLuaCppClass );

	template < typename T >
	IBERBAR_LUA_API( LuaCppClassInterface_Method_New );

	template < typename T >
	IBERBAR_LUA_API( LuaCppClassInterface_Method_Delete );

	template < typename T >
	IBERBAR_LUA_API( LuaCppClassInterface_Method_UnknownNew );

	IBERBAR_LUA_API( LuaCppClassInterface_Method_UnknownNewAbstract );

	IBERBAR_LUA_API( LuaCppClassInterface_Method_UnknownRelease );

	IBERBAR_LUA_API( LuaCppClassInterface_Method___tostring );



	template < typename T >
	T* LuaCppClassGetObject( lua_State* pLuaState, int idx = 1 )
	{
		LuaCppClassUsertype* s = (LuaCppClassUsertype*)lua_touserdata( pLuaState, idx );
		if ( s == NULL )
			return NULL;
		return (T*)s->data;
	}

#define lua_tocppobject( L, type, idx )           iberbar::LuaCppClassGetObject< type >( L, idx )
#define lua_pushcppobject( L, type, val, gc )     iberbar::CLuaCppClass::PushInstance( L, type, val, gc, NULL )
//#define lua_pushcppunknown( L, type, val, gc )  \
//	{ bool lc_bAlloc = false; iberbar::CLuaCppClass::PushInstance( L, type, val, gc, &lc_bAlloc ); if ( lc_bAlloc == true ) (val)->addRef(); }





	class CLuaUnknownEventDelegateBinder
		: public iberbar::CUnknownObject
	{
		IBERBAR_UNKNOWN_CLONE_DISABLED( CLuaUnknownEventDelegateBinder );

	public:
		CLuaUnknownEventDelegateBinder( PTR_CLuaStateRef ptrLua, PTR_CLuaCppClass ptrCppClass, const char* pzLuaFunction );
		~CLuaUnknownEventDelegateBinder() OVERRIDE
		{
			int kk = 0;
		}

		void execute( PTR_CUnknownObject pEventSender, const UnknownEventParams& refEventParams );

	protected:
		PTR_CLuaStateRef m_ptrLuaRef;        // lua接口
		PTR_CLuaCppClass m_ptrCppClass;   // 事件发送者luacpp类型
		std::string      m_pzLuaFunction;   // 在lua脚本的回掉函数签名
		//CUnknown*        m_pEventSender;  // 事件发送者
	};


	class CLuaUnknownEventDelegateBinderArray
	{
	public:
		CLuaUnknownEventDelegateBinderArray( void ) {}
		~CLuaUnknownEventDelegateBinderArray();

		void addBinder( CLuaUnknownEventDelegateBinder* pBinder )
		{
			if ( pBinder )
			{
				m_ArrayBinders.push_back( pBinder );
				pBinder->addRef();
			}
		}

	private:
		std::vector< CLuaUnknownEventDelegateBinder* > m_ArrayBinders;
	};



	// 添加常量到lua
	void LuaSetConstVariable_Integer( PTR_CLuaStateRef pLuaRef, const char* strName, lua_Integer value );
	void LuaSetConstVariable_Number( PTR_CLuaStateRef pLuaRef, const char* strName, lua_Number value );
	void LuaSetConstVariable_String( PTR_CLuaStateRef pLuaRef, const char* strName, const char* value );
	void LuaSetConstVariable_Enum();





	class CLuaCFunctionHelper
	{
	public:
		CLuaCFunctionHelper( lua_State* pLuaState, const char* pzFunctionName, int nReturns );
		~CLuaCFunctionHelper();

	public:
		bool isArgNumber( int nArg );
		bool isArgInteger( int nArg );
		bool isArgBoolean( int nArg );
		bool isArgString( int nArg );
		bool isArgUserdata( int nArg );
		bool isArgCppCUnknown( int nArg ) { return isArgUserdata( nArg ); }

	public:
		lua_Number  getArgNumber( int nArg );
		lua_Integer getArgInteger( int nArg );
		bool        getArgBoolean( int nArg );
		const char* getArgString( int nArg );
		void*       getArgCppCUnknown( int nArg );

	public:
		void pushReturnNil();
		void pushReturnNumber( lua_Number value );
		void pushReturnBoolean( bool value );
		void pushReturnString( const char* value );
		void pushReturnCppCUnknown( const char* type, CUnknown* value, bool gc = true );
		void pushReturnCppPointer( const char* type, void* value, bool gc = true );
		int  getReturnsPushed();


	protected:
		lua_State* m_pLuaState;
		const char* m_pzFunctionName;
		int m_nReturns;
		int m_nReturnsPushed;
	};
}





template < typename T >
IBERBAR_LUA_API( iberbar::LuaCppClassInterface_Method_New )
{
	int n = lua_gettop( pLuaState );
	lua_pushstring( pLuaState, "classname" );
	lua_gettable( pLuaState, 1 );
	const char* classname = lua_tostring( pLuaState, -1 );
	lua_pop( pLuaState, 1 );
	//lua_remove( L, 1 );   // use classname:new(), instead of classname.new()
	T *obj = new T();  // call constructor for T objects
	iberbar::CLuaCppClass::PushInstance( pLuaState, classname, obj, true, NULL ); // gc_T will delete this object
	return 1;           // userdata containing pointer to T object


// 	lua_remove( pLuaState, 1 );   // use classname:new(), instead of classname.new()
// 	T *obj = new T();  // call constructor for T objects
// 	iberbar::CLuaCppClass::PushInstance( pLuaState, "Student", obj, true ); // gc_T will delete this object
// 	return 1;           // userdata containing pointer to T object
}


template < typename T >
IBERBAR_LUA_API( iberbar::LuaCppClassInterface_Method_Delete )
{
	int n = lua_gettop( pLuaState );

	lua_pushstring( pLuaState, "classname" );
	lua_gettable( pLuaState, 1 );
	const char* classname = lua_tostring( pLuaState, -1 );
	lua_pop( pLuaState, 1 );

	LuaCppClassUsertype* lc_UserType = (LuaCppClassUsertype*)luaL_checkudata( pLuaState, 1, classname );
	if ( lc_UserType )
	{
		delete ( (T*)( lc_UserType->data ) );  // call destructor for T objects
	}
	return 0;
}


template < typename T >
IBERBAR_LUA_API( iberbar::LuaCppClassInterface_Method_UnknownNew )
{
	int n = lua_gettop( pLuaState );
	lua_pushstring( pLuaState, "classname" );
	lua_gettable( pLuaState, 1 );
	const char* classname = lua_tostring( pLuaState, -1 );
	lua_pop( pLuaState, 1 );

	//lua_remove( L, 1 );   // use classname:new(), instead of classname.new()
	T *obj = new T();  // call constructor for T objects
	iberbar::CLuaCppClass::PushInstance( pLuaState, classname, obj, true, NULL ); // gc_T will delete this object
	return 1;           // userdata containing pointer to T object
}


static IBERBAR_LUA_API( iberbar::LuaCppClassInterface_Method_UnknownNewAbstract )
{
	lua_pushnil( pLuaState );
	return 1;
}


static IBERBAR_LUA_API( iberbar::LuaCppClassInterface_Method_UnknownRelease )
{
	int n = lua_gettop( pLuaState );

	lua_pushstring( pLuaState, "classname" );
	lua_gettable( pLuaState, 1 );
	const char* classname = lua_tostring( pLuaState, -1 );
	lua_pop( pLuaState, 1 );

	LuaCppClassUsertype* lc_UserType = (LuaCppClassUsertype*)luaL_checkudata( pLuaState, 1, classname );
	if ( lc_UserType )
	{
		( (CUnknown*)( lc_UserType->data ) )->release();  // call destructor for T objects
	}
	return 0;
}


static IBERBAR_LUA_API( iberbar::LuaCppClassInterface_Method___tostring )
{
// 	T* lc_obj = iberbar::LuaCppClassGetObject< T >( pLuaState );
// 	if ( lc_obj == NULL )
// 	{
// 		luaL_argcheck( pLuaState, lc_obj != NULL, 1, "invalid user data" );
// 		lua_pushnil( pLuaState );
// 		return 1;
// 	}
// 
// 	lua_settop( pLuaState, 0 );
// 	lua_pushstring( pLuaState, NAME );

	int n = lua_gettop( pLuaState );
	if ( n != 1 )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	lua_pushstring( pLuaState, "classname" );
	lua_gettable( pLuaState, 1 );
	const char* classname = lua_tostring( pLuaState, -1 );
	lua_pop( pLuaState, 1 );

	if ( classname == NULL )
		lua_pushnil( pLuaState );
	else
		lua_pushstring( pLuaState, classname );

	return 1;
}







inline lua_Number  iberbar::CLuaCFunctionHelper::getArgNumber( int nArg )        { return lua_tonumber( m_pLuaState, nArg ); }
inline lua_Integer iberbar::CLuaCFunctionHelper::getArgInteger( int nArg )       { return lua_tointeger( m_pLuaState, nArg ); }
inline bool        iberbar::CLuaCFunctionHelper::getArgBoolean( int nArg )       { return ( lua_toboolean( m_pLuaState, nArg ) == 0 ) ? false : true; }
inline const char* iberbar::CLuaCFunctionHelper::getArgString( int nArg )        { return lua_tostring( m_pLuaState, nArg ); }
inline void*       iberbar::CLuaCFunctionHelper::getArgCppCUnknown( int nArg )   {
	LuaCppClassUsertype* s = (LuaCppClassUsertype*)lua_touserdata( m_pLuaState, nArg );
	if ( s == NULL )
		return NULL;
	return s->data;
}
inline void iberbar::CLuaCFunctionHelper::pushReturnNil()                                                                    { m_nReturnsPushed ++; lua_pushnil( m_pLuaState ); }
inline void iberbar::CLuaCFunctionHelper::pushReturnNumber( lua_Number value )                                               { m_nReturnsPushed ++;lua_pushnumber( m_pLuaState, value ); }
inline void iberbar::CLuaCFunctionHelper::pushReturnBoolean( bool value )                                                    { m_nReturnsPushed ++; lua_pushboolean( m_pLuaState, ( ( value == true ) ? 1 : 0 ) ); }
inline void iberbar::CLuaCFunctionHelper::pushReturnString( const char* value )                                              { m_nReturnsPushed ++; lua_pushstring( m_pLuaState, value ); }
inline void iberbar::CLuaCFunctionHelper::pushReturnCppCUnknown( const char* type, CUnknown* value, bool gc )         { m_nReturnsPushed ++; bool lc_bAlloc = false; iberbar::CLuaCppClass::PushInstance( m_pLuaState, type, value, gc, &lc_bAlloc ); if ( lc_bAlloc == true ) value->addRef(); }
inline void iberbar::CLuaCFunctionHelper::pushReturnCppPointer( const char* type, void* value, bool gc )              { m_nReturnsPushed ++; iberbar::CLuaCppClass::PushInstance( m_pLuaState, type, value, gc, NULL ); }
inline int  iberbar::CLuaCFunctionHelper::getReturnsPushed()                                                                 { return m_nReturnsPushed; }




//#define IBERBAR_LUACPP_CLASS( ns, type )                 iberbar::PTR_CLuaCppClass LuaCppClass_##type( iberbar::PTR_CLuaStateRef ptrLuaStateRef )
//#define IBERBAR_LUACPP_CLASS_DEFINE( type )          iberbar::PTR_CLuaCppClass jasmine_ex::LuaCppClass_##type( iberbar::PTR_CLuaStateRef ptrLuaStateRef )
#define IBERBAR_LUACPP_CLASSNAME( ns, type )                    LuaCppClassName_##ns##_##type
#define IBERBAR_LUACPP_CLASSNAME_DEFINE( ns, type, name )       static const char* IBERBAR_LUACPP_CLASSNAME( ns, type ) = name


#define IBERBAR_LUACPP_API_NAME( ns, type, method )      LuaCppAPI_##ns##_##type##_##method
#define IBERBAR_LUACPP_API( ns, type, method )           IBERBAR_LUA_API( IBERBAR_LUACPP_API_NAME( ns, type, method ) )


#define IBERBAR_LUACPP_REG_NAME( ns, type )                  LuaCppReg_##ns##_##type
#define IBERBAR_LUACPP_REG_DECLARE( ns, type )               static const luaL_Reg IBERBAR_LUACPP_REG_NAME( ns, type ) []
#define IBERBAR_LUACPP_REG( ns, type, methodname, method  )  { methodname, IBERBAR_LUACPP_API_NAME( ns, type, method ) }
#define IBERBAR_LUACPP_REG_END                               { NULL, NULL }

#define IBERBAR_LUACPP_API_TOSTRING( type )  iberbar::LuaCppClassInterface_Method___tostring
#define IBERBAR_LUACPP_API_NEW( type )         iberbar::LuaCppClassInterface_Method_UnknownNew< type >
#define IBERBAR_LUACPP_API_NEW_ABSTRACT( type ) iberbar::LuaCppClassInterface_Method_UnknownNewAbstract
#define IBERBAR_LUACPP_API_GC( type )          iberbar::LuaCppClassInterface_Method_UnknownRelease



inline void lua_pushcppunknown( lua_State* L, const char* type, iberbar::CUnknown* val, bool gc )
{
	bool alloc = false;
	iberbar::CLuaCppClass::PushInstance( L, type, val, gc, &alloc );
	if ( alloc == true ) 
		val->addRef();
}
//	{ bool lc_bAlloc = false; iberbar::CLuaCppClass::PushInstance( L, type, val, gc, &lc_bAlloc ); if ( lc_bAlloc == true ) (val)->addRef(); }