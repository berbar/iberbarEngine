#pragma once

#include <iberbar/Utility/Result.h>

struct lua_State;

namespace iberbar
{
	namespace IO
	{
		struct USocketOptions;
		class ISocketProtocol;
        class ISocketClient;

        class IHttpClient;

		typedef CResult ( *PFunctionInitial )( );
		typedef void ( *PFunctionDestroy )( );
		typedef void ( *PFunctionRead )( );
		typedef CResult ( *PFunctionCreateSocketClient )( ISocketClient** ppOutClient, const char* strUrl, const USocketOptions* Options );
		typedef void (*PFunctionLuaCppRegister)( lua_State* pLuaState );
	}
}