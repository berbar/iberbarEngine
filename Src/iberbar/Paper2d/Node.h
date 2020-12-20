#pragma once

#include <iberbar/Paper2d/Headers.h>
#include <iberbar/Paper2d/Transform.h>
#include <iberbar/Utility/Rect.h>
#include <iberbar/Utility/Clonable.h>
#include <DirectXMath.h>

namespace iberbar
{
	namespace RHI
	{
		class IShaderState;
		class IShaderVariableTable;
	}

	namespace Paper2d
	{
		class CNodeTransform;
		class CDrawContext;
		class CComponentContainer;

		class __iberbarPaper2dApis__ CNode
			: public CRef, public IClonable
		{
		public:
			CNode();
			virtual ~CNode();
		protected:
			CNode( const CNode& node );

		public:
			virtual CNode* Clone() const override;

		public:
			void SetVisiable( bool bVisible );
			void SetEnable( bool bEnable );
			// 只获取当前节点的visible值，要判断当前节点是否可见，使用IsVisible
			bool GetVisible() const;
			// 只获取当前节点的enable值，要判断当前节点是否可用，使用IsEnable
			bool GetEnable() const;
			// 判断当前节点是否可见
			bool IsVisible() const;
			// 判断当前节点是否可用
			bool IsEnable() const;

			void SetZOrder( int nZOrder );
			void SetId( const char* strId );
			void SetAnchor( float nX, float nY );
			void SetPivot( float nX, float nY );
			void SetPosition( float nX, float nY );
			void SetPositionAbsolutely( float nX, float nY );
			void SetContentSize( float nW, float nH );
			

			int GetZOrder() const;
			const std::string& GetId() const;
			const DirectX::XMFLOAT2& GetAnchor() const;
			float GetAnchorX() const;
			float GetAnchorY() const;
			const DirectX::XMFLOAT2& GetPivot() const;
			float GetPivotX() const;
			float GetPivotY() const;
			const DirectX::XMFLOAT2& GetPosition() const;
			float GetPositionX() const;
			float GetPositionY() const;
			const DirectX::XMFLOAT2& GetContentSize() const;
			float GetContentSizeX() const;
			float GetContentSizeY() const;
			const CNodeTransform* GetTransform() const;
			bool IsTransformDirty() const;
			void ConvertFromAbsolutePoint( DirectX::XMFLOAT2& Point );
			void ConvertToAbsolutePoint( DirectX::XMFLOAT2& Point );

			CComponentContainer* GetComponentContainer() const;

		public:
			CNode* GetParentNode() const { return m_pParentNode; }
			void InsertChild( CNode* pNode );
			void RemoveChild( CNode* pNode );
			void RemoveAllChildren();
			CNode* FindChild( const char* strName ) const;
			const std::list<CNode*>& GetChildren() const { return m_Children; }


		public:
			virtual void Init();
			virtual void UpdateTransform();
			virtual void Draw( CDrawContext* pContext );
			virtual void Update( float nDelta );


		protected:
			void SetTransformDirty();
			virtual void DrawSelf( CDrawContext* pContext );
			virtual void UpdateSelf( float nDelta );

		private:
			CNode* m_pParentNode;
			CComponentContainer* m_pComponentContainer;
		protected:
			bool m_bVisible; // 是否可见，控制渲染
			bool m_bEnable; // 主要控制鼠标、键盘输入和updatable

			RHI::IShaderState* m_pShaderState;
			RHI::IShaderVariableTable* m_pShaderVariableTable;
			int m_nZOrder;
			std::string m_strId;
			std::list<CNode*> m_Prepare;
			std::list<CNode*> m_Children;

			CNodeTransform* m_pTransform;
			bool m_bTransformDirty;
		};
	}
}





inline void iberbar::Paper2d::CNode::SetVisiable( bool bVisible )
{
	m_bVisible = bVisible;
}


inline void iberbar::Paper2d::CNode::SetEnable( bool bEnable )
{
	m_bEnable = bEnable;
}


