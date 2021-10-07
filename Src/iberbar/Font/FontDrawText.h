

#include <iberbar/Font/Types.h>
#include <iberbar/Font/TextAdjust.h>
#include <iberbar/Utility/Math.h>


namespace iberbar
{





//	// 用户需要自己写这些类和函数的内容
//	class CxFontChBitmap;
//	int  XFontGetChWidth( CxFont* pFont );
//	int  XFontChBitmapGetCharWidth( CxFontChBitmap* pChBitmap );
//	int  XFontChBitmapGetBmpWidth( CxFontChBitmap* pChBitmap );
//	int  XFontChBitmapGetBmpHeight( CxFontChBitmap* pChBitmap );
	//void XFontChBitmapDraw( CxFontChBitmap* pChBitmap, const CRect2i* pRcDesc, const CRect2i* pRcSrc, const CColor4B& nFillColor, const CRect2i* pViewport );
	//void XFontChBitmapDraw( CxFontChBitmap* pChBitmap, const CPoint2i& ptDest, const CColor4B& color, const CRect2i* pRectViewport );
//
//	int XFontDrawText(
//		CxFont* pFont,
//		const CRect* pRcViewport,
//		const wchar_t* text,
//		int nCount,
//		__in __out CRect* pRect,
//		DWORD nFormat,
//		const Color4B& nFillColor );
//
//
//
//	static TGrowArray< int > s_TempTextLineLeft;
//	static TGrowArray< int > s_DT_TempTextCountPerLine;
//
//#define s_DT_TempTextLineWidth s_TempTextLineLeft
//




	template< typename TFont, typename TFontCharBitmap >
	void XFontDrawText_GetWord( TFont* pFont, int nFontSize, const wchar_t* word, int* pWordChCount, int* pWordChWidth );

	/*
		渲染文本

		- 模板参数:
		TFont 字体类型
		TFontCharBitmap 字体字符的纹理类型
		TRenderTarget 渲染目标类型

		- 方法参数:
		@pRenderTarget - 渲染目标
		@pFont - 字体对象
		@pRcViewport - 用于特殊裁剪，不需要裁剪时传nullptr
		@text - 文本
		@nCount - 渲染文本字符数，-1则全部
		@pRect - 渲染区域，如果特殊选项中设置计算矩形，则改参数会返回实际渲染后的区域
		@nFillColor - 填充颜色
		@options - 特殊选项
	*/
	template< typename TFont, typename TFontCharBitmap, typename TRenderTarget >
	int XFontDrawText(
		TRenderTarget* pRenderTarget,
		TFont* pFont,
		const CRect2i* pRcViewport,
		const wchar_t* text,
		int nCount,
		CRect2i* pRect,
		const CColor4B& nFillColor,
		const UFontDrawTextOptions& options );


	/*
	渲染文本

	- 模板参数:
	TFont 字体类型
	TFontCharBitmap 字体字符的纹理类型
	TRenderTarget 渲染目标类型

	- 方法参数:
	@pRenderTarget - 渲染目标
	@pFont - 字体对象
	@pRcViewport - 用于特殊裁剪，不需要裁剪时传nullptr
	@text - 文本
	@nCount - 渲染文本字符数，-1则全部
	@pRect - 渲染区域，如果特殊选项中设置计算矩形，则改参数会返回实际渲染后的区域
	@nFillColor - 填充颜色
	@options - 特殊选项
	*/
	template< typename TFont, typename TFontCharBitmap, typename TRenderTarget >
	int XFontDrawText_BreakWord(
		TRenderTarget* pRenderTarget,
		TFont* pFont,
		const CRect2i* pRcViewport,
		const wchar_t* text,
		int nCount,
		CRect2i* pRect,
		const CColor4B& nFillColor,
		const UFontDrawTextOptions& options );


	template< typename TFont, typename TFontCharBitmap, typename TRenderTarget >
	int XFontDrawText_BreakAll(
		TRenderTarget* pRenderTarget,
		TFont* pFont,
		const CRect2i* pRcViewport,
		const wchar_t* text,
		int nCount,
		CRect2i* pRect,
		const CColor4B& nFillColor,
		const UFontDrawTextOptions& options );



	/*
		单行快速渲染
	*/
	template< typename TFont, typename TFontCharBitmap, typename TRenderTarget >
	int XFontDrawText_SingleLine(
		TRenderTarget* pRenderTarget,
		TFont* pFont,
		const CRect2i* pRcViewport,
		const wchar_t* text,
		int nCount,
		CRect2i* pRect,
		const CColor4B& nFillColor,
		const UFontDrawTextOptions& nFormat );

	template< typename TFont, typename TFontCharBitmap, typename TRenderTarget >
	int XFontDrawText_MultiLines(
		TRenderTarget* pRenderTarget,
		TFont* pFont,
		const CRect2i* pRcViewport,
		const wchar_t* text,
		int nCount,
		CRect2i* pRect,
		const CColor4B& nFillColor,
		const UFontDrawTextOptions& options );

	template< typename TFont, typename TFontCharBitmap, typename TRenderTarget >
	int XFontDrawText_MultiLines_WithAdjustment(
		TRenderTarget* pRenderTarget,
		TFont* pFont,
		const CRect2i* pRcViewport,
		const wchar_t* text,
		int nCount,
		CRect2i* pRect,
		const CColor4B& nFillColor,
		const UFontDrawTextOptions& options,
		const UTextAdjustResult& adjustment );

	//int  XFontDrawText_WordBreak(
	//	CxFont* pFont,
	//	const CRect* pRcViewport,
	//	const wchar_t* text,
	//	int nCount,
	//	CRect* pRect,
	//	DWORD nFormat,
	//	const Color4B& nFillColor );

	//int  XFontDrawText_Normal(
	//	CxFont* pFont,
	//	const CRect* pRcViewport,
	//	const wchar_t* text,
	//	int nCount,
	//	CRect* pRect,
	//	DWORD nFormat,
	//	const Color4B& nFillColor );


