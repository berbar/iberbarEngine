
#include <iberbar/LuaCppApis/Game.h>
#include <iberbar/LuaCppApis/Game_Names.h>
#include <iberbar/GameEngine/Application.h>
#include <iberbar/GameEngine/Paper2dLoader.h>
#include <iberbar/GameEngine/BaseResourceManager.h>
#include <iberbar/GameEngine/TextureManager.h>
#include <iberbar/GameEngine/FontManager.h>
#include <iberbar/GameEngine/Timer.h>
#include <iberbar/GameEngine/LoadingThread.h>
#include <iberbar/Paper2d/Animation.h>
#include <iberbar/Lua/LuaError.h>
#include <iberbar/Lua/LuaCppBuilder.h>
#include <iberbar/Lua/LuaCppInstantiateDef.h>
#include <iberbar/Lua/LuaCallback.h>
#include <iberbar/Lua/FunctionHelper.h>
#include <iberbar/Utility/Log/OutputDevice.h>
#include <iberbar/Utility/String.h>




namespace iberbar
{
	namespace Game
	{
		//int LuaCFunction_GetGuiEngine( lua_State* pLuaState );
		int LuaCFunction_GetGuiXmlParser( lua_State* pLuaState );
		int LuaCFunction_GetTexture( lua_State* pLuaState );
		int LuaCFunction_GetFont( lua_State* pLuaState );
		int LuaCFunction_GetResourcePath( lua_State* pLuaState );
		int LuaCFunction_Paper2dLoadAnimations( lua_State* pLuaState );
		int LuaCFunction_Paper2dLoadGridTerrain( lua_State* pLuaState );


		int LuaCFunction_CreateTimer( lua_State* pLuaState );
		int LuaCFunction_DestroyTimer( lua_State* pLuaState );
		int LuaCFunction_FindTimer( lua_State* pLuaState );

		int LuaCppFunction_Timer_Start( lua_State* pLuaState );
		int LuaCppFunction_Timer_Stop( lua_State* pLuaState );
		int LuaCppFunction_Timer_Resume( lua_State* pLuaState );
		int LuaCppFunction_Timer_Pause( lua_State* pLuaState );
		int LuaCppFunction_Timer_Scale( lua_State* pLuaState );
		int LuaCppFunction_Timer_SetCallbackExecute( lua_State* pLuaState );
		int LuaCppFunction_Timer_SetId( lua_State* pLuaState );
		int LuaCppFunction_Timer_GetId( lua_State* pLuaState );
		void LuaCppCallbackExecute_Timer_Execute( Lua::PTR_CLuaCallbackHandler pLuaCallback, CTimer* pTimer );


		int LuaCFunction_AddLoadingTask( lua_State* pLuaState );
		int LuaCFunction_WakeupLoadingThread( lua_State* pLuaState );


		int LuaCppFunction_LoadingTask_SetTaskName( lua_State* pLuaState );


		int LuaCppFunction_LoadingTaskTextures_SetSleepFor( lua_State* pLuaState );
		int LuaCppFunction_LoadingTaskTextures_AddTexture( lua_State* pLuaState );
		int LuaCppFunction_LoadingTaskTextures_SetCallbackOnStep( lua_State* pLuaState );
		int LuaCppFunction_LoadingTaskTextures_SetCallbackOnFinal( lua_State* pLuaState );
		void LuaCppCallbackExecute_LoadingTaskTextures_OnStep_Execute(
			Lua::PTR_CLuaCallbackHandler pLuaCallback,
			const std::string& strStepName, int nProgressTotal, int nProgressIndex, bool nLoadedRet, const std::string& strLoadedError );
		void LuaCppCallbackExecute_LoadingTaskTextures_OnFinal_Execute( Lua::PTR_CLuaCallbackHandler pLuaCallback );


		int LuaCppFunction_LoadingTaskKeyTask_SetCallback( lua_State* pLuaState );
		void LuaCppCallbackExecute_LoadingTaskKeyTask( Lua::PTR_CLuaCallbackHandler pLuaCallback );


