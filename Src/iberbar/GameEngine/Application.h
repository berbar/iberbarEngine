#pragma once

#include <iberbar/GameEngine/Headers.h>
#include <iberbar/Utility/Result.h>
#include <iberbar/RHI/Types.h>
#include <thread>
#include <memory_resource>





namespace iberbar
{
	class CLuaDevice;
	class CInput;
	class CGlobalTimer;
	class CCommandQueue;

	namespace OS
	{
		class CDynamicLibrary;
		class CDpiHelper;
	}

	namespace Logging
	{
		class COutputDevice;
	}

	namespace RHI
	{
		class IDevice;
	}

	namespace Renderer
	{
		class CRenderer;
	}

	namespace Gui
	{
		class CEngine;
		class CXmlParser;
	}

	namespace Paper2d
	{
		class CDirector;
	}

	namespace Game
	{
		class CPaper2dLoader;
		class CResourcePreloader;
		class CTextureManager;
		class CShaderLoader;
		class CShaderManager;
		class CFontManager;
		class CTimerEasySystem;

		struct UNetworkProcAddressInfo;

		class CLoadingTask;
		class CLoadingThread;


		struct UApplicationCreateParams
		{
		public:
			RHI::UApiType nRHIApi;
			std::tstring strAppId;
			std::tstring strAppName;
			int nWndWidth;
			int nWndHeight;
			bool bUseLoadingThread;
			bool bUseNetwork;

#ifdef _WIN32
			bool bWindow;
			bool bFullScreen;
#endif
		};

		class __iberbarGameEngineApi__ CApplication
		{
		public:
			typedef void (CApplication::*PCallbackHandleEvent)(int nEventHandleId);

		public:
			CApplication();
			CApplication( const CApplication& app ) = delete;
			virtual ~CApplication();

			// ≥ı ºªØ≈‰÷√
			void SetConfiguration( const UApplicationCreateParams& Configuration ) { assert( m_bInit == false ); m_Configuration = Configuration; }

		protected:
			bool m_bInit;

		protected:
			virtual CResult OnCreated() { return CResult(); }
			virtual void OnDestroy() {}
			virtual void OnResume() {}
			virtual void OnPause() {}
			virtual void OnQuit() {}
			virtual void OnUpdate( int64 nElapsedTimeMilliSecond, float nElapsedTimeSecond ) {}
			virtual void OnRender() {}
			virtual void OnRhiDeviceLost() {}
			virtual CResult OnRhiDeviceReset() { return CResult(); }

#ifdef _WINDOWS
			virtual void OnHandleEvent( int nEventId ) {}
			virtual void OnWndProc( UINT nMsg, WPARAM wParam, LPARAM lParam ) {}
#endif
#ifdef __ANDROID__
			virtual CResult ExportAssetsPreInitial();
#endif

		private:
			void Destroy();
			void Resume();
			void Pause();
			void RhiDeviceLost();
			CResult RhiDeviceReset();
			CResult CreateAll();
			CResult CreateLog();
			CResult CreateRHI();
			CResult CreateNetwork();
			CResult InitDefaultRenderState();
			CResult LoadDefaultShaders();
			void OnRunTimer( int64 nElapsedTimeMilliSecond, float nElapsedTimeSecond );

		
#ifdef _WINDOWS
		public:
			CResult Initial( HINSTANCE hInstance );
			HINSTANCE GetHInstance() { return m_hInstance; }
			HWND GetHWnd() { return m_hWnd; }

		private:
			HINSTANCE m_hInstance;
			HWND m_hWnd;
			HANDLE* m_hEvents;
			uint32 m_nEventCount;
			UINT_PTR m_nSysTimer;

			CResult CreateWnd();
			void CreateHandleEvents();
			void HandleEvent( int nEventIndex );
			static LRESULT CALLBACK sWndProc( HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam );
			static void CALLBACK sTimerProc( HWND hWnd, UINT, UINT_PTR, DWORD);
#endif

#ifdef __ANDROID__
		public:
			CResult SetJNIEnv( JNIEnv* pJNIEnv );
			CResult Initial();