	// 单个点拾取字符所在的Rect包围体
	template< typename TFont, typename TFontCharBitmap >
	int CalculateTextRectAtPoint(
		CRect2i* pOutRect,
		int* pOutCharIndex,
		TFont* pFont,
		const wchar_t* strText,
		const CRect2i& RectDraw,
		UAlignHorizental nTextAlign,
		const CPoint2i& Point );

	// 两个点拾取字符串，用于文字选择
	template< typename TFont, typename TFontCharBitmap >
	int CalculateTextRectAtPoints(
		CRect2i* pOutRect,
		int* pOutCharIndex1,
		int* pOutCharIndex2,
		TFont* pFont,
		const wchar_t* strText,
		const CRect2i& RectDraw,
		UAlignHorizental nTextAlign,
		const CPoint2i& Point1,
		const CPoint2i& Point2 );

	template< typename TFont, typename TFontCharBitmap >
	int CalculateTextRect(
		CRect2i* pOutRect,
		TFont* pFont,
		const wchar_t* strText,
		int nIndex,
		int nCount,
		const CRect2i& RectDraw,
		UAlignHorizental nTextAlign );





	enum _Ch_Type
	{
		Ch_Eng,
		Ch_Space,
		Ch_Else,
	};

	class CFontDrawTextOptionalModule_WorkBreak_Hyphenate
	{

	};


	//class CFontDrawTextBreakAllHelper
	//{
	//public:
	//	struct ULineInfo
	//	{
	//		int width;
	//		int charCount;
	//	};

	//public:
	//	CFontDrawTextBreakAllHelper(
	//		const CRect2i& rcRender );

	//private:

	//	int m_lineWordCount;   // 当前行的首个词
	//	int m_lineWidth;   // 当前行的宽度
	//	int m_lineCharCount;   // 当前行的字符数

	//	_Ch_Type m_wordCharType;
	//	int m_wordCharWidth;
	//	int m_wordCharCount;

	//	int m_lineCount;
	//	std::vector< ULineInfo > m_lines;
	//	CRect2i m_rcRender;

	//	//int nChOffset = 0;

	//	void AddChar( wchar_t character, int width );
	//	void AddChar_EndLine();
	//	void AddChar_WhiteSpace( int width );
	//	void Wrap();

	//	// 判断是否当前行的第一个单词或字符
	//	bool IsFirstWordAtLine();

	//	//int GetCurrentWordCharCount();
	//	// 弹出当前单词并返回单词的字符数
	//	int PopCurrentWord();

	//	int GetLineWidth();
	//};

	struct DT_TOOL_1
	{
	public:
		int  nLineCount;

		int      nTemp_Line_FirstWord;   // 当前行的首个词
		int      nTemp_Line_ChWidth;     // 当前行的宽度
		int      nTemp_Line_ChCount;     // 当前行的字符数
		int      nTemp_Word_ChCount;
		int      nTemp_Word_ChWidth;
		_Ch_Type nTemp_Word_ChType;

		std::vector< int >* pArrayLine_ChWidth;
		std::vector< int >* pArrayLine_ChCount;

		const wchar_t* mytext;

		int nChOffset = 0;

	public:
		void changeLine()
		{
			nLineCount ++;

			if ( pArrayLine_ChWidth->size() < (uint32)nLineCount )
				pArrayLine_ChWidth->push_back( 0 );
			pArrayLine_ChWidth->operator[]( nLineCount - 1 ) = nTemp_Line_ChWidth;

			if ( pArrayLine_ChCount->size() < (uint32)nLineCount )
				pArrayLine_ChCount->push_back( 0 );
			pArrayLine_ChCount->operator[]( nLineCount - 1 ) = nTemp_Line_ChCount;

			nTemp_Line_ChWidth = 0;
			nTemp_Line_ChCount = 0;
			nTemp_Line_FirstWord = 0;
			nTemp_Word_ChCount = 0;
			nTemp_Word_ChWidth = 0;
		}


		bool this_line_first_word() const
		{
			return nTemp_Line_FirstWord == 1;
		}

		void end_line_first_word()
		{
			nTemp_Line_FirstWord = false;
		}

	};
}






//FORCEINLINE iberbar::CFontDrawTextBreakAllHelper::CFontDrawTextBreakAllHelper(
//	const iberbar::CRect2i& rcRender )
//	: m_rcRender( rcRender )
//	, m_lineWordCount( 0 )
//	, m_lineWidth( 0 )
//	, m_lineCharCount( 0 )
//	, m_wordCharType( _Ch_Type::Ch_Else )
//	, m_wordCharCount( 0 )
//	, m_wordCharWidth( 0 )
//{
//}
//
//FORCEINLINE void iberbar::CFontDrawTextBreakAllHelper::AddChar( wchar_t character, int width )
//{
//	if ( (character >= L'a' && character <= L'z') ||
//		(character >= L'A' && character <= L'Z') )
//	{
//		// 连接成单词
//		if ( m_wordCharType != Ch_Eng || m_wordCharCount == 0 )
//		{
//			m_wordCharCount = 0;
//			m_wordCharWidth = 0;
//
//			m_lineWordCount++;
//		}
//		m_wordCharType = Ch_Eng;
//		m_wordCharCount++;
//		m_wordCharWidth += width;
//	}
//	else
//	{
//		// 中文和标点等符号不组成词语
//		m_wordCharType = Ch_Else;
//		m_wordCharCount = 1;
//		m_wordCharWidth += width;
//
//		m_lineWordCount++;
//	}
//
//	m_lineWidth += width;
//	m_lineCharCount++;
//}
//
//FORCEINLINE void iberbar::CFontDrawTextBreakAllHelper::AddChar_EndLine()
//{
//	m_lineCharCount++;
//	Wrap();
//}
//
//FORCEINLINE void iberbar::CFontDrawTextBreakAllHelper::AddChar_WhiteSpace( int width )
//{
//	m_wordCharType = Ch_Space;
//	m_wordCharCount = 1;
//	m_wordCharWidth = width;
//
//	m_lineWidth += width;
//	m_lineCharCount++; //空格也需要增进字符数量
//}
//
//FORCEINLINE void iberbar::CFontDrawTextBreakAllHelper::Wrap()
//{
//	m_lineCount++;
//
//	ULineInfo lineInfo;
//	lineInfo.width = m_lineWidth;
//	lineInfo.charCount = m_lineCharCount;
//	m_lines.push_back( lineInfo );
//
//	m_lineWidth = 0;
//	m_lineCharCount = 0;
//	m_lineWordCount = 0;
//	m_wordCharCount = 0;
//	m_wordCharWidth = 0;
//	m_wordCharType = _Ch_Type::Ch_Else;
//}
//
//FORCEINLINE int iberbar::CFontDrawTextBreakAllHelper::PopCurrentWord()
//{
//	m_lineCharCount -= m_wordCharCount;
//	m_lineWidth -= m_wordCharWidth;
//	return m_wordCharCount;
//}
//
//FORCEINLINE bool iberbar::CFontDrawTextBreakAllHelper::IsFirstWordAtLine()
//{
//	return m_lineWordCount == 1;
//}
//
//FORCEINLINE int iberbar::CFontDrawTextBreakAllHelper::GetLineWidth()
//{
//	return m_lineWidth;
//}


