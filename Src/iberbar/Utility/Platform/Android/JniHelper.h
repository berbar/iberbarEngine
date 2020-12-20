

#pragma once

#include <iberbar/Utility/Platform.h>
#include <iberbar/Utility/Result.h>
#include <iberbar/Utility/Buffer.h>
#include <iberbar/Utility/FileHelper.h>
#include <jni.h>
#include <android/asset_manager.h>

namespace iberbar
{
	namespace Android
	{
		class __iberbarExports__ CFileUtil
			: public iberbar::CFileUtilBase
		{
		public:
			CFileUtil();

		public:
			void Init( JNIEnv* env );
			
			const std::string& GetFilesDir() const { assert( m_bInit ); return m_strFilesDir; }
			const std::string& GetExternalFilesDir() const { assert( m_bInit ); return m_strExternalFilesDir; };
			const std::string& GetBestFilesDir() const {
				const std::string& e = GetExternalFilesDir();
				if ( e.empty() == false )
					return e;
				return GetFilesDir();
			}

		private:
			bool m_bInit;
			std::string m_strFilesDir;
			std::string m_strExternalFilesDir;
			
		public:
			static CFileUtil* GetShared()
			{
				return &s_shared;
			}
		private:
			static CFileUtil s_shared;
		};


		class __iberbarExports__ CAssetsUtil
		{
		public:
			CAssetsUtil();

		public:
			void SetAssets( AAssetManager* pAssets ) { m_pAssets = pAssets; }
			std::vector<std::string> ListFiles( const std::string& strDir );
			CResult GetFileContents( const std::string& strFile, TBuffer<uint8>* pBuffer, uint32* pReadSize );
			CResult GetFileContentsText( const std::string& strFile, std::string& strText );

		private:
			AAssetManager* m_pAssets;

		public:
			static CAssetsUtil* GetShared()
			{
				return &s_shared;
			}
		private:
			static CAssetsUtil s_shared;
		};

		inline long GetCurrentTime()
		{
			struct timeval tv;
			gettimeofday( &tv, NULL );
			return tv.tv_sec * 1000 + tv.tv_usec / 1000;
		}


		typedef std::function<void( const std::string& strAssetFile, const std::string& strExportFile, const CResult& )> UFunctionExportAsset;

		CResult __iberbarExports__ ExportAsset(
			const std::string& strFile,
			const std::string& strExternalDir,
			bool forceWrite = false );

		void __iberbarExports__ ExportAssetBatch(
			const std::vector<std::string>& filelist,
			const std::string& strExternalDir,
			UFunctionExportAsset exportCallback,
			bool forceWrite = false );

		class __iberbarExports__ CAssetsExportUtil
		{
		public:
			CAssetsExportUtil();
			~CAssetsExportUtil();

			void ExportDir(
				const std::string& strDir,
				const std::string& strExportDir,
				UFunctionExportAsset exportCallback,
				bool forceWrite = false );

		private:
			CFileUtil* m_pFileUtil;
			CAssetsUtil* m_pAssetsUtil;

			TBuffer<uint8> m_Buffer;
			CFileHelper m_FileHelper;
		};

	}
}
