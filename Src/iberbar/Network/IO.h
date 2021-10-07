#pragma once

#include <iberbar/Network/Headers.h>
#include <iberbar/Utility/Result.h>
#include <string>
#include <functional>
#include <optional>

namespace iberbar
{
	namespace IO
	{
        class ISocketClient;
        struct USocketOptions;

        extern "C" {
            //__iberbarNetworkApis__ CResult Initial();
            //__iberbarNetworkApis__ void Destroy();
            //__iberbarNetworkApis__ void Read();
            //__iberbarNetworkApis__ CResult CreateSocketClient( ISocketClient** ppOutClient, const char* strUrl, const USocketOptions* Options );

            __iberbarNetworkApis__ CResult iberbarNetworkInitial();
            __iberbarNetworkApis__ void iberbarNetworkDestroy();
            __iberbarNetworkApis__ void iberbarNetworkRead();
            CResult CreateSocketClient( ISocketClient** ppOutClient, const char* strUrl, const USocketOptions* Options );
        }
	}
}
