#pragma once

#include <iberbar/Font/Types.h>

namespace iberbar
{

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
	int CalculateTextRectAtCharIndex(
		CRect2i* pOutRect,
		TFont* pFont,
		const wchar_t* strText,
		const CRect2i& RectDraw,
		UAlignHorizental nTextAlign,
		int nCharIndex );
}




template< typename TFont, typename TFontCharBitmap >
int iberbar::CalculateTextRectAtPoint(
	CRect2i* pOutRect,
	int* pOutCharIndex,
	TFont* pFont,
	const wchar_t* strText,
	const CRect2i& RectDraw,
	UAlignHorizental nTextAlign,
	const CPoint2i& Point )
{
	assert( strText[0] != 0 );

	if ( RectPointHitTest( &RectDraw, &Point ) == false )
		return 0;

	UFontDesc FontDesc = pFont->GetFontDesc();
	const int nFontHeight = FontDesc.Size;
	const wchar_t* ptr = nullptr;
	const TFontCharBitmap* pBitmap = nullptr;

	int  nLineWidth = 0;
	CRect2i RectTextLine;

	//if ( nTextAlign == UAlignVertical::Center )
	//	RectTextLine.t = ((RectDraw.t + RectDraw.b) - nFontHeight) / 2;
	//else if ( nTextAlign == UAlignVertical::Bottom )
	//	RectTextLine.t = RectDraw.b - nFontHeight;
	//else if ( nTextAlign == UAlignVertical::Top )
	//	RectTextLine.t = RectDraw.t;

	// 单行渲染，不用管垂直对齐
	RectTextLine.t = RectDraw.t;
	RectTextLine.b = RectDraw.b;

	if ( nTextAlign == UAlignHorizental::Center || nTextAlign == UAlignHorizental::Right )
	{
		ptr = strText;
		while ( ptr[0] )
		{
			if ( *ptr != L'\n' && *ptr != L'\r' )
			{
				pBitmap = pFont->GetCharBitmap( *ptr );

				if ( pBitmap )
					nLineWidth += pBitmap->GetCharWidth();
				else
					nLineWidth += nFontHeight;
			}

			ptr++;
		} // end while

		if ( nTextAlign == UAlignHorizental::Center )
			RectTextLine.l = (RectDraw.l + RectDraw.r - nLineWidth) / 2;
		else
			RectTextLine.l = RectDraw.r - nLineWidth;

		RectTextLine.r = RectTextLine.l + nLineWidth;

		if ( RectPointHitTest( &RectTextLine, &Point ) == false )
		{
			wchar_t nCharTemp = 0;
			if ( Point.x < RectTextLine.l )
			{
				*pOutCharIndex = 0;
				*pOutRect = RectTextLine;
				nCharTemp = strText[ 0 ];
				if ( nCharTemp == '\n' || nCharTemp == '\r' )
				{
					pOutRect->r = pOutRect->l;
				}
				else
				{
					pBitmap = pFont->GetCharBitmap( nCharTemp );
					if ( pBitmap == nullptr )
					{
						pOutRect->r = pOutRect->l + nFontHeight;
					}
					else
					{
						pOutRect->r = pOutRect->l + pBitmap->GetCharWidth();
					}
				}

				return 1;
			}
			else if ( Point.x > RectTextLine.r )
			{
				*pOutCharIndex = wcslen( strText ) - 1;
				*pOutRect = RectTextLine;
				nCharTemp = strText[ *pOutCharIndex ];
				if ( nCharTemp == L'\n' || nCharTemp == L'\r' )
				{
					pOutRect->l = pOutRect->r;
				}
				else
				{
					pBitmap = pFont->GetCharBitmap( nCharTemp );
					if ( pBitmap == nullptr )
					{
						pOutRect->l = pOutRect->r - nFontHeight;
					}
					else
					{
						pOutRect->l = pOutRect->r - pBitmap->GetCharWidth();
					}
				}

				return 1;
			}

			return 0;
		}
	}
	else if ( nTextAlign == UAlignHorizental::Left )
	{
		RectTextLine.l = RectDraw.l;
	}




	CRect2i RectChar;
	RectChar.l = RectTextLine.l;
	RectChar.t = RectTextLine.t;
	RectChar.b = RectChar.t + nFontHeight;

	ptr = strText;

	int nCharIndex = 0;
	while ( ptr[0] )
	{
		if ( *ptr != L'\n' && *ptr != L'\r' )
		{
			pBitmap = pFont->GetCharBitmap( *ptr );

			if ( pBitmap )
			{
				RectChar.r = RectChar.l + pBitmap->GetCharWidth();
			}
			else
			{
				RectChar.r = RectChar.l + nFontHeight;
			}

			if ( RectChar.l <= Point.x && RectChar.r >= Point.x )
			{
				break;
			}

			RectChar.l = RectChar.r;
			//if ( RectChar.l >= RectViewport.r )
			//	break;
		}

		nCharIndex++;
		ptr++;
	} // end while

	*pOutCharIndex = nCharIndex;
	*pOutRect = RectChar;

	return 1;
}




