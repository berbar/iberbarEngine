

#include <iberbar/Utility/Platform/Android/JniHelper.h>
#include <iberbar/Utility/FileHelper.h>

//#include <android/asset_manager_jni.h>


iberbar::Android::CFileUtil iberbar::Android::CFileUtil::s_shared;

iberbar::Android::CFileUtil::CFileUtil()
	: m_bInit( false )
	, m_strFilesDir( "" )
	, m_strExternalFilesDir( "" )
{

}

void iberbar::Android::CFileUtil::Init( JNIEnv* env )
{
	m_bInit = true;

	jclass pClass = env->FindClass( "com/iberbar/lib/FileUtil" );
	if ( pClass == nullptr )
		return;

	jmethodID pMethodId = nullptr;
	jstring pJStr = nullptr;
	jboolean bCopy = JNI_FALSE;

	pMethodId = env->GetStaticMethodID( pClass, "GetFilesDir", "()Ljava/lang/String;" );
	if ( pMethodId != nullptr )
	{
		pJStr = (jstring)env->CallStaticObjectMethod( pClass, pMethodId );
		if ( pJStr != nullptr )
		{
			m_strFilesDir = env->GetStringUTFChars( pJStr, &bCopy );
		}
	}

	pMethodId = env->GetStaticMethodID( pClass, "GetExternalFilesDir", "()Ljava/lang/String;" );
	if ( pMethodId != nullptr )
	{
		pJStr = (jstring)env->CallStaticObjectMethod( pClass, pMethodId );
		if ( pJStr != nullptr )
		{
			m_strExternalFilesDir = env->GetStringUTFChars( pJStr, &bCopy );
		}
	}
}




iberbar::Android::CAssetsUtil iberbar::Android::CAssetsUtil::s_shared;

iberbar::Android::CAssetsUtil::CAssetsUtil()
	: m_pAssets( nullptr )
{
}

std::vector<std::string> iberbar::Android::CAssetsUtil::ListFiles( const std::string& strDir )
{
    AAssetDir* pAssetDir = AAssetManager_openDir( m_pAssets, strDir.c_str() );
    const char* strFileName = nullptr;
    std::vector<std::string> fileNameList;
    while ( (strFileName = AAssetDir_getNextFileName( pAssetDir )) != NULL )
    {
        fileNameList.push_back( strFileName );
    }
    return fileNameList;
}

iberbar::CResult iberbar::Android::CAssetsUtil::GetFileContents( const std::string& strFile, TBuffer<uint8>* pBuffer, uint32* pReadSize )
{
	AAsset* pAsset = AAssetManager_open( m_pAssets, strFile.c_str(), AASSET_MODE_UNKNOWN );
	if ( pAsset == nullptr ) {
		return MakeResult( ResultCode::Bad, "failed to open" );
	}

	auto nAssetSize = AAsset_getLength( pAsset );
	pBuffer->Resize( nAssetSize );

	int nReadSize = AAsset_read( pAsset, pBuffer->GetPointer(), nAssetSize );
	AAsset_close( pAsset );

	if ( nReadSize < nAssetSize ) {
		return MakeResult( ResultCode::Bad, "failed to read" );
	}

	*pReadSize = nAssetSize;

	return CResult();
}

iberbar::CResult iberbar::Android::CAssetsUtil::GetFileContentsText( const std::string& strFile, std::string& strText )
{
	AAsset* pAsset = AAssetManager_open( m_pAssets, strFile.c_str(), AASSET_MODE_UNKNOWN );
	if ( pAsset == nullptr ) {
		return MakeResult( ResultCode::Bad, "failed to open" );
	}

	auto nAssetSize = AAsset_getLength( pAsset );
	strText.resize( nAssetSize + 1 );

	int nReadSize = AAsset_read( pAsset, strText.data(), nAssetSize );
	AAsset_close( pAsset );
	strText[ nReadSize ] = 0;

	if ( nReadSize < nAssetSize ) {
		return MakeResult( ResultCode::Bad, "failed to read" );
	}

	return CResult();
}