		template < Logging::ULevel tLevel >
		int LuaCFunction_Log( lua_State* pLuaState );
		static const luaL_Reg s_LuaReg_Logs[] = {
			{ "LogTrace", &LuaCFunction_Log<Logging::ULevel::Trace> },
			{ "LogDebug", &LuaCFunction_Log<Logging::ULevel::Debug> },
			{ "LogInfo", &LuaCFunction_Log<Logging::ULevel::Info> },
			{ "LogWarn", &LuaCFunction_Log<Logging::ULevel::Warn> },
			{ "LogError", &LuaCFunction_Log<Logging::ULevel::Error> },
			{ "LogCritical", &LuaCFunction_Log<Logging::ULevel::Critical> },
			{ "LogFatal", &LuaCFunction_Log<Logging::ULevel::Fatal> },
			{ nullptr, nullptr }
		};
	}
}


extern const char iberbar::Game::s_ClassName_Timer[] = "CTimer";
extern const char iberbar::Game::s_ClassName_Timer_FullName[] = "iberbar.Game.CTimer";

extern const char iberbar::Game::s_ClassName_LoadingTask[] = "CLoadingTask";
extern const char iberbar::Game::s_ClassName_LoadingTask_FullName[] = "iberbar.Game.CLoadingTask";

extern const char iberbar::Game::s_ClassName_LoadingTaskTextures[] = "CLoadingTask_Textures";
extern const char iberbar::Game::s_ClassName_LoadingTaskTextures_FullName[] = "iberbar.Game.CLoadingTask_Textures";

extern const char iberbar::Game::s_ClassName_LoadingTaskKeyTask[] = "CLoadingTask_KeyTask";
extern const char iberbar::Game::s_ClassName_LoadingTaskKeyTask_FullName[] = "iberbar.Game.CLoadingTask_KeyTask";



