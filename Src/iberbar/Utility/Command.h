#pragma once

#include <iberbar/Utility/Platform.h>
#include <xmemory>

namespace iberbar
{
	// 游戏命令基类
	// 用于处理一些回调内部
	class __iberbarUtilityApi__ CBaseCommand abstract
	{
	public:
		virtual ~CBaseCommand() {}
		virtual void Execute() = 0;
	};

	struct UCommandQueueOptions
	{
		bool bSync;
	};

	class __iberbarUtilityApi__ CCommandQueue abstract
	{
	public:
		virtual ~CCommandQueue() {}
		virtual void AddCommand( CBaseCommand* pCommand ) = 0;
		virtual void Execute() = 0;
	};




	__iberbarUtilityApi__ CCommandQueue* CreateCommandQueue(
		const UCommandQueueOptions& Options,
		std::pmr::memory_resource* pMemoryRes );

	__iberbarUtilityApi__ void DestroyCommandQueue( CCommandQueue* pQueue );



}



