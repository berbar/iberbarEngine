
#include <iberbar/Gui/Widgets/EditBox.h>
#include <iberbar/Gui/Engine.h>
#include <iberbar/Gui/RenderElement.h>
#include <iberbar/Gui/Element/ElemColorRect.h>
#include <iberbar/Renderer/Font.h>
#include <iberbar/Renderer/RendererSprite.h>
#include <iberbar/Font/FontDrawText.h>
#include <iberbar/Utility/Input.h>
#include <iberbar/Utility/Math.h>
#include <iberbar/Utility/RectClip2d.h>

//
//#define DXUT_MAX_EDITBOXLENGTH 0xffff
//
//iberbar::Gui::CUniBuffer::CUniBuffer( int )
//	: m_pwszBuffer( nullptr )
//	, m_nBufferSize( 0 )
//	, m_pFont( nullptr )
//{
//}
//
//
//iberbar::Gui::CUniBuffer::~CUniBuffer()
//{
//	SAFE_DELETE_ARRAY( m_pwszBuffer );
//	UNKNOWN_SAFE_RELEASE_NULL( m_pFont );
//}
//
//
//bool iberbar::Gui::CUniBuffer::SetBufferSize( int nNewSize )
//{
//    // If the current size is already the maximum allowed,
//    // we can't possibly allocate more.
//    if ( m_nBufferSize == DXUT_MAX_EDITBOXLENGTH )
//        return false;
//
//    int nAllocateSize = (nNewSize == -1 || nNewSize < m_nBufferSize * 2) ? (m_nBufferSize ? m_nBufferSize *
//        2 : 256) : nNewSize * 2;
//
//    // Cap the buffer size at the maximum allowed.
//    if ( nAllocateSize > DXUT_MAX_EDITBOXLENGTH )
//        nAllocateSize = DXUT_MAX_EDITBOXLENGTH;
//
//    wchar_t* pTempBuffer = new wchar_t[nAllocateSize];
//    if ( !pTempBuffer )
//        return false;
//
//    ZeroMemory( pTempBuffer, sizeof( wchar_t ) * nAllocateSize );
//
//    if ( m_pwszBuffer )
//    {
//        CopyMemory( pTempBuffer, m_pwszBuffer, m_nBufferSize * sizeof( wchar_t ) );
//        delete[] m_pwszBuffer;
//    }
//
//    m_pwszBuffer = pTempBuffer;
//    m_nBufferSize = nAllocateSize;
//    return true;
//}
//
//
//void iberbar::Gui::CUniBuffer::SetFont( Renderer::CFont* pFont )
//{
//	UNKNOWN_SAFE_RELEASE_NULL( m_pFont );
//	m_pFont = pFont;
//	UNKNOWN_SAFE_ADDREF( m_pFont );
//}




//namespace iberbar
//{
//    namespace Gui
//    {
//        class CEditBoxTextUniBuffer
//        {
//
//        };
//    }
//}






namespace iberbar
{
    namespace Gui
    {
        class CEditBoxTextUniBuffer
        {
        public:
            struct _CharNode
            {
                const Renderer::UFontCharBitmap* pBitmap;
                int nCharWidth;
                CColor4B Color;
                CPoint2i Point;
            };

            struct _LineNode
            {
                int nLineIndex;
                int nCharFirst;
                int nCharCount;
                int nTop;
                int nBottom;

                _LineNode()
                    : nLineIndex( -1 )
                    , nCharFirst( -1 )
                    , nCharCount( 0 )
                    , nTop( 0 )
                    , nBottom( 0 )
                {
                }
                _LineNode( int nLineIndex, int nCharFirst, int nTop, int nLineHeight )
                    : nLineIndex( nLineIndex )
                    , nCharFirst( nCharFirst )
                    , nCharCount( 1 )
                    , nTop( nTop )
                    , nBottom( nTop + nLineHeight )
                {
                }
                void AddChar()
                {
                    nCharCount++;
                }
            };

            struct _SelectRects
            {
                int RectCount;
                CRect2i Rects[3];
            };

        public:
            CEditBoxTextUniBuffer();

            void BeginDrawCharBitmaps( const TextDraw::TContextBeginDrawCharBitmaps<Renderer::CFont>* pContext );
            void EndDrawCharBitmaps();
            void DrawCharBitmap( const TextDraw::TContextDrawCharBitmap<Renderer::CFont, Renderer::UFontCharBitmap>* pContext );

            const _CharNode* GetData() const;
            int GetDataSize() const;
            int GetFontHeight() const;
            int GetLineHeight() const;
            const CRect2i& GetRectDraw() const;

            void SetColor( int nStart, int nCount, CColor4B Color );
            void SetOffsetX( int nOffset, bool bAppend );
            bool HitTest( const CPoint2i& Point, int* pCharIndex, CRect2i* pCharRect );
            bool HitTest( const CPoint2i& Point1, const CPoint2i& Point2, int* pCharSelectStart, int* pCharSelectCount, _SelectRects* pSelectRects, CRect2i* pCharRect );