void iberbar::Game::RegisterLuaCpp( lua_State* pLuaState )
{
	LuaCpp::CBuilder builder( pLuaState );
	builder.ResolveScope( []( LuaCpp::CScopeBuilder* scope )
		{
			//scope->AddFunctionOne( "GetGuiEngine", &LuaCFunction_GetGuiEngine );
			scope->AddFunctionOne( "GetGuiXmlParser", &LuaCFunction_GetGuiXmlParser );
			scope->AddFunctionOne( "GetTexture", &LuaCFunction_GetTexture );
			scope->AddFunctionOne( "GetFont", &LuaCFunction_GetFont );
			scope->AddFunctionOne( "GetResourcePath", LuaCFunction_GetResourcePath );

			scope->AddFunctionOne( "Paper2dLoadAnimations", &LuaCFunction_Paper2dLoadAnimations );
			scope->AddFunctionOne( "Paper2dLoadGridTerrain", &LuaCFunction_Paper2dLoadGridTerrain );

			scope->AddFunctionOne( "AddLoadingTask", &LuaCFunction_AddLoadingTask );
			scope->AddFunctionOne( "WakeupLoadingThread", &LuaCFunction_WakeupLoadingThread );

			scope->AddFunctionOne( "CreateTimer", &LuaCFunction_CreateTimer );
			scope->AddFunctionOne( "DestroyTimer", &LuaCFunction_DestroyTimer );
			scope->AddFunctionOne( "FindTimer", &LuaCFunction_FindTimer );
			scope->AddClass( s_ClassName_Timer,
				[]( const char*, LuaCpp::CClassBuilder* pClass )
				{
					pClass->AddMemberMethod( "Start", &LuaCppFunction_Timer_Start );
					pClass->AddMemberMethod( "Stop", &LuaCppFunction_Timer_Stop );
					pClass->AddMemberMethod( "Resume", &LuaCppFunction_Timer_Resume );
					pClass->AddMemberMethod( "Pause", &LuaCppFunction_Timer_Pause );
					pClass->AddMemberMethod( "Scale", &LuaCppFunction_Timer_Scale );
					pClass->AddMemberMethod( "SetCallbackExecute", &LuaCppFunction_Timer_SetCallbackExecute );
					pClass->AddMemberMethod( "SetId", &LuaCppFunction_Timer_SetId );
					pClass->AddMemberMethod( "GetId", &LuaCppFunction_Timer_GetId );
					pClass->AddDistructor( &LuaCpp::Class_Unknown_Distructor_Release<CTimer> );
				}
			);

			scope->AddClass( s_ClassName_LoadingTask,
				[]( const char*, LuaCpp::CClassBuilder* pClass )
				{
					pClass->AddDistructor( &LuaCpp::Class_Unknown_Distructor_Release<CLoadingTask> );
					pClass->AddMemberMethod( "SetTaskName", &LuaCppFunction_LoadingTask_SetTaskName );
				} );

			scope->AddClass( s_ClassName_LoadingTaskTextures,
				[]( const char*, LuaCpp::CClassBuilder* pClass )
				{
					pClass->AddConstructor( &LuaCpp::Class_Unknown_Constructor_New<CLoadingTask_Textures> );
					pClass->AddDistructor( &LuaCpp::Class_Unknown_Distructor_Release<CLoadingTask_Textures> );
					pClass->AddMemberMethod( "SetSleepFor", &LuaCppFunction_LoadingTaskTextures_SetSleepFor );
					pClass->AddMemberMethod( "AddTexture", &LuaCppFunction_LoadingTaskTextures_AddTexture );
					pClass->AddMemberMethod( "SetCallbackOnStep", &LuaCppFunction_LoadingTaskTextures_SetCallbackOnStep );
					pClass->AddMemberMethod( "SetCallbackOnFinal", &LuaCppFunction_LoadingTaskTextures_SetCallbackOnFinal );
				}, s_ClassName_LoadingTask_FullName );

			scope->AddClass( s_ClassName_LoadingTaskKeyTask,
				[]( const char*, LuaCpp::CClassBuilder* pClass )
				{
					pClass->AddConstructor( &LuaCpp::Class_Unknown_Constructor_New<CLoadingTask_KeyTask> );
					pClass->AddDistructor( &LuaCpp::Class_Unknown_Distructor_Release<CLoadingTask_KeyTask> );
					pClass->AddMemberMethod( "SetCallback", &LuaCppFunction_LoadingTaskKeyTask_SetCallback );
				}, s_ClassName_LoadingTask_FullName );

			scope->AddFunctions( s_LuaReg_Logs );
		},
		"iberbar.Game" );

	builder.ResolveScope(
		[]( LuaCpp::CScopeBuilder* scope )
		{
			scope->AddClass( s_ClassName_Font,
				[]( const char*, LuaCpp::CClassBuilder* pClass )
				{
					pClass->AddDistructor( &LuaCpp::Class_Unknown_Distructor_Release<Renderer::CFont> );
				}
			);
		}, "iberbar.Renderer"
	);
}


//int iberbar::Game::LuaCFunction_GetGuiEngine( lua_State* pLuaState )
//{
//	auto pEngine = GetApp()->GetGuiEngine();
//	lua_pushcppobject( pLuaState, "iberbar.Gui.CEngine", pEngine );
//	return 1;
//}


int iberbar::Game::LuaCFunction_GetGuiXmlParser( lua_State* pLuaState )
{
	auto pEngine = GetApp()->GetGuiXmlParser();
	lua_pushcppobject( pLuaState, "iberbar.Gui.CXmlParser", pEngine );
	return 1;
}


int iberbar::Game::LuaCFunction_GetTexture( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	const char* strFile = lua_tostring( pLuaState, 1 );
	if ( StringIsNullOrEmpty( strFile ) )
	{
		lua_pushnil( pLuaState );
	}
	else
	{
		TSmartRefPtr<RHI::ITexture> pTexture = nullptr;
		CResult ret = GetApp()->GetTextureManager()->GetOrCreateTextureA( strFile, &pTexture );
		if ( ret.IsOK() == false )
		{
			lua_pushnil( pLuaState );
		}
		else
		{
			lua_pushcppref( pLuaState, "iberbar.Rhi.CTexture", pTexture );
		}
	}

	return 1;
}


