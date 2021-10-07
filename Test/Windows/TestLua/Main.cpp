
#include "iberbar/Lua/LuaDevice.h"
#include <iberbar/Lua/LuaError.h>
#include <iberbar/Lua/LuaTable.h>
#include <iberbar/Lua/LuaCppBuilder.h>
#include <iberbar/Lua/LuaCppInstantiateDef.h>
#include <iberbar/Lua/LuaCallback.h>
#include <iberbar/Lua/FunctionHelper.h>
#include <functional>
#include <xmemory>
#include <memory_resource>


std::pmr::unsynchronized_pool_resource pool;


extern "C" int LuaCFunction_TestArgumentsNotMatch( lua_State* L )
{
	int ntop = lua_gettop( L );
	if (ntop != 2)
	{
		return iberbar_LuaError_ArgumentsCountIsNotMatch( L, 2 );
	}
	return 0;
}

int lrTest( lua_State* L )
{
	int u1 = lua_tointeger( L, lua_upvalueindex( 1 ) );
	printf( "lrTest:%d\n", u1 );
	return 0;
}

luaL_Reg lr = {
	"lrTest", lrTest
};


class CFooNumbers
	: public iberbar::CRef
{
public:
	CFooNumbers()
		: a( 0 )
		, b( 0 )
		, m_Callback()
	{
	}
	virtual ~CFooNumbers()
	{

	}
public:
	void SetA( int a ) { this->a = a; }
	void SetB( int b ) { this->b = b; }
	int GetA() { return this->a; }
	int GetB() { return this->b; }
	void BindCallback( std::function<void( int, int )> callback ) { m_Callback = callback; }
	void Print() { if ( m_Callback) m_Callback( a, b ); }
private:
	int a;
	int b;
	std::function<void( int, int )> m_Callback;
};

class CFoo
	: public iberbar::CRef
{
public:
	CFoo() : m_ns( nullptr ) {}
	~CFoo()
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_ns );
		std::printf( "delete foo\n" );
	}
public:
	int Add( int a, int b ) { return a + b; }
	int Sub( int a, int b ) { return a - b; }
	void Hello( const char* name ) { printf( "(%d,%d)hello, %s.\n", m_ns->GetA(), m_ns->GetB(), name ); }
	void SetNumbers( CFooNumbers* ns ) { m_ns = ns; m_ns->AddRef(); }
	CFooNumbers* GetNumbers() { return m_ns; }
private:
	CFooNumbers* m_ns;
};


class CTestStlAllocator_0
{
public:
	CTestStlAllocator_0() {}
	virtual ~CTestStlAllocator_0() {
		std::printf( "stl allocator destory: 0\n" );
	}

	void* operator new( size_t n)
	{
		std::pmr::polymorphic_allocator<CTestStlAllocator_0> a( &pool );
		return a.allocate( 1 );
	}

	void* operator new[]( size_t n ) = delete;

	void operator delete( void* p )
	{
		std::pmr::polymorphic_allocator<CTestStlAllocator_0> a( &pool );
		a.deallocate( (CTestStlAllocator_0*)p, 1 );
	}

	void operator delete[]( void* p ) = delete;
};

class CTestStlAllocator_1
	: public CTestStlAllocator_0
{
public:
	CTestStlAllocator_1(): gaga(0) {}
	virtual ~CTestStlAllocator_1() {
		std::printf( "stl allocator destory: 1\n" );
	}
private:
	int gaga;

public:
	void* operator new(size_t n)
	{
		std::pmr::polymorphic_allocator<CTestStlAllocator_1> a( &pool );
		return a.allocate( 1 );
	}

	//void* operator new[]( size_t n ) = delete;
	//{
	//	std::pmr::polymorphic_allocator<CTestStlAllocator_1> a( &pool );
	//	return a.allocate( n/sizeof(CTestStlAllocator_1) );
	//}

	void operator delete(void* p)
	{
		std::pmr::polymorphic_allocator<CTestStlAllocator_1> a( &pool );
		a.deallocate( (CTestStlAllocator_1*)p, 1 );
	}

	//void operator delete[]( void* p ) = delete;
	//{
	//	std::pmr::polymorphic_allocator<CTestStlAllocator_1> a( &pool );
	//	a.deallocate( (CTestStlAllocator_1*)p, 1 );
	//}
};

