

#include <iberbar/Poster/ResourcesManager.h>
#include <iberbar/Poster/Surface.h>
#include <iberbar/Utility/String.h>
#include <iberbar/Utility/Net/EasyWebRequestFactory.h>
#include <iberbar/Utility/Net/EasyWebRequest.h>

iberbar::Poster::CResourcesManager::~CResourcesManager()
{
	m_textureScale9s.Clear();
	m_surfaces.Clear();
}

iberbar::CResult iberbar::Poster::CResourcesManager::GetTextureFromFile( const char* name, CSurface** ppOut, bool cache )
{
	auto node = m_surfaces.find( name );
	if ( node == m_surfaces.end() )
	{
		CSurface* surface = nullptr;
		if ( tstring_start_with( name, "http" ) == true )
		{
			// 从http下载
			Net::CEasyWebRequestReference requestRef = Net::CEasyWebRequestFactory::sGetInstance()->GetRequest();
			if ( requestRef.Request() != nullptr )
			{
				Net::UEasyWebRequestOptions requestOptions;
				requestOptions.url = name;
				requestOptions.method = Net::UEasyWebRequestMethod::GET;
				requestOptions.ignoreSSL = true;
				requestRef.Request()->Send( requestOptions );
				const Net::CEasyWebResponse* response = requestRef.Request()->GetResponse();
				Net::CEasyWebResponse::_Binary responseBinary = response->ToBinary();
				if ( responseBinary.data != nullptr )
				{
					surface = new CSurface();
					surface->LoadFromFileMemory( responseBinary.data, responseBinary.size );
				}
			}
		}
		else
		{
			// 加载本地文件
			surface = new CSurface();
			surface->Load( name );
		}
		
		if ( cache == true )
		{
			m_surfaces.insert( std::make_pair( name, surface ) );
		}
		
		(*ppOut) = surface;
	}
	else
	{
		(*ppOut) = node->second;
		(*ppOut)->AddRef();
	}
	return CResult();
}

//iberbar::CResult iberbar::Poster::CResourcesManager::GetTextureFromUrl( const char* filepath, CSurface** ppOut )
//{
//	return CResult( ResultCode::Bad, "not implements" );
//}

iberbar::CResult iberbar::Poster::CResourcesManager::GetTextureScale9( const char* filepath, CTextureScale9** ppOut )
{
	auto node = m_textureScale9s.find( filepath );
	if ( node == m_textureScale9s.end() )
	{

	}
	else
	{

	}
	return CResult();
}

iberbar::CResult iberbar::Poster::CResourcesManager::GetFont( const char* fontName, int fontSize, int fontWeight, CFont** ppOut )
{
	UFontDesc fontDesc;
	fontDesc.FamilyName = fontName;
	fontDesc.Size = fontSize;
	fontDesc.Weight = fontWeight;
	fontDesc.Italic = false;
	return GetFont( fontDesc, ppOut );
}

iberbar::CResult iberbar::Poster::CResourcesManager::GetFont( const UFontDesc& fontDesc, CFont** ppOut )
{
	m_mutex.lock();

	CFont* font = nullptr;

	// 先查找字体
	auto iter = m_fonts.begin();
	auto end = m_fonts.end();
	for ( ; iter != end; iter++ )
	{
		const UFontDesc& fontDescTemp = (*iter)->GetFontDesc();
		if ( strcmp( fontDescTemp.FamilyName.c_str(), fontDesc.FamilyName.c_str() ) == 0 &&
			fontDescTemp.Size == fontDesc.Size &&
			fontDescTemp.Weight == fontDesc.Weight )
		{
			font = (*iter);
			break;
		}
	}

	// 创建新的字体
	if ( font == nullptr )
	{
		CFontFaceFreeType* fontFace = nullptr;

		if ( m_fontDevice->CreateFontFace( fontDesc, &fontFace ) == true )
		{
			font = new CFont( fontFace );
			m_fonts.push_back( font );
		}
	}

	// 成功获取字体后，返回时增加引用计数
	if ( font != nullptr )
	{
		if ( ppOut != nullptr )
		{
			if ( (*ppOut) != nullptr )
				(*ppOut)->Release();
			(*ppOut) = font;
			font->AddRef();
		}
	}

	m_mutex.unlock();

	return CResult();
}

void iberbar::Poster::CResourcesManager::GC( bool )
{
	//auto node = m_surfaces.begin();
	//auto end = m_surfaces.end();
	//for ( ; node != end; node++ )
	//{
	//	node->second->release();
	//	node->second = nullptr;
	//}
	m_surfaces.Clear();
}


//void iberbar::Poster::CResourcesManager::sInit()
//{
//	if ( s_instance == nullptr )
//	{
//		s_instance = new CResourcesManager();
//	}
//}
//
//void iberbar::Poster::CResourcesManager::sDestory()
//{
//	if ( s_instance )
//	{
//		delete s_instance;
//		s_instance = nullptr;
//	}
//}