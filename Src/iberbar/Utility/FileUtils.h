
#include <iberbar/Utility/Platform.h>
#include <functional>

namespace iberbar
{
	class CFileUtils;
	struct FileUtilsFindData;
	struct FileFind;


	struct FileUtilsFindData
	{
		uint32      attrib;
		int64       time_create;    /* -1 for FAT file systems */
		int64       time_access;    /* -1 for FAT file systems */
		int64       time_write;
		uint32      size;
		std::string name;


		bool attrib_is_sub() const;
	};


	



	class CFileUtils
	{
	public:
		typedef void _CallbackForeach( const FileUtilsFindData& data, bool* pbShouldGoAhead );


	private:
		CFileUtils( void );
		~CFileUtils();

	public:
		bool getFullPath( const char* pzRelPath, char* pzAbsPath, int nSizeInWords );
		bool getWorkPath( char* pzPath, int nSizeInWords );
		bool setWorkPath( const char* pzPath );
		bool setOriginWorkPath() { return setWorkPath( m_strOriginWorkPath.c_str() ); }

	public:
		bool foreachFile( const TCHAR* strFileFilter, std::function<_CallbackForeach> callback );

	private:
		std::string m_strOriginWorkPath;

		void init();

	public:
		static const TCHAR* sm_DefaultFileFilter;

		static CFileUtils* GetShared()
		{
			static CFileUtils s_shared;
			static bool s_init = false;
			if ( s_init == false )
			{
				s_shared.init();
				s_init = true;
			}
			return &s_shared;
		}
	};

}






