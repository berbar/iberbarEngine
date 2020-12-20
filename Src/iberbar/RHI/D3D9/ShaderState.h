#pragma once

#include <iberbar/RHI/D3D9/Headers.h>
#include <iberbar/RHI/Types.h>
#include <iberbar/RHI/ShaderState.h>


namespace iberbar
{
	namespace RHI
	{
		namespace D3D9
		{
			class CDevice;
			class CShader;
			class CVertexDeclaration;


			struct UShaderVariableDeclarationNode
			{
				UShaderVariableDeclarationNode()
					: nShaderType()
					, nVarType( UShaderVariableType::VT_Unknown )
					, nOffset( 0 )
					, nElementSize( 0 )
					, nElementCount( 0 )
					, nTotalSize( 0 )
					, D3DHandle( nullptr )
					, D3DDesc()
				{
				}
				UShaderType nShaderType;
				UShaderVariableType nVarType;
				uint32 nOffset;
				uint32 nElementSize;
				uint32 nElementCount;
				uint32 nTotalSize;
				D3DXHANDLE D3DHandle;
				D3DXCONSTANT_DESC D3DDesc;
			};

			class __iberbarD3DApi__ CShaderVariableDeclaration
			{
			public:
				friend class CShaderState;

			public:
				CShaderVariableDeclaration();

				const std::vector< UShaderVariableDeclarationNode >* GetList( UShaderType nShaderType ) const { return &m_Nodes[ (int)nShaderType ]; }
				const UShaderVariableDeclarationNode* GetNode( UShaderType nShaderType, const char* strName ) const;
				int GetTotalByteSize() const;
				int GetSamplerStageMax() const;

			private:
				void AddNode( UShaderType nShaderType, D3DXHANDLE Handle, D3DXCONSTANT_DESC Desc );

			private:
				int m_nTotalByteSize;
				int m_nSamplerStageMax;
				std::vector< UShaderVariableDeclarationNode > m_Nodes[ 2 ];
			};


			class __iberbarD3DApi__ CShaderState
				: public IShaderState
			{
			public:
				CShaderState(
					CDevice* pDevice,
					CShader* pShader,
					CVertexDeclaration* pVertexDeclaration
				);
				virtual ~CShaderState();

			public:
				CShader* GetShader();
				CVertexDeclaration* GetVertexDeclaration();
				const CVertexDeclaration* GetVertexDeclaration() const;
				const CShaderVariableDeclaration* GetShaderVariableDeclaration() const;
				CResult GenarateConstTable();

			protected:
				CResult GenarateConstTable( UShaderType nShaderType, ID3DXConstantTable* pD3DConstTable );

			protected:
				CDevice* m_pDevice;
				CShader* m_pShader;
				CVertexDeclaration* m_pVertexDeclaration;
				CShaderVariableDeclaration* m_pShaderVariableDeclaration;
			};
		}
	}
}



FORCEINLINE int iberbar::RHI::D3D9::CShaderVariableDeclaration::GetTotalByteSize() const
{
	return m_nTotalByteSize;
}


FORCEINLINE int iberbar::RHI::D3D9::CShaderVariableDeclaration::GetSamplerStageMax() const
{
	return m_nSamplerStageMax;
}


FORCEINLINE const iberbar::RHI::D3D9::CShaderVariableDeclaration* iberbar::RHI::D3D9::CShaderState::GetShaderVariableDeclaration() const
{
	return m_pShaderVariableDeclaration;
}


FORCEINLINE iberbar::RHI::D3D9::CShader* iberbar::RHI::D3D9::CShaderState::GetShader()
{
	return m_pShader;
}


FORCEINLINE iberbar::RHI::D3D9::CVertexDeclaration* iberbar::RHI::D3D9::CShaderState::GetVertexDeclaration()
{
	return m_pVertexDeclaration;
}


FORCEINLINE const iberbar::RHI::D3D9::CVertexDeclaration* iberbar::RHI::D3D9::CShaderState::GetVertexDeclaration() const
{
	return m_pVertexDeclaration;
}
