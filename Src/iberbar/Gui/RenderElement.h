#pragma once

#include <iberbar/Gui/Object.h>

namespace iberbar
{

	namespace Gui
	{
		class CRenderElement;

		IBERBAR_UNKNOWN_PTR_DECLARE( CRenderElement );


		class __iberbarGuiApi__ CRenderElement
			: public CObject
		{
		public:
			CRenderElement();
			virtual ~CRenderElement();

		protected:
			CRenderElement( const CRenderElement& );

		public:
			virtual CRenderElement* Clone() const override;


		public:
			
			int  AddChildElement( CRenderElement* pElement );
			int  AddChildElement( int index, CRenderElement* pElement );
			bool FindElement( const char* strName, CRenderElement** ppOutElement );
			int  ReplaceChildElement( int index, CRenderElement* pNew, CRenderElement** ppOld );
			int  GetChildElementCount() const;
			void RemoveChildElement( int index );
			void RemoveChildElementsAll();
			bool GetChildElement( int index, CRenderElement** ppElement ) const;
			CRenderElement* GetElement( int index ) const;
			void CopyChildElementsFormOther( CRenderElement* pSrcRoot );

		public:
			void SetState( int nState );

		public:
			virtual void Init() {}
			virtual void SetZOrder( int nZOrder ) { m_nZOrder = nZOrder; }
			virtual void UpdateRect();
			virtual void Update( float nElapsedTime ) override;
			virtual void Refresh() override;
			virtual void Render() override;


		protected:
			int m_nZOrder;
			//Renderer::UVertex_V3F_C4B_T2F m_MeshVertices[ 4 ];
			//uint16 m_MeshIndices[ 6 ];
			//Renderer::CMaterial* m_pMaterial;
			//Renderer::CTrianglesCommand m_RenderCommand;
			//RHI::IShaderState* m_pShaderState;
			//RHI::CShaderVariableTableUnion m_pShaderVariableTables;
			std::vector< PTR_CRenderElement >  m_ChildElements;
			int m_nState;


		public:
			static CRenderElement* sCloneRenderElementRecurse( CRenderElement* pElementSrc );
		};

		typedef CRenderElement CGuiElemBlank;
		IBERBAR_UNKNOWN_PTR_DECLARE( CGuiElemBlank );
	}
}





inline iberbar::Gui::CRenderElement* iberbar::Gui::CRenderElement::GetElement( int nIndex ) const
{
	if ( nIndex < 0 || nIndex >= (int)m_ChildElements.size() )
		return nullptr;
	return m_ChildElements[ nIndex ];
}

