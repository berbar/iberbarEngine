

#include <iberbar/GameEngine/Paper2dLoader.h>
#include <iberbar/GameEngine/Application.h>
#include <iberbar/GameEngine/BaseResourceManager.h>
#include <iberbar/GameEngine/TextureManager.h>

#include <iberbar/Paper2d/Animation.h>
#include <iberbar/Paper2d/Terrain.h>

#include <iberbar/Utility/JsonConvert.h>
#include <iberbar/Utility/FileHelper.h>
#include <iberbar/Utility/String.h>
#include <iberbar/Utility/StringConvert.h>




namespace iberbar
{
	namespace Game
	{
		class CPaper2dLoader_Animations
		{
		public:
			CResult Load( const char* strFile, std::vector<Paper2d::CAnimationController*>& outAnimList );
			CResult LoadJson( const char* strJsonText, std::vector<Paper2d::CAnimationController*>& outAnimList );
		private:
			void LoadJson_AnimNode( const nlohmann::json& jsonAnimNode );
			void LoadJson_FrameNode( const nlohmann::json& jsonFrameNode, std::vector<Paper2d::UAnimationFrameNode>& FrameNodes, const CSize2f& TextureSize );
		private:
			std::vector<TSmartRefPtr<Paper2d::CAnimationController>> m_AnimList;
		};


		class CPaper2dLoader_GridTerrian
		{
		public:
			CResult Load( const char* strFile, Paper2d::CGridTerrain* pTerrain );
			CResult LoadJson( const char* strJsonText, Paper2d::CGridTerrain* pTerrain );
		};
	}
}





iberbar::Game::CPaper2dLoader* iberbar::Game::CPaper2dLoader::sm_pSharedInstance = nullptr;


iberbar::Game::CPaper2dLoader::CPaper2dLoader()
{
	sm_pSharedInstance = this;
}


iberbar::Game::CPaper2dLoader::~CPaper2dLoader()
{
	sm_pSharedInstance = nullptr;
}


iberbar::CResult iberbar::Game::CPaper2dLoader::LoadAnimations( const char* strFile, std::vector<Paper2d::CAnimationController*>& outAnimList )
{
	std::string strFileFull = CResourceFileSystem::GetResoucePath( strFile );
	CPaper2dLoader_Animations Loader;
	return Loader.Load( strFileFull.c_str(), outAnimList );
}


iberbar::CResult iberbar::Game::CPaper2dLoader::LoadGridTerrain( const char* strFile, Paper2d::CGridTerrain* pTerrain )
{
	std::string strFileFull = CResourceFileSystem::GetResoucePath( strFile );
	CPaper2dLoader_GridTerrian Loader;
	return Loader.Load( strFileFull.c_str(), pTerrain );
}







iberbar::CResult iberbar::Game::CPaper2dLoader_Animations::Load( const char* strFile, std::vector<Paper2d::CAnimationController*>& outAnimList )
{
	int nLen = strlen( strFile );
	int nExtPosition = tstring_rfind_ch( strFile, '.', nLen );
	if ( nExtPosition < 0 || (nExtPosition + 1) >= nLen )
		return MakeResult( ResultCode::Bad, "Invalid format of filepath" );

	std::string strExt = StringToLower( strFile + nExtPosition );

	if ( strcmp( strExt.c_str(), ".json" ) == 0 )
	{
		CFileHelper FileHelper;
		if ( FileHelper.OpenFileA( strFile, "r" ) == false )
			return MakeResult( ResultCode::Bad, "" );
		std::string strJsonText = FileHelper.ReadAsText();
		FileHelper.CloseFile();

		LoadJson( strJsonText.c_str(), outAnimList );
	}
	else
	{
		return MakeResult( ResultCode::Bad, "Invalid format of filepath" );
	}

	Paper2d::CAnimationController* pAnimController = nullptr;
	for ( size_t i = 0, s = m_AnimList.size(); i < s; i++ )
	{
		pAnimController = m_AnimList[ i ];
		pAnimController->AddRef();
		outAnimList.push_back( pAnimController );
	}

	return CResult();
}


iberbar::CResult iberbar::Game::CPaper2dLoader_Animations::LoadJson( const char* strJsonText, std::vector<Paper2d::CAnimationController*>& outAnimList )
{
	nlohmann::json jsonRoot = nlohmann::json::parse( strJsonText );

	if ( jsonRoot.contains( "AnimationList" ) == false )
		return MakeResult( ResultCode::Bad, "" );

	const nlohmann::json& jsonAnimList = jsonRoot.at( "AnimationList" );
	if ( jsonAnimList.is_null() == true || jsonAnimList.is_array() == false )
		return MakeResult( ResultCode::Bad, "" );

	size_t nAnimCount = jsonAnimList.size();
	for ( size_t i = 0; i < nAnimCount; i++ )
	{
		LoadJson_AnimNode( jsonAnimList.at( i ) );
	}

	return CResult();
}