template< typename TFont, typename TFontCharBitmap >
void iberbar::XFontDrawText_GetWord( TFont* pFont, int nFontSize, const wchar_t* word, int* pWordChCount, int* pWordChWidth )
{
	assert( pWordChCount );
	assert( pWordChWidth );

	int lc_width = 0;
	int lc_length = 0;
	const wchar_t* lc_ptr = word;
	bool lc_bEndWord = false;
	const TFontCharBitmap* lc_pChBitmap = nullptr;

	while (
		(lc_ptr[0] >= L'a' && lc_ptr[0] <= L'z') ||
		(lc_ptr[0] >= L'A' && lc_ptr[0] <= L'Z') )
	{
		lc_pChBitmap = pFont->GetCharBitmap( lc_ptr[0] );
		if ( lc_pChBitmap )
			lc_width += lc_pChBitmap->GetCharWidth();
		else
			lc_width += nFontSize;
		lc_length++;
		lc_ptr++;
	}

	if ( lc_ptr[0] == L' ' )
	{
		int lc_ChSpaceWidth = 0;
		lc_pChBitmap = pFont->GetCharBitmap( lc_ptr[0] );
		if ( lc_pChBitmap )
			lc_ChSpaceWidth = lc_pChBitmap->GetCharWidth();
		else
			lc_ChSpaceWidth = nFontSize;
		while ( lc_ptr[0] == L' ' )
		{
			lc_width += lc_ChSpaceWidth;
			lc_length++;
			lc_ptr++;
		}
	}

	*pWordChCount = lc_length;
	*pWordChWidth = lc_width;
}



//
////------------------------------------------------------------------------------------------------
//int iberbar::XFontDrawText(
//	CxFont* pFont,
//	const CRect* pRcViewport,
//	const wchar_t* text,
//	int nCount,
//	CRect* pRect,
//	DWORD nFormat,
//	const Color4B& nFillColor )
//{
//	assert( pFont );
//
//	if ( text == nullptr || text[ 0 ] == 0 )
//		return 0;
//	if ( pRect == nullptr )
//		return 0;
//	if ( nFillColor.a == 0 )
//		return 0;
//
//	if ( tHasMask( nFormat, (DWORD)LILY_DT_SINGLELINE ) )
//		return XFontDrawText_SimgleLine( pFont, pRcViewport, text, nCount, pRect, nFormat, nFillColor );
//
//	if ( tHasMask( nFormat, (DWORD)LILY_DT_WORDBREAK ) )
//		return XFontDrawText_WordBreak( pFont, pRcViewport, text, nCount, pRect, nFormat, nFillColor );
//
//	return XFontDrawText_Normal( pFont, pRcViewport, text, nCount, pRect, nFormat, nFillColor );
//}
//
//



