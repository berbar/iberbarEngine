#pragma once

#include <iberbar/Gui/Widget.h>
#include <iberbar/Gui/RenderElement.h>
#include <iberbar/Utility/Color.h>

namespace iberbar
{
    namespace Renderer
    {
        struct UFontCharBitmap;
        class CFont;
    }

	namespace Gui
	{
        class CEditBoxTextUniBuffer;




        class __iberbarGuiApi__ CEditBoxTextElementBase abstract
            : public CRenderElement
        {
        public:
            CEditBoxTextElementBase();
        protected:
            CEditBoxTextElementBase( const CEditBoxTextElementBase& Element );

        public:
            virtual void InsertChar( wchar_t nChar ) = 0;
            virtual void InsertString( const wchar_t* pStr ) = 0;
            virtual void RemoveString() = 0;
            
            virtual void SetCaret( int nAtCharIndex ) = 0;
            virtual void SetCaret( const CPoint2i& Point ) = 0;
            int GetCaretAtCharIndex() { return m_nCaretAtCharIndex; }
            void SetCaretVisible( bool bValue ) { m_bCaretVisible = bValue; };

            virtual void SelectText( const CPoint2i& Point ) = 0;
            virtual void SelectText( const CPoint2i& PointFrom, const CPoint2i& PointTo ) = 0;
            virtual void ClearSelectedText() = 0;

            const wchar_t* GetText() { return m_strText.c_str(); }
            const wchar_t* GetTextSelected() { return m_strTextSelected.c_str(); }

        protected:
            std::wstring m_strText;
            std::wstring m_strTextSelected;

            int m_nCaretAtCharIndex;
            bool m_bCaretVisible;
        };

        class __iberbarGuiApi__ CEditBoxTextElement
            : public CEditBoxTextElementBase
        {
        public:
            CEditBoxTextElement();
            virtual ~CEditBoxTextElement();
        protected:
            CEditBoxTextElement( const CEditBoxTextElement& element );

        public:
            virtual CEditBoxTextElement* Clone() const override  { return new CEditBoxTextElement( *this ); }
            virtual void UpdateRect() override;
            virtual void Update( float nDelta ) override;
            virtual void Render() override;
            

        public:
            void SetFont( Renderer::CFont* pFont );
            void SetTextAlignHorizental( UAlignHorizental nAlign );
            void SetTextColor( const CColor4B& Color );
            void SetSelTextColor( const CColor4B& Color );
            void SetSelBgColor( const CColor4B& Color );
            void SetCaretColor( const CColor4B& Color ) { m_CaretColor = Color; }
            void SetBgZOrder( int nZOrder ) { m_nBgZOrder = nZOrder; }

            Renderer::CFont* GetFont() { return m_pFont; }
            UAlignHorizental GetTextAlignHorizental() const { return m_nTextAlign; }
            const CColor4B& GetTextColor() const { return m_TextColor; }
            const CColor4B& GetSelTextColor() const { return m_SelTextColor; }
            const CColor4B& GetSelBgColor() const { return m_SelBgColor; }
            const CColor4B& GetCaretColor() const { return m_CaretColor; }
            int GetBgZOrder() const { return m_nBgZOrder; }

            virtual void InsertChar( wchar_t nChar ) override;
            virtual void InsertString( const wchar_t* pStr ) override;
            virtual void RemoveString() override;
            virtual void SetCaret( int nAtCharIndex ) override;
            virtual void SetCaret( const CPoint2i& Point ) override;
            virtual void SelectText( const CPoint2i& Point ) override;
            virtual void SelectText( const CPoint2i& PointFrom, const CPoint2i& PointTo ) override;
            virtual void ClearSelectedText() override;
            

        private:
            void UpdateUniBuffer();

        private:
            Renderer::CFont* m_pFont;
            UAlignHorizental m_nTextAlign;
            bool m_bSingleLine;
            CColor4B m_TextColor;
            CColor4B m_SelTextColor;
            CColor4B m_SelBgColor;
            CColor4B m_CaretColor;
            int m_nBgZOrder;

            float m_nCaretKick;
            bool m_bCaretShowState;
            CPoint2i m_nCaretPos;
            CRect2f m_CaretBounding;

            int m_nSelectStart;
            int m_nSelectCount;
            CRect2i m_SelectBounding;

            int m_nTabStyle;
            int m_nTabOfWhiteSpace;

            CEditBoxTextUniBuffer* m_pUniBuffer;
        };









        class __iberbarGuiApi__ CEditBox
            : public CWidget
        {
        public:
            typedef std::function<const wchar_t* (const wchar_t*)> _Validator;
        public:
            CEditBox();
            virtual ~CEditBox();
        protected:
            CEditBox( const CEditBox& editBox );


        public:
            virtual void UpdateRect() override;
            virtual void OnFocusIn() override;
            virtual void OnFocusOut() override;
            virtual UHandleMessageResult HandleKeyboard( const UKeyboardEventData* pEvent ) override;
            virtual UHandleMessageResult HandleMouse( const UMouseEventData* pEvent ) override;



        public:
            void SetTextElementRef( CEditBoxTextElementBase* pElement );
            void SetStringValidator( _Validator FuncValidator ) { m_FuncStringValidator = FuncValidator; }

            void SetValueText( const wchar_t* strText ) {}
            const wchar_t* GetValueText() const { return m_pTextElementRef->GetText(); }

        protected:
            void            CopyToClipboard();
            void            PasteFromClipboard();


        protected:
            CEditBoxTextElementBase* m_pTextElementRef;
            bool m_bEnableSelectText;
            bool m_bPressed;
            CPoint2i m_PressPoint;
            _Validator m_FuncStringValidator;
        };



        const wchar_t* EditBoxValidator_OnlyInt( const char);
        const wchar_t* EditBoxValidator_OnlyFloat();
	}
}