const char uLuaCpp_CFoo_ClassName[] = "CFoo";
const char uLuaCpp_CFooNumbers_ClassName[] = "CFooNumbers";

int LuaCpp_CFoo_Add( lua_State* L )
{
	CFoo* foo = lua_tocppobject( L, 1, CFoo );
	int a = lua_tointeger( L, 2 );
	int b = lua_tointeger( L, 3 );
	lua_pushinteger( L, foo->Add( a, b ) );
	return 1;
}

int LuaCpp_CFoo_Sub( lua_State* L )
{
	CFoo* foo = lua_tocppobject( L, 1, CFoo );
	int a = lua_tointeger( L, 2 );
	int b = lua_tointeger( L, 3 );
	lua_pushinteger( L, foo->Sub( a, b ) );
	return 1;
}

int LuaCpp_CFoo_Hello( lua_State* L )
{
	CFoo* foo = lua_tocppobject( L, 1, CFoo );
	const char* name = lua_tostring( L, 2 );
	foo->Hello( name );
	return 0;
}

int LuaCpp_CFoo_SetNumbers( lua_State* L )
{
	CFoo* foo = lua_tocppobject( L, 1, CFoo );
	CFooNumbers* fooNumbers = lua_tocppobject( L, 2, CFooNumbers );
	foo->SetNumbers( fooNumbers );
	return 0;
}

int LuaCpp_CFoo_GetNumbers( lua_State* L )
{
	CFoo* foo = lua_tocppobject( L, 1, CFoo );
	CFooNumbers* fooNumbers = foo->GetNumbers();
	lua_pushcppref( L, uLuaCpp_CFooNumbers_ClassName, fooNumbers );
	//fooNumbers->AddRef();
	return 1;
}

int LuaCpp_CFooNumbers_Set( lua_State* L )
{
	CFooNumbers* fooNumbers = lua_tocppobject( L, 1, CFooNumbers );
	int a = lua_tointeger( L, 2 );
	int b = lua_tointeger( L, 3 );
	fooNumbers->SetA( a );
	fooNumbers->SetB( b );
	return 0;
}

int LuaCpp_CFooNumbers_Get( lua_State* L )
{
	CFooNumbers* fooNumbers = lua_tocppobject( L, 1, CFooNumbers );
	lua_pushinteger( L, fooNumbers->GetA() );
	lua_pushinteger( L, fooNumbers->GetB() );
	return 2;
}

int LuaCpp_CFooNumbers_BindCallback( lua_State* L )
{
	CFooNumbers* fooNumbers = lua_tocppobject( L, 1, CFooNumbers );
	lua_Integer nCallback = lua_toluacallback( L, 2 );
	iberbar::Lua::PTR_CLuaCallbackHandler pCallbackHandler = iberbar::Lua::PTR_CLuaCallbackHandler::_sNew( L, nCallback );
	fooNumbers->BindCallback(
		[=]( int a, int b )
		{
			if ( pCallbackHandler->PushFunction() == true )
			{
				lua_pushinteger( pCallbackHandler->GetLuaState(), a );
				lua_pushinteger( pCallbackHandler->GetLuaState(), b );
				lua_pcall( pCallbackHandler->GetLuaState(), 2, LUA_MULTRET, 0 );
			}
		}
	);
	return 0;
}

int LuaCpp_CFooNumbers_Print( lua_State* L )
{
	CFooNumbers* fooNumbers = lua_tocppobject( L, 1, CFooNumbers );
	fooNumbers->Print();
	return 0;
}

void LuaCpp_CFoo(const char*, iberbar::Lua::CClassBuilder* classBuilder)
{
	classBuilder->AddMemberMethod( "Add", &LuaCpp_CFoo_Add );
	classBuilder->AddMemberMethod( "Sub", &LuaCpp_CFoo_Sub );
	classBuilder->AddMemberMethod( "Hello", &LuaCpp_CFoo_Hello );
	classBuilder->AddMemberMethod( "SetNumbers", &LuaCpp_CFoo_SetNumbers );
	classBuilder->AddMemberMethod( "GetNumbers", &LuaCpp_CFoo_GetNumbers );
	classBuilder->AddConstructor( &iberbar::Lua::Class_Unknown_Constructor_New<CFoo> );
	classBuilder->AddDistructor( &iberbar::Lua::Class_Unknown_Distructor_Release<CFoo> );
}

