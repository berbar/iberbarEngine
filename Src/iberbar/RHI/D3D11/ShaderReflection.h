#pragma once

#include <iberbar/RHI/D3D11/Headers.h>
#include <iberbar/RHI/ShaderReflection.h>


namespace iberbar
{
	namespace RHI
	{
		namespace D3D11
		{
			class CShaderReflectionBuffer;
			class CShaderReflectionVariable;
			class CShaderReflectionType;
			class CShaderReflection;
			class CShaderReflectionMember;




			class CShaderReflectionType
				: public IShaderReflectionType
			{
			public:
				CShaderReflectionType();

				virtual const IShaderReflectionMember* GetMemberByIndex( int nIndex ) const override;
				virtual const IShaderReflectionMember* GetMemberByName( const char* pstrName ) const override;

				CResult Initial( ID3D11ShaderReflectionType* pD3DShaderReflectionType );

			protected:
				std::vector<CShaderReflectionMember*> m_Members;
			};




			class CShaderReflectionMember
				: public IShaderReflectionMember
			{
			public:
				CShaderReflectionMember();

				virtual const IShaderReflectionType* GetType() const override { return &m_ReflectionType; }

				CResult Initial( ID3D11ShaderReflectionType* pD3DShaderReflectionType, const char* pstrName );

			protected:
				uint32 m_nOffset;
				std::string m_strName;
				CShaderReflectionType m_ReflectionType;
			};



			class CShaderReflectionVariable
				: public IShaderReflectionVariable
			{
			public:
				CShaderReflectionVariable();

				virtual const IShaderReflectionType* GetType() const override { return &m_ReflectionType; }

				CResult Initial( ID3D11ShaderReflectionVariable* pD3DShaderReflectionVariable, uint32 nCBufferBytesOffset );

				FORCEINLINE const CShaderReflectionType* GetTypeInternal() const { return &m_ReflectionType; }

			protected:
				CShaderReflectionType m_ReflectionType;
			};



			class CShaderReflectionBuffer
				: public IShaderReflectionBuffer
			{
			public:
				CShaderReflectionBuffer();

				virtual const IShaderReflectionVariable* GetVariableByIndex( int nIndex ) const override { return GetVariableByIndexInternal( nIndex ); }
				virtual const IShaderReflectionVariable* GetVariableByName( const char* pstrName ) const override;

				CResult Initial( const D3D11_SHADER_INPUT_BIND_DESC& D3DBindDesc, ID3D11ShaderReflectionConstantBuffer* pD3DShaderReflectionConstantBuffer, uint32 nOffset );

				FORCEINLINE const CShaderReflectionVariable* GetVariableByIndexInternal( int nIndex ) const { return &m_Variables[ nIndex ]; }

			public:
				std::vector<CShaderReflectionVariable> m_Variables;
			};


			class CShaderReflectionBindResource
				: public IShaderReflectionBindResource
			{
			public:
				CShaderReflectionBindResource( UShaderVariableType nVarType, const D3D11_SHADER_INPUT_BIND_DESC& Desc )
				{
					m_nVarType = nVarType;
					m_nBindSlot = Desc.BindPoint;
					m_nBindCount = Desc.BindCount;
					m_strName = Desc.Name;
				}
			};


			class CShaderReflection
				: public IShaderReflection
			{
			public:
				CShaderReflection();

				virtual const IShaderReflectionBuffer* GetBufferByIndex( int nIndex ) const { return GetBufferByIndexInternal( nIndex ); }
				virtual const IShaderReflectionBuffer* GetBufferByName( const char* pstrName ) const;
				virtual const IShaderReflectionVariable* GetVariableByName( const char* pstrName ) const override;
				virtual const IShaderReflectionBindResource* GetTextureByName( const char* pstrName ) const override;
				virtual const IShaderReflectionBindResource* GetSamplerStateByName( const char* pstrName ) const override;

				CResult Initial( const void* pCodes, uint32 nCodeLen );

				FORCEINLINE const CShaderReflectionBuffer* GetBufferByIndexInternal( int nIndex ) const { return &m_ConstBuffers[ nIndex ]; }
				
			protected:
				std::vector<CShaderReflectionBuffer> m_ConstBuffers;
				std::vector<const CShaderReflectionVariable*> m_Vars;
				std::vector<CShaderReflectionBindResource> m_SamplerStates;
				std::vector<CShaderReflectionBindResource> m_Textures;
			};
		}
	}
}

