
#pragma once

#include <iberbar/Utility/Platform.h>

namespace iberbar
{
	class __iberbarUtilityApi__ CFileHelper
	{
	public:
		CFileHelper( void );
		~CFileHelper(); // 自动关闭文件

		bool OpenFileA( const char* filename, const char* mode );
#ifdef UNICODE
		bool OpenFileW( const wchar_t* filename, const wchar_t* mode );
#endif
		bool OpenFile( const TCHAR* filename, const TCHAR* mode );
		void CloseFile();

		FILE* GetFileHandle() const { return m_pFile; }

		std::string ReadAsText();

		template< typename Allocator = std::allocator<char> >
		size_t ReadAsBinary( std::basic_string< char, std::char_traits<char>, Allocator>& Buf );

	protected:
		FILE* m_pFile;
	};


#ifdef __ANDROID__
#define fread_s(_DstBuf, _DstSize, _ElementSize, _Count, _File)  fread(_DstBuf, _ElementSize, _Count, _File)
#endif


	class __iberbarUtilityApi__ CFileUtilBase
	{
	public:
		bool CreateDirectory( const std::tstring& strPath, bool recurse ) const;
		bool IsExistDirectory( const std::tstring& strPath ) const;
		std::tstring GetDirectory( const std::tstring& strPath ) const;
		bool IsExistFile( const std::tstring& strPath ) const;
		std::tstring Trim( const std::tstring& strPath ) const;

	protected:
		//bool CreateDirectoryInner( const std::tstring& strPath ) const;
	};

}





inline iberbar::CFileHelper::CFileHelper( void )
	: m_pFile( NULL )
{
}


inline iberbar::CFileHelper::~CFileHelper( void )
{
	if ( m_pFile )
		fclose( m_pFile );
}


inline bool iberbar::CFileHelper::OpenFileA( const char* filename, const char* mode )
{
	assert( filename && filename[ 0 ] );
	assert( mode && mode[ 0 ] );

	CloseFile();

#ifdef WIN32
	errno_t err = fopen_s( &m_pFile, filename, mode );
#else
	m_pFile = fopen( filename, mode );
#endif

	if ( m_pFile == NULL )
		return false;

	return true;
}

#ifdef UNICODE
inline bool iberbar::CFileHelper::OpenFileW( const wchar_t* filename, const wchar_t* mode )
{
	assert( filename && filename[ 0 ] );
	assert( mode && mode[ 0 ] );

	CloseFile();

#ifdef WIN32
	_wfopen_s( &m_pFile, filename, mode );
#else
	m_pFile = _wfopen( filename, mode );
#endif

	if ( m_pFile == NULL )
		return false;

	return true;
}
#endif

inline bool iberbar::CFileHelper::OpenFile( const TCHAR* filename, const TCHAR* mode )
{
#ifdef UNICODE
	return OpenFileW( filename, mode );
#else
	return OpenFileA( filename, mode );
#endif
}


inline void iberbar::CFileHelper::CloseFile()
{
	if ( m_pFile )
	{
		fclose( m_pFile );
		m_pFile = NULL;
	}
}


template< typename Allocator >
inline size_t iberbar::CFileHelper::ReadAsBinary( std::basic_string< char, std::char_traits<char>, Allocator>& Buf )
{
	fseek( m_pFile, 0, SEEK_END );
	long nFileSize = ftell( m_pFile );
	fseek( m_pFile, 0, SEEK_SET );
	if ( nFileSize == 0 )
		return 0;
	Buf.resize( (size_t)nFileSize + 1 );
	size_t nReadSize = fread_s( &Buf.front(), nFileSize, 1, nFileSize, m_pFile );
	return nReadSize;
}


