
#include <iberbar/LuaCppApis/Network.h>
#include <iberbar/LuaCppApis/Network_Names.h>
#include <iberbar/Lua/LuaCppBuilder.h>
#include <iberbar/Lua/LuaCallback.h>
#include <iberbar/Lua/LuaCppInstantiateDef.h>
#include <iberbar/Lua/FunctionHelper.h>
#include <iberbar/Lua/LuaError.h>
#include <iberbar/Network/IO.h>
#include <iberbar/Utility/String.h>






namespace iberbar
{
	namespace IO
	{
		int LuaCFunction_CreateSocketClient( lua_State* pLuaState );
		int LuaCppFunction_SocketClient_Listen( lua_State* pLuaState );
		int LuaCppFunction_SocketClient_Send( lua_State* pLuaState );
		int LuaCppFunction_SocketClient_Reconnect( lua_State* pLuaState );
		int LuaCppFunction_SocketClient_Disconnect( lua_State* pLuaState );

		//int LuaCFunction_CreateLogger( lua_State* pLuaState );
		//int LuaCFunction_DestroyLogger( lua_State* pLuaState );
		//int LuaCFunction_DestroyLogger( lua_State* pLuaState );


		void LuaCppCallbackExecute_SocketClient_Listen(
			Lua::PTR_CLuaCallbackHandler pLuaCallback,
			ISocketClient* pSocketClient,
			const char* pData,
			size_t nDataLen );
	}
}





extern const char iberbar::IO::LuaCppNames::s_ModuleName[] = "iberbar.IO";

extern const char iberbar::IO::LuaCppNames::s_Function_CreateSocketClient[] = "CreateSocketClient";
extern const char iberbar::IO::LuaCppNames::s_Function_NewSocketProtocol[] = "NewSocketProtocol";

extern const char iberbar::IO::LuaCppNames::s_ClassName_SocketClient[] = "CSocketClient";
extern const char iberbar::IO::LuaCppNames::s_ClassName_SocketClient_FullName[] = "iberbar.IO.CSocketClient";

extern const char iberbar::IO::LuaCppNames::s_ClassName_SocketProtocolHead[] = "ISocketProtocol";
extern const char iberbar::IO::LuaCppNames::s_ClassName_SocketProtocolHead_FullName[]= "iberbar.IO.ISocketProtocol";





void iberbar::IO::RegisterLuaCpp( lua_State* pLuaState )
{
	LuaCpp::CBuilder builder( pLuaState );
	builder.ResolveScope(
		[]( LuaCpp::CScopeBuilder* pScope )
		{
			pScope->AddFunctionOne( LuaCppNames::s_Function_CreateSocketClient, &LuaCFunction_CreateSocketClient );

			pScope->AddClass( LuaCppNames::s_ClassName_SocketClient,
				[]( const char*, LuaCpp::CClassBuilder* pClass )
				{
					pClass->AddDistructor( &LuaCpp::Class_Unknown_Distructor_Release<ISocketClient> );
					pClass->AddMemberMethod( "Listen", &LuaCppFunction_SocketClient_Listen );
					pClass->AddMemberMethod( "Send", &LuaCppFunction_SocketClient_Send );
					pClass->AddMemberMethod( "Reconnect", &LuaCppFunction_SocketClient_Reconnect );
					pClass->AddMemberMethod( "Disconnect", &LuaCppFunction_SocketClient_Disconnect );
				}
			);

			pScope->AddClass( LuaCppNames::s_ClassName_SocketProtocolHead, []( const char*, LuaCpp::CClassBuilder* pClass ) {} );
		}, LuaCppNames::s_ModuleName
	);
}





int iberbar::IO::LuaCFunction_CreateSocketClient( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	if ( top != 3 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 3 );

	const char* strUrl = lua_tostring( pLuaState, 1 );
	if ( StringIsNullOrEmpty( strUrl ) == true )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	size_t nHeadLen = 0;
	const char* pHead = lua_tolstring( pLuaState, 2, &nHeadLen );

	size_t nTailLen = 0;
	const char* pTail = lua_tolstring( pLuaState, 3, &nTailLen );

	if ( pHead == nullptr || nHeadLen == 0 || pTail == nullptr || nTailLen == 0 )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	USocketOptions SocketOpts;
	SocketOpts.bEnableRead = true;
	SocketOpts.bEnableWrite = true;
	SocketOpts.pHead = pHead;
	SocketOpts.pTail = pTail;
	SocketOpts.nHeadLen = nHeadLen;
	SocketOpts.nTailLen = nTailLen;

	TSmartRefPtr<ISocketClient> pSocketClient = nullptr;
	CResult ret = CreateSocketClient( &pSocketClient, strUrl, SocketOpts );

	if ( ret.IsOK() == false || pSocketClient == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	lua_pushcppref( pLuaState, LuaCppNames::s_ClassName_SocketClient_FullName, pSocketClient );

	return 1;
}






int iberbar::IO::LuaCppFunction_SocketClient_Listen( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments2( pLuaState, top, 2, 3 );

	ISocketClient* pSocketClient = lua_tocppobject( pLuaState, 1, ISocketClient );
	if ( pSocketClient == nullptr )
		return 0;

	lua_Integer nFunc = lua_toluacallback( pLuaState, 2 );
	lua_Integer nExt = 0;
	if ( nFunc == 0 )
		return 0;

	if ( top == 3 )
	{
		nExt = lua_toluacallback_extparam( pLuaState, 3 );
	}

	iberbar::Lua::PTR_CLuaCallbackHandler pLuaCallback = Lua::PTR_CLuaCallbackHandler::_sNew( pLuaState, nFunc, nExt );

	pSocketClient->Listen(
		[=]( ISocketClient* pSocketClientTemp, const char* pData, size_t nDataLen )
		{
			LuaCppCallbackExecute_SocketClient_Listen( pLuaCallback, pSocketClientTemp, pData, nDataLen );
		}
	);

	return 0;
}


int iberbar::IO::LuaCppFunction_SocketClient_Send( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	if ( top != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	ISocketClient* pSocketClient = lua_tocppobject( pLuaState, 1, ISocketClient );
	if ( pSocketClient == nullptr )
		return 0;

	size_t nLen = 0;
	const char* pData = lua_tolstring( pLuaState, 2, &nLen );
	if ( pData == nullptr || nLen == 0 )
		return 0;

	pSocketClient->Send( pData, nLen );

	return 0;
}


int iberbar::IO::LuaCppFunction_SocketClient_Reconnect( lua_State* pLuaState )
{
	return 0;
}


int iberbar::IO::LuaCppFunction_SocketClient_Disconnect( lua_State* pLuaState )
{
	return 0;
}






void iberbar::IO::LuaCppCallbackExecute_SocketClient_Listen(
	Lua::PTR_CLuaCallbackHandler pLuaCallback,
	ISocketClient* pSocketClient,
	const char* pData,
	size_t nDataLen )
{
	if ( pLuaCallback->PushFunction() )
	{
		lua_State* pLuaState = pLuaCallback->GetLuaState();

		int nArg = 2;
		if ( pLuaCallback->HasExt() )
		{
			pLuaCallback->PushExt();
			nArg++;
		}

		lua_pushcppref( pLuaState, LuaCppNames::s_ClassName_SocketClient_FullName, pSocketClient );
		lua_pushlstring( pLuaState, pData, nDataLen );

		Lua::CFunctionHelper::sExecuteFunction( pLuaState, nArg );
	}
}

