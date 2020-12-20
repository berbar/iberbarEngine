
#pragma once

#ifdef _WIN32

#include <Windows.h>
#include <assert.h>
#include <regex>
#include <tchar.h>

#endif

//#define __iberbarPlatform_Unknown__  0
//#define __iberbarPlatform_Win32__    1
//#define __iberbarPlatform_Android__  2
//#define __iberbarPlatform_Ios__      3
//
//#ifdef _WIN32
//#undef __iberbarPlatformTarget__
//#define __iberbarPlatformTarget__ __iberbarPlatform_Win32__
//#endif
//
//#ifdef __ANDROID__
//#undef __iberbarPlatformTarget__
//#define __iberbarPlatformTarget__ __iberbarPlatformAndroid__
//#endif


#ifdef __ANDROID__

#include <assert.h>
#include <jni.h>

#define abstract
#define override
#define memcpy_s( dst, dstSize, src, srcSize ) memcpy( dst, src, srcSize )

#ifdef UNICODE
#define TEXT(a) L##a
#else
#define TEXT(a) a
#endif

#define FORCEINLINE __inline

#endif



#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef GET_NAME
#define GET_NAME(x) #x
#endif


#include <stdint.h>
#include <string>
#include <vector>
#include <list>


typedef signed char        int8;
typedef short              int16;
typedef int                int32;
typedef long long          int64;
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;

typedef unsigned char      byte;

typedef char CHAR;
typedef wchar_t WCHAR;


#ifdef UNICODE

typedef wchar_t   TCHAR;
typedef wchar_t   tchar;
#define tstrcmp   wcscmp
#define tstrlen   wcslen
#define _tstr( str )   L##str

#else


typedef char         TCHAR;
typedef char tchar;
#define tstrcmp   strcmp
#define tstrlen   strlen
#define _tstr( str )   str

#endif

namespace std
{
#ifdef UNICODE
	typedef wstring tstring;
#else
	typedef string tstring;
#endif
}





#ifndef SAFE_DELETE
#define SAFE_DELETE(p)          { if((p)!=nullptr) { delete (p); (p)=nullptr; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)    { if((p)!=nullptr) { delete[] (p); (p)=nullptr; } }
#endif




#define IBERBAR_EXTERN_SINGLETON( type )           iberbar::type##Ptr iberbar::type::sm_pSharedInstance = NULL;

// #define IBERBAR_DECLARE_SINGLETON( type ) \
// 	public: \
// static void      SetSharedInstance( type##Ptr instance )        { sm_pSharedInstance = instance; } \
// static type##Ptr GetSharedInstance()                                    { assert( sm_pSharedInstance ); return sm_pSharedInstance; } \
// 	private: \
// static type##Ptr sm_pSharedInstance;


// #define IBERBAR_DECLARE_SINGLETON_INSTANCE( type )         static PTR_##type sm_pSharedInstance
// #define IBERBAR_DECLARE_SINGLETON_INSTANCE_GET( type )     static PTR_##type GetSharedInstance()        { assert( sm_pSharedInstance ); return sm_pSharedInstance; }
// #define IBERBAR_DECLARE_SINGLETON_INSTANCE_SET( type )     static PTR_##type CreateSharedInstance()     { assert( sm_pSharedInstance == NULL ); sm_pSharedInstance = PTR_##type::_sNew(); return sm_pSharedInstance; }
// #define IBERBAR_DECLARE_SINGLETON_INSTANCE_RELEASE( type ) static void      ReleaseSharedInstance()    { sm_pSharedInstance = NULL; }
// #define IBERBAR_DECLARE_SINGLETON_CLASS( type )  \
// 	private:IBERBAR_DECLARE_SINGLETON_INSTANCE( type ); \
// 	public:IBERBAR_DECLARE_SINGLETON_INSTANCE_SET( type ); \
// 	IBERBAR_DECLARE_SINGLETON_INSTANCE_GET( type ) \
// 	IBERBAR_DECLARE_SINGLETON_INSTANCE_RELEASE( type )
// 
// #define IBERBAR_SINGLETON_SET( type, object )              type::CreateSharedInstance()
// #define IBERBAR_SINGLETON_GET( type )                      type::GetSharedInstance()
// #define IBERBAR_SINGLETON_RELEASE( type )                  type::ReleaseSharedInstance( NULL )
// #define IBERBAR_SINGLETON_EXTERN( type )                   iberbar::PTR_##type iberbar::type::sm_pSharedInstance = NULL;



//#ifdef WIN32
//
//#	ifdef _LIB
//#		define IBERBAR_DLL
//#	endif
//
//#	ifdef IBERBAR_DLL_LINK
//#		ifdef _USRDLL
//#			define IBERBAR_DLL  _declspec( dllexport )
//#		else
//#			define IBERBAR_DLL  _declspec( dllimport )
//#		endif
//#	else
//#		define IBERBAR_DLL
//#	endif
//
//#endif


//#ifndef IBERBAR_EXPORTS
//#if defined ( _WINDOWS ) && ( IBERBAR_API_EXPORTS )
//#define IBERBAR_EXPORTS _declspec( dllexport )
//#endif
//#endif
//
//
//#ifndef IBERBAR_EXPORTS
//#define IBERBAR_EXPORTS
//#endif
//
//#define __iberbarExports__ IBERBAR_EXPORTS

//#ifdef ANDROID
//#define CALLBACK
//#define nullptr 0
//#endif


#ifndef __iberbarExports__
#	if defined ( _WINDOWS )
#		if defined ( __iberbarDll__ )
#			define __iberbarExports__ __declspec(dllexport)
#		else
#			define __iberbarExports__
#		endif
#	else
#		define __iberbarExports__
#	endif
#endif

#ifndef __iberbarUtilityApi__
#	if defined ( _WINDOWS )
#		if defined ( __iberbarUtilityApiExport__ )
#			define __iberbarUtilityApi__ __declspec(dllexport)
#		else
#			define __iberbarUtilityApi__ __declspec(dllimport)
#		endif
#	else
#		define __iberbarUtilityApi__
#	endif
#endif


#define __iberbarExportTemplateRef( api, cls )  template class api TSmartRefPtr<cls>
#define __iberbarExportTemplateStdVector( api, cls ) \
 	template class api std::allocator<cls>; \
	template class api std::vector<cls>;



