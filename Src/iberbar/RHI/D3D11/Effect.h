#pragma once



#include <iberbar/RHI/D3D11/Headers.h>
#include <iberbar/RHI/D3D11/Types.h>
#include <iberbar/RHI/Effect.h>


namespace iberbar
{
	namespace RHI
	{
		namespace D3D11
		{
			class CShaderState;
			class CUniformBuffer;
			class CUniformMemoryBuffer;

			class CEffect
				: public IEffect
			{
			public:
				CEffect( IShaderState* pShaderState );
				virtual ~CEffect();

				virtual void SetShaderVariables( EShaderType nShaderType, IShaderVariableTable* pShaderVariables ) override;

				//FORCEINLINE void ClearDirty()
				//{
				//	memset( m_UniformBuffersDirty, 0, sizeof( m_UniformBuffersDirty ) );
				//}

			protected:
				CShaderState* m_pShaderState;
				uint8* m_UniformMemorys[ (int)EShaderType::__Count ];
				CUniformBuffer* m_UniformBuffers[ (int)EShaderType::__Count ][ D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ];
				//uint8 m_UniformBuffersDirty[ (int)EShaderType::__Count ][ D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ];
				
			};
		}
	}
}

