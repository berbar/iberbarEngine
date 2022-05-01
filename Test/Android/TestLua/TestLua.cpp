#include "TestLua.h"

#include <iberbar/Lua/LuaDevice.h>
#include <iberbar/Lua/LuaCppBuilder.h>
#include <iberbar/Utility/Platform/Android/JniHelper.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "TestLua", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "TestLua", __VA_ARGS__))

extern "C" {
	/*此简单函数返回平台 ABI，此动态本地库为此平台 ABI 进行编译。*/
	const char * TestLua::getPlatformABI()
	{
	#if defined(__arm__)
	#if defined(__ARM_ARCH_7A__)
	#if defined(__ARM_NEON__)
		#define ABI "armeabi-v7a/NEON"
	#else
		#define ABI "armeabi-v7a"
	#endif
	#else
		#define ABI "armeabi"
	#endif
	#elif defined(__i386__)
		#define ABI "x86"
	#else
		#define ABI "unknown"
	#endif
		LOGI("This dynamic shared library is compiled with ABI: %s", ABI);
		return "This native library is compiled with ABI: %s" ABI ".";
	}

	void TestLua()
	{
	}

	TestLua::TestLua()
	{
	}

	TestLua::~TestLua()
	{
	}
}



iberbar::CLuaDevice* g_pLuaDevice = nullptr;



int LuaCFunction_LogCat( lua_State* pLuaState )
{
	if ( lua_gettop( pLuaState ) == 2 )
	{
		const char* tag = lua_tolstring( pLuaState, 1, nullptr );
		const char* txt = lua_tolstring( pLuaState, 2, nullptr );
		__android_log_print( ANDROID_LOG_INFO, tag, txt );
	}
	
	return 0;
}


void LuaScopeTest( iberbar::LuaCpp::CScopeBuilder* scope )
{
	scope->AddFunctionOne( "LogCat", &LuaCFunction_LogCat );
}

const char strLuaExecuteText[] = R"(
	LogCat( "TestLua", "123456" );
)";


extern "C"
{
	JNIEXPORT void JNICALL Java_com_TestLuaApp_TestLuaApp_NativeTestLua( JNIEnv* env, jobject obj )
	{
		g_pLuaDevice = new iberbar::CLuaDevice();
		g_pLuaDevice->Initial();

		iberbar::LuaCpp::CBuilder LuaCppBuilder( g_pLuaDevice->GetLuaState() );
		LuaCppBuilder.ResolveScope( &LuaScopeTest );

		g_pLuaDevice->ExecuteSource( strLuaExecuteText );
	}


	JNIEXPORT void JNICALL Java_com_TestLuaApp_TestLuaApp_NativeTestLuaDestroy( JNIEnv* env, jobject obj )
	{
		g_pLuaDevice->Release();
		g_pLuaDevice = nullptr;
	}
}


