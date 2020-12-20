
#include <iberbar/Utility/Log/OutputDeviceFile.h>
#include <iberbar/Utility/Log/Helper.h>


iberbar::Logging::COutputDeviceFile::COutputDeviceFile( const char* strFileName, bool bDisableBackup, bool bAppendIfExists )
	: m_pFile( nullptr )
	, m_bDisableBackup( bDisableBackup )
	, m_bAppendIfExists( bAppendIfExists )
	, m_bOpened( false )
	, m_bDead( false )
	, m_Mutex()
	, m_strFileName()
{
	strcpy_s( m_strFileName, strFileName );
}


iberbar::Logging::COutputDeviceFile::~COutputDeviceFile()
{
	if ( m_pFile )
	{
		fclose( m_pFile );
		m_pFile = nullptr;
	}
}


void iberbar::Logging::COutputDeviceFile::Serialize( ULevel nLevel, const char* strText, const char* strCategory, int64 nTime )
{
	m_Mutex.lock();

	if ( m_pFile == nullptr && m_bDead == false )
	{
		if ( m_bDisableBackup == false && m_bOpened == false )
		{
			// 备份文件
		}
		
		if ( CreateWriter() )
		{
			m_bOpened = true;
		}
		else
		{
			m_bDead = true;
		}
	}
	else
	{

	}

	if ( m_pFile )
	{
		std::string strTextFinal = FormatLogText( nLevel, strText, strCategory, nTime );
		fwrite( strTextFinal.c_str(), strlen( strTextFinal.c_str() ), 1, m_pFile );
		fflush( m_pFile );
	}

	m_Mutex.unlock();
}


bool iberbar::Logging::COutputDeviceFile::CreateWriter()
{
	m_pFile = _fsopen( m_strFileName, "wt+", _SH_DENYWR );
	if ( m_pFile )
		return true;
	return false;
}
