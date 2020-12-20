#pragma once


#include <iberbar/Utility/Platform.h>
#ifdef __ANDROID__
#include <iberbar/Utility/Platform/Android/JniHelper.h>
#endif


namespace iberbar
{
	namespace Game
	{
		class CResourceFileSystem
		{
		public:
			FORCEINLINE static std::string GetResoucePath( const std::string& strName )
			{
#ifdef _WIN32
				return strName;
#endif
#ifdef __ANDROID__
				return Android::CFileUtil::GetShared()->GetBestFilesDir() + "/" + strName;
#endif
			}
		};
	}
}