int iberbar::Game::LuaCFunction_GetFont( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments3( pLuaState, top, 0, 2, 3 );

	TSmartRefPtr<Renderer::CFont> pFont = nullptr;
	bool ret = false;
	if ( top == 0 )
	{
		ret = GetApp()->GetFontManager()->GetFontDefault( &pFont );
	}
	else
	{
		if ( lua_isnil( pLuaState, 1 ) )
		{
			lua_pushnil( pLuaState );
			return 1;
		}

		UFontDesc FontDesc;
		FontDesc.FamilyName = lua_tostring( pLuaState, 1 );
		FontDesc.Size = (int)lua_tointeger( pLuaState, 2 );
		if ( top >= 3 )
			FontDesc.Weight = (int)lua_tointeger( pLuaState, 3 );
		ret = GetApp()->GetFontManager()->GetFont( &pFont, FontDesc );
	}

	if ( ret == false )
	{
		lua_pushnil( pLuaState );
	}
	else
	{
		lua_pushcppref( pLuaState, s_ClassName_Font_FullName, pFont );
	}


	return 1;
}


int iberbar::Game::LuaCFunction_GetResourcePath( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	const char* str = lua_tostring( pLuaState, 1 );
	std::string strPathNew = CResourceFileSystem::GetResoucePath( str );
	lua_pushstring( pLuaState, strPathNew.c_str() );
	return 1;
}


int iberbar::Game::LuaCFunction_Paper2dLoadAnimations( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	const char* strFile = lua_tostring( pLuaState, 1 );
	if ( StringIsNullOrEmpty( strFile ) == true )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	std::vector<Paper2d::CAnimationController*> AnimList;
	CResult ret = CPaper2dLoader::GetShared()->LoadAnimations( strFile, AnimList );
	if ( ret.IsOK() == false )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	lua_newtable( pLuaState );
	int nLuaTable = lua_gettop( pLuaState );
	for ( size_t i = 0, s = AnimList.size(); i < s; i++ )
	{
		lua_pushcppref( pLuaState, "iberbar.Paper2d.CAnimationController", AnimList[ i ] );
		lua_rawseti( pLuaState, -2, (lua_Integer)i + 1 );
		UNKNOWN_SAFE_RELEASE_NULL( AnimList[ i ] );
	}
	AnimList.clear();

	return 1;
}


int iberbar::Game::LuaCFunction_Paper2dLoadGridTerrain( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	const char* strFile = lua_tostring( pLuaState, 1 );
	if ( StringIsNullOrEmpty( strFile ) == true )
		return 0;

	Paper2d::CGridTerrain* pTerrain = lua_tocppobject( pLuaState, 2, Paper2d::CGridTerrain );
	if ( pTerrain == nullptr )
		return 0;

	CResult ret = CPaper2dLoader::GetShared()->LoadGridTerrain( strFile, pTerrain );
	if ( ret.IsOK() == false )
	{
		//lua_pushnil( pLuaState );
		return 0;
	}

	return 0;
}









int iberbar::Game::LuaCFunction_CreateTimer( lua_State* pLuaState )
{
	TSmartRefPtr<CTimer> pTimer = TSmartRefPtr<CTimer>::_sNew();
	GetApp()->GetTimerSystem()->AddTimer( pTimer );
	lua_pushcppref( pLuaState, s_ClassName_Timer_FullName, pTimer );
	return 1;
}


int iberbar::Game::LuaCFunction_DestroyTimer( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 1 );

	CTimer* pTimer = lua_tocppobject( pLuaState, 1, CTimer );
	if ( pTimer == nullptr )
	{
		lua_pushboolean( pLuaState, 0 );
		return 1;
	}

	GetApp()->GetTimerSystem()->RemoveTimer( pTimer );

	lua_pushboolean( pLuaState, 1 );
	return 1;
}


int iberbar::Game::LuaCFunction_FindTimer( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 1 );

	const char* strId = lua_tostring( pLuaState, 1 );
	if ( StringIsNullOrEmpty( strId ) )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	TSmartRefPtr<CTimer> pTimer = nullptr;

	if ( GetApp()->GetTimerSystem()->FindTimer( strId, &pTimer ) == false && pTimer == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	lua_pushcppref( pLuaState, s_ClassName_Timer_FullName, pTimer );
	return 1;
}


