#pragma once

#include <iberbar/Gui/Headers.h>
#include <iberbar/Gui/BaseTypes.h>
#include <iberbar/Utility/Ref.h>
#include <iberbar/Utility/Clonable.h>
#include <iberbar/Utility/Transform.h>

namespace iberbar
{
	namespace Gui
	{
		class __iberbarGuiApi__ CObject abstract
			: public CRef, public IClonable
		{
		public:
			CObject();
			virtual ~CObject();

		protected:
			CObject( const CObject& );

		public:
			void SetId( const char* strId );
			const std::string& GetId() const;
			void SetName( const std::string& strName );
			const std::string& GetName() const;
			void SetVisible( bool bVisible = true );
			bool GetVisible() const;
			void SetEnable( bool bEnable = true );
			bool GetEnable() const;

			void SetPosition( const CPoint2i& point );
			void SetPosition( int nX, int nY );
			const CPoint2i& GetPosition() const;
			void SetSize( const CSize2i& size );
			void SetSize( int nW, int nH );
			const CSize2i& GetSize() const;
			void SetAlignHorizental( UAlignHorizental align );
			void SetAlignVertical( UAlignVertical align );
			void SetPercentX( bool bValue );
			void SetPercentY( bool bValue );
			void SetPercentW( bool bValue );
			void SetPercentH( bool bValue );

			UAlignHorizental GetAlignHorizental() const;
			UAlignVertical GetAlignVertical() const;
			const CRect2i& GetBounding() const;
			PTR_CTransform2D GetTransform() const;

		public:
			virtual void UpdateRect() {}
			virtual void OnShow() {}
			virtual void OnHide() {}
			virtual void OnEnable() {}
			virtual void OnDisable() {}

			virtual bool HitTest( const CPoint2i& point ) const;
			virtual void Update( float nElapsedTime ) = 0;
			virtual void Refresh() = 0;
			virtual void Render() = 0;

		protected:
			std::string m_nId;
			std::string m_strName;

			bool m_bVisible;
			bool m_bEnable;

			PTR_CTransform2D m_pTransform;
		};
	}
}





inline void iberbar::Gui::CObject::SetId( const char* nId )
{
	m_nId = nId;
}

inline const std::string& iberbar::Gui::CObject::GetId() const
{
	return m_nId;
}

inline void iberbar::Gui::CObject::SetName( const std::string& strName )
{
	m_strName = strName;
}

inline const std::string& iberbar::Gui::CObject::GetName() const
{
	return m_strName;
}

inline bool iberbar::Gui::CObject::GetVisible() const
{
	return m_bVisible;
}

inline bool iberbar::Gui::CObject::GetEnable() const
{
	return m_bEnable;
}

inline void iberbar::Gui::CObject::SetPosition( const CPoint2i& point )
{
	m_pTransform->SetPosition( point );
	UpdateRect();
}

inline void iberbar::Gui::CObject::SetPosition( int nX, int nY )
{
	m_pTransform->SetPosition( CPoint2i( nX, nY ) );
	UpdateRect();
}

inline const iberbar::CPoint2i& iberbar::Gui::CObject::GetPosition() const
{
	return m_pTransform->GetPosition();
}

inline void iberbar::Gui::CObject::SetSize( const CSize2i& size )
{
	m_pTransform->SetSize( size );
	UpdateRect();
}

inline void iberbar::Gui::CObject::SetSize( int nW, int nH )
{
	m_pTransform->SetSize( CSize2i( nW, nH ) );
	UpdateRect();
}

inline const iberbar::CSize2i& iberbar::Gui::CObject::GetSize() const
{
	return m_pTransform->GetSize();
}

inline void iberbar::Gui::CObject::SetAlignHorizental( UAlignHorizental nAlign )
{
	m_pTransform->SetAlignHorizental( nAlign );
	UpdateRect();
}

inline void iberbar::Gui::CObject::SetAlignVertical( UAlignVertical nAlign )
{
	m_pTransform->SetAlignVertical( nAlign );
	UpdateRect();
}

inline void iberbar::Gui::CObject::SetPercentX( bool bValue )
{
	m_pTransform->SetPercentX( bValue );
	UpdateRect();
}

inline void iberbar::Gui::CObject::SetPercentY( bool bValue )
{
	m_pTransform->SetPercentY( bValue );
	UpdateRect();
}

inline void iberbar::Gui::CObject::SetPercentW( bool bValue )
{
	m_pTransform->SetPercentW( bValue );
	UpdateRect();
}

inline void iberbar::Gui::CObject::SetPercentH( bool bValue )
{
	m_pTransform->SetPercentH( bValue );
	UpdateRect();
}

inline iberbar::UAlignHorizental iberbar::Gui::CObject::GetAlignHorizental() const
{
	return m_pTransform->GetAlignHorizental();
}

inline iberbar::UAlignVertical iberbar::Gui::CObject::GetAlignVertical() const
{
	return m_pTransform->GetAlignVertical();
}

inline const iberbar::CRect2i& iberbar::Gui::CObject::GetBounding() const
{
	return m_pTransform->GetBounding();
}

inline iberbar::PTR_CTransform2D iberbar::Gui::CObject::GetTransform() const
{
	return m_pTransform;
}