        private:
            int m_nFontHeight;
            int m_nLineHeight;
            int m_nCharTopTemp;
            int m_nLineIndexTemp;
            int m_nOffsetX;
            int m_nOffsetY;
            CRect2i m_RectDraw;
            std::pmr::vector< _CharNode > m_Chars;
            std::pmr::vector< _LineNode > m_Lines;
            std::pmr::vector< int > m_CharIndicesDraw;
        };
    }
}








iberbar::Gui::CEditBoxTextUniBuffer::CEditBoxTextUniBuffer()
    : m_nFontHeight( 0 )
    , m_nLineHeight( 0 )
    , m_nCharTopTemp( 0 )
    , m_nLineIndexTemp( -1 )
    , m_nOffsetX( 0 )
    , m_nOffsetY( 0 )
    , m_Chars( std::pmr::polymorphic_allocator< _CharNode >( CEngine::sGetInstance()->GetMemoryPool() ) )
    , m_Lines( std::pmr::polymorphic_allocator< _LineNode >( CEngine::sGetInstance()->GetMemoryPool() ) )
    , m_CharIndicesDraw( std::pmr::polymorphic_allocator< int >( CEngine::sGetInstance()->GetMemoryPool() ) )
{
}


FORCEINLINE void iberbar::Gui::CEditBoxTextUniBuffer::BeginDrawCharBitmaps( const TextDraw::TContextBeginDrawCharBitmaps<Renderer::CFont>* pContext )
{
    auto& FontDesc = pContext->pFont->GetFontDesc();
    m_nFontHeight = FontDesc.Size;
    m_nLineHeight = pContext->pOptions->lineHeight;
    m_nCharTopTemp = 0;
    m_nLineIndexTemp = -1;
    m_RectDraw = pContext->Rect;
    m_Chars.resize( pContext->nCount );
    //m_Chars.clear();
    m_Lines.clear();
    m_CharIndicesDraw.clear();
    //m_Lines.push_back( _LineNode( 0, pContext->Rect.t, m_nLineHeight ) );
}

FORCEINLINE void iberbar::Gui::CEditBoxTextUniBuffer::EndDrawCharBitmaps()
{
    //int nChar;
    //int nCharCount;
    //for ( int i = 0, s = m_Lines.size(); i < s; i++ )
    //{
    //    nChar = m_Lines[i].nCharFirst;
    //    nCharCount = m_Lines[i].nCharCount;
    //    for ( int j = 0; j < nCharCount; j++ )
    //    {
    //        m_CharIndicesDraw.push_back( nChar + j );
    //    }
    //}
}

FORCEINLINE void iberbar::Gui::CEditBoxTextUniBuffer::DrawCharBitmap( const TextDraw::TContextDrawCharBitmap<Renderer::CFont, Renderer::UFontCharBitmap>* pContext )
{
    _CharNode& Node = m_Chars[pContext->nIndexAtText];
    //_CharNode Node;
    Node.pBitmap = pContext->pBitmap;
    Node.Color = pContext->FillColor;
    Node.Point = *pContext->pPointDrawAt;
    Node.Point.x += m_nOffsetX;
    Node.Point.y += m_nOffsetY;
    Node.nCharWidth = (pContext->pBitmap == nullptr) ? m_nFontHeight : pContext->pBitmap->GetCharWidth();
    //m_Chars.push_back( Node );


    if ( m_Lines.empty() || m_nLineIndexTemp != pContext->nLine )
    {
        m_Lines.push_back( _LineNode(
            pContext->nLine,
            pContext->nIndexAtText,
            m_RectDraw.t + pContext->nLine * m_nLineHeight,
            m_nLineHeight ) );
        m_nLineIndexTemp = pContext->nLine;
    }
    else
    {
        m_Lines.rbegin()->AddChar();
    }
}


inline const iberbar::Gui::CEditBoxTextUniBuffer::_CharNode* iberbar::Gui::CEditBoxTextUniBuffer::GetData() const
{
    return m_Chars.data();
}


inline int iberbar::Gui::CEditBoxTextUniBuffer::GetDataSize() const
{
    return (int)m_Chars.size();
}


inline int iberbar::Gui::CEditBoxTextUniBuffer::GetFontHeight() const
{
    return m_nFontHeight;
}


inline int iberbar::Gui::CEditBoxTextUniBuffer::GetLineHeight() const
{
    return m_nLineHeight;
}


inline const iberbar::CRect2i& iberbar::Gui::CEditBoxTextUniBuffer::GetRectDraw() const
{
    return m_RectDraw;
}