void iberbar::Game::CPaper2dLoader_Animations::LoadJson_AnimNode( const nlohmann::json& jsonAnimNode )
{
	std::string strName = JsonGetString( jsonAnimNode, "Name", "" );
	if ( strName.empty() == true )
		return;

	float nTimePerFrame = JsonGetFloat( jsonAnimNode, "TimePerFrame", 0.0f );
	if ( nTimePerFrame <= 0.0f )
		return;

	std::string strTextureName = JsonGetString( jsonAnimNode, "Texture", "" );
	if ( strTextureName.empty() == true )
		return;

	TSmartRefPtr<RHI::ITexture> pTexture = nullptr;
	CResult retLoadTexture = CApplication::sGetApp()->GetTextureManager()->GetOrCreateTextureA( strTextureName.c_str(), &pTexture );
	if ( retLoadTexture.IsOK() == false )
		return;

	if ( jsonAnimNode.contains( "FrameList" ) == false )
		return;
	const nlohmann::json jsonFrameList = jsonAnimNode.at( "FrameList" );
	if ( jsonFrameList.is_null() == true || jsonFrameList.is_array() == false )
		return;

	std::vector<Paper2d::UAnimationFrameNode> FrameNodes;
	CSize2f TextureSize = CSize2f( (float)pTexture->GetSize().w, (float)pTexture->GetSize().h );
	for ( size_t i = 0, s = jsonFrameList.size(); i < s; i++ )
	{
		LoadJson_FrameNode( jsonFrameList.at( i ), FrameNodes, TextureSize );
	}
	if ( FrameNodes.empty() == true )
		return;

	TSmartRefPtr<Paper2d::CAnimationController> pAnimController = TSmartRefPtr<Paper2d::CAnimationController>::_sNew();
	pAnimController->SetName( strName.c_str() );
	pAnimController->SetTimePerFrame( nTimePerFrame );
	pAnimController->SetTexture( pTexture );
	pAnimController->AddFrameList( FrameNodes );
	m_AnimList.push_back( pAnimController );
}


void iberbar::Game::CPaper2dLoader_Animations::LoadJson_FrameNode( const nlohmann::json& jsonFrameNode, std::vector<Paper2d::UAnimationFrameNode>& FrameList, const CSize2f& TextureSize )
{
	if ( jsonFrameNode.is_null() == true )
		return;

	if ( jsonFrameNode.is_object() == false )
		return;

	CColor4B Color = CColor4B( 0xff, 0xff, 0xff, 0xff );
	CRect2f rcTexCoord = CRect2f( 0, 0, 1, 1 );

	std::string strColor = JsonGetString( jsonFrameNode, "Color", "" );
	if ( strColor.empty() == false )
	{
		CStringEasySplitHelper< char, 32, 4 > SplitHelper;
		int nSplitCount = SplitHelper.Split( strColor.c_str(), ',' );
		if ( nSplitCount == 4 )
		{
			Color = CColor4B(
				(uint8)strtoul( SplitHelper[ 0 ], nullptr, 0 ),
				(uint8)strtoul( SplitHelper[ 1 ], nullptr, 0 ),
				(uint8)strtoul( SplitHelper[ 2 ], nullptr, 0 ),
				(uint8)strtoul( SplitHelper[ 3 ], nullptr, 0 )
			);
		}
	}

	std::string strUV = JsonGetString( jsonFrameNode, "UV", "" );
	if ( strUV.empty() == false )
	{
		rcTexCoord = StringConvertToUV( strUV.c_str(), TextureSize );
		//CStringEasySplitHelper< char, 32, 4 > SplitHelper;
		//int nSplitCount = SplitHelper.Split( strUV.c_str(), ',' );
		//if ( nSplitCount == 4 )
		//{
		//	//rcTexCoord = CRect2f(
		//	//	strtof( SplitHelper[ 0 ], nullptr ) / (float)TextureSize.w,
		//	//	strtof( SplitHelper[ 1 ], nullptr ) / (float)TextureSize.h,
		//	//	strtof( SplitHelper[ 2 ], nullptr ) / (float)TextureSize.w,
		//	//	strtof( SplitHelper[ 3 ], nullptr ) / (float)TextureSize.h
		//	//);

		//	rcTexCoord = CRect2f(
		//		( strtof( SplitHelper[ 0 ], nullptr ) + 0.5f ) / (float)TextureSize.w,
		//		( strtof( SplitHelper[ 1 ], nullptr ) + 0.5f ) / (float)TextureSize.h,
		//		( strtof( SplitHelper[ 2 ], nullptr ) + 0.5f ) / (float)TextureSize.w,
		//		( strtof( SplitHelper[ 3 ], nullptr ) + 0.5f ) / (float)TextureSize.h
		//	);
		//}
	}

	Paper2d::UAnimationFrameNode FrameNode;
	FrameNode.Color = Color;
	FrameNode.rcTexCoord = rcTexCoord;
	FrameList.push_back( FrameNode );
}






