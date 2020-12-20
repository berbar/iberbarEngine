#pragma once

#include <iberbar/RHI/Headers.h>
#include <iberbar/RHI/Types.h>
#include <iberbar/Utility/Result.h>
#include <functional>

namespace iberbar
{
	namespace RHI
	{
		class ITexture;
		class IVertexBuffer;
		class IIndexBuffer;
		class IShader;
		class IVertexDeclaration;
		class IShaderState;
		class IShaderVariableTable;
		class ICommandContext;
		class IDevice;
		struct UVertexElement;

		typedef CResult( UCallbackDeviceOnCreated )(IDevice* pDevice);
		typedef void(UCallbackDeviceOnLost)( IDevice* device );
		typedef CResult( UCallbackDeviceOnReset )(IDevice* device);
		typedef void(UCallbackDeviceOnRender)( IDevice* device );

		class __iberbarRHIApi__ IDevice abstract
			: public CRef
		{
		public:
			IDevice( UApiType nApiType );

		public:
			virtual void LostDevice() = 0;
			virtual CResult ResetDevice( int nBackBufferWidth, int nBackBufferHeight, bool bIsWindow ) = 0;
			virtual void CreateTexture( ITexture** ppTexture ) = 0;
			virtual CResult CreateVertexBuffer( uint32 nInSize, uint32 nUsage, IVertexBuffer** ppOutBuffer ) = 0;
			virtual CResult CreateIndexBuffer( uint32 nStride, uint32 nInSize, uint32 nUsage, IIndexBuffer** ppOutBuffer ) = 0;
			virtual CResult CreateShader( IShader** ppOutShader ) = 0;
			virtual CResult CreateVertexDeclaration( IVertexDeclaration** ppOutDeclaration, const UVertexElement* pVertexElements, uint32 nVertexElementsCount, uint32 nStride ) = 0;
			virtual CResult CreateShaderState( IShaderState** ppOutShaderState, IShader* pShader, IVertexDeclaration* pVertexDeclaration ) = 0;
			virtual void CreateShaderVariableTable( IShaderVariableTable** ppOutShaderVariableTable ) = 0;
			virtual void CreateCommandContext( ICommandContext** ppOutContext ) = 0;
			//virtual void Render() = 0;
			virtual void Begin() = 0;
			virtual void End() = 0;

		public:
			inline UApiType GetApiType() const { return m_nApiType; }
			const CSize2i& GetContextSize() const { return m_ContextSize; }
			void SetClearColor( const CColor4B& color ) { m_ClearColor = CColor4F( color ); }

		public:
			void SetCallbackOnCreated( std::function<UCallbackDeviceOnCreated> func ) { m_CallbackCreated = func; }
			void SetCallbackOnLost( std::function<UCallbackDeviceOnLost> func ) { m_CallbackLost = func; }
			void SetCallbackOnReset( std::function<UCallbackDeviceOnReset> func ) { m_CallbackReset = func; }
			void SetCallbackOnRender( std::function<UCallbackDeviceOnRender> func ) { m_CallbackRender = func; }

		protected:
			UApiType m_nApiType;
			bool m_bIsWindow;
			CSize2i m_ContextSize;
			CColor4F m_ClearColor;

			std::function<UCallbackDeviceOnCreated> m_CallbackCreated;
			std::function<UCallbackDeviceOnLost> m_CallbackLost;
			std::function<UCallbackDeviceOnReset> m_CallbackReset;
			std::function<UCallbackDeviceOnRender> m_CallbackRender;
		};
	}
}
