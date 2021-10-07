#pragma once

#include <iberbar/RHI/D3D9/Headers.h>
#include <iberbar/RHI/StateBlock.h>



namespace iberbar
{
	namespace RHI
	{
		namespace D3D9
		{
			class CDevice;

			class __iberbarD3DApi__ CStateBlock
				: public RHI::CStateBlock
			{
			public:
				CStateBlock( CDevice* pDevice, UStateBlockType nBlockState );
				virtual ~CStateBlock();

			public:
				CResult Initial();
				virtual CResult Capture() override;
				virtual CResult Apply() override;

			protected:
				CDevice* m_pDevice;
				UStateBlockType m_nBlockState;
				IDirect3DStateBlock9* m_pD3DStateBlock;
			};
		}
	}
}