#pragma once

#include <iberbar/RHI/OpenGL/Headers.h>
#include <iberbar/RHI/Device.h>
#include <functional>

namespace iberbar
{
	namespace RHI
	{
		namespace OpenGL
		{
			class CDevice;

			typedef CResult(UCallbackDeviceOnCreated)(CDevice* device);
			typedef void(UCallbackDeviceOnLost)(CDevice* device);
			typedef CResult(UCallbackDeviceOnReset)(CDevice* device);
			typedef void(UCallbackDeviceOnRender)( CDevice* device );

			class __iberbarRHIOpenGLApi__ CDevice
				: public IDevice
			{
			public:
				CDevice();
				virtual ~CDevice();

			public:
#ifdef _WINDOWS
				virtual CResult CreateDevice( HWND hWnd, bool bWindowed, int nSuitedWidth, int nSuitedHeight ) override;
#else
				CResult CreateDevice( int nSuitedWidth, int nSuitedHeight );
#endif
				virtual void LostDevice() override;
				virtual CResult ResetDevice( int nBackBufferWidth, int nBackBufferHeight, bool bIsWindow ) override;
				virtual void CreateTexture( ITexture** ppTexture ) override;
				virtual CResult CreateVertexBuffer( uint32 nInSize, uint32 nUsage, IVertexBuffer** ppOutBuffer ) override;
				virtual CResult CreateIndexBuffer( uint32 nStride, uint32 nInSize, uint32 nUsage, IIndexBuffer** ppOutBuffer ) override;
				virtual CResult CreateShader( IShader** ppShader ) override;
				virtual CResult CreateVertexDeclaration( IVertexDeclaration** ppOutDeclaration, const UVertexElement* pVertexElements, uint32 nVertexElementsCount, uint32 nStride ) override;
				virtual CResult CreateShaderState( IShaderState** ppOutShaderState, IShader* pShader, IVertexDeclaration* pVertexDeclaration ) override;
				virtual void CreateShaderVariableTable( IShaderVariableTable** ppOutShaderVariableTable ) override;
				virtual void CreateCommandContext( ICommandContext** ppOutContext ) override;
				//virtual void Render() override;
				virtual CResult Begin() override;
				virtual void End() override;

			public:
				void BindArrayBuffer( GLuint Buffer );
				void BindElementArrayBuffer( GLuint Buffer );


			public:
				void SetSamplerState( uint32 nStage, const UTextureSamplerState& SamplerState );

			protected:
				void OnCreated();


			protected:
#ifdef _WINDOWS
				HWND m_hWnd;
				HDC m_hDC;
				HGLRC m_hRC;
#endif
				bool m_bHasLostDevice;

				GLuint m_ArrayBufferBound;
				GLuint m_ElementArrayBufferBound;

				GLuint m_SamplerStateList[ 8 ];
			};
		}
	}
}


inline void iberbar::RHI::OpenGL::CDevice::BindArrayBuffer( GLuint Buffer )
{
	if ( m_ArrayBufferBound != Buffer )
	{
		glBindBuffer( GL_ARRAY_BUFFER, Buffer );
		m_ArrayBufferBound = Buffer;
	}
}

inline void iberbar::RHI::OpenGL::CDevice::BindElementArrayBuffer( GLuint Buffer )
{
	if ( m_ElementArrayBufferBound != Buffer )
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, Buffer );
		m_ElementArrayBufferBound = Buffer;
	}
}