int iberbar::Game::LuaCppFunction_Timer_Start( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 3 );

	CTimer* pTimer = lua_tocppobject_safe( pLuaState, 1, CTimer, s_ClassName_Timer_FullName );
	if ( pTimer == nullptr )
	{
		return 0;
	}

	float nTimeout = (float)lua_tonumber( pLuaState, 2 );
	bool bRepeat = (bool)lua_toboolean( pLuaState, 3 );
	pTimer->Start( nTimeout, bRepeat );

	return 0;
}


int iberbar::Game::LuaCppFunction_Timer_Stop( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 1 );

	CTimer* pTimer = lua_tocppobject_safe( pLuaState, 1, CTimer, s_ClassName_Timer_FullName );
	if ( pTimer == nullptr )
	{
		return 0;
	}

	pTimer->Stop();

	return 0;
}


int iberbar::Game::LuaCppFunction_Timer_Resume( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 1 );

	CTimer* pTimer = lua_tocppobject_safe( pLuaState, 1, CTimer, s_ClassName_Timer_FullName );
	if ( pTimer == nullptr )
	{
		return 0;
	}

	pTimer->Resume();

	return 0;
}


int iberbar::Game::LuaCppFunction_Timer_Pause( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 1 );

	CTimer* pTimer = lua_tocppobject_safe( pLuaState, 1, CTimer, s_ClassName_Timer_FullName );
	if ( pTimer == nullptr )
	{
		return 0;
	}

	pTimer->Pause();

	return 0;
}


int iberbar::Game::LuaCppFunction_Timer_Scale( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CTimer* pTimer = lua_tocppobject_safe( pLuaState, 1, CTimer, s_ClassName_Timer_FullName );
	if ( pTimer == nullptr )
	{
		return 0;
	}

	float nScale = (float)lua_tonumber( pLuaState, 2 );

	pTimer->Scale( nScale );

	return 0;
}


int iberbar::Game::LuaCppFunction_Timer_SetCallbackExecute( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments2( pLuaState, top, 2, 3 );

	CTimer* pTimer = lua_tocppobject_safe( pLuaState, 1, CTimer, s_ClassName_Timer_FullName );
	if ( pTimer == nullptr )
	{
		return 0;
	}

	lua_Integer nFunc = lua_toluacallback( pLuaState, 2 );
	lua_Integer nExt = 0;
	if ( top == 3 )
	{
		nExt = lua_toluacallback_extparam( pLuaState, 3 );
	}

	if ( nFunc == 0 )
		return 0;

	iberbar::Lua::PTR_CLuaCallbackHandler pLuaCallback = iberbar::Lua::PTR_CLuaCallbackHandler::_sNew( pLuaState, nFunc, nExt );

	pTimer->SetCallbackExecute(
		[pLuaCallback]( CTimer* pTimerTemp )
		{
			LuaCppCallbackExecute_Timer_Execute( pLuaCallback, pTimerTemp );
		}
	);

	return 0;
}


int iberbar::Game::LuaCppFunction_Timer_SetId( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CTimer* pTimer = lua_tocppobject_safe( pLuaState, 1, CTimer, s_ClassName_Timer_FullName );
	if ( pTimer == nullptr )
	{
		return 0;
	}

	const char* strId = lua_tostring( pLuaState, 2 );

	pTimer->SetId( strId );

	return 0;
}


