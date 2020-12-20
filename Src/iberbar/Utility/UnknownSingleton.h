

//////////////////////////////////////////////////////////////////////////
//
// iberbar\Base\UnknownSingleton.h
//
// 该头文件中的内容只有在宏 _DEBUG 存在时才有效（即Debug模式下）
//////////////////////////////////////////////////////////////////////////
#ifndef __iberbar__Base__UnknownSingleton_h__
#define __iberbar__Base__UnknownSingleton_h__

#include "iberbar\Base\Unknown.h"

#ifdef _DEBUG



namespace iberbar
{
#ifndef IBERBAR_DEBUG_METHOD
#define IBERBAR_DEBUG_METHOD( func )       Debug_##func
#endif

	// 在Unknown单例被创建后添加到管理器中
	// 宏 IBERBAR_DECLARE_SINGLETON_CLASS 已经自动添加到管理器中
	void Debug_AddUnknownSingleton( CUnknown* singleton );

	// 在Unknown单例被释放时从管理器中移除掉
	// 宏 IBERBAR_DECLARE_SINGLETON_CLASS 已经自动从管理器中移除
	void Debug_RemoveUnknownSingleton( CUnknown* singleton );

	// 在程序结束前调用该函数检查Unknown单例是否被释放掉
	void Debug_CheckUnknownSingleton();






}

#else

#ifndef IBERBAR_DEBUG_METHOD
#define IBERBAR_DEBUG_METHOD( func )
#endif

#endif


#define IBERBAR_DECLARE_SINGLETON_INSTANCE( type )         static PTR_##type sm_pShared
#define IBERBAR_DECLARE_SINGLETON_INSTANCE_GET( type )     static PTR_##type GetShared()        { assert( sm_pShared ); return sm_pShared; }
#define IBERBAR_DECLARE_SINGLETON_INSTANCE_CREATE( type )  static PTR_##type CreateShared()     { assert( sm_pShared == NULL ); sm_pShared = PTR_##type::_sNew(); IBERBAR_DEBUG_METHOD( AddUnknownSingleton )( sm_pShared ); return sm_pShared; }
#define IBERBAR_DECLARE_SINGLETON_INSTANCE_DELETE( type )  static void       DeleteShared()     { IBERBAR_DEBUG_METHOD( RemoveUnknownSingleton )( sm_pShared ); sm_pShared = NULL; }
#define IBERBAR_DECLARE_SINGLETON_CLASS( type )  \
	private:\
		IBERBAR_DECLARE_SINGLETON_INSTANCE( type ); \
	public: \
		IBERBAR_DECLARE_SINGLETON_INSTANCE_CREATE( type ); \
		IBERBAR_DECLARE_SINGLETON_INSTANCE_GET( type ) \
		IBERBAR_DECLARE_SINGLETON_INSTANCE_DELETE( type )

#endif