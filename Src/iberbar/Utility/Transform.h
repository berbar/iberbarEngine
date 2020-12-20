
#pragma once


#include <iberbar/Utility/Ref.h>
#include <iberbar/Utility/Rect.h>
#include <iberbar/Utility/Clonable.h>

#include <vector>

namespace iberbar
{
	class CTransform2D;
	
	IBERBAR_UNKNOWN_PTR_DECLARE( CTransform2D );

	
	
	
	class __iberbarUtilityApi__ CTransform2D
		: public CRef, public IClonable
	{
	protected:
		typedef std::vector< PTR_CTransform2D >   Array_Transfrom2D;


	public:
		CTransform2D();
		virtual ~CTransform2D() override;
		virtual CTransform2D* Clone() const override;

	protected:
		CTransform2D( const CTransform2D& trans );

	public:
		void SetPosition( const CPoint2i& pt ) { m_Position = pt; UpdateTransform(); }
		void SetSize( const CSize2i& size ) { m_Size = size; UpdateTransform(); }
		void SetAlignHorizental( UAlignHorizental nAlign ) { m_alignHorizental = nAlign; UpdateTransform(); }
		void SetAlignVertical( UAlignVertical nAlign ) { m_alignVertical = nAlign; UpdateTransform(); }
		void SetPercentX( bool bValue ) { m_bPercentX = bValue; UpdateTransform(); }
		void SetPercentY( bool bValue ) { m_bPercentY = bValue; UpdateTransform(); }
		void SetPercentW( bool bValue ) { m_bPercentW = bValue; UpdateTransform(); }
		void SetPercentH( bool bValue ) { m_bPercentH = bValue; UpdateTransform(); }

		void SetParentTransform( CTransform2D* parent );
		void UpdateTransform();


	public:
		const CPoint2i& GetPosition() const { return m_Position; }
		int GetPositionX() const { return m_Position.x; }
		int GetPositionY() const { return m_Position.y; }

		const CSize2i& GetSize() const { return m_Size; }

		CTransform2D* GetParentTransform() const { return m_ParentTransform; }

		UAlignHorizental GetAlignHorizental() const { return m_alignHorizental; }

		UAlignVertical GetAlignVertical() const { return m_alignVertical; }

		const CRect2i& GetBounding() const { return m_rcBounding; }

		bool HitTest( const CPoint2i& point ) const { return RectPointHitTest( &GetBounding(), &point ); }

	private:
		bool m_bPercentX : 1;
		bool m_bPercentY : 1;
		bool m_bPercentW : 1;
		bool m_bPercentH : 1;
		UAlignStyle m_alignStyle;
		UAlignHorizental m_alignHorizental;
		UAlignVertical m_alignVertical;

		CPoint2i m_Position;
		CSize2i m_Size;

		CTransform2D*    m_ParentTransform;   // do not add reference

		CRect2i m_rcBounding;

		Array_Transfrom2D  m_ChildTransformArray; // add reference +1
	};

	__iberbarExportTemplateRef( __iberbarUtilityApi__, CTransform2D );

	//template class __iberbarUtilityApi__ std::allocator<PTR_CTransform2D>;
	//template class __iberbarUtilityApi__ std::vector<PTR_CTransform2D, std::allocator<PTR_CTransform2D>>;
}

