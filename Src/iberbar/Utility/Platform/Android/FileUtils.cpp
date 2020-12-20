
#include "jni.h"
#include "iberbar\Base\FileUtils.h"
#include "iberbar\Base\Platform\Android\JNIEnvHelper.h"
#include "iberbar\Base\LogText.h"
#include "android\log.h"
#include "Java_FileUtil.h"




bool iberbar::FileUtilsFindData::attrib_is_sub() const
{
	return false;
}





const TCHAR* iberbar::CFileUtils::sm_DefaultFileFilter = _tstr( "*.*" );


iberbar::CFileUtils::CFileUtils( void )
	: m_strOriginWorkPath( _tstr( "" ) )
{
}


iberbar::CFileUtils::~CFileUtils()
{
}


bool iberbar::CFileUtils::getFullPath( const TCHAR* pzRelPath, TCHAR* pzAbsPath, int nSizeInWords )
{
	int lc_lenWorkPath = strlen( m_strOriginWorkPath.c_str() );
	int lc_lenCopy = (lc_lenWorkPath > nSizeInWords ) ? nSizeInWords : lc_lenWorkPath;
	int lc_lenLeave = nSizeInWords - lc_lenCopy;
	int lc_lenRel = strlen( pzRelPath );
	if ( lc_lenLeave <= lc_lenRel )
		return false;

	strncpy( pzAbsPath, m_strOriginWorkPath.c_str(), lc_lenCopy );
	pzAbsPath += lc_lenCopy;
	strncpy( pzAbsPath, pzRelPath, lc_lenRel );
	pzAbsPath += lc_lenRel;
	pzAbsPath[ 0 ] = 0;

	return false;
}


bool iberbar::CFileUtils::getWorkPath( TCHAR* pzPath, int nSizeInWords )
{
	return false;
}


bool iberbar::CFileUtils::setWorkPath( const TCHAR* pzPath )
{
	return false;
}


bool iberbar::CFileUtils::foreachFile( const TCHAR* strFileFilter, CUnknown* pHandleObject, PFileForeach1 pHandleFunction )
{
	return false;
}


bool iberbar::CFileUtils::foreachFile( const TCHAR* strFileFilter, PFileForeach2 pHandleFunction )
{
	return false;
}


void iberbar::CFileUtils::init()
{
// 	JNIEnv* lc_env = JNIEnvHelper::GetEnv();
// 	jclass lc_cls = lc_env->FindClass( "com/iberbar/engine/lib/FileUtil" );
// 	jmethodID lc_method = lc_env->GetStaticMethodID( lc_cls, "GetSdcardRootPath", "()Ljava/lang/String;" );
// 	if ( lc_method == 0 )
// 	{
// 		return ;
// 	}
// 	jstring lc_s = (jstring)lc_env->CallStaticObjectMethod( lc_cls, lc_method );
// 	jstring_to_stdstring( lc_env, lc_s, &m_strOriginWorkPath );

	JavaFileUtil::GetSdcardRootPath( &m_strOriginWorkPath );

	GetLogConsole()->debug( "iberbarEngine", m_strOriginWorkPath.c_str() );
}