iberbar::CResult iberbar::Android::ExportAsset( const std::string& strFile, const std::string& strExternalDir, bool forceWrite )
{
	auto pAssetsUtil = CAssetsUtil::GetShared();
	auto pFileUtil = CFileUtil::GetShared();
	std::string strExternalFile = pFileUtil->GetBestFilesDir() + "/" + strExternalDir + "/" + strFile;

	// 文件已存在，直接跳过
	CFileHelper fileHelper;
	if ( forceWrite == false && pFileUtil->IsExistFile( strExternalFile ) == true )
		return CResult();

	// 读取asset
	TBuffer<uint8> pBuffer;
	uint32 nReadSize = 0;
	auto retReadAsset = pAssetsUtil->GetFileContents( strFile, &pBuffer, &nReadSize );
	if ( retReadAsset.IsOK() == false )
		return retReadAsset;

	if ( pFileUtil->CreateDirectory( pFileUtil->GetDirectory( strExternalFile ), true ) == false )
		return MakeResult( ResultCode::Bad, "failed to create directory" );

	if ( fileHelper.OpenFileA( strExternalFile.c_str(), "wb+" ) == false )
		return MakeResult( ResultCode::Bad, "failed to open file" );

	fwrite( pBuffer.GetPointer(), nReadSize, 1, fileHelper.GetFileHandle() );
	fileHelper.CloseFile();

	return CResult();
}
//
//void iberbar::Android::ExportAssetBatch(
//	const std::vector<std::string>& filelist,
//	const std::string& strExternalDir,
//	std::function<void( const std::string&, const CResult& )> exportCallback,
//	bool forceWrite )
//{
//	auto pAssetsUtil = CAssetsUtil::GetShared();
//	auto pFileUtil = CFileUtil::GetShared();
//	std::string strExternalDirFull = pFileUtil->GetBestFilesDir() + strExternalDir + "/";
//
//	std::string strFile;
//	std::string strExternalFile;
//	TBuffer<uint8> pBuffer;
//	uint32 nReadSize = 0;
//	CFileHelper fileHelper;
//
//	auto iter = filelist.begin();
//	auto end  = filelist.end();
//	for ( ; iter != end; iter++ )
//	{
//		strFile = *iter;
//		strExternalFile = strExternalDirFull + strFile;
//		auto retReadAsset = pAssetsUtil->GetFileContents( strFile, &pBuffer, &nReadSize );
//		if ( retReadAsset.IsOK() == false )
//		{
//			exportCallback( strFile, retReadAsset );
//			continue;
//		}
//
//		// 文件已存在，直接跳过
//		if ( forceWrite == false )
//		{
//			if ( pFileUtil->IsExistFile( strExternalFile ) == true )
//				continue;
//		}
//
//		if ( pFileUtil->CreateDirectory( strExternalDirFull, true ) == false )
//		{
//			exportCallback( strFile, MakeResult( ResultCode::Bad, "failed to open file" ) );
//			continue;
//		}
//
//		if ( fileHelper.OpenFileA( strExternalFile.c_str(), "wb" ) == false )
//		{
//			exportCallback( strFile, MakeResult( ResultCode::Bad, "failed to open file" ) );
//			continue;
//		}
//
//		fwrite( pBuffer.GetPointer(), nReadSize, 1, fileHelper.GetFileHandle() );
//		fileHelper.CloseFile();
//
//		exportCallback( strFile, CResult() );
//	}
//}


iberbar::Android::CAssetsExportUtil::CAssetsExportUtil()
	: m_pFileUtil( CFileUtil::GetShared() )
	, m_pAssetsUtil( CAssetsUtil::GetShared() )
	, m_Buffer()
	, m_FileHelper()
{
}

iberbar::Android::CAssetsExportUtil::~CAssetsExportUtil()
{
}

void iberbar::Android::CAssetsExportUtil::ExportDir(
	const std::string& strDir,
	const std::string& strExportDir,
	UFunctionExportAsset exportCallback,
	bool forceWrite )
{
	std::string strExportDirFull = m_pFileUtil->GetBestFilesDir() + strExportDir + "/" + strDir;
	//const char* strExportDirFullTemp = strExportDirFull.c_str();
	if ( m_pFileUtil->CreateDirectory( strExportDirFull, true ) == false )
		return;

	auto filelist = m_pAssetsUtil->ListFiles( strDir );
	if ( filelist.empty() == false )
	{
		//int nFileCount = filelist.size();

		std::string strAssetFile;
		std::string strExportFile;
		uint32 nReadSize = 0;

		auto iter = filelist.begin();
		auto end = filelist.end();
		for ( ; iter != end; iter++ )
		{
			strAssetFile = strDir + "/" + (*iter);
			strExportFile = strExportDirFull + "/" + (*iter);
			//const char* strExportFileTemp = strExportFile.c_str();
			auto retReadAsset = m_pAssetsUtil->GetFileContents( strAssetFile, &m_Buffer, &nReadSize );
			if ( retReadAsset.IsOK() == false )
			{
				exportCallback( strAssetFile, strExportFile, retReadAsset );
				continue;
			}

			// 文件已存在，直接跳过
			if ( forceWrite == false )
			{
				if ( m_pFileUtil->IsExistFile( strExportFile ) == true )
					continue;
			}

			if ( m_FileHelper.OpenFileA( strExportFile.c_str(), "wb" ) == false )
			{
				exportCallback( strAssetFile, strExportFile, MakeResult( ResultCode::Bad, "failed to open file" ) );
				continue;
			}

			fwrite( m_Buffer.GetPointer(), nReadSize, 1, m_FileHelper.GetFileHandle() );
			m_FileHelper.CloseFile();

			exportCallback( strAssetFile, strExportFile, CResult() );
		}
	}
}