		private:
			static JNIEXPORT void sJNI_nativeInit( JNIEnv* env, jobject obj, jint width, jint height );
			static void sJNI_nativeRender( JNIEnv* env, jobject obj );
			static void sJNI_nativeOnSurfaceChanged( JNIEnv* env, jobject obj, jint width, jint height );
			static void sJNI_nativeOnPause( JNIEnv* env, jobject obj );
			static void sJNI_nativeOnResume( JNIEnv* env, jobject obj );
			static void sJNI_nativeOnDestroy( JNIEnv* env, jobject obj );
			static void sJNI_nativeTouchesBegin( JNIEnv* env, jobject obj, jint id, jfloat x, jfloat y );
			static void sJNI_nativeTouchesEnd( JNIEnv* env, jobject obj, jint id, jfloat x, jfloat y );
			static void sJNI_nativeTouchesMove( JNIEnv* env, jobject obj, jintArray ids, jfloatArray xs, jfloatArray ys );
			static void sJNI_nativeTouchesCancel( JNIEnv* env, jobject obj, jintArray ids, jfloatArray xs, jfloatArray ys );
			static jboolean sJNI_nativeKeyEvent( JNIEnv* env, jobject obj, jint keyCode, jboolean isPressed );
			static JNIEXPORT void sJNI_nativeSetContext( JNIEnv* env, jobject obj, jobject context, jobject assetManager );

		private:
			JNIEnv* m_pJNIEnv;
#endif

		public:
			int Run();
			void Quit();
			void WakeupLoadingThread();
			void AddLoadingTask( CLoadingTask* pTask );

			std::thread::id GetMainThreadId() { return std::this_thread::get_id(); }
			Logging::COutputDevice* GetLoggingOutputDevice() { return m_pLoggingOutputDevice; }
			RHI::IDevice* GetRHIDevice() { return m_pRHIDevice; }
			Gui::CEngine* GetGuiEngine() { return m_pGuiEngine; }
			//Gui::CXmlParser* GetGuiXmlParser() { return m_pGuiXmlParser; }
			CTextureManager* GetTextureManager() { return m_pTextureManager; }
			CFontManager* GetFontManager() { return m_pFontManager; }
			CLuaDevice* GetLuaDevice() { return m_pLuaDevice; }
			CTimerEasySystem* GetTimerSystem() { return m_pTimerEasySystem; }
			OS::CDpiHelper* GetDpiHelper() { return m_pDpiHelper; }

		protected:

			bool m_bWndActive;
			UApplicationCreateParams m_Configuration;

			Logging::COutputDevice* m_pLoggingOutputDevice;

			Gui::CEngine* m_pGuiEngine;
			//Gui::CXmlParser* m_pGuiXmlParser;
			RHI::IDevice* m_pRHIDevice;

			CTextureManager* m_pTextureManager;
			CShaderLoader* m_pShaderLoader;
			CShaderManager* m_pShaderManager;
			CFontManager* m_pFontManager;
			//CPaper2dLoader* m_pPaper2dLoader;

			Renderer::CRenderer* m_pRenderer;

			//Paper2d::CDirector* m_pPaper2dDirector;
			
			CLuaDevice* m_pLuaDevice;

			CGlobalTimer* m_pGlobalTimer;
			CTimerEasySystem* m_pTimerEasySystem;
			CInput* m_pInput;

			CResourcePreloader* m_ResourcePreloader;

			OS::CDynamicLibrary* m_pDynamicLib_Rhi;
			OS::CDynamicLibrary* m_pDynamicLib_Network;
			UNetworkProcAddressInfo* m_pNetworkProcAddressInfo;
			OS::CDpiHelper* m_pDpiHelper;

			CLoadingThread* m_pLoadingThread;

			CCommandQueue* m_pCommandQueue;
			std::pmr::memory_resource* m_pMemoryRes;


		private:
			static CApplication* sm_pInstance;
		public:
			static CApplication* sGetApp() { return sm_pInstance; }
			template < typename T >
			static T* sGetApp() { return (T*)sm_pInstance; }
		};

		//__iberbarGameEngineApi__ CApplication* GetApp();

		const char* GetRhiApiName( RHI::UApiType nApiType );
	}
}


