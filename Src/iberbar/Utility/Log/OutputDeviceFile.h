#pragma once

#include <iberbar/Utility/Log/OutputDevice.h>
#include <mutex>


namespace iberbar
{
	namespace Logging
	{
		class __iberbarUtilityApi__ COutputDeviceFile
			: public COutputDevice
		{
		public:
			COutputDeviceFile( const char* strFileName, bool bDisableBackup = false, bool bAppendIfExists = false );
			virtual ~COutputDeviceFile();

			virtual void Serialize( ULevel nLevel, const char* strText, const char* strCategory, int64 nTime = -1 ) override;

		private:
			bool CreateWriter();

		private:
			FILE* m_pFile;
			bool m_bDisableBackup;
			bool m_bAppendIfExists;
			bool m_bOpened;
			bool m_bDead;
			std::mutex m_Mutex;
			char m_strFileName[ 1024 ];
		};
	}
}