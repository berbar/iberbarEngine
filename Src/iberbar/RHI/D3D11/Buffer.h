#pragma once

#include <iberbar/RHI/D3D11/Headers.h>
#include <iberbar/RHI/Buffer.h>
#include <iberbar/RHI/D3D11/Device.h>


namespace iberbar
{
	namespace RHI
	{
		namespace D3D11
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
				FORCEINLINE ID3D11Buffer* GetD3DBuffer() { return m_pD3DBuffer.Get(); }
				FORCEINLINE D3D11_USAGE GetD3DUsage() const {
					D3D11_USAGE nD3DUsage = D3D11_USAGE_DEFAULT;
					if ( IsDynamic() )
					{
						nD3DUsage = D3D11_USAGE_DYNAMIC;
					}
					//nD3DUsage |= D3DUSAGE_WRITEONLY;
					return nD3DUsage;
				}
				FORCEINLINE UINT GetD3DCpuAccess()
				{
					UINT nFlag = 0;
					if ( IsDynamic() )
					{
						nFlag = (UINT)D3D11_CPU_ACCESS_WRITE;
					}
					return nFlag;
				}

			protected:
				CDevice* m_pDevice;
				ComPtr<ID3D11Buffer> m_pD3DBuffer;
				bool m_bLocked;
				D3D11_MAPPED_SUBRESOURCE m_D3DMappedSubResource;
			};




			class __iberbarD3DApi__ CIndexBuffer
				: public IIndexBuffer
			{
			public:
				CIndexBuffer( CDevice* pDevice, uint32 nInStride, uint32 nInSize, uint32 nUsage );
				virtual ~CIndexBuffer();

			public:
				virtual void OnLost() override;
				virtual CResult OnReset() override;
				virtual CResult Lock( uint32 nOffset, uint32 nSize, void** ppOut, uint32 nFlags = 0 ) override;
				virtual CResult Unlock() override;

			public:
				CResult Initial();
				void Destroy();
				FORCEINLINE ID3D11Buffer* GetD3DBuffer() { return m_pD3DBuffer.Get(); }
				FORCEINLINE D3D11_USAGE GetD3DUsage() const {
					D3D11_USAGE nD3DUsage = D3D11_USAGE_DEFAULT;
					if ( IsDynamic() )
					{
						nD3DUsage = D3D11_USAGE_DYNAMIC;
					}
					return nD3DUsage;
				}
				FORCEINLINE UINT GetD3DCpuAccess()
				{
					UINT nFlag = 0;
					if ( IsDynamic() )
					{
						nFlag = (UINT)D3D11_CPU_ACCESS_WRITE;
					}
					return nFlag;
				}

			protected:
				CDevice* m_pDevice;
				ComPtr<ID3D11Buffer> m_pD3DBuffer;
				bool m_bLocked;
				D3D11_MAPPED_SUBRESOURCE m_D3DMappedSubResource;
			};


			class CConstantBuffer
			{
			public:
				CConstantBuffer( CDevice* pDevice, uint32 nSize, uint32 nUsage );
				~CConstantBuffer();

			public:
				CResult Initial();
				void Destroy();

				FORCEINLINE ID3D11Buffer* GetD3DBuffer() { return m_pD3DBuffer.Get(); }
				FORCEINLINE ID3D11Buffer* const* GetD3DBufferAddress() { return m_pD3DBuffer.GetAddressOf(); }
				FORCEINLINE bool IsDynamic() const { return ( m_nUsage & UBufferUsageFlags::AnyDynamic ) != 0; }
				FORCEINLINE D3D11_USAGE GetD3DUsage() const {
					D3D11_USAGE nD3DUsage = D3D11_USAGE_DEFAULT;
					if ( IsDynamic() )
					{
						nD3DUsage = D3D11_USAGE_DYNAMIC;
					}
					return nD3DUsage;
				}
				FORCEINLINE UINT GetD3DCpuAccess()
				{
					UINT nFlag = 0;
					if ( IsDynamic() )
					{
						nFlag = (UINT)D3D11_CPU_ACCESS_WRITE;
					}
					return nFlag;
				}

			protected:
				CDevice* m_pDevice;
				uint32 m_nSize;
				uint32 m_nUsage;
				ComPtr<ID3D11Buffer> m_pD3DBuffer;
			};
		}
	}
}