int iberbar::Game::LuaCppFunction_Timer_GetId( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CTimer* pTimer = lua_tocppobject_safe( pLuaState, 1, CTimer, s_ClassName_Timer_FullName );
	if ( pTimer == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	lua_pushstring( pLuaState, pTimer->GetId().c_str() );

	return 1;
}


void iberbar::Game::LuaCppCallbackExecute_Timer_Execute( Lua::PTR_CLuaCallbackHandler pLuaCallback, CTimer* pTimer )
{
	if ( pLuaCallback->PushFunction() )
	{
		lua_State* pLuaState = pLuaCallback->GetLuaState();
		int nArg = 1;
		if ( pLuaCallback->HasExt() )
		{
			pLuaCallback->PushExt();
			nArg++;
		}

		lua_pushcppref( pLuaState, s_ClassName_Timer_FullName, pTimer );

		Lua::CFunctionHelper::sExecuteFunction( pLuaState, nArg );
	}
}







int iberbar::Game::LuaCFunction_AddLoadingTask( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 1 );

	CLoadingTask* pTask = lua_tocppobject( pLuaState, 1, CLoadingTask );
	if ( pTask == nullptr )
		return 0;

	GetApp()->AddLoadingTask( pTask );

	return 0;
}


int iberbar::Game::LuaCFunction_WakeupLoadingThread( lua_State* pLuaState )
{
	GetApp()->WakeupLoadingThread();

	return 0;
}





int iberbar::Game::LuaCppFunction_LoadingTask_SetTaskName( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CLoadingTask* pTask = lua_tocppobject( pLuaState, 1, CLoadingTask );
	if ( pTask == nullptr )
		return 0;

	const char* strTaskName = lua_tostring( pLuaState, 2 );
	pTask->SetStepName( strTaskName );

	return 0;
}






int iberbar::Game::LuaCppFunction_LoadingTaskTextures_SetSleepFor( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CLoadingTask_Textures* pTask = lua_tocppobject( pLuaState, 1, CLoadingTask_Textures );
	if ( pTask == nullptr )
		return 0;

	uint32 nMilliseconds = (uint32)lua_tointeger( pLuaState, 2 );
	pTask->SetSleepFor( nMilliseconds );

	return 0;
}


int iberbar::Game::LuaCppFunction_LoadingTaskTextures_AddTexture( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CLoadingTask_Textures* pTask = lua_tocppobject( pLuaState, 1, CLoadingTask_Textures );
	if ( pTask == nullptr )
		return 0;

	const char* strTextureName = lua_tostring( pLuaState, 2 );
	pTask->AddTexture( strTextureName );

	return 0;
}


int iberbar::Game::LuaCppFunction_LoadingTaskTextures_SetCallbackOnStep( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments2( pLuaState, top, 2, 3 );

	CLoadingTask_Textures* pTask = lua_tocppobject( pLuaState, 1, CLoadingTask_Textures );
	if ( pTask == nullptr )
		return 0;

	lua_Integer nFunc = lua_toluacallback( pLuaState, 2 );
	lua_Integer nExt = 0;
	if ( lua_isnoneornil( pLuaState, 3 ) == false )
		nExt = lua_toluacallback_extparam( pLuaState, 3 );

	if ( nFunc == 0 )
		return 0;

	Lua::PTR_CLuaCallbackHandler pLuaCallback = Lua::PTR_CLuaCallbackHandler::_sNew( pLuaState, nFunc, nExt );
	pTask->SetCallbackOnStep(
		[pLuaCallback]( const std::string& strStepName, int nProgressTotal, int nProgressIndex, bool nLoadedRet, const std::string& strLoadedError )
		{
			LuaCppCallbackExecute_LoadingTaskTextures_OnStep_Execute( pLuaCallback, strStepName, nProgressTotal, nProgressIndex, nLoadedRet, strLoadedError );
		}
	);

	return 0;
}


int iberbar::Game::LuaCppFunction_LoadingTaskTextures_SetCallbackOnFinal( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments2( pLuaState, top, 2, 3 );

	CLoadingTask_Textures* pTask = lua_tocppobject( pLuaState, 1, CLoadingTask_Textures );
	if ( pTask == nullptr )
		return 0;

	lua_Integer nFunc = lua_toluacallback( pLuaState, 2 );
	lua_Integer nExt = 0;
	if ( lua_isnoneornil( pLuaState, 3 ) == false )
		nExt = lua_toluacallback_extparam( pLuaState, 3 );

	if ( nFunc == 0 )
		return 0;

	Lua::PTR_CLuaCallbackHandler pLuaCallback = Lua::PTR_CLuaCallbackHandler::_sNew( pLuaState, nFunc, nExt );
	pTask->SetCallbackOnFinal(
		[pLuaCallback]()
		{
			LuaCppCallbackExecute_LoadingTaskTextures_OnFinal_Execute( pLuaCallback );
		}
	);

	return 0;
}


