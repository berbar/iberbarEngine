
#include "FileHelper.h"
#include <iberbar/Utility/String.h>


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



bool iberbar::FileApi::CreateDirectoryA( const char* strPath, bool recurse )
{
	if ( IsExistDirectoryA( strPath ) == true )
		return true;

    if ( recurse == false )
    {
#ifdef _WIN32
        BOOL ret = ::CreateDirectoryA( strPath, nullptr );
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

    std::string strDir = GetDirectoryA( strPath );
    //const tchar* strDirTemp = strDir.c_str();
    if ( strDir.empty() == false )
    {
        CreateDirectoryA( strDir.c_str(), true );
    }

    CreateDirectoryA( strPath, false );
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


bool iberbar::FileApi::CreateDirectoryW( const wchar_t* strPath, bool recurse )
{
    if ( IsExistDirectoryW( strPath ) == true )
        return true;

    if ( recurse == false )
    {
#ifdef _WIN32
        BOOL ret = ::CreateDirectoryW( strPath, nullptr );
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

    std::wstring strDir = GetDirectoryW( strPath );
    if ( strDir.empty() == false )
    {
        CreateDirectoryW( strDir.c_str(), true );
    }

    CreateDirectoryW( strPath, false );
}


bool iberbar::FileApi::IsExistDirectoryA( const char* strPath )
{
    
#ifdef _WIN32
    WIN32_FIND_DATAA  FindFileData;
    HANDLE hFind = FindFirstFileA( strPath, &FindFileData );
    bool bExist = false;
    if ( (hFind != INVALID_HANDLE_VALUE) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 )
    {
        bExist = true;
    }
    FindClose( hFind );
    return bExist;
#else
    struct stat st;
    if ( stat( strPath, &st ) == 0 )
    {
        return S_ISDIR( st.st_mode );
    }
    return false;
#endif
}


bool iberbar::FileApi::IsExistDirectoryW( const wchar_t* strPath )
{

#ifdef _WIN32
    WIN32_FIND_DATAW  FindFileData;
    HANDLE hFind = FindFirstFileW( strPath, &FindFileData );
    bool bExist = false;
    if ( (hFind != INVALID_HANDLE_VALUE) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 )
    {
        bExist = true;
    }
    FindClose( hFind );
    return bExist;

#else
    std::string strPathUtf8 = UnicodeToUtf8( strPath );
    struct stat st;
    if ( stat( strPathUtf8.c_str(), &st ) == 0 )
    {
        return S_ISDIR( st.st_mode );
    }
    return false;
#endif
}


std::string iberbar::FileApi::GetDirectoryA( const char* strPath )
{
    std::string strPathTemp = TrimA( strPath );
    size_t nFound = strPathTemp.find_last_of( "/\\" );
    if ( nFound == std::string::npos )
        return "";
    
    //const char* strDirTemp = strDir.c_str();
    std::string strDir;
    strDir = strPathTemp.substr( 0, nFound );
    //strDirTemp = strDir.c_str();
    strDir = TrimA( strDir.c_str() );
    //strDirTemp = strDir.c_str();
    return strDir;
}


std::wstring iberbar::FileApi::GetDirectoryW( const wchar_t* strPath )
{
    std::wstring strPathTemp = TrimW( strPath );
    size_t nFound = strPathTemp.find_last_of( L"/\\" );
    if ( nFound == std::wstring::npos )
        return L"";

    //const char* strDirTemp = strDir.c_str();
    std::wstring strDir;
    strDir = strPathTemp.substr( 0, nFound );
    //strDirTemp = strDir.c_str();
    strDir = TrimW( strDir.c_str() );
    //strDirTemp = strDir.c_str();
    return strDir;
}


bool iberbar::FileApi::IsExistFileA( const char* strPath )
{
#ifdef _WIN32
    WIN32_FIND_DATAA  FindFileData;
    HANDLE hFind = FindFirstFileA( strPath, &FindFileData );
    bool bExist = false;
    if ( (hFind != INVALID_HANDLE_VALUE) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 )
    {
        bExist = true;
    }
    FindClose( hFind );
    return bExist;
#else
    struct stat st;
    if ( stat( strPath, &st ) == 0 )
    {
        return S_ISREG( st.st_mode );
    }
    return false;
#endif
}


bool iberbar::FileApi::IsExistFileW( const wchar_t* strPath )
{
#ifdef _WIN32
    WIN32_FIND_DATAW  FindFileData;
    HANDLE hFind = FindFirstFileW( strPath, &FindFileData );
    bool bExist = false;
    if ( (hFind != INVALID_HANDLE_VALUE) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 )
    {
        bExist = true;
    }
    FindClose( hFind );
    return bExist;
#else
    std::string strPathUtf8 = UnicodeToUtf8( strPath );
    struct stat st;
    if ( stat( strPathUtf8.c_str(), &st ) == 0 )
    {
        return S_ISREG( st.st_mode );
    }
    return false;
#endif
}


std::string iberbar::FileApi::TrimA( const char* strPath )
{
    std::string strPathTemp = strPath;
    size_t nFound = strPathTemp.find_last_not_of( "/\\" );
    if ( nFound == std::string::npos )
        return strPath;
    return strPathTemp.substr( 0, nFound + 1 );

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


std::wstring iberbar::FileApi::TrimW( const wchar_t* strPath )
{
    std::wstring strPathTemp = strPath;
    size_t nFound = strPathTemp.find_last_not_of( L"/\\" );
    if ( nFound == std::wstring::npos )
        return strPath;
    return strPathTemp.substr( 0, nFound + 1 );

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