//------------------------------------------------------------------------------------------------
template < typename TFont, typename TFontCharBitmap, typename TRenderTarget >
int iberbar::XFontDrawText_SingleLine(
	TRenderTarget* pRenderTarget,
	TFont* pFont,
	const CRect2i* pRcViewport,
	const wchar_t* strText,
	int nCount,
	CRect2i* pRect,
	const CColor4B& nFillColor,
	const UFontDrawTextOptions& options )
{

	//-------------------------------------------
	//  准备临时变量
	//-------------------------------------------

	const UFontDesc& FontDesc = pFont->GetFontDesc();
	const int nFontHeight = FontDesc.Size;
	const int nLineHeight = (options.lineHeight <= 0) ? nFontHeight : options.lineHeight;

	int nCountVisitChar = 0;
	int nCountDrew = 0;
	nCount = ( nCount==-1 ) ? wcslen( strText ) : nCount;
	const wchar_t* lc_ptr = nullptr;

	const TFontCharBitmap* pBitmap = nullptr;

	int  lc_nLineWidth = 0;
	CRect2i RcTextLine;

	//-------------------------------------------
	//  计算边界
	//-------------------------------------------

	if ( options.alignVertical == UAlignVertical::Center )
		RcTextLine.t = ( ( pRect->t + pRect->b ) - nLineHeight) / 2;
	else if ( options.alignVertical == UAlignVertical::Bottom )
		RcTextLine.t = pRect->b - nLineHeight;
	else if ( options.alignVertical == UAlignVertical::Top )
		RcTextLine.t = pRect->t;
	RcTextLine.b = RcTextLine.t + nLineHeight;

	if ( options.calculateRect == true ||
		options.alignHorizental == UAlignHorizental::Center ||
		options.alignHorizental == UAlignHorizental::Right )
	{
		lc_ptr = strText;
		while ( lc_ptr[ 0 ] )
		{
			if ( *lc_ptr != L'\n' && *lc_ptr != L'\r' )
			{
				pBitmap = pFont->GetCharBitmap( *lc_ptr );

				if ( pBitmap )
					lc_nLineWidth += pBitmap->GetCharWidth();
				else
					lc_nLineWidth += nFontHeight;
			}

			lc_ptr ++;
		} // end while

		if ( options.alignHorizental == UAlignHorizental::Center )
			RcTextLine.l = ( pRect->l + pRect->r - lc_nLineWidth ) / 2;
		else if ( options.alignHorizental == UAlignHorizental::Right )
			RcTextLine.l = pRect->r - lc_nLineWidth;
		else if ( options.alignHorizental == UAlignHorizental::Left )
			RcTextLine.l = pRect->l;
	}
	else if ( options.alignHorizental == UAlignHorizental::Left )
	{
		RcTextLine.l = pRect->l;
	}
	RcTextLine.r = RcTextLine.l + lc_nLineWidth;


	//-------------------------------------------
	//  渲染文本
	//-------------------------------------------

	if ( options.calculateRect == true )
	{
		*pRect = RcTextLine;
	}

	if ( options.bRenderText == false )
		return 0;

	TextDraw::TContextBeginDrawCharBitmaps<TFont> ContextBeginDraw;
	ContextBeginDraw.pFont = pFont;
	ContextBeginDraw.pRcViewport = pRcViewport;
	ContextBeginDraw.strText = strText;
	ContextBeginDraw.nCount = nCount;
	ContextBeginDraw.Rect = RcTextLine;
	ContextBeginDraw.FillColor = nFillColor;
	ContextBeginDraw.pOptions = &options;
	pRenderTarget->BeginDrawCharBitmaps( (const TextDraw::TContextBeginDrawCharBitmaps<TFont>*)(&ContextBeginDraw) );

	CPoint2i PointDrawAt;
	PointDrawAt.x = RcTextLine.l;
	PointDrawAt.y = RcTextLine.t;

	TextDraw::TContextDrawCharBitmap<TFont, TFontCharBitmap> ContextDraw;
	ContextDraw.FillColor = nFillColor;
	ContextDraw.nLine = 0;
	ContextDraw.pFont = pFont;
	ContextDraw.pRcViewport = pRcViewport;
	ContextDraw.pPointDrawAt = &PointDrawAt;

	lc_ptr = strText;

	while ( lc_ptr[0] && nCountVisitChar < nCount )
	{
		if ( pRcViewport != nullptr && PointDrawAt.x > pRcViewport->r )
			break;

		if ( *lc_ptr == L'\n' || *lc_ptr == L'\r' )
			break;

		pBitmap = pFont->GetCharBitmap( *lc_ptr );
		ContextDraw.pBitmap = pBitmap;
		ContextDraw.nIndexAtText = nCountVisitChar;
		ContextDraw.nChar = *lc_ptr;
		pRenderTarget->DrawCharBitmap( (const TextDraw::TContextDrawCharBitmap<TFont, TFontCharBitmap>*)(&ContextDraw) );

		if ( pBitmap == nullptr )
			PointDrawAt.x += nFontHeight;
		else
			PointDrawAt.x += pBitmap->GetCharWidth();

		nCountDrew++;

		//if ( *lc_ptr != L'\n' && *lc_ptr != L'\r' )
		//{
		//	pBitmap = pFont->GetCharBitmap( *lc_ptr );

		//	//if ( pBitmap )
		//	//{
		//	//	ContextDraw.pBitmap = pBitmap;
		//	//	ContextDraw.nIndexAtText = nCountVisitChar;
		//	//	ContextDraw.nChar = *lc_ptr;
		//	//	pRenderTarget->DrawCharBitmap( (const TextDraw::TContextDrawCharBitmap<TFont, TFontCharBitmap>*)(&ContextDraw) );

		//	//	PointDrawAt.x += pBitmap->GetCharWidth();

		//	//	nCountDrew++;
		//	//}
		//	//else
		//	//{
		//	//	PointDrawAt.x += nFontHeight;
		//	//}
		//	ContextDraw.pBitmap = pBitmap;
		//	ContextDraw.nIndexAtText = nCountVisitChar;
		//	ContextDraw.nChar = *lc_ptr;
		//	pRenderTarget->DrawCharBitmap( (const TextDraw::TContextDrawCharBitmap<TFont, TFontCharBitmap>*)(&ContextDraw) );

		//	PointDrawAt.x += pBitmap->GetCharWidth();

		//	nCountDrew++;
		//}

		nCountVisitChar++;
		lc_ptr++;
	} // end while

	pRenderTarget->EndDrawCharBitmaps();

	return nCountDrew;
}


template< typename TFont, typename TFontCharBitmap, typename TRenderTarget >
int iberbar::XFontDrawText(
	TRenderTarget* pRenderTarget,
	TFont* pFont,
	const CRect2i* pRcViewport,
	const wchar_t* strText,
	int nCount,
	CRect2i* pRect,
	const CColor4B& nFillColor,
	const UFontDrawTextOptions& Options )
{
	if ( Options.nWrapType == UFontDrawTextWorkBreak::BreakAll )
	{

	}
	else if ( Options.nWrapType == UFontDrawTextWorkBreak::Hyphenate )
	{
		return XFontDrawText_BreakWord< TFont, TFontCharBitmap, TRenderTarget >( pRenderTarget, pFont, pRcViewport, strText, nCount, pRect, nFillColor, Options );
	}
	else
	{
		return XFontDrawText_SingleLine<TFont, TFontCharBitmap, TRenderTarget>( pRenderTarget, pFont, pRcViewport, strText, nCount, pRect, nFillColor, Options );
	}
	return 0;
}


