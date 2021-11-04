#pragma once


#include <iberbar/Renderer/Headers.h>
#include <iberbar/Renderer/ShaderVariables.h>
#include <iberbar/Utility/Clonable.h>



namespace iberbar
{
	namespace RHI
	{
		class IShaderState;
	}

	namespace Renderer
	{


		class __iberbarRendererApi__ CMaterial
			: public CRef, public IClonable
		{
		public:
			CMaterial();
			CMaterial( CMaterial* pMaterialOrigin );
			virtual ~CMaterial();

			virtual CMaterial* Clone() const override { return new CMaterial( *this ); }

			void SetShaderState( RHI::IShaderState* pShaderState );
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
			bool SetTexture( const char* pstrName, RHI::ITexture** ppTextures, uint32 nElementCount = 1, uint32 nElementStart = 0 );
			bool SetSamplerState( const char* pstrName, RHI::ISamplerState** pSamplerStates, uint32 nElementCount = 1, uint32 nElementStart = 0 );
			void Reset();

			RHI::IShaderState* GetShaderState() const { return m_pShaderState; }
			CShaderVariableTable* GetShaderVariableTable( RHI::EShaderType nShaderType ) { return &m_VariableTables[ (int)nShaderType ]; }
			const CShaderVariableTable* GetShaderVariableTable( RHI::EShaderType nShaderType ) const { return &m_VariableTables[ (int)nShaderType ]; }

			bool CampareWithMaterial( const CMaterial* pMaterialOther ) const;

		protected:
			CMaterial* m_pMaterialParent;
			RHI::IShaderState* m_pShaderState;
			CShaderVariableTable m_VariableTables[ (int)RHI::EShaderType::__Count ];
		};
	}
}

