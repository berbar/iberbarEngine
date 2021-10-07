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


			UShaderVariableType ConvertShaderParameterType( D3D11_SHADER_VERSION_TYPE, CShaderReflectionType** ppOutReflectionType );








			class CShaderReflectionType
				: public IShaderReflectionType
			{
			public:
				CShaderReflectionType();

				virtual int GetMemberCount() const override;
				virtual const IShaderReflectionMember* GetMemberByIndex( int nIndex ) const override;
				virtual const IShaderReflectionMember* GetMemberByName( const char* pstrName ) const override;
				virtual void GetDesc( UShaderReflectionTypeDesc* pOutDesc ) const override;

				CResult Initial( ID3D11ShaderReflectionType* pD3DShaderReflectionType );

				FORCEINLINE UShaderVariableType GetVariableType() const { return m_nVarType; }
				FORCEINLINE UShaderVariableClass GetVariableClass() const { return m_nVarClass; }
				FORCEINLINE uint32 GetElements() const { return m_nElementCount; }
				FORCEINLINE uint32 GetRowCount() const { return m_nRowCount; }
				FORCEINLINE uint32 GetColumnCount() const { return m_nColumnCount; }

			protected:
				UShaderVariableType m_nVarType;
				UShaderVariableClass m_nVarClass;
				uint32 m_nElementCount;
				uint32 m_nRowCount;
				uint32 m_nColumnCount;
				std::string m_strName;
				std::vector<CShaderReflectionMember*> m_Members;
			};




			class CShaderReflectionMember
				: public IShaderReflectionMember
			{
			public:
				CShaderReflectionMember();

				virtual IShaderReflectionType* GetType() override { return &m_ReflectionType; }
				virtual void GetDesc( UShaderReflectionMemberDesc* pOutDesc ) const override;

				CResult Initial( ID3D11ShaderReflectionType* pD3DShaderReflectionType, const char* pstrName );

				FORCEINLINE const char* GetName() const { return m_strName.c_str(); }
				FORCEINLINE uint32 GetOffset() const { return m_nOffset; }

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
				virtual void GetDesc( UShaderReflectionVariableDesc* pOutDesc ) const override;

				CResult Initial( ID3D11ShaderReflectionVariable* pD3DShaderReflectionVariable, uint32 nCBufferBytesOffset );

				FORCEINLINE const char* GetName() const { return m_strName.c_str(); }
				FORCEINLINE const CShaderReflectionType* GetTypeInternal() const { return &m_ReflectionType; }
				FORCEINLINE uint32 GetOffset() const { return m_nOffset; }

			protected:
				
				uint32 m_nOffsetLocal;
				uint32 m_nOffset;
				uint32 m_nRows;
				uint32 m_nColumns;
				uint32 m_nElementSize;
				uint32 m_nElementCount;
				uint32 m_nTotalSize;
				std::string m_strName;
				CShaderReflectionType m_ReflectionType;
			};



			class CShaderReflectionBuffer
				: public IShaderReflectionBuffer
			{
			public:
				CShaderReflectionBuffer();

				virtual int GetVariableCount() const override { return m_Variables.size(); }
				virtual const IShaderReflectionVariable* GetVariableByIndex( int nIndex ) const override { return GetVariableByIndexInternal( nIndex ); }
				virtual const IShaderReflectionVariable* GetVariableByName( const char* pstrName ) const override;

				CResult Initial( const D3D11_SHADER_INPUT_BIND_DESC& D3DBindDesc, ID3D11ShaderReflectionConstantBuffer* pD3DShaderReflectionConstantBuffer, uint32 nOffset );

				FORCEINLINE uint32 GetBindPoint() const { return m_nBindPoint; }
				FORCEINLINE uint32 GetOffset() const { return m_nOffset; }
				FORCEINLINE uint32 GetSize() const { return m_nSize; }
				FORCEINLINE uint32 GetVariableCountInternal() const { return m_Variables.size(); }
				FORCEINLINE const CShaderReflectionVariable* GetVariableByIndexInternal( int nIndex ) const { return &m_Variables[ nIndex ]; }
				FORCEINLINE const std::string& GetName() const { return m_strName; }

			public:
				uint32 m_nBindPoint;
				uint32 m_nOffset;
				uint32 m_nSize;
				std::vector< CShaderReflectionVariable> m_Variables;
				std::string m_strName;
			};


			class CShaderReflectionBindResource
			{
			public:
				CShaderReflectionBindResource( UShaderVariableType nVarType, const D3D11_SHADER_INPUT_BIND_DESC& Desc )
					: m_nVarType( nVarType )
					, m_nBindPoint( Desc.BindPoint )
					, m_strName( Desc.Name )
				{
				}

				FORCEINLINE UShaderVariableType GetVariableType() { return m_nVarType; }
				FORCEINLINE uint32 GetBindPoint() const { return m_nBindPoint; }
				FORCEINLINE const char* GetName() const { return m_strName.c_str(); }

			public:
				UShaderVariableType m_nVarType;
				uint32 m_nBindPoint;
				std::string m_strName;
			};


			//class CShaderReflectionSampler
			//{
			//public:
			//	CShaderReflectionSampler( UShaderVariableType nVarType, const D3D11_SHADER_INPUT_BIND_DESC& Desc )
			//		: m_nVarType( nVarType )
			//		, m_nBindPoint( Desc.BindPoint )
			//		, m_strName( Desc.Name )
			//	{
			//	}

			//	FORCEINLINE uint32 GetBindPoint() const { return m_nBindPoint; }
			//	FORCEINLINE const char* GetName() const { return m_strName.c_str(); }

			//public:
			//	UShaderVariableType m_nVarType;
			//	uint32 m_nBindPoint;
			//	std::string m_strName;
			//};


			class CShaderReflection
				: public IShaderReflection
			{
			public:
				CShaderReflection();

				virtual int GetBufferCount() const { return GetBufferCountInternal(); }
				virtual const IShaderReflectionBuffer* GetBufferByIndex( int nIndex ) const { return GetBufferByIndexInternal( nIndex ); }
				virtual const IShaderReflectionBuffer* GetBufferByName( const char* pstrName ) const;

				CResult Initial( const void* pCodes, uint32 nCodeLen );

				FORCEINLINE uint32 GetBufferSizeTotal() const { return m_nBufferSizeTotal; }
				FORCEINLINE int GetBufferCountInternal() const { return (int)m_ConstBuffers.size(); }
				FORCEINLINE const CShaderReflectionBuffer* GetBufferByIndexInternal( int nIndex ) const { return &m_ConstBuffers[ nIndex ]; }
				const CShaderReflectionVariable* GetVariableByName( const char* pstrName ) const;
				const CShaderReflectionBindResource* GetTextureByName( const char* pstrName ) const;
				const CShaderReflectionBindResource* GetSamplerByName( const char* pstrName ) const;

			protected:
				uint32 m_nBufferSizeTotal;
				std::vector<CShaderReflectionBuffer> m_ConstBuffers;
				std::vector<const CShaderReflectionVariable*> m_Vars;
				std::vector<CShaderReflectionBindResource> m_Samplers;
				std::vector<CShaderReflectionBindResource> m_Textures;
			};
		}
	}
}

