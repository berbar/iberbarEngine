#include "TestNetwork.h"

#include <event.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "TestNetwork", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "TestNetwork", __VA_ARGS__))

extern "C" {
	/*此简单函数返回平台 ABI，此动态本地库为此平台 ABI 进行编译。*/
	const char * TestNetwork::getPlatformABI()
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

	void TestNetwork()
	{
	}

	TestNetwork::TestNetwork()
	{
	}

	TestNetwork::~TestNetwork()
	{
	}
}


struct event ev;
struct timeval tv;

void timer_cb( int fd, short event, void* arg )    //回调函数
{
	LOGI( "timer_cb\n" );
	event_add( &ev, &tv );    //重新注册
}


extern "C"
{
	JNIEXPORT void JNICALL Java_com_TestNetworkApp_TestNetworkApp_NativeTestNetwork( JNIEnv* env, jobject obj )
	{
		LOGI( "yes, test network" );

		struct event_base* base = event_init();  //初始化libevent库
		tv.tv_sec = 1;
		tv.tv_usec = 0;

		event_set( &ev, -1, 0, timer_cb, NULL );  //初始化event结构中成员
		event_base_set( base, &ev );
		event_add( &ev, &tv );  //将event添加到events事件链表，注册事件
		event_base_dispatch( base );  //循环、分发事件
	}
}