//
//int extract_file( AAssetManager* pAssets, const char* szDexPath, const char* fileName )
//{
//        AAsset* asset = AAssetManager_open( pAssets, fileName, AASSET_MODE_STREAMING );   // 获取 asset 对象，对应的文件名    UNKNOWN 0, RANDOM 1, STREAMING 2, BUFFER 3
//        FILE* file = fopen( szDexPath, "wb" );        // wb 只写打开或新建一个二进制文件；只允许写数据
//        int bufferSize = AAsset_getLength( asset );       // 长度
//        //LOGD( "[+]Asset FileName:%s,extract path:%s,size:%d\n", fileName, szDexPath, bufferSize );      // Asset FileName:jiami.dat, extract path:/data/data/com.wangwz.ndk_test/files/.jiagu/jiami.dat, size:2639568
//        void* buffer = malloc( 4096 );
//        while ( true ) {
//            int numBytesRead = AAsset_read( asset, buffer, 4096 );
//            if ( numBytesRead <= 0 )
//                break;
//            fwrite( buffer, numBytesRead, 1, file );      // 写入到 file 内
//        }
//        free( buffer );
//        fclose( file );
//        AAsset_close( asset );
//}

/*******************************************************************************
    * Function Name : java_com_fontlose_ReadAssets_readFromAssets
* Description : 定义：public native void readFromAssets(AssetManager ass,String filename);
    * Input : AssetManager 对象 filename资源名
    * Output : None
    * Return : None
    *******************************************************************************/
//void Java_com_example_hellojni_HelloJni_nativeReadFromAssets
//( JNIEnv* env, jclass tis, jobject assetManager, jstring filename )
//{
//    //LOGI( "ReadAssets" );
//    AAssetManager* mgr = AAssetManager_fromJava( env, assetManager );
//    if ( mgr == NULL )
//    {
//        LOGI( " %s", "AAssetManager==NULL" );
//        return;
//    }
//    /*获取文件名并打开*/
//    jboolean iscopy = JNI_FALSE;
//    const char* mfile = env->GetStringUTFChars( filename, &iscopy );
//    AAsset* asset = AAssetManager_open( mgr, mfile, AASSET_MODE_UNKNOWN );
//    //(*env)->ReleaseStringUTFChars( env, filename, mfile );
//    if ( asset == NULL )
//    {
//        LOGI( " %s", "asset==NULL" );
//        return;
//    }
//    /*获取文件大小*/
//    off_t bufferSize = AAsset_getLength( asset );
//    LOGI( "file size : %d\n", bufferSize );
//    char* buffer = (char*)malloc( bufferSize + 1 );
//    buffer[ bufferSize ] = 0;
//    int numBytesRead = AAsset_read( asset, buffer, bufferSize );
//    LOGI( ": %s", buffer );
//    free( buffer );
//    /*关闭文件*/
//    AAsset_close( asset );
//}



//bool iberbar::Android::CFileUtil::ExportAssetsFile( const char* filename, bool overwrite )
//{
//	JNIEnv* lc_env = JNIEnvHelper::GetEnv();
//	jclass lc_cls = lc_env->FindClass( "com/iberbar/engine/lib/FileUtil" );
//	jmethodID lc_method = lc_env->GetStaticMethodID( lc_cls, "ExportAssetsFile", "(Ljava/lang/String;Z)Z" );
//	if (lc_method == 0)
//	{
//		GetLogConsole()->warn( "iberbarEngine", "can't find method com/iberbar/engine/lib/FileUtil:ExportAssetsFile[(Ljava/lang/String;Z)Z]" );
//		return false;
//	}
//
//	jstring lc_filename = stdstring_to_jstring( lc_env, filename );
//	jboolean lc_overwrite = (overwrite == true) ? 1 : 0;
//	jboolean lc_ret = lc_env->CallStaticBooleanMethod( lc_cls, lc_method, lc_filename, lc_overwrite );
//
//	return (lc_ret == 0) ? false : true;
//}


