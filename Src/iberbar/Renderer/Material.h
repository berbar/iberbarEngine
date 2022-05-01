#pragma once


#include <iberbar/Renderer/Headers.h>
#include <iberbar/Renderer/ShaderVariables.h>
#include <iberbar/Utility/Clonable.h>



namespace iberbar
{
	namespace RHI
	{
		class IShaderProgram;
		class IShaderState;
		class IUniformBuffer;
	}

	namespace Renderer
	{

		class __iberbarRendererApi__ CMaterial
			: public CRef
		{
		public:
			CMaterial();
			CMaterial( CMaterial* pMaterialOrigin );
			CMaterial( const CMaterial& Material ) = delete;
			virtual ~CMaterial();

			bool SetInt( const char* strName, int32 Value );
			bool SetIntArray( const char* strName, const int32* pValues, uint32 nCount );
			bool SetFloat( const char* strName, float Value );
			bool SetFloatArray( const char* strName, const float* pValues, uint32 nCount );
			bool SetVector( const char* strName, const DirectX::XMFLOAT4& Value );
			bool SetVectorArray( const char* strName, const DirectX::XMFLOAT4* pValues, uint32 nCount );
			bool SetMatrix( const char* strName, const DirectX::XMFLOAT4X4& Value );
			bool SetMatrixArray( const char* strName, const DirectX::XMFLOAT4X4* pValues, uint32 nCount );
			bool SetColor( const char* strName, const CColor4F& Value );
			bool SetColorArray( const char* strName, const CColor4F* pValues, uint32 nCount );
			bool SetStruct( const char* strName, const void* Value, uint32 nSize );
			bool SetTexture( const char* pstrName, RHI::ITexture* pTexture );
			bool SetTexture( const char* pstrName, RHI::ITexture** ppTextures, uint32 nElementCount, uint32 nElementStart );
			bool SetSamplerState( const char* pstrName, RHI::ISamplerState* pSamplerState );
			bool SetSamplerState( const char* pstrName, RHI::ISamplerState** ppSamplerStates, uint32 nElementCount, uint32 nElementStart );
			void Reset();

			const std::vector<uint32>& GetStreams() const { return m_Streams; }
			RHI::IShaderState* GetShaderState() const { return m_pShaderState; }
			//CShaderVariableTable* GetShaderVariableTable( RHI::EShaderType nShaderType ) { return &m_VariableTables[ (int)nShaderType ]; }
			//const CShaderVariableTable* GetShaderVariableTable( RHI::EShaderType nShaderType ) const { return &m_VariableTables[ (int)nShaderType ]; }
			inline CShaderVariableTable *const *const GetShaderVariableTables() const { return m_VariableTables; }
			//inline const std::vector<RHI::IUniformBuffer*> GetShaderUniformBuffers( RHI::EShaderType nShaderType ) const { return m_ShaderUniformBuffers[ (int)nShaderType ]; }

			bool CampareWithMaterial( const CMaterial* pMaterialOther ) const;

		public:
			void Initial( RHI::IShaderProgram* pShaderProgram );

		protected:
			void InitialVariableTables();

		protected:
			CMaterial* m_pMaterialParent;
			RHI::IShaderState* m_pShaderState;
			std::vector<uint32> m_Streams;
			
			CShaderVariableTable* m_VariableTables[ (int)RHI::EShaderType::__Count ];
			//std::vector<int> m_ShaderUniformBuffersDirty[ (int)RHI::EShaderType::__Count ];
			//std::vector<RHI::IUniformBuffer*> m_ShaderUniformBuffers[ (int)RHI::EShaderType::__Count ];
		};
	}
}