iberbar::CResult iberbar::Game::CPaper2dLoader_GridTerrian::Load( const char* strFile, Paper2d::CGridTerrain* pTerrain )
{
	int nLen = strlen( strFile );
	int nExtPosition = tstring_rfind_ch( strFile, '.', nLen );
	if ( nExtPosition < 0 || (nExtPosition + 1) >= nLen )
		return MakeResult( ResultCode::Bad, "Invalid format of filepath" );

	std::string strExt = StringToLower( strFile + nExtPosition );

	if ( strcmp( strExt.c_str(), ".json" ) == 0 )
	{
		CFileHelper FileHelper;
		if ( FileHelper.OpenFileA( strFile, "r" ) == false )
			return MakeResult( ResultCode::Bad, "" );
		std::string strJsonText = FileHelper.ReadAsText();
		FileHelper.CloseFile();

		LoadJson( strJsonText.c_str(), pTerrain );
	}
	else
	{
		return MakeResult( ResultCode::Bad, "Invalid format of filepath" );
	}

	return CResult();
}


iberbar::CResult iberbar::Game::CPaper2dLoader_GridTerrian::LoadJson( const char* strJsonText, Paper2d::CGridTerrain* pTerrain )
{
	nlohmann::json jsonRoot = nlohmann::json::parse( strJsonText );

	std::vector<Paper2d::CGridTerrain::UGridPaletteNode> Palette;
	std::vector<int> Grids;


	if ( jsonRoot.contains( "Size" ) == false )
		return MakeResult( ResultCode::Bad, "" );
	const nlohmann::json& jsonSize = jsonRoot.at( "Size" );
	if ( jsonSize.is_null() || jsonSize.is_object() == false )
		return MakeResult( ResultCode::Bad, "" );

	int nRow = JsonGetInt32( jsonSize, "Row", 0 );
	int nCol = JsonGetInt32( jsonSize, "Col", 0 );
	if ( nRow <= 0 || nCol <= 0 )
		return MakeResult( ResultCode::Bad, "" );
	int nGridCount = nRow * nCol;

	std::string strTextureFile = JsonGetString( jsonRoot, "Texture", "" );
	TSmartRefPtr<RHI::ITexture> pTexture = nullptr;
	if ( StringIsNullOrEmpty( strTextureFile.c_str() ) == true )
		return MakeResult( ResultCode::Bad, "" );
	CResult retLoadTexture = CApplication::sGetApp()->GetTextureManager()->GetOrCreateTextureA( strTextureFile.c_str(), &pTexture );
	if ( retLoadTexture.IsOK() == false )
		return retLoadTexture;
	

	if ( jsonRoot.contains( "Palette" ) == false )
		return MakeResult( ResultCode::Bad, "" );
	const nlohmann::json& jsonPalette = jsonRoot.at( "Palette" );
	if ( jsonPalette.is_null() == true || jsonPalette.is_array() == false )
		return MakeResult( ResultCode::Bad, "" );
	size_t nPaletteSize = jsonPalette.size();
	if ( nPaletteSize > 0 )
	{
		Palette.resize( nPaletteSize );
		CSize2f TextureSize = CSize2f( (float)pTexture->GetSize().w, (float)pTexture->GetSize().h );
		for ( size_t i = 0; i < nPaletteSize; i++ )
		{
			const nlohmann::json& jsonPaletteNode = jsonPalette.at( i );
			Palette[ i ].Color = StringConvertToColor( JsonGetString( jsonPaletteNode, "Color", "" ).c_str() );
			Palette[ i ].rcTexCoord = StringConvertToUV( JsonGetString( jsonPaletteNode, "UV", "" ).c_str(), TextureSize );
		}
	}



	const nlohmann::json& jsonGrids = jsonRoot.at( "Grids" );
	if ( jsonGrids.is_null() == true || jsonGrids.is_array() == false )
		return MakeResult( ResultCode::Bad, "" );
	if ( jsonGrids.size() < nGridCount )
		return MakeResult( ResultCode::Bad, "" );

	Grids.resize( nGridCount );
	for ( int i = 0; i < nGridCount; i++ )
	{
		Grids[ i ] = JsonGetInt32( jsonGrids, i, -1 );
	}

	pTerrain->SetTexture( pTexture );
	pTerrain->SetGrids( nRow, nCol, Palette.data(), Palette.size(), Grids.data() );

	return CResult();
}


