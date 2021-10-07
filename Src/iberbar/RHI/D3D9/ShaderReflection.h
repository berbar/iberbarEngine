#pragma once

#include <iberbar/RHI/D3D9/Headers.h>
#include <iberbar/RHI/ShaderReflection.h>


namespace iberbar
{
	namespace RHI
	{
		namespace D3D9
		{
			class CShaderReflectionBuffer;
			class CShaderReflectionVariable;
			class CShaderReflectionType;
			class CShaderReflection;
			class CShaderReflectionMember;


			UShaderVariableType ConvertShaderParameterType( ID3DXConstantTable* pD3DConstantTable, D3DXHANDLE pD3DHandle, const D3DXCONSTANT_DESC& Desc, CShaderReflectionType** ppOutReflectionType );



			class CShaderReflectionType
				: public IShaderReflectionType
			{
			public:
				CShaderReflectionType();
				virtual ~CShaderReflectionType();

				virtual int GetMemberCount() const override { return (int)m_ReflectionMembers.size(); }
				virtual const IShaderReflectionMember* GetMemberByIndex( int nIndex ) const override;
				virtual const IShaderReflectionMember* GetMemberByName( const char* pstrName ) const override;
				virtual void GetDesc( UShaderReflectionTypeDesc* pOutDesc ) const override;

				CResult Initial( ID3DXConstantTable* pD3DConstantTable, D3DXHANDLE pD3DHandle, const D3DXCONSTANT_DESC& Desc );

				FORCEINLINE UShaderVariableType GetVarType() const { return m_eVarType; }
				FORCEINLINE UShaderVariableClass GetVarClass() const { return m_eVarClass; }
				FORCEINLINE uint32 GetElements() const { return m_nElementCount; }
				FORCEINLINE uint32 GetRowCount() const { return m_nRowCount; }
				FORCEINLINE uint32 GetColumnCount() const { return m_nColumnCount; }

			private:
				UShaderVariableType m_eVarType;
				UShaderVariableClass m_eVarClass;
				uint32 m_nElementCount;
				uint32 m_nRowCount;
				uint32 m_nColumnCount;
				std::string m_strName;
				std::vector<CShaderReflectionMember*> m_ReflectionMembers;
			};



			class CShaderReflectionVariable
				: public IShaderReflectionVariable
			{
			public:
				CShaderReflectionVariable();
				virtual ~CShaderReflectionVariable();

				virtual const IShaderReflectionType* GetType() const override;
				virtual void GetDesc( UShaderReflectionVariableDesc* pOutDesc ) const override;

				CResult Initial( ID3DXConstantTable* pD3DConstantTable, D3DXHANDLE pD3DHandle, const D3DXCONSTANT_DESC& Desc, uint32 nOffsetTotal );

				FORCEINLINE const CShaderReflectionType* GetTypeInternal() const { return &m_ReflectionType; }
				FORCEINLINE D3DXHANDLE GetD3DHandle() const { return m_pD3DHandle; }
				FORCEINLINE uint32 GetOffset() const { return m_nOffset; }
				FORCEINLINE uint32 GetTotalSize() const { return m_nTotalSize; }
				FORCEINLINE uint32 GetRegisterIndex() const { return m_nRegisterIndex; }
				FORCEINLINE const char* GetName() const { return m_strName.c_str(); }

			private:
				D3DXHANDLE m_pD3DHandle;
				uint32 m_nOffset;
				uint32 m_nRows;
				uint32 m_nColumns;
				uint32 m_nElementSize;
				uint32 m_nElementCount;
				uint32 m_nTotalSize;
				uint32 m_nRegisterIndex;
				CShaderReflectionType m_ReflectionType;
				std::string m_strName;
			};





			class CShaderReflectionMember
				: public IShaderReflectionMember
			{
			public:
				CShaderReflectionMember();
				virtual ~CShaderReflectionMember();

				virtual IShaderReflectionType* GetType() override { return m_pReflectionType; }
				virtual void GetDesc( UShaderReflectionMemberDesc* pOutDesc ) const override;

				CResult Initial( ID3DXConstantTable* pD3DConstantTable, D3DXHANDLE pD3DHandle, const D3DXCONSTANT_DESC& Desc );
				const char* GetName() { return m_strName.c_str(); }

			private:
				std::string m_strName;
				UShaderVariableType m_eVarType;
				CShaderReflectionType* m_pReflectionType;
				uint32 m_nOffset;
			};



			class CShaderReflectionBuffer
				: public IShaderReflectionBuffer
			{
			public:
				CShaderReflectionBuffer();

				virtual int GetVariableCount() const override { return GetVariableCountInternal(); }
				virtual const IShaderReflectionVariable* GetVariableByIndex( int nIndex ) const override { return GetVariableByIndexInternal( nIndex ); }
				virtual const IShaderReflectionVariable* GetVariableByName( const char* pstrName ) const override { return GetVariableByNameInternal( pstrName ); }

				CResult Initial( ID3DXConstantTable* pD3DConstantTable );

				FORCEINLINE int GetVariableCountInternal() const { return (int)m_Variables.size(); }
				FORCEINLINE const CShaderReflectionVariable* GetVariableByIndexInternal( int nIndex ) const { return &m_Variables[ (size_t)nIndex ]; }
				const CShaderReflectionVariable* GetVariableByNameInternal( const char* pstrName ) const;
				FORCEINLINE int GetBufferSize() const { return m_nSize; }
				FORCEINLINE int GetSamplerCount() const { return m_nSamplerCount; }

			private:
				uint32 m_nSize;
				uint32 m_nSamplerCount;
				std::vector<CShaderReflectionVariable> m_Variables;
			};



			class CShaderReflection
				: public IShaderReflection
			{
			public:
				CShaderReflection();

				virtual int GetBufferCount() const { return 1; }
				virtual const IShaderReflectionBuffer* GetBufferByIndex( int nIndex ) const { return &m_Buffer; }
				virtual const IShaderReflectionBuffer* GetBufferByName( const char* pstrName ) const { return &m_Buffer; }

				CResult Initial( ID3DXConstantTable* pD3DConstantTable );

				FORCEINLINE const CShaderReflectionBuffer* GetBuffer() const { return &m_Buffer; }

			private:
				CShaderReflectionBuffer m_Buffer;
			};
		}
	}
}