void iberbar::Gui::CEditBoxTextUniBuffer::SetColor( int nStart, int nCount, CColor4B Color )
{
    int i = tMax( nStart, 0 );
    int s = nCount < 0 ? (int)m_Chars.size() : (tMin( nStart + nCount, (int)m_Chars.size() ));
    for ( ; i < s; i++ )
    {
        m_Chars[i].Color = Color;
    }
}


void iberbar::Gui::CEditBoxTextUniBuffer::SetOffsetX( int nOffset, bool bAppend )
{
    if ( bAppend == true )
    {
        m_nOffsetX += nOffset;

        for ( int i = 0, s = m_Chars.size(); i < s; i++ )
        {
            m_Chars[i].Point.x += nOffset;
        }
    }
    else
    {
        if ( nOffset == m_nOffsetX )
            return;

        int nOffsetNew = nOffset - m_nOffsetX;

        for ( int i = 0, s = m_Chars.size(); i < s; i++ )
        {
            m_Chars[i].Point.x += nOffsetNew;
        }

        m_nOffsetX = nOffset;
    }
}


bool iberbar::Gui::CEditBoxTextUniBuffer::HitTest( const CPoint2i& Point, int* pCharIndex, CRect2i* pCharRect )
{
    if ( m_Chars.empty() == true )
        return false;

    int nRight = 0;
    const _CharNode* pCharNode = nullptr;

    int i;
    int s;

    i = 0;
    s = m_Lines.size();
    for ( ; i < s; i++ )
    {

    }

    pCharNode = &m_Chars[0];
    if ( pCharNode->Point.x >= Point.x )
    {
        *pCharIndex = -1;
        *pCharRect = CRect2i(
            pCharNode->Point,
            CSize2i( 0, m_nFontHeight )
        );
        return true;
    }

    i = 0;
    s = (int)m_Chars.size();
    for ( ; i < s; i++ )
    {
        pCharNode = &m_Chars[i];
        if ( pCharNode->Point.x < Point.x )
        {
            nRight = pCharNode->Point.x + pCharNode->pBitmap->GetCharWidth();
            if ( nRight >= Point.x )
            {
                *pCharIndex = i;
                *pCharRect = CRect2i(
                    pCharNode->Point,
                    CSize2i( pCharNode->pBitmap->GetCharWidth(), m_nFontHeight )
                );
                return true;
            }
        }
    }

    pCharNode = &m_Chars[s-1];
    *pCharIndex = s-1;
    *pCharRect = CRect2i(
        pCharNode->Point,
        CSize2i( pCharNode->pBitmap->GetCharWidth(), m_nFontHeight )
    );

    return true;
}


bool iberbar::Gui::CEditBoxTextUniBuffer::HitTest( const CPoint2i& Point1, const CPoint2i& Point2, int* pSelectStart, int* pSelectCount, _SelectRects* pSelectRects, CRect2i* pCharRect )
{
    return false;
}








iberbar::Gui::CEditBoxTextElementBase::CEditBoxTextElementBase()
    : m_strText( L"" )
    , m_strTextSelected( L"" )
    , m_nCaretAtCharIndex( -1 )
{
}


iberbar::Gui::CEditBoxTextElementBase::CEditBoxTextElementBase( const CEditBoxTextElementBase& Element )
    : m_strText( L"" )
    , m_strTextSelected( L"" )
    , m_nCaretAtCharIndex( -1 )
{
}








iberbar::Gui::CEditBoxTextElement::CEditBoxTextElement()
    : m_pFont( nullptr )
    , m_nTextAlign( UAlignHorizental::Left )
    , m_bSingleLine( true )
    , m_TextColor()
    , m_SelTextColor()
    , m_SelBgColor()
    , m_CaretColor()
    , m_nBgZOrder( 0 )

    , m_nCaretKick( 0 )
    , m_bCaretShowState( false )
    , m_CaretBounding( 0, 0, 0, 0 )

    , m_nSelectStart( 0 )
    , m_nSelectCount( 0 )
    , m_SelectBounding()

    , m_pUniBuffer( new CEditBoxTextUniBuffer() )
{
}


iberbar::Gui::CEditBoxTextElement::CEditBoxTextElement( const CEditBoxTextElement& Element )
    : CEditBoxTextElementBase( Element )
    , m_pFont( Element.m_pFont )
    , m_nTextAlign( Element.m_nTextAlign )
    , m_bSingleLine( true )
    , m_TextColor( Element.m_TextColor )
    , m_SelTextColor( Element.m_SelTextColor )
    , m_SelBgColor( Element.m_SelBgColor )
    , m_CaretColor( Element.m_CaretColor )
    , m_nBgZOrder( Element.m_nBgZOrder )

    , m_nCaretKick( 0 )
    , m_bCaretShowState( false )
    , m_CaretBounding( 0, 0, 0, 0 )

    , m_nSelectStart( 0 )
    , m_nSelectCount( 0 )
    , m_SelectBounding()

    , m_pUniBuffer( new CEditBoxTextUniBuffer() )
{
    UNKNOWN_SAFE_ADDREF( m_pFont );
}


