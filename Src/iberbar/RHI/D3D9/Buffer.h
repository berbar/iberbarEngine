#pragma once

#include <iberbar/RHI/D3D9/Headers.h>
#include <iberbar/RHI/Buffer.h>
#include <iberbar/RHI/D3D9/Device.h>


namespace iberbar
{
	namespace RHI
	{
		namespace D3D9
		{
			class CDevice;



			class __iberbarD3DApi__ CVertexBuffer
				: public IVertexBuffer
			{
			public:
				CVertexBuffer( CDevice* pDevice, uint32 nInSize, uint32 nUsage );
				virtual ~CVertexBuffer();

			public:
				virtual void OnLost() override;
				virtual CResult OnReset() override;
				virtual CResult Lock( uint32 nOffset, uint32 nSize, void** ppOut, uint32 nFlags = 0 ) override;
				virtual CResult Unlock() override;

			public:
				CResult Initial();
				void Destroy();
				inline IDirect3DVertexBuffer9* GetD3DVertexBuffer() { return m_pD3DVertexBuffer; }
				inline uint32 GetD3DUsage() const {
					uint32 nD3DUsage = 0;
					if ( IsDynamic() )
					{
						nD3DUsage |= D3DUSAGE_DYNAMIC;
					}
					nD3DUsage |= D3DUSAGE_WRITEONLY;
					return nD3DUsage;
				}

			protected:
				CDevice* m_pDevice;
				IDirect3DVertexBuffer9* m_pD3DVertexBuffer;
				bool m_bLocked;
			};




			class __iberbarD3DApi__ CIndexBuffer
				: public IIndexBuffer
			{
			public:
				CIndexBuffer( CDevice* pDevice, uint32 nInSize, uint32 nUsage );
				virtual ~CIndexBuffer();

			public:
				virtual void OnLost() override;
				virtual CResult OnReset() override;
				virtual CResult Lock( uint32 nOffset, uint32 nSize, void** ppOut, uint32 nFlags = 0 ) override;
				virtual CResult Unlock() override;

			public:
				CResult Initial();
				void Destroy();
				inline IDirect3DIndexBuffer9* GetD3DIndexBuffer() { return m_pD3DIndexBuffer; }
				inline uint32 GetD3DUsage() const {
					uint32 nD3DUsage = 0;
					if ( IsDynamic() )
					{
						nD3DUsage |= D3DUSAGE_DYNAMIC;
					}
					nD3DUsage |= D3DUSAGE_WRITEONLY;
					return nD3DUsage;
				}

			protected:
				CDevice* m_pDevice;
				IDirect3DIndexBuffer9* m_pD3DIndexBuffer;
				bool m_bLocked;
			};
		}
	}
}