template< typename TFont, typename TFontCharBitmap >
int iberbar::CalculateTextRectAtPoints(
	CRect2i* pOutRect,
	int* pOutCharIndex1,
	int* pOutCharIndex2,
	TFont* pFont,
	const wchar_t* strText,
	const CRect2i& RectDraw,
	UAlignHorizental nTextAlign,
	const CPoint2i& Point1,
	const CPoint2i& Point2 )
{
	return 0;
}





template< typename TFont, typename TFontCharBitmap >
int iberbar::CalculateTextRectAtCharIndex(
	CRect2i* pOutRect,
	TFont* pFont,
	const wchar_t* strText,
	const CRect2i& RectDraw,
	UAlignHorizental nTextAlign,
	int nCharIndex )
{
	UFontDesc FontDesc = pFont->GetFontDesc();
	const int nFontHeight = FontDesc.Size;
	const wchar_t* ptr = nullptr;
	const TFontCharBitmap* pBitmap = nullptr;

	int  nLineWidth = 0;
	CRect2i RectTextLine;

	//if ( nTextAlign == UAlignVertical::Center )
	//	RectTextLine.t = ((RectDraw.t + RectDraw.b) - nFontHeight) / 2;
	//else if ( nTextAlign == UAlignVertical::Bottom )
	//	RectTextLine.t = RectDraw.b - nFontHeight;
	//else if ( nTextAlign == UAlignVertical::Top )
	//	RectTextLine.t = RectDraw.t;

	// 单行渲染，不用管垂直对齐
	RectTextLine.t = RectDraw.t;
	RectTextLine.b = RectDraw.b;

	if ( nTextAlign == UAlignHorizental::Center || nTextAlign == UAlignHorizental::Right )
	{
		ptr = strText;
		while ( ptr[0] )
		{
			if ( *ptr != L'\n' && *ptr != L'\r' )
			{
				pBitmap = pFont->GetCharBitmap( *ptr );

				if ( pBitmap )
					nLineWidth += pBitmap->GetCharWidth();
				else
					nLineWidth += nFontHeight;
			}

			ptr++;
		} // end while

		if ( nTextAlign == UAlignHorizental::Center )
			RectTextLine.l = (RectDraw.l + RectDraw.r - nLineWidth) / 2;
		else
			RectTextLine.l = RectDraw.r - nLineWidth;
	}
	else if ( nTextAlign == UAlignHorizental::Left )
	{
		RectTextLine.l = RectDraw.l;
	}



	CRect2i RectChar = CRect2i(
		RectTextLine.l,
		RectTextLine.t,
		RectTextLine.l,
		RectTextLine.t + nFontHeight
	);
	//RectChar.l = RectTextLine.l;
	//RectChar.r = RectTextLine.l;
	//RectChar.t = RectTextLine.t;
	//RectChar.b = RectChar.t + nFontHeight;

	wchar_t nChar;
	for ( int i = 0; i <= nCharIndex; i ++ )
	{
		nChar = strText[ i ];
		if ( nChar == 0 )
		{
			break;
		}

		if ( nChar != L'\n' && nChar != L'\r' )
		{
			pBitmap = pFont->GetCharBitmap( nChar );

			if ( pBitmap )
			{
				RectChar.r = RectChar.l + pBitmap->GetCharWidth();
			}
			else
			{
				RectChar.r = RectChar.l + nFontHeight;
			}

			RectChar.l = RectChar.r;
		}
	} // end for

	*pOutRect = RectChar;

	return 1;
}