iberbar::Gui::CEditBoxTextElement::~CEditBoxTextElement()
{
    UNKNOWN_SAFE_RELEASE_NULL( m_pFont );
    SAFE_DELETE( m_pUniBuffer );
}


void iberbar::Gui::CEditBoxTextElement::UpdateRect()
{
    CRenderElement::UpdateRect();

    UpdateUniBuffer();
    SetCaret( m_nCaretAtCharIndex );
}


void iberbar::Gui::CEditBoxTextElement::SetFont( Renderer::CFont* pFont )
{
    if ( m_pFont == pFont )
        return;

    if ( m_pFont != nullptr )
        m_pFont->Release();

    m_pFont = pFont;

    if ( m_pFont != nullptr )
    {
        m_pFont->AddRef();
        m_pFont->LoadText( m_strText.c_str() );
    }
}


void iberbar::Gui::CEditBoxTextElement::SetTextAlignHorizental( UAlignHorizental nAlign )
{
    m_nTextAlign = nAlign;
    UpdateUniBuffer();
    SetCaret( m_nCaretAtCharIndex );
}


void iberbar::Gui::CEditBoxTextElement::SetTextColor( const CColor4B& Color )
{
    m_TextColor = Color;
    m_pUniBuffer->SetColor( 0, m_nSelectStart, m_TextColor );
    m_pUniBuffer->SetColor( m_nSelectStart + m_nSelectCount, -1, m_TextColor );
}


void iberbar::Gui::CEditBoxTextElement::SetSelTextColor( const CColor4B& Color )
{
    m_SelTextColor = Color;
    m_pUniBuffer->SetColor( m_nSelectStart, m_nSelectCount, m_SelTextColor );
}


void iberbar::Gui::CEditBoxTextElement::SetSelBgColor( const CColor4B& Color )
{
    m_SelBgColor = Color;
}


void iberbar::Gui::CEditBoxTextElement::InsertChar( wchar_t nChar )
{
    if ( m_pFont == nullptr )
        return;

    if ( m_nSelectCount > 0 )
    {
        m_strText.erase( m_nSelectStart, m_nSelectCount );
    }

    int nTextLen = wcslen( m_strText.c_str() );

    if ( m_nCaretAtCharIndex >= (nTextLen - 1) )
        m_strText.push_back( nChar );
    else
        m_strText.insert( m_nCaretAtCharIndex + 1, 1, nChar );

    m_pFont->LoadText( nChar, nChar );

    UpdateUniBuffer();
    SetCaret( m_nCaretAtCharIndex + 1 );
}


void iberbar::Gui::CEditBoxTextElement::InsertString( const wchar_t* pStr )
{
    if ( m_pFont == nullptr )
        return;

    if ( m_nSelectCount > 0 )
    {
        m_strText.erase( m_nSelectStart, m_nSelectCount );
    }

    int nTextLen = wcslen( m_strText.c_str() );

    if ( m_nCaretAtCharIndex >= nTextLen )
        m_strText = m_strText + pStr;
    else
        m_strText.insert( m_nCaretAtCharIndex + 1, pStr );

    m_pFont->LoadText( pStr );

    UpdateUniBuffer();
    SetCaret( m_nCaretAtCharIndex + wcslen( pStr ) );
}


void iberbar::Gui::CEditBoxTextElement::RemoveString()
{
    int nTextLen = wcslen( m_strText.c_str() );
    if ( nTextLen == 0 )
        return;

    if ( m_nSelectCount > 0 )
    {
        m_strText.erase( m_nSelectStart, m_nSelectCount );
        UpdateUniBuffer();
        SetCaret( m_nSelectStart-1 );
    }
    else
    {
        if ( m_nCaretAtCharIndex < 0 )
            return;

        if ( m_nCaretAtCharIndex == nTextLen )
        {
            m_strText.pop_back();
        }
        else
        {
            m_strText.erase( m_nCaretAtCharIndex, 1 );
        }
        UpdateUniBuffer();
        SetCaret( m_nCaretAtCharIndex - 1 );
    }
}


//void iberbar::Gui::CEditBoxTextElement::SetCaret()
//{
//    SetCaret( m_nCaretAtCharIndex );
//}


void iberbar::Gui::CEditBoxTextElement::SetCaret( int nAtCharIndex )
{
    m_nCaretAtCharIndex = nAtCharIndex;
    int nStrLen = wcslen( m_strText.c_str() );
    if ( m_nCaretAtCharIndex < -1 )
        m_nCaretAtCharIndex = -1;
    else if ( m_nCaretAtCharIndex >= nStrLen )
        m_nCaretAtCharIndex = nStrLen - 1;

    if ( m_nCaretAtCharIndex >= 0 )
    {
        const CRect2i& RectBounding = GetBounding();
        const CRect2i& RectDrawOrigin = m_pUniBuffer->GetRectDraw();
        const CEditBoxTextUniBuffer::_CharNode* pCharNodeAtCaret = &(m_pUniBuffer->GetData()[m_nCaretAtCharIndex]);
        const CEditBoxTextUniBuffer::_CharNode* pCharNode = nullptr;
        int nFontSize = m_pUniBuffer->GetFontHeight();
        if ( m_bSingleLine == true )
        {
            int nOffset;

            if ( RectDrawOrigin.Width() > RectBounding.Width() )
            {
                // 如果最后一个字符出现在viewport中，就右边界对齐
                pCharNode = &(m_pUniBuffer->GetData()[m_pUniBuffer->GetDataSize() - 1]);
                if ( pCharNode->Point.x < RectBounding.r )
                {
                    nOffset = RectBounding.r - (pCharNode->Point.x + pCharNode->nCharWidth);
                    if ( nOffset > 0 )
                    {
                        m_pUniBuffer->SetOffsetX( nOffset, true );
                    }
                }

                // 如果第一个字符出现在viewport中，就左边对齐
                pCharNode = &(m_pUniBuffer->GetData()[0]);
                if ( (pCharNode->Point.x + pCharNode->nCharWidth) > RectBounding.l )
                {
                    nOffset = RectBounding.l - pCharNode->Point.x;
                    if ( nOffset < 0 )
                    {
                        m_pUniBuffer->SetOffsetX( nOffset, true );
                    }
                }

                int nCaretPos = pCharNodeAtCaret->Point.x + pCharNodeAtCaret->nCharWidth;
                // 光标超出右边界
                if ( nCaretPos > RectBounding.r )
                {
                    m_pUniBuffer->SetOffsetX( RectBounding.r - nCaretPos, true );
                }
                // 光标超出左边界
                else if ( nCaretPos < RectBounding.l )
                {
                    m_pUniBuffer->SetOffsetX( RectBounding.l - nCaretPos, true );
                }
                else
                {
                }
            }
            else
            {
                m_pUniBuffer->SetOffsetX( 0, false );
            }
        }
        int nRight = pCharNodeAtCaret->Point.x + pCharNodeAtCaret->nCharWidth;
        m_CaretBounding = CRect2f(
            nRight - 1,
            pCharNodeAtCaret->Point.y,
            nRight + 1,
            pCharNodeAtCaret->Point.y + m_pUniBuffer->GetFontHeight()
        );
    }
    else
    {
        if ( nStrLen == 0 )
        {
            const CRect2i& RectDraw = m_pUniBuffer->GetRectDraw();
            m_CaretBounding = CRect2f(
                RectDraw.l - 1,
                RectDraw.t,
                RectDraw.l + 1,
                RectDraw.b
            );
        }
        else
        {
            const CEditBoxTextUniBuffer::_CharNode* pCharNodeFirst = &(m_pUniBuffer->GetData()[0]);
            m_pUniBuffer->SetOffsetX( 0, false );
            m_CaretBounding = CRect2f(
                pCharNodeFirst->Point.x - 1,
                pCharNodeFirst->Point.y,
                pCharNodeFirst->Point.x + 1,
                pCharNodeFirst->Point.y + m_pUniBuffer->GetFontHeight()
            );
        }
    }

    if ( m_nSelectCount > 0 )
    {
        ClearSelectedText();
    }
}


void iberbar::Gui::CEditBoxTextElement::SetCaret( const CPoint2i& Point )
{
    if ( m_strText[0] == 0 )
    {
        m_nCaretAtCharIndex = -1;
    }
    else
    {
        CRect2i RectChar;
        int nCaretAtCharIndex = -1;
        bool bRet = m_pUniBuffer->HitTest( Point, &nCaretAtCharIndex, &RectChar );
        if ( bRet == true )
        {
            int nBorder = RectChar.l + RectChar.Width() * 3 / 5;
            if ( Point.x <= nBorder )
            {
                if ( m_nCaretAtCharIndex >= 0 )
                    m_nCaretAtCharIndex = nCaretAtCharIndex-1;
                m_CaretBounding = CRect2f(
                    RectChar.l - 1,
                    RectChar.t,
                    RectChar.l + 1,
                    RectChar.b
                );
            }
            else
            {
                m_nCaretAtCharIndex = nCaretAtCharIndex;
                m_CaretBounding = CRect2f(
                    RectChar.r - 1,
                    RectChar.t,
                    RectChar.r + 1,
                    RectChar.b
                );
            }
            m_bCaretShowState = true;
            m_nCaretKick = 0.0f;
        }
    }

    if ( m_nSelectCount > 0 )
    {
        ClearSelectedText();
    }
}


void iberbar::Gui::CEditBoxTextElement::SelectText( const CPoint2i& PointFrom )
{
    CRect2i RectChar;
    int nCharIndex = -1;
    bool bRet = m_pUniBuffer->HitTest( PointFrom, &nCharIndex, &RectChar );
    if ( bRet == true )
    {
        int nBorder = RectChar.l + RectChar.Width() * 3 / 5;
        if ( PointFrom.x <= nBorder )
        {
            nCharIndex = nCharIndex - 1;
        }
        else
        {
            nCharIndex = nCharIndex;
        }
        if ( nCharIndex != m_nCaretAtCharIndex )
        {
            int nSelectStart = 0;
            int nSelectCount = 0;
            if ( nCharIndex < m_nCaretAtCharIndex )
            {
                nSelectStart = nCharIndex + 1;
                nSelectCount = m_nCaretAtCharIndex - nCharIndex;
            }
            else
            {
                nSelectStart = m_nCaretAtCharIndex + 1;
                nSelectCount = nCharIndex - m_nCaretAtCharIndex;
            }
            if ( nSelectStart == -1 )
            {
                nSelectStart = 0;
                nSelectCount--;
            }
            m_pUniBuffer->SetColor( 0, m_pUniBuffer->GetDataSize(), m_TextColor );
            m_pUniBuffer->SetColor( nSelectStart, nSelectCount, m_SelTextColor );
            m_strTextSelected.resize( nSelectCount + 1 );
            memcpy_s( &m_strTextSelected.front(), nSelectCount, m_strText.c_str() + nSelectStart, nSelectCount );
            m_strTextSelected[nSelectCount] = 0;
            m_nSelectStart = nSelectStart;
            m_nSelectCount = nSelectCount;
            const CEditBoxTextUniBuffer::_CharNode* pCharNodeList = m_pUniBuffer->GetData();
            m_SelectBounding.l = m_SelectBounding.r = pCharNodeList[nSelectStart].Point.x;
            m_SelectBounding.t = pCharNodeList[nSelectStart].Point.y;
            m_SelectBounding.b = m_SelectBounding.t + m_pUniBuffer->GetFontHeight();
            for ( int i = nSelectStart, s = nSelectStart + nSelectCount; i < s; i++ )
            {
                m_SelectBounding.r += pCharNodeList[i].nCharWidth;
            }
        }
        else
        {
            ClearSelectedText();
        }
    }
}


void iberbar::Gui::CEditBoxTextElement::SelectText( const CPoint2i& PointFrom, const CPoint2i& PointTo )
{
    // 暂时这么些
    SetCaret( PointTo );
    SelectText( PointFrom );
}


void iberbar::Gui::CEditBoxTextElement::ClearSelectedText()
{
    m_pUniBuffer->SetColor( 0, m_pUniBuffer->GetDataSize(), m_TextColor );
    m_strTextSelected = L"";
    m_nSelectStart = 0;
    m_nSelectCount = 0;
    m_SelectBounding = CRect2i( 0, 0, 0, 0 );
}


void iberbar::Gui::CEditBoxTextElement::Update( float nDelta )
{
    CRenderElement::Update( nDelta );

    m_nCaretKick += nDelta;
    float nKickStep = 0.3f;
    if ( m_bCaretShowState == true )
        nKickStep = 0.5f;
    if ( m_nCaretKick > nKickStep )
    {
        m_nCaretKick -= nKickStep;
        m_bCaretShowState = !m_bCaretShowState;
    }
}


void iberbar::Gui::CEditBoxTextElement::Render()
{
    if ( GetVisible() == false )
        return;

    CRenderElement::Render();

    if ( m_pFont )
    {
        const CRect2i* pViewport = CEngine::sGetInstance()->GetViewportState()->GetClipViewport();

        CRect2i RectViewportFinal = GetBounding();
        if ( pViewport != nullptr )
        {
            RectFindIntersection( &RectViewportFinal, &RectViewportFinal, pViewport );
        }

        if ( RectViewportFinal.IsEmpty() == false )
        {
            if ( m_SelectBounding.IsEmpty() == false )
            {
                CRect2i RectDrawBg = m_SelectBounding;
                if ( RectTestIntersection( &RectDrawBg, &RectViewportFinal ) )
                {
                    RectClip2d( &RectDrawBg, &RectViewportFinal );
                }
                CEngine::sGetInstance()->GetRendererSprite()->DrawRect( m_nBgZOrder, RectToFloat( RectDrawBg ), m_SelBgColor );
            }

            CEngine::sGetInstance()->GetRendererSprite()->DrawTextWithUserData(
                m_nZOrder,
                m_pFont,
                &RectViewportFinal,
                m_pUniBuffer->GetData(),
                sizeof( CEditBoxTextUniBuffer::_CharNode ),
                m_pUniBuffer->GetDataSize(),
                offsetof( CEditBoxTextUniBuffer::_CharNode, pBitmap ),
                offsetof( CEditBoxTextUniBuffer::_CharNode, Point ),
                offsetof( CEditBoxTextUniBuffer::_CharNode, Color ) );
        }
    }

    if ( m_bCaretShowState == true )
    {
        CEngine::sGetInstance()->GetRendererSprite()->DrawRect( m_nBgZOrder, m_CaretBounding, m_CaretColor );
    }
}


void iberbar::Gui::CEditBoxTextElement::UpdateUniBuffer()
{
    if ( m_pFont == nullptr )
        return;

    if ( m_bSingleLine == true )
    {
        UFontDrawTextOptions Options;
        Options.alignHorizental = m_nTextAlign;
        Options.alignVertical = UAlignVertical::Center;
        Options.nWrapType = TextDraw::UFontDrawTextWorkBreak::NoBreak;
        Options.calculateRect = true;
        Options.bRenderText = false;
        CRect2i RectDraw = GetBounding();
        iberbar::XFontDrawText< Renderer::CFont, Renderer::UFontCharBitmap, CEditBoxTextUniBuffer >( m_pUniBuffer, m_pFont, nullptr, m_strText.c_str(), -1, &RectDraw, m_TextColor, Options );
        const CRect2i& RectBounding = GetBounding();
        if ( RectDraw.Width() > RectBounding.Width() )
        {
            Options.alignHorizental = UAlignHorizental::Left;
            RectOffsetX( &RectDraw, RectBounding.l - RectDraw.l );
        }
        Options.bRenderText = true;
        iberbar::XFontDrawText< Renderer::CFont, Renderer::UFontCharBitmap, CEditBoxTextUniBuffer >( m_pUniBuffer, m_pFont, nullptr, m_strText.c_str(), -1, &RectDraw, m_TextColor, Options );
    }
    else
    {
        UFontDrawTextOptions Options;
        Options.alignHorizental = m_nTextAlign;
        Options.nWrapType = TextDraw::UFontDrawTextWorkBreak::BreakAll;
        CRect2i RectDraw = GetBounding();
        iberbar::XFontDrawText< Renderer::CFont, Renderer::UFontCharBitmap, CEditBoxTextUniBuffer >( m_pUniBuffer, m_pFont, nullptr, m_strText.c_str(), -1, &RectDraw, m_TextColor, Options );
    }
}








iberbar::Gui::CEditBox::CEditBox()
    : m_pTextElementRef( nullptr )
    , m_bEnableSelectText( false )
    , m_bPressed( false )
    , m_PressPoint()
{
    m_bCanFocus = true;
}


iberbar::Gui::CEditBox::CEditBox( const CEditBox& editBox )
    : CWidget( editBox )
    , m_pTextElementRef( nullptr )
    , m_bEnableSelectText( editBox.m_bEnableSelectText )
    , m_bPressed( false )
    , m_PressPoint()
{
    m_bCanFocus = true;
}


iberbar::Gui::CEditBox::~CEditBox()
{
    UNKNOWN_SAFE_RELEASE_NULL( m_pTextElementRef );
}


void iberbar::Gui::CEditBox::UpdateRect()
{
    CWidget::UpdateRect();
}


void iberbar::Gui::CEditBox::OnFocusIn()
{
    CWidget::OnFocusIn();
}


void iberbar::Gui::CEditBox::OnFocusOut()
{
    CWidget::OnFocusOut();

    if ( m_pTextElementRef )
        m_pTextElementRef->ClearSelectedText();
}


iberbar::Gui::UHandleMessageResult iberbar::Gui::CEditBox::HandleMouse( const UMouseEventData* pEvent )
{
    switch ( pEvent->nMouseEvent )
    {
        case UMouseEvent::LDown:
        {
            if ( HitTest( pEvent->MousePoint ) == true )
            {
                if ( m_bFocus == false )
                {
                    RequestFocus();
                }

                m_bPressed = true;
                m_PressPoint = pEvent->MousePoint;

                m_pTextElementRef->SetCaret( pEvent->MousePoint );

                return UHandleMessageResult::Succeed;
            }
            break;
        }

        case UMouseEvent::Move:
        {
            if ( m_bPressed == true )
            {
                m_pTextElementRef->SelectText( m_PressPoint, pEvent->MousePoint );

                return UHandleMessageResult::Succeed;
            }
            break;
        }

        case UMouseEvent::LUp:
        {
            m_bPressed = false;
            break;
        }

        default:break;
    }

    return UHandleMessageResult::Ignore;
}


iberbar::Gui::UHandleMessageResult iberbar::Gui::CEditBox::HandleKeyboard( const UKeyboardEventData* pEvent )
{
    if ( GetFocus() == false )
        return UHandleMessageResult::Ignore;

#ifdef _WINDOWS
    switch ( pEvent->nEvent )
    {
        case UKeyboardEvent::KeyDown:
        {
            if ( pEvent->nKeyCode == VK_LEFT )
            {
                m_pTextElementRef->SetCaret( m_pTextElementRef->GetCaretAtCharIndex() - 1 );
            }
            else if ( pEvent->nKeyCode == VK_RIGHT )
            {
                m_pTextElementRef->SetCaret( m_pTextElementRef->GetCaretAtCharIndex() + 1 );
            }
            else if ( pEvent->nKeyCode == VK_HOME )
            {
                m_pTextElementRef->SetCaret( -1 );
            }
            else if ( pEvent->nKeyCode == VK_END )
            {
                m_pTextElementRef->SetCaret( wcslen( m_pTextElementRef->GetText() ) );
            }
            return UHandleMessageResult::Succeed;
            break;
        }

        case UKeyboardEvent::KeyUp:
        {
            break;
        }

        case UKeyboardEvent::Char:
        {
            switch ( pEvent->nKeyCode )
            {
                case VK_BACK:
                {
                    m_pTextElementRef->RemoveString();
                    break;
                }

                // 复制 ctrl + c
                case VK_CANCEL:
                {
                    CopyToClipboard();
                    break;
                }

                // 粘贴 ctrl + v
                case 22:
                {
                    PasteFromClipboard();
                    break;
                }

                default:
                {
                    
                    wchar_t strChar[2]{ pEvent->nKeyCode, 0 };
                    if ( m_FuncStringValidator )
                    {
                        const wchar_t* strCharNew = m_FuncStringValidator( strChar );
                        if ( strCharNew == nullptr || strCharNew[0] == 0 )
                            strChar[0] = 0;
                        else
                            strChar[0] = strCharNew[0];
                    }
                    if ( strChar[0] != 0 )
                    {
                        m_pTextElementRef->InsertChar( strChar[0] );
                    }
                    break;
                }
            }
            
            return UHandleMessageResult::Succeed;
            break;
        }
            

        default:
            break;
    }
#endif

    return UHandleMessageResult::Ignore;
}


void iberbar::Gui::CEditBox::SetTextElementRef( CEditBoxTextElementBase* pElement )
{
    if ( m_pTextElementRef == pElement )
        return;
    UNKNOWN_SAFE_RELEASE_NULL( m_pTextElementRef );
    m_pTextElementRef = pElement;
    UNKNOWN_SAFE_ADDREF( m_pTextElementRef );
}


void iberbar::Gui::CEditBox::CopyToClipboard()
{
    // Copy the selection text to the clipboard
    if ( OpenClipboard( NULL ) )
    {
        EmptyClipboard();

        const wchar_t* strText = m_pTextElementRef->GetText();
        int nTextLen = wcslen( strText );
        HGLOBAL hBlock = GlobalAlloc( GMEM_MOVEABLE, sizeof( wchar_t ) * (nTextLen + 1) );
        if ( hBlock )
        {
            WCHAR* pwszText = (WCHAR*)GlobalLock( hBlock );
            if ( pwszText )
            {
                //int nFirst = tMin( m_nCaret, m_nSelStart );
                //int nLast = tMax( m_nCaret, m_nSelStart );
                //if ( nLast - nFirst > 0 )
                //    CopyMemory( pwszText, m_Buffer.GetBuffer() + nFirst, (nLast - nFirst) * sizeof( WCHAR ) );
                //pwszText[nLast - nFirst] = L'\0';  // Terminate it
                //GlobalUnlock( hBlock );

                memcpy( pwszText, strText, nTextLen * sizeof( wchar_t ) );
                pwszText[nTextLen] = L'\0';  // Terminate it
                GlobalUnlock( hBlock );
            }
            SetClipboardData( CF_UNICODETEXT, hBlock );
        }
        CloseClipboard();
        // We must not free the object until CloseClipboard is called.
        if ( hBlock )
            GlobalFree( hBlock );
    }
}


void iberbar::Gui::CEditBox::PasteFromClipboard()
{
    if ( OpenClipboard( NULL ) )
    {
        HANDLE handle = GetClipboardData( CF_UNICODETEXT );
        if ( handle )
        {
            // Convert the ANSI string to Unicode, then
            // insert to our buffer.
            WCHAR* pwszText = (WCHAR*)GlobalLock( handle );
            if ( pwszText )
            {
                // Copy all characters up to null.
                //if ( m_Buffer.InsertString( m_nCaret, pwszText ) )
                //    PlaceCaret( m_nCaret + lstrlenW( pwszText ) );
                //m_nSelStart = m_nCaret;
                //GlobalUnlock( handle );

                m_pTextElementRef->InsertString( pwszText );
                GlobalUnlock( handle );
            }
        }
        CloseClipboard();
    }
}