//------------------------------------------------------------------------------------------------
template< typename TFont, typename TFontCharBitmap, typename TRenderTarget >
int iberbar::XFontDrawText_BreakWord(
	TRenderTarget* pRenderTarget,
	TFont* pFont,
	const CRect2i* pRcViewport,
	const wchar_t* text,
	int nCount,
	CRect2i* pRect,
	const CColor4B& nFillColor,
	const UFontDrawTextOptions& options )
{
	if ( pRect == nullptr ||
		pRect->l >= pRect->r ||
		pRect->t >= pRect->b )
		return 0;

	UFontDesc lc_FontDesc = pFont->GetFontDesc();
	const int lc_nFontHeight = lc_FontDesc.Size;
	const int lineHeight = (options.lineHeight > 0) ? options.lineHeight : lc_nFontHeight;

	if ( ( pRect->r - pRect->l ) <= lc_nFontHeight )
		return 0;

	std::vector< int > s_TempTextLineLeft;
	std::vector< int > s_DT_TempTextCountPerLine;

	DT_TOOL_1 lc_Tag;
	lc_Tag.nLineCount = 0;
	lc_Tag.nTemp_Line_ChCount = 0;
	lc_Tag.nTemp_Line_ChWidth = 0;
	lc_Tag.nTemp_Line_FirstWord = 0;
	lc_Tag.nTemp_Word_ChCount = 0;
	lc_Tag.nTemp_Word_ChType = Ch_Eng;
	lc_Tag.nTemp_Word_ChWidth = 0;
	lc_Tag.pArrayLine_ChWidth = &s_TempTextLineLeft;
	lc_Tag.pArrayLine_ChCount = &s_DT_TempTextCountPerLine;
	lc_Tag.mytext = text;
	lc_Tag.nChOffset = 0;



	//-------------------------------------------
	//  准备临时变量
	//-------------------------------------------



	uint32 lc_nDrawCount = 0;
	uint32 lc_nSucDrawCount = 0;
	uint32 lc_nDrawCountMax = ( nCount==-1 ) ? 0xffffffff : (uint32)nCount;
	const wchar_t* lc_ptr = nullptr;

	const TFontCharBitmap* pBitmap = nullptr;

	int lc_nLineWidth = 0;
	int lc_nTempChWidth = 0;
	

	const wchar_t* lc_ptrWordBegin = nullptr;


	//-------------------------------------------
	//  扫描换行符和需要断开换行，得到行数和每行的宽度
	//-------------------------------------------
	lc_ptr = text;

	while ( lc_ptr[ 0 ] )
	{
		if ( lc_ptr[ 0 ] == L'\n' ||
			lc_ptr[ 0 ] == L'\r' )
		{
			lc_Tag.nTemp_Line_ChCount ++;
			lc_Tag.changeLine();
		}
		else
		{
			pBitmap = pFont->GetCharBitmap( *lc_ptr );

			// 先记录字符逻辑宽度
			if ( pBitmap )
				lc_nTempChWidth = pBitmap->GetCharWidth();
			else
				lc_nTempChWidth = lc_nFontHeight;

			// 当改行的字符超出边界时
			if ( ( lc_Tag.nTemp_Line_ChWidth + lc_nTempChWidth ) > ( pRect->r-pRect->l ) )
			{
				bool lc_bChangeLine = false;
				// 如果当前字符为英文
				if ( ( lc_ptr[ 0 ] >= L'a' && lc_ptr[ 0 ] <= L'z' ) ||
					( lc_ptr[ 0 ] >= L'A' && lc_ptr[ 0 ] <= L'Z' ) )
				{
					// 连接成单词
					if ( lc_Tag.nTemp_Word_ChType != Ch_Eng )
						lc_Tag.nTemp_Word_ChCount = 0;
					lc_Tag.nTemp_Word_ChType = Ch_Eng;

					// 如果一个英文单词宽度大于rect宽度而且是第一个单词，先写完再换行
					if ( lc_Tag.this_line_first_word() == true )
					{
						int lc_nTempWordChCount = 0;
						int lc_nTempWordWidth = 0;
						XFontDrawText_GetWord<TFont, TFontCharBitmap>( pFont, lc_nFontHeight, lc_ptr, &lc_nTempWordChCount, &lc_nTempWordWidth );
						if ( ( lc_nTempWordWidth + lc_Tag.nTemp_Word_ChWidth ) > ( pRect->r-pRect->l ) )
						{
							lc_Tag.nTemp_Line_ChCount += lc_nTempWordChCount;
							lc_Tag.nTemp_Line_ChWidth += lc_nTempWordWidth;
							lc_ptr += lc_nTempWordChCount - 1;

							if ( lc_ptr[ 0 ] == L'\n' || lc_ptr[ 0 ] == L'\r' )
							{
								lc_Tag.nTemp_Line_ChCount ++; //空格也需要增进字符数量
								lc_ptr ++;
							}
						}
					}
					else
					{
						lc_ptr -= lc_Tag.nTemp_Word_ChCount + 1;
						lc_Tag.nTemp_Line_ChCount -= lc_Tag.nTemp_Word_ChCount;
						lc_Tag.nTemp_Line_ChWidth -= lc_Tag.nTemp_Word_ChWidth;
					}

					lc_Tag.changeLine();
				}
				// 如果当前字符为空格
				else if ( lc_ptr[ 0 ] == L' ' )
				{
					// 空格符超出范围不换行，知道空格结束
					while ( lc_ptr[ 1 ] == L' ' )
					{
						lc_Tag.nTemp_Line_ChWidth += lc_nTempChWidth;
						lc_Tag.nTemp_Line_ChCount ++; //空格也需要增进字符数量

						lc_ptr ++;
					}
					if ( lc_ptr[ 0 ] == L'\n' || lc_ptr[ 0 ] == L'\r' )
					{
						lc_Tag.nTemp_Line_ChWidth += lc_nFontHeight;
						lc_Tag.nTemp_Line_ChCount ++; //空格也需要增进字符数量

						lc_ptr ++;
					}

					// 不组成词语
					lc_Tag.nTemp_Word_ChType = Ch_Space;
					lc_Tag.nTemp_Word_ChCount = 1;

					// 空格结束后换行
					lc_Tag.nTemp_Line_ChCount += lc_Tag.nTemp_Word_ChCount;

					lc_Tag.changeLine();
				}
				// 如果当前字符为其他字符
				else
				{
					// 中文和标点等符号不组成词语
					lc_Tag.nTemp_Word_ChType = Ch_Else;
					lc_Tag.nTemp_Word_ChCount = 1;

					lc_ptr -= lc_Tag.nTemp_Word_ChCount;

					lc_Tag.changeLine();
				}
			}

			// 没有超出边界
			else
			{
				if ( ( lc_ptr[ 0 ] >= L'a' && lc_ptr[ 0 ] <= L'z' ) ||
					( lc_ptr[ 0 ] >= L'A' && lc_ptr[ 0 ] <= L'Z' ) )
				{
					// 连接成单词
					if ( lc_Tag.nTemp_Word_ChType != Ch_Eng || lc_Tag.nTemp_Word_ChCount == 0 )
					{
						lc_Tag.nTemp_Word_ChCount = 0;
						lc_Tag.nTemp_Word_ChWidth = 0;

						lc_Tag.nTemp_Line_FirstWord ++;
					}
					lc_Tag.nTemp_Word_ChType = Ch_Eng;
					lc_Tag.nTemp_Word_ChCount ++;
					lc_Tag.nTemp_Word_ChWidth += lc_nTempChWidth;
				}
				// 如果当前字符为空格
				else if ( lc_ptr[ 0 ] == L' ' )
				{
					// 不组成词语
					lc_Tag.nTemp_Word_ChType = Ch_Space;
					lc_Tag.nTemp_Word_ChCount = 1;
					lc_Tag.nTemp_Word_ChWidth = lc_nTempChWidth;
				}
				// 如果当前字符为其他字符
				else
				{
					// 中文和标点等符号不组成词语
					lc_Tag.nTemp_Word_ChType = Ch_Else;
					lc_Tag.nTemp_Word_ChCount = 1;
					lc_Tag.nTemp_Word_ChWidth = lc_nTempChWidth;

					lc_Tag.nTemp_Line_FirstWord ++;
				}

				lc_Tag.nTemp_Line_ChWidth += lc_nTempChWidth;
				lc_Tag.nTemp_Line_ChCount ++;
			}
		}

		lc_ptr ++;
	} // end while

	//-------------------------------------------
	//  最后一行
	//-------------------------------------------
	lc_Tag.changeLine();



	//-------------------------------------------
	//  渲染文本
	//-------------------------------------------
	CRect2i  lc_rcText;
	CRect2i  lc_rcSrc;
	CRect2i  lc_rcBounding;
	CRect2i  lc_rcViewport;
	int lc_nDrawLine = 0;

	if ( pRcViewport )
	{
		lc_rcViewport.l = tMax( pRcViewport->l, pRect->l );
		lc_rcViewport.t = tMax( pRcViewport->t, pRect->t );
		lc_rcViewport.r = tMin( pRcViewport->r, pRect->r );
		lc_rcViewport.b = tMin( pRcViewport->b, pRect->b );
	}
	else
	{
		lc_rcViewport = *pRect;
	}

	lc_rcText.l = pRect->l;
	lc_rcText.t = pRect->t;
	lc_rcText.b = lc_rcText.t + lc_nFontHeight;

	lc_ptr = text;
	int lc_nChRest = lc_Tag.pArrayLine_ChCount->operator[]( lc_nDrawLine );
	while ( lc_ptr[ 0 ] )
	{
		if ( lc_ptr[ 0 ] == L'\n' ||
			lc_ptr[ 0 ] == L'\r' )
		{
			lc_nDrawLine ++;
			lc_rcText.l = pRect->l;
			lc_rcText.t += lineHeight;
			lc_rcText.b = lc_rcText.t + lc_nFontHeight;

			if ( lc_nDrawLine < lc_Tag.nLineCount )
				lc_nChRest = lc_Tag.pArrayLine_ChCount->operator[]( lc_nDrawLine );
		}
		else
		{
			pBitmap = pFont->GetCharBitmap( *lc_ptr );

			if ( pBitmap )
			{
				lc_rcText.r = lc_rcText.l + pBitmap->GetCharWidth();

				//pRenderTarget->DrawCharBitmap( lc_nDrawCount, *lc_ptr, pFont, pBitmap, CPoint2i( lc_rcText.l, lc_rcText.t ), nFillColor, pRcViewport );
			}
			else
			{
				lc_rcText.r = lc_rcText.l + lc_nFontHeight;
			}

			lc_rcText.l = lc_rcText.r;

			lc_nChRest --;

			if ( lc_nChRest == 0 )
			{
				lc_nDrawLine ++;
				lc_rcText.l = pRect->l;
				lc_rcText.t += lineHeight;
				lc_rcText.b = lc_rcText.t + lc_nFontHeight;

				if ( lc_nDrawLine < lc_Tag.nLineCount )
					lc_nChRest = lc_Tag.pArrayLine_ChCount->operator[]( lc_nDrawLine );
			}
		}

		lc_nDrawCount ++;
		lc_ptr ++;
	}


	return lc_nDrawCount;
}




//------------------------------------------------------------------------------------------------
template< typename TFont, typename TFontCharBitmap, typename TRenderTarget >
int iberbar::XFontDrawText_MultiLines_WithAdjustment(
	TRenderTarget* pRenderTarget,
	TFont* pFont,
	const CRect2i* pRcViewport,
	const wchar_t* text,
	int nCount,
	CRect2i* pRect,
	const CColor4B& nFillColor,
	const UFontDrawTextOptions& options,
	const UTextAdjustResult& adjustment )
{
	if ( pRect == nullptr ||
		pRect->l >= pRect->r ||
		pRect->t >= pRect->b )
		return 0;

	const UFontDesc& fontDesc = pFont->GetFontDesc();
	const int fontSize = fontDesc.Size;
	const int lineHeight = (options.lineHeight > 0) ? options.lineHeight : fontSize;

	if ( (pRect->r - pRect->l) <= fontSize )
		return 0;


	CRect2i rcText = *pRect;
	RectOffsetY( &rcText, ( lineHeight - fontSize ) >> 1 );

	const std::vector<UTextLineInfo>& lines = adjustment.lines;

	int drawCount = 0;
	int currentLine = 0;
	int drawLines = 0;
	int drawLinesMax = tMin( (int)lines.size(), ( options.maxLine > 0 ) ? options.maxLine : INT_MAX );
	const UTextLineInfo* lineInfo = nullptr;
	const wchar_t* ptr = text;
	TFontCharBitmap* bitmap = nullptr;
	for ( ; currentLine < drawLinesMax; currentLine++ )
	{
		lineInfo = &lines[currentLine];
		for ( int i = 0; i < lineInfo->charCount; i++ )
		{
			bitmap = pFont->GetCharBitmap( ptr[i] );
			if ( bitmap == nullptr )
			{
				rcText.l += fontSize;
			}
			else
			{
				bitmap->Draw( pRenderTarget, CPoint2i( rcText.l, rcText.t ), nFillColor, pRcViewport );
				rcText.l += bitmap->GetCharWidth();
			}
		}
		rcText.l = pRect->l;
		rcText.t += lineHeight;
		drawCount += lineInfo->charCount;
		ptr += lineInfo->charCount;
		drawLines++;
	}

	if ( adjustment.overflow == true )
	{
		const wchar_t* ellipsisText = options.ellipsis;
		while ( ellipsisText[0] != 0 )
		{
			bitmap = pFont->GetCharBitmap( ptr[0] );
			if ( bitmap == nullptr )
			{
				rcText.l += fontSize;
			}
			else
			{
				bitmap->Draw( pRenderTarget, CPoint2i( rcText.l, rcText.t ), nFillColor, pRcViewport );
				rcText.l += bitmap->GetCharWidth();
			}
			ellipsisText++;
		}
	}

	return drawCount;

	//-------------------------------------------
	//  准备临时变量
	//-------------------------------------------



	//uint32 lc_nDrawCount = 0;
	//uint32 lc_nSucDrawCount = 0;
	//uint32 lc_nDrawCountMax = (nCount == -1) ? 0xffffffff : (uint32)nCount;
	//const wchar_t* lc_ptr = nullptr;

	//TFontCharBitmap* lc_pChBitmap = nullptr;

	//int lc_nLineWidth = 0;
	//int lc_nTempChWidth = 0;


	//const wchar_t* lc_ptrWordBegin = nullptr;


	////-------------------------------------------
	////  扫描换行符和需要断开换行，得到行数和每行的宽度
	////-------------------------------------------
	//lc_ptr = text;

	//int rcWidth = pRect->Width();

	//wchar_t ch = 0;

	//TTextAdjustSolution<TFont, TFontCharBitmap> solution;
	//solution.Adjust( pRect );
	//const std::vector<TTextAdjustSolution<TFont, TFontCharBitmap>::ULineInfo> lines = solution.GetLines();
	//int lineCount = (int)lines.size();

	////-------------------------------------------
	////  渲染文本
	////-------------------------------------------
	//CRect2i  lc_rcText;
	//CRect2i  lc_rcSrc;
	//CRect2i  lc_rcBounding;
	//CRect2i  lc_rcViewport;
	//int lc_nDrawLine = 0;

	//if ( pRcViewport )
	//{
	//	lc_rcViewport.l = tMax( pRcViewport->l, pRect->l );
	//	lc_rcViewport.t = tMax( pRcViewport->t, pRect->t );
	//	lc_rcViewport.r = tMin( pRcViewport->r, pRect->r );
	//	lc_rcViewport.b = tMin( pRcViewport->b, pRect->b );
	//}
	//else
	//{
	//	lc_rcViewport = *pRect;
	//}

	//lc_rcText.l = pRect->l;
	//lc_rcText.t = pRect->t;
	//lc_rcText.b = lc_rcText.t + lc_nFontHeight;

	//lc_ptr = text;
	//int lc_nChRest = lines->operator[]( lc_nDrawLine );
	//while ( lc_ptr[0] )
	//{
	//	if ( lc_ptr[0] == L'\n' ||
	//		lc_ptr[0] == L'\r' )
	//	{
	//		lc_nDrawLine++;
	//		lc_rcText.l = pRect->l;
	//		lc_rcText.t += lineHeight;
	//		lc_rcText.b = lc_rcText.t + lc_nFontHeight;

	//		if ( lc_nDrawLine < lc_Tag.nLineCount )
	//			lc_nChRest = lc_Tag.pArrayLine_ChCount->operator[]( lc_nDrawLine );
	//	}
	//	else
	//	{
	//		lc_pChBitmap = pFont->GetChBitmap( *lc_ptr );

	//		if ( lc_pChBitmap )
	//		{
	//			lc_rcText.r = lc_rcText.l + lc_pChBitmap->GetCharWidth();

	//			//XFontChBitmapDraw( lc_pChBitmap, CPoint2i( lc_rcText.l, lc_rcText.t ), nFillColor, pRcViewport );
	//			lc_pChBitmap->Draw( pRenderTarget, CPoint2i( lc_rcText.l, lc_rcText.t ), nFillColor, pRcViewport );
	//		}
	//		else
	//		{
	//			lc_rcText.r = lc_rcText.l + lc_nFontHeight;
	//		}

	//		lc_rcText.l = lc_rcText.r;

	//		lc_nChRest--;

	//		if ( lc_nChRest == 0 )
	//		{
	//			lc_nDrawLine++;
	//			lc_rcText.l = pRect->l;
	//			lc_rcText.t += lineHeight;
	//			lc_rcText.b = lc_rcText.t + lc_nFontHeight;

	//			if ( lc_nDrawLine < lc_Tag.nLineCount )
	//				lc_nChRest = lc_Tag.pArrayLine_ChCount->operator[]( lc_nDrawLine );
	//		}
	//	}

	//	lc_nDrawCount++;
	//	lc_ptr++;
	//}


	//return lc_nDrawCount;
}











//
////------------------------------------------------------------------------------------------------
//int iberbar::XFontDrawText_Normal(
//	CxFont* pFont,
//	const CRect* pRcViewport,
//	const wchar_t* text,
//	int nCount,
//	CRect* pRect,
//	DWORD nFormat,
//	const Color4B& nFillColor )
//{
//	//-------------------------------------------
//	//  准备临时变量
//	//-------------------------------------------
//
//	FontDescW lc_FontDesc;
//	pFont->getFontDesc( &lc_FontDesc );
//	const int lc_nFontWidth = lc_FontDesc.Width;
//	const int lc_nFontHeight = lc_FontDesc.Height;
//
//	uint32 lc_nDrawCount = 0;
//	uint32 lc_nSucDrawCount = 0;
//	uint32 lc_nDrawCountMax = ( nCount==-1 ) ? 0xffffffff : (uint32)nCount;
//	const wchar_t* lc_ptr = nullptr;
//
//	CxFontChBitmap* lc_pChBitmap = nullptr;
//
//	int lc_nLineWidth = 0;
//	int lc_nTextLineCount = 0;
//	int lc_nTempTextLineWidth = 0;
//
//
//	//-------------------------------------------
//	//  扫描换行符，得到行数和每行的宽度
//	//-------------------------------------------
//	lc_nTempTextLineWidth = 0;
//	lc_nTextLineCount = 0;
//	lc_ptr = text;
//
//	if ( FAILED( pFont->beginDevice() ) )
//		return 0;
//
//	while ( lc_ptr[ 0 ] )
//	{
//		if ( lc_ptr[ 0 ] == L'\n' || lc_ptr[ 0 ] == L'\r' )
//		{
//			lc_nTextLineCount ++;
//			if ( lc_nTextLineCount > (int)s_TempTextLineLeft.size() )
//			{
//				s_TempTextLineLeft.push_back( 0 );
//			}
//			s_TempTextLineLeft[ lc_nTextLineCount - 1 ] = lc_nTempTextLineWidth;
//
//			lc_nTempTextLineWidth = 0;
//		}
//		else
//		{
//			lc_pChBitmap = pFont->getChBitmap( *lc_ptr );
//
//			if ( lc_pChBitmap )
//				lc_nTempTextLineWidth += XFontChBitmapGetCharWidth( lc_pChBitmap );
//			else
//				lc_nTempTextLineWidth += lc_nFontHeight;
//		}
//
//		lc_ptr ++;
//	} // end while
//
//
//	//-------------------------------------------
//	//  最后一行
//	//-------------------------------------------
//	lc_nTextLineCount ++;
//	if ( lc_nTextLineCount > (int)s_TempTextLineLeft.size() )
//	{
//		s_TempTextLineLeft.push_back( 0 );
//	}
//	s_TempTextLineLeft[ lc_nTextLineCount - 1 ] = lc_nTempTextLineWidth;
//
//
//	//-------------------------------------------
//	//  通过宽度变换成左边界
//	//-------------------------------------------
//	if ( tHasMask( nFormat, (DWORD)LILY_DT_RIGHT ) )
//	{
//		for ( int lc_i = 0; lc_i < lc_nTextLineCount; lc_i ++ )
//		{
//			s_TempTextLineLeft[ lc_i ] = pRect->right - s_TempTextLineLeft[ lc_i ];
//		}
//	}
//	else if ( tHasMask( nFormat, (DWORD)LILY_DT_CENTER ) )
//	{
//		int lc_nTempRectOrigin = ( pRect->left + pRect->right ) / 2;
//		for ( int lc_i = 0; lc_i < lc_nTextLineCount; lc_i ++ )
//		{
//			s_TempTextLineLeft[ lc_i ] = lc_nTempRectOrigin - s_TempTextLineLeft[ lc_i ] / 2;
//		}
//	}
//	else if ( tHasMask( nFormat, (DWORD)LILY_DT_LEFT ) )
//	{
//		for ( int lc_i=0; lc_i < lc_nTextLineCount; lc_i ++ )
//		{
//			s_TempTextLineLeft[ lc_i ] = pRect->left;
//		}
//	}
//
//
//	//-------------------------------------------
//	//  渲染文本
//	//-------------------------------------------
//	CRect  lc_rcText;
//	CRect  lc_rcSrc;
//	CRect  lc_rcBounding;
//	CRect  lc_rcViewport;
//	int lc_nDrawLine = 0;
//
//	if ( tHasMask( nFormat, (DWORD)LILY_DT_BOTTOM ) )
//	{
//		lc_rcText.top = pRect->bottom - lc_nTextLineCount * lc_nFontHeight;
//	}
//	else if ( tHasMask( nFormat, (DWORD)LILY_DT_VCENTER ) )
//	{
//		lc_rcText.top = ( pRect->top + pRect->bottom - lc_nTextLineCount * lc_nFontHeight ) / 2;
//	}
//	else if ( tHasMask( nFormat, (DWORD)LILY_DT_TOP ) )
//	{
//		lc_rcText.top = pRect->top;
//	}
//
//	lc_rcText.left = s_TempTextLineLeft[ lc_nDrawLine ];
//	lc_rcText.bottom = lc_rcText.top + lc_nFontHeight;
//
//	if ( pRcViewport )
//	{
//		lc_rcViewport.left   = tMax( pRcViewport->left, pRect->left );
//		lc_rcViewport.top    = tMax( pRcViewport->top, pRect->top );
//		lc_rcViewport.right  = tMin( pRcViewport->right, pRect->right );
//		lc_rcViewport.bottom = tMin( pRcViewport->bottom, pRect->bottom );
//	}
//	else
//	{
//		lc_rcViewport = *pRect;
//	}
//
//	lc_ptr = text;
//
//	while ( lc_ptr[ 0 ] && lc_nDrawCount < lc_nDrawCountMax )
//	{
//		if ( *lc_ptr == L'\n' || *lc_ptr == L'\r' )
//		{
//			lc_nDrawLine ++;
//			lc_rcText.left = s_TempTextLineLeft[ lc_nDrawLine ];
//			lc_rcText.top += lc_nFontHeight;
//			lc_rcText.bottom += lc_nFontHeight;
//		}
//		else
//		{
//			lc_pChBitmap = pFont->getChBitmap( *lc_ptr );
//
//			if ( lc_pChBitmap )
//			{
//				lc_rcText.right = lc_rcText.left + XFontChBitmapGetCharWidth( lc_pChBitmap );
//// 				lc_rcBounding = lc_rcText;
//// 				lc_rcBounding.right = lc_rcBounding.left + XFontChBitmapGetBmpWidth( lc_pChBitmap );
//// 				lc_rcBounding.bottom = lc_rcBounding.top + XFontChBitmapGetBmpHeight( lc_pChBitmap );
//				XFontChBitmapDraw( lc_pChBitmap, CPoint( lc_rcText.left, lc_rcText.top ), nFillColor, pRcViewport );
//				lc_nSucDrawCount ++;
//
//			}
//			else
//			{
//				lc_rcText.right = lc_rcText.left + lc_nFontHeight;
//			}
//
//			lc_rcText.left = lc_rcText.right;
//		}
//
//		lc_nDrawCount ++;
//		lc_ptr ++;
//	} // end while
//
//	pFont->endDevice();
//
//	return lc_nSucDrawCount;
//}