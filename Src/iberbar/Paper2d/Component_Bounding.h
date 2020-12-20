#pragma once



#include <iberbar/Paper2d/Component.h>
#include <DirectXCollision.h>


namespace iberbar
{
	namespace Paper2d
	{
		enum class UComponentBoundingType
		{
			// ÓëXYÖáÆ½ÐÐ
			BoundingBox = 1,
			//BoundingOrientedBox = 2,
			//BoundingFrustum = 3,
			Custom = 0xffff
		};

		class __iberbarPaper2dApis__ CComponent_Bounding abstract
			: public CComponent
		{
		public:
			CComponent_Bounding( UComponentBoundingType nBoundingType );
		public:
			virtual void OnUpdateTransform() = 0;
			UComponentBoundingType GetBoundingType() const;
		private:
			UComponentBoundingType m_nBoundingType;
		};


		class __iberbarPaper2dApis__ CComponent_BoundingBox
			: public CComponent_Bounding
		{
		public:
			CComponent_BoundingBox();

		public:
			virtual void OnUpdateTransform() override;

			DirectX::ContainmentType Contains( const DirectX::XMFLOAT2& Point ) const;
			DirectX::ContainmentType Contains( const CComponent_BoundingBox* pComponent ) const;
			const DirectX::BoundingBox& GetBox() const;
			

		private:
			DirectX::BoundingBox m_Box;
		};
	}
}


FORCEINLINE iberbar::Paper2d::UComponentBoundingType iberbar::Paper2d::CComponent_Bounding::GetBoundingType() const
{
	return m_nBoundingType;
}


FORCEINLINE DirectX::ContainmentType iberbar::Paper2d::CComponent_BoundingBox::Contains( const DirectX::XMFLOAT2& Point ) const
{
	DirectX::XMVECTOR vec_Point = DirectX::XMLoadFloat2( &Point );
	return m_Box.Contains( vec_Point );
}


FORCEINLINE DirectX::ContainmentType iberbar::Paper2d::CComponent_BoundingBox::Contains( const CComponent_BoundingBox* pComponent ) const
{
	return m_Box.Contains( pComponent->m_Box );
}


FORCEINLINE const DirectX::BoundingBox& iberbar::Paper2d::CComponent_BoundingBox::GetBox() const
{
	return m_Box;
}

