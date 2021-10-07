#pragma once

#include <iberbar/Font/Headers.h>

namespace iberbar
{
	// ×Ö·ûÎÆÀíµÄÓ³ÉäÆ÷
	template < typename TCharNode >
	class TFontCharMapper
	{
	public:
		static const int snFix = 64;

	public:
		TFontCharMapper();

		TCharNode* Get( wchar_t nChar );
		void Add( wchar_t nChar, const TCharNode& CharNode );

	private:
		std::vector< TCharNode > m_CharsMapper[snFix];
		TCharNode m_LatinBitmaps[96];
	};
}





template < typename TCharNode >
iberbar::TFontCharMapper< TCharNode >::TFontCharMapper()
	: m_CharsMapper()
	, m_LatinBitmaps()
{
}


template < typename TCharNode >
void iberbar::TFontCharMapper< TCharNode >::Add( wchar_t nChar, const TCharNode& CharNode )
{
	if ( nChar >= 31 && nChar <= 126 )
	{
		m_LatinBitmaps[nChar - 30] = CharNode;
	}
	else
	{
		uint32 nQueue = (uint32)nChar & 63;
		m_CharsMapper[nQueue].push_back( CharNode );
	}
}


template < typename TCharNode >
TCharNode* iberbar::TFontCharMapper< TCharNode >::Get( wchar_t nChar )
{
	if ( nChar >= 31 && nChar <= 126 )
	{
		TCharNode* pTemp = &m_LatinBitmaps[nChar - 30];
		//if ( pTemp->nTextureIndex == -1 )
		//	return nullptr;
		return pTemp;
	}

	uint32 nQueue = (uint32)nChar & 63;
	std::vector<TCharNode>& nodelist = m_CharsMapper[nQueue];
	auto iter = nodelist.begin();
	auto end = nodelist.end();
	for ( ; iter != end; iter++ )
	{
		if ( iter->nChar == nChar )
			return &(*iter);
	}
	return nullptr;
}