void LuaCpp_CFooNumbers( const char*, iberbar::Lua::CClassBuilder* classBuilder )
{
	classBuilder->AddConstructor( &iberbar::Lua::Class_Unknown_Constructor_New<CFooNumbers> );
	classBuilder->AddDistructor( &iberbar::Lua::Class_Unknown_Distructor_Release<CFooNumbers> );
	classBuilder->AddMemberMethod( "Get", &LuaCpp_CFooNumbers_Get );
	classBuilder->AddMemberMethod( "Set", &LuaCpp_CFooNumbers_Set );
	classBuilder->AddMemberMethod( "BindCallback", &LuaCpp_CFooNumbers_BindCallback );
	classBuilder->AddMemberMethod( "Print", &LuaCpp_CFooNumbers_Print );
}



int main()
{
	

	iberbar::TSmartRefPtr< iberbar::CLuaDevice > luaDevice;
	luaDevice.attach( new iberbar::CLuaDevice() );
	luaDevice->Initial();
	lua_State* pLuaState = luaDevice->GetLuaState();

	lua_pushinteger( pLuaState, 1 );
	lua_pushinteger( pLuaState, 2 );
	lua_pushinteger( pLuaState, 3 );
	lua_pushinteger( pLuaState, 4 );
	lua_pushinteger( pLuaState, 5 );
	lua_pushinteger( pLuaState, 6 );

	lua_rotate( pLuaState, -4, 2 );

	for ( int i = 6; i > 0; i-- )
	{
		printf( "%d\n", (int)lua_tointeger( pLuaState, 0 - i ) );
	}

	//printf( iberbar::LuaCpp::ClassStandardMethod_ToString );

	iberbar::Lua::CBuilder builder( pLuaState );
	builder.ResolveScope( []( iberbar::Lua::CScopeBuilder* scope )
	{
		//luaL_Reg cfs[] = {
		//	{ "TestArgumentsNotMatch", &LuaCFunction_TestArgumentsNotMatch },
		//	{ "JsonDeserialize", &iberbar::LuaCFunction_JsonDeserializeObject },
		//	{ "JsonSerialize", &iberbar::LuaCFunction_JsonSerializeObject },
		//	{ nullptr, nullptr }
		//};
		//scope->AddFunctions( cfs );

		scope->AddClass( uLuaCpp_CFoo_ClassName, &LuaCpp_CFoo );
		scope->AddClass( uLuaCpp_CFooNumbers_ClassName, &LuaCpp_CFooNumbers );
	} );

	builder.ResolveScope( []( iberbar::Lua::CScopeBuilder* scope ) {
		scope->AddClass( uLuaCpp_CFoo_ClassName, &LuaCpp_CFoo );
		scope->AddClass( uLuaCpp_CFooNumbers_ClassName, &LuaCpp_CFooNumbers );
	}, "Biubiu" );

	//luaL_newmetatable( pLuaState, "eeee" );
	//lua_pop( pLuaState, 1 );
	//luaL_getmetatable( pLuaState, "eeee" );
	//lua_setglobal( pLuaState, "eeee_metatable" );

	int top;
	top = lua_gettop( pLuaState );
	lua_newtable( pLuaState );
	int table = lua_gettop( pLuaState );
	lua_pushstring( pLuaState, lr.name );
	//lua_pushlightuserdata( pLuaState, (void*)&lr );
	lua_pushinteger( pLuaState, 555 );
	lua_pushcclosure( pLuaState, lr.func, 1 );
	lua_settable( pLuaState, table );
	top = lua_gettop( pLuaState );
	lua_setglobal( pLuaState, "lrTest" );
	//lua_pop( pLuaState, 1 );
	top = lua_gettop( pLuaState );


	//lua_pushstring( pLuaState, TOLUA_REFID_FUNCTION_MAPPING );
	//lua_newtable( pLuaState );
	//lua_rawset( pLuaState, LUA_REGISTRYINDEX );

	//CWidget* pWidget = new CWidget();


	//pWidget->Print();
	//delete pWidget;

	iberbar::CResult ret = luaDevice->ExecuteFile( "./main.lua" );
	if ( ret.IsOK() == false)
		printf( ret.data.c_str() );

	iberbar::Lua::CFunctionHelper::sExecuteGlobalFunction( luaDevice->GetLuaState(), "Do_1" );
	iberbar::Lua::CFunctionHelper::sExecuteGlobalFunction( luaDevice->GetLuaState(), "Main" );

	return 0;
}
