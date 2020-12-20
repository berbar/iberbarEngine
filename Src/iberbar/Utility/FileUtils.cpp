
#include <iberbar/Utility/FileUtils.h>
#include <tchar.h>
#include <io.h>
#include <filesystem>




bool iberbar::FileUtilsFindData::attrib_is_sub() const
{
	if ( attrib & _A_SUBDIR )
		return true;
	return false;
}





const TCHAR* iberbar::CFileUtils::sm_DefaultFileFilter = _tstr( "*.*" );


iberbar::CFileUtils::CFileUtils( void )
	: m_strOriginWorkPath( "" )
{
}


iberbar::CFileUtils::~CFileUtils()
{
}


bool iberbar::CFileUtils::getFullPath( const char* pzRelPath, TCHcharAR* pzAbsPath, int nSizeInWords )
{
	if ( _fullpath( pzAbsPath, pzRelPath, nSizeInWords ) == NULL )
		return false;
	return true;
}


bool iberbar::CFileUtils::getWorkPath( TCHAR* pzPath, int nSizeInWords )
{
	if ( _tgetcwd( pzPath, nSizeInWords ) == NULL )
		return false;
	return true;
}


bool iberbar::CFileUtils::setWorkPath( const TCHAR* pzPath )
{
	if ( _tchdir( pzPath ) != 0 )
		return false;
	return true;
}


bool iberbar::CFileUtils::foreachFile( const TCHAR* strFileFilter, CUnknown* pHandleObject, PFileForeach1 pHandleFunction )
{
	if ( strFileFilter == NULL )
		return false;
	if ( pHandleObject == NULL || pHandleFunction == NULL )
		return false;

	FileUtilsFindData lc_ffData;
	bool lc_bGoAhead = true;

	long lc_hFile = 0;
	_tfinddata_t fileinfo;
	if ( ( lc_hFile = _tfindfirst( strFileFilter, &fileinfo ) ) != -1 )
	{
		_tfindnext( lc_hFile, &fileinfo );
		while ( _tfindnext( lc_hFile, &fileinfo ) == 0 )
		{
			lc_ffData.attrib      = fileinfo.attrib;
			lc_ffData.time_access = fileinfo.time_access;
			lc_ffData.time_create = fileinfo.time_create;
			lc_ffData.time_write  = fileinfo.time_write;
			lc_ffData.size        = fileinfo.size;
			lc_ffData.name        = fileinfo.name;
			( pHandleObject->*pHandleFunction )( lc_ffData, &lc_bGoAhead );
			if ( lc_bGoAhead == false )
				break;
		};

		_findclose( lc_hFile );
	}

	return true;
}


bool iberbar::CFileUtils::foreachFile( const TCHAR* strFileFilter, PFileForeach2 pHandleFunction )
{
	if ( strFileFilter == NULL )
		return false;
	if ( pHandleFunction == NULL )
		return false;

	FileUtilsFindData lc_ffData;
	bool lc_bGoAhead = true;

	long lc_hFile = 0;
	_tfinddata_t fileinfo;
	if ( ( lc_hFile = _tfindfirst( strFileFilter, &fileinfo ) ) != -1 )
	{
		_tfindnext( lc_hFile, &fileinfo );
		while ( _tfindnext( lc_hFile, &fileinfo ) == 0 )
		{
			lc_ffData.attrib      = fileinfo.attrib;
			lc_ffData.time_access = fileinfo.time_access;
			lc_ffData.time_create = fileinfo.time_create;
			lc_ffData.time_write  = fileinfo.time_write;
			lc_ffData.size        = fileinfo.size;
			lc_ffData.name        = fileinfo.name;
			pHandleFunction( lc_ffData, &lc_bGoAhead );
			if ( lc_bGoAhead == false )
				break;
		};

		_findclose( lc_hFile );
	}

	return true;
}


void iberbar::CFileUtils::init()
{
	wchar_t lc_pzWorkPath[ MAX_PATH ] = { 0 };
	if ( getWorkPath( lc_pzWorkPath, MAX_PATH ) == true )
		m_strOriginWorkPath = lc_pzWorkPath;
}

