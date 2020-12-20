
#include <iberbar/GameEngine/ResourcePreloader.h>
#include <iberbar/GameEngine/BaseResourceManager.h>
#include <iberbar/Utility/String.h>
#include <iberbar/Utility/Xml/Base.h>
#include <iberbar/Utility/Xml/RapidXml.h>



namespace iberbar
{
	namespace Game
	{
		class CResourceJsonReader
		{
		public:
			CResourceJsonReader( CResourcePreloader* pPreloader )
				: m_pPreloader( pPreloader )
			{
			}
		public:
			CResult Read( const char* strFile );

		private:
			CResourcePreloader* m_pPreloader;
		};

		class CResourceXmlReader
		{
		public:
			CResourceXmlReader( CResourcePreloader* pPreloader )
				: m_pPreloader( pPreloader )
			{
			}

		public:
			CResult Read( const char* strFile );
		private:
			void ReadInner( Xml::PTR_CNodeA pXmlNode );
			void ReadFontNode( Xml::PTR_CNodeA pXmlNode );
			void ReadTextureNode( Xml::PTR_CNodeA pXmlNode );

		private:
			CResourcePreloader* m_pPreloader;
		};
	}
}


iberbar::Game::CResourcePreloader::CResourcePreloader()
	: m_LoadTexture()
	, m_LoadFont()
{
}


iberbar::CResult iberbar::Game::CResourcePreloader::ReadFile( const char* strFile )
{
	std::string strFileFull = CResourceFileSystem::GetResoucePath( strFile );
	size_t npos = strFileFull.rfind( '.' );
	if ( npos == std::string::npos )
		return MakeResult( ResultCode::Bad, "Invalid filepath" );

	std::string strExt = strFileFull.substr( npos );
	char* ptr = &strExt.front();
	while ( *ptr != 0 )
	{
		*ptr = tolower( *ptr );
		ptr++;
	}

	CResult ret;

	if ( strExt == ".json" )
	{
		CResourceJsonReader reader( this );
		ret = reader.Read( strFileFull.c_str() );
	}
	else if ( strExt == ".xml" )
	{
		CResourceXmlReader reader( this );
		ret = reader.Read( strFileFull.c_str() );
	}
	else
	{
		ret = MakeResult( ResultCode::Bad, "Invalid filepath" );
	}

	return ret;
}



iberbar::CResult iberbar::Game::CResourceXmlReader::Read( const char* strFile )
{
	CResult ret;

	Xml::PTR_CDocumentA pXmlDoc = Xml::CreateRapidXmlDocumentA();
	ret = pXmlDoc->LoadFromFile( strFile );
	if ( ret.IsOK() == false )
		return ret;

	Xml::PTR_CNodeA pNode_Root;
	pXmlDoc->GetRoot( &pNode_Root );
	if ( pNode_Root == nullptr )
		return MakeResult( ResultCode::Bad, "" );

	Xml::PTR_CNodeA pXmlNode_Preload;
	pNode_Root->SelectSingleNode( "Preload", &pXmlNode_Preload );
	if ( pXmlNode_Preload != nullptr )
	{
		ReadInner( pXmlNode_Preload );
	}

	return CResult();
}


void iberbar::Game::CResourceXmlReader::ReadInner( Xml::PTR_CNodeA pXmlNode )
{
	Xml::PTR_CNodeListA pXmlNodeList;

	pXmlNode->SelectNodes( "Font", &pXmlNodeList );
	if ( pXmlNodeList && pXmlNodeList->GetNodeCount() > 0 )
	{
		Xml::PTR_CNodeA pXmlNode_Font;
		for ( int i = 0; i < pXmlNodeList->GetNodeCount(); i++ )
		{
			pXmlNodeList->GetNodeAt( i, &pXmlNode_Font );
			ReadFontNode( pXmlNode_Font );
		}
	}

	pXmlNode->SelectNodes( "Texture", &pXmlNodeList );
	if ( pXmlNodeList && pXmlNodeList->GetNodeCount() > 0 )
	{
		Xml::PTR_CNodeA pXmlNode_Texture;
		for ( int i = 0; i < pXmlNodeList->GetNodeCount(); i++ )
		{
			pXmlNodeList->GetNodeAt( i, &pXmlNode_Texture );
			ReadTextureNode( pXmlNode_Texture );
		}
	}
}


void iberbar::Game::CResourceXmlReader::ReadFontNode( Xml::PTR_CNodeA pXmlNode )
{
	const char* strFamily = pXmlNode->GetAttribute( "Family" );
	const char* strSize = pXmlNode->GetAttribute( "Size" );
	const char* strWeight = pXmlNode->GetAttribute( "Weight" );
	const char* strVocabular = pXmlNode->GetAttribute( "Vocabular" );
	if ( StringIsNullOrEmpty( strFamily ) == true )
	{
		return;
	}
	if ( StringIsNullOrEmpty( strSize ) == true )
	{
		return;
	}
	CResourcePreloader::ULoadFontContext Context;
	Context.strFamily = strFamily;
	Context.nSize = strtol( strSize, nullptr, 10 );
	Context.bIsBold = false;
	if ( StringIsNullOrEmpty( strWeight ) == false )
	{
		if ( strcmp( strWeight, "Bold" ) == 0 )
			Context.bIsBold = true;
	}
	m_pPreloader->m_LoadFont( Context );
}


void iberbar::Game::CResourceXmlReader::ReadTextureNode( Xml::PTR_CNodeA pXmlNode )
{
	const char* strFile = pXmlNode->GetAttribute( "File" );
	CResourcePreloader::ULoadTextureContext Context;
	Context.strFile = strFile;
	m_pPreloader->m_LoadTexture( Context );
}


iberbar::CResult iberbar::Game::CResourceJsonReader::Read( const char* strFile )
{
	return CResult();
}
