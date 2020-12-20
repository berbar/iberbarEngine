
#include "FileHelper.h"


#ifdef _WIN32
#else
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#endif


std::string iberbar::CFileHelper::ReadAsText()
{
	fseek( m_pFile, 0, SEEK_END );
	int fileSize = ftell( m_pFile );
	fseek( m_pFile, 0, SEEK_SET );
	if ( fileSize == 0 )
		return "";
	std::string text;
	text.resize( fileSize + 1 );
	int readSize = fread_s( &text.front(), fileSize + 1, 1, fileSize, m_pFile );
	text[ readSize ] = 0;
	return text;
}



bool iberbar::CFileUtilBase::CreateDirectory( const std::tstring& strPath, bool recurse ) const
{
    //const tchar* strPathTemp = strPath.c_str();
	if ( IsExistDirectory( strPath ) == true )
		return true;

    if ( recurse == false )
    {
#ifdef _WIN32
        BOOL ret = ::CreateDirectoryW( strPath.c_str(), nullptr );
        if ( ret == FALSE )
        {
            return false;
        }
#else
        int ret = mkdir( strPath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO );
        if ( ret != 0 && (errno != EEXIST) )
        {
            return false;
        }
#endif
        return true;
    }

    std::tstring strDir = GetDirectory( strPath );
    //const tchar* strDirTemp = strDir.c_str();
    if ( strDir.empty() == false )
    {
        CreateDirectory( Trim( strDir ), true );
    }

    CreateDirectory( strPath, false );
//
//	size_t nStart = 0;
//	size_t nFound = strPath.find_first_of( "/\\", nStart );
//    std::string strSubPath;
//    std::vector<std::string> dirs;
//
//    if ( nFound != std::string::npos )
//    {
//        while ( true )
//        {
//            strSubPath = strPath.substr( nStart, nFound - nStart + 1 );
//            if ( !strSubPath.empty() )
//                dirs.push_back( strSubPath );
//            nStart = nFound + 1;
//            nFound = strPath.find_first_of( "/\\", nStart );
//            if ( nFound == std::string::npos )
//            {
//                if ( nStart < strPath.length() )
//                {
//                    dirs.push_back( strPath.substr( nStart ) );
//                }
//                break;
//            }
//        }
//    }
//
//#ifdef _WIN32
//#else
//    DIR* dir = NULL;
//
//    // Create path recursively
//    strSubPath = "";
//    for ( const auto& iter : dirs )
//    {
//        strSubPath += iter;
//        dir = opendir( strSubPath.c_str() );
//
//        if ( !dir )
//        {
//            // directory doesn't exist, should create a new one
//
//            int ret = mkdir( strSubPath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO );
//            if ( ret != 0 && (errno != EEXIST) )
//            {
//                // current directory can not be created, sub directories can not be created too
//                // should return
//                return false;
//            }
//        }
//        else
//        {
//            // directory exists, should close opened dir
//            closedir( dir );
//        }
//    }
//#endif

    return true;
}

bool iberbar::CFileUtilBase::IsExistDirectory( const std::tstring& strPath ) const
{
    
#ifdef _WIN32
    WIN32_FIND_DATA  FindFileData;
    HANDLE hFind = FindFirstFile( strPath.c_str(), &FindFileData );
    bool bExist = false;
    if ( (hFind != INVALID_HANDLE_VALUE) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 )
    {
        bExist = true;
    }
    CloseHandle( hFind );
    return bExist;
#else
    struct stat st;
    if ( stat( strPath.c_str(), &st ) == 0 )
    {
        return S_ISDIR( st.st_mode );
    }
    return false;
#endif
}

std::tstring iberbar::CFileUtilBase::GetDirectory( const std::tstring& strPath ) const
{
    std::tstring strDir = Trim( strPath );
    size_t nFound = strDir.find_last_of( TEXT("/\\") );
    if ( nFound == std::tstring::npos )
        return TEXT( "" );
    
    const tchar* strDirTemp = strDir.c_str();
    strDir = strPath.substr( 0, nFound );
    strDirTemp = strDir.c_str();
    strDir = Trim( strDir );
    strDirTemp = strDir.c_str();
    return strDir;
}

bool iberbar::CFileUtilBase::IsExistFile( const std::tstring& strPath ) const
{
#ifdef _WIN32
    WIN32_FIND_DATA  FindFileData;
    HANDLE hFind = FindFirstFile( strPath.c_str(), &FindFileData );
    bool bExist = false;
    if ( (hFind != INVALID_HANDLE_VALUE) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 )
    {
        bExist = true;
    }
    CloseHandle( hFind );
    return bExist;
#else
    struct stat st;
    if ( stat( strPath.c_str(), &st ) == 0 )
    {
        return S_ISREG( st.st_mode );
    }
    return false;
#endif
}

std::tstring iberbar::CFileUtilBase::Trim( const std::tstring& strPath ) const
{
    size_t nFound = strPath.find_last_not_of( TEXT("/\\") );
    if ( nFound == std::tstring::npos )
        return strPath;
    return strPath.substr( 0, nFound + 1 );

    //const char* ptr = strPath.c_str();
    //int nLen = strPath.length();
    //int nLenNew = nLen;
    //for ( int i = nLen - 1; i >= 0; i-- )
    //{
    //    if ( ptr[ i ] != '/' && ptr[ i ] != '\\' )
    //        break;
    //    nLenNew--;
    //}
    //if ( nLenNew == nLen )
    //    return strPath;
    //return strPath.substr( 0, nLenNew );
}

//bool iberbar::CFileUtilBase::CreateDirectoryInner( const std::tstring& strPath ) const
//{
//    int ret = mkdir( strPath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO );
//    if ( ret != 0 && (errno != EEXIST) )
//    {
//        return false;
//    }
//    return true;
//}