inline bool iberbar::Paper2d::CNode::GetVisible() const
{
	return m_bVisible;
}


inline bool iberbar::Paper2d::CNode::GetEnable() const
{
	return m_bEnable;
}


inline void iberbar::Paper2d::CNode::SetZOrder( int nZOrder )
{
	m_nZOrder = nZOrder;
}


inline void iberbar::Paper2d::CNode::SetId( const char* strId )
{
	m_strId = strId;
}


inline void iberbar::Paper2d::CNode::SetAnchor( float nX, float nY )
{
	m_pTransform->SetAnchor( nX, nY );
	SetTransformDirty();
}


inline void iberbar::Paper2d::CNode::SetPivot( float nX, float nY )
{
	m_pTransform->SetPivot( nX, nY );
	SetTransformDirty();
}


inline void iberbar::Paper2d::CNode::SetPosition( float nX, float nY )
{
	m_pTransform->SetPosition( nX, nY );
	SetTransformDirty();
}


inline void iberbar::Paper2d::CNode::SetPositionAbsolutely( float nX, float nY )
{
	m_pTransform->SetPositionAbsolutely( nX, nY );
	SetTransformDirty();
}


inline void iberbar::Paper2d::CNode::SetContentSize( float nW, float nH )
{
	m_pTransform->SetContentSize( nW, nH );
	SetTransformDirty();
}


inline int iberbar::Paper2d::CNode::GetZOrder() const
{
	return m_nZOrder;
}


inline const std::string& iberbar::Paper2d::CNode::GetId() const
{
	return m_strId;
}


inline const DirectX::XMFLOAT2& iberbar::Paper2d::CNode::GetAnchor() const
{
	return m_pTransform->GetAnchor();
}


inline float iberbar::Paper2d::CNode::GetAnchorX() const
{
	return m_pTransform->GetAnchor().x;
}


inline float iberbar::Paper2d::CNode::GetAnchorY() const
{
	return m_pTransform->GetAnchor().y;
}


inline const DirectX::XMFLOAT2& iberbar::Paper2d::CNode::GetPivot() const
{
	return m_pTransform->GetPivot();
}


inline float iberbar::Paper2d::CNode::GetPivotX() const
{
	return m_pTransform->GetPivot().x;
}


inline float iberbar::Paper2d::CNode::GetPivotY() const
{
	return m_pTransform->GetPivot().y;
}


inline const DirectX::XMFLOAT2& iberbar::Paper2d::CNode::GetPosition() const
{
	return m_pTransform->GetPosition();
}


inline float iberbar::Paper2d::CNode::GetPositionX() const
{
	return m_pTransform->GetPosition().x;
}


inline float iberbar::Paper2d::CNode::GetPositionY() const
{
	return m_pTransform->GetPosition().y;
}


inline const DirectX::XMFLOAT2& iberbar::Paper2d::CNode::GetContentSize() const
{
	return m_pTransform->GetContentSize();
}


inline float iberbar::Paper2d::CNode::GetContentSizeX() const
{
	return m_pTransform->GetContentSize().x;
}


inline float iberbar::Paper2d::CNode::GetContentSizeY() const
{
	return m_pTransform->GetContentSize().y;
}


inline const iberbar::Paper2d::CNodeTransform* iberbar::Paper2d::CNode::GetTransform() const
{
	return m_pTransform;
}


inline bool iberbar::Paper2d::CNode::IsTransformDirty() const
{
	return m_bTransformDirty;
}


inline void iberbar::Paper2d::CNode::ConvertFromAbsolutePoint( DirectX::XMFLOAT2& Point )
{
	m_pTransform->ConvertFromAbsolutePoint( Point );
}


inline void iberbar::Paper2d::CNode::ConvertToAbsolutePoint( DirectX::XMFLOAT2& Point )
{
	m_pTransform->ConvertToAbsolutePoint( Point );
}


inline iberbar::Paper2d::CComponentContainer* iberbar::Paper2d::CNode::GetComponentContainer() const
{
	return m_pComponentContainer;
}