void iberbar::Game::LuaCppCallbackExecute_LoadingTaskTextures_OnStep_Execute(
	Lua::PTR_CLuaCallbackHandler pLuaCallback,
	const std::string& strStepName, int nProgressTotal, int nProgressIndex, bool nLoadedRet, const std::string& strLoadedError )
{
	if ( pLuaCallback->PushFunction() )
	{
		lua_State* pLuaState = pLuaCallback->GetLuaState();
		int nArg = 5;
		if ( pLuaCallback->HasExt() )
		{
			pLuaCallback->PushExt();
			nArg++;
		}

		lua_pushstring( pLuaState, strStepName.c_str() );
		lua_pushinteger( pLuaState, nProgressTotal );
		lua_pushinteger( pLuaState, nProgressIndex );
		lua_pushboolean( pLuaState, nLoadedRet );
		lua_pushstring( pLuaState, strLoadedError.c_str() );

		Lua::CFunctionHelper::sExecuteFunction( pLuaState, nArg );
	}
}


void iberbar::Game::LuaCppCallbackExecute_LoadingTaskTextures_OnFinal_Execute( Lua::PTR_CLuaCallbackHandler pLuaCallback )
{
	if ( pLuaCallback->PushFunction() )
	{
		lua_State* pLuaState = pLuaCallback->GetLuaState();
		int nArg = 0;
		if ( pLuaCallback->HasExt() )
		{
			pLuaCallback->PushExt();
			nArg++;
		}

		Lua::CFunctionHelper::sExecuteFunction( pLuaState, nArg );
	}
}










int iberbar::Game::LuaCppFunction_LoadingTaskKeyTask_SetCallback( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments2( pLuaState, top, 2, 3 );

	CLoadingTask_KeyTask* pTask = lua_tocppobject( pLuaState, 1, CLoadingTask_KeyTask );
	if ( pTask == nullptr )
		return 0;

	lua_Integer nFunc = lua_toluacallback( pLuaState, 2 );
	lua_Integer nExt = 0;
	if ( lua_isnoneornil( pLuaState, 3 ) == false )
		nExt = lua_toluacallback_extparam( pLuaState, 3 );

	if ( nFunc == 0 )
		return 0;

	Lua::PTR_CLuaCallbackHandler pLuaCallback = Lua::PTR_CLuaCallbackHandler::_sNew( pLuaState, nFunc, nExt );
	pTask->SetCallback(
		[pLuaCallback]()
		{
			LuaCppCallbackExecute_LoadingTaskKeyTask( pLuaCallback );
		}
	);

	return 0;
}


void iberbar::Game::LuaCppCallbackExecute_LoadingTaskKeyTask( Lua::PTR_CLuaCallbackHandler pLuaCallback )
{
	if ( pLuaCallback->PushFunction() )
	{
		lua_State* pLuaState = pLuaCallback->GetLuaState();
		int nArg = 0;
		if ( pLuaCallback->HasExt() )
		{
			pLuaCallback->PushExt();
			nArg++;
		}

		Lua::CFunctionHelper::sExecuteFunction( pLuaState, nArg );
	}
}










template < iberbar::Logging::ULevel tLevel >
int iberbar::Game::LuaCFunction_Log( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments2( pLuaState, top, 1, 2 );

	const char* strCategory = nullptr;
	const char* strText = nullptr;
	if ( top == 1 )
	{
		strCategory = "Game";
		strText = lua_tostring( pLuaState, 1 );
	}
	else
	{
		strCategory = lua_tostring( pLuaState, 1 );
		strText = lua_tostring( pLuaState, 2 );
	}
	if ( StringIsNullOrEmpty( strCategory ) || StringIsNullOrEmpty( strText ) )
		return 0;

	GetApp()->GetLoggingOutputDevice()->Serialize( tLevel, strText, strCategory );

	return 0;
}





