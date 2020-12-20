
//#include <iberbar/Font/CharMapper.h>

//
//
//iberbar::CFontCharMapper::CFontCharMapper()
//	: m_CharsMapper()
//	, m_LatinBitmaps()
//{
//}
//
//
//void iberbar::CFontCharMapper::AddCharBitmap( wchar_t nChar, const UFontCharBitmap& pBitmap )
//{
//	if ( nChar >= 31 && nChar <= 126 )
//	{
//		m_LatinBitmaps[ nChar - 30 ] = pBitmap;
//	}
//	else
//	{
//		uint32 nQueue = (uint32)nChar & 63;
//		_CharNode node;
//		node.nChar = nChar;
//		node.nDrawCount = 0;
//		node.pBitmap = pBitmap;
//		m_CharsMapper[ nQueue ].push_back( node );
//	}
//
//}
//
//
//const iberbar::UFontCharBitmap* iberbar::CFontCharMapper::GetCharBitmap( wchar_t nChar )
//{
//	if ( nChar >= 31 && nChar <= 126 )
//	{
//		const iberbar::UFontCharBitmap* pTemp = &m_LatinBitmaps[ nChar - 30 ];
//		if ( pTemp->nTextureIndex == -1 )
//			return nullptr;
//		return pTemp;
//	}
//
//	uint32 nQueue = (uint32)nChar & 63;
//	std::vector<_CharNode>& nodelist = m_CharsMapper[ nQueue ];
//	auto iter = nodelist.begin();
//	auto end = nodelist.end();
//	for ( ; iter != end; iter++ )
//	{
//		if ( iter->nChar == nChar )
//			return &(iter->pBitmap);
//	}
//	return nullptr;
//}
//
//
