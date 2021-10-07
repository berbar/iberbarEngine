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
			class CVertexDeclaration;
			class CShader;
			class CVertexShader;
			class CPixelShader;


			//struct UShaderVariableDeclarationNode
			//{
			//	UShaderVariableDeclarationNode()
			//		: nShaderType()
			//		, nVarType( UShaderVariableType::VT_Unknown )
			//		, nOffset( 0 )
			//		, nElementSize( 0 )
			//		, nElementCount( 0 )
			//		, nTotalSize( 0 )
			//		, D3DHandle( nullptr )
			//		, D3DDesc()
			//	{
			//	}
			//	UShaderType nShaderType;
			//	UShaderVariableType nVarType;
			//	uint32 nOffset;
			//	uint32 nElementSize;
			//	uint32 nElementCount;
			//	uint32 nTotalSize;
			//	D3DXHANDLE D3DHandle;
			//	D3DXCONSTANT_DESC D3DDesc;
			//};

			//class __iberbarD3DApi__ CShaderVariableDeclaration
			//{
			//public:
			//	friend class CShaderState;

			//public:
			//	CShaderVariableDeclaration();

			//	const std::vector< UShaderVariableDeclarationNode >* GetList( UShaderType nShaderType ) const { return &m_Nodes[ (int)nShaderType ]; }
			//	const UShaderVariableDeclarationNode* GetNode( UShaderType nShaderType, const char* strName ) const;
			//	int GetTotalByteSize() const;
			//	int GetSamplerStageMax() const;

			//private:
			//	void AddNode( UShaderType nShaderType, D3DXHANDLE Handle, D3DXCONSTANT_DESC Desc );

			//private:
			//	int m_nTotalByteSize;
			//	int m_nSamplerStageMax;
			//	std::vector< UShaderVariableDeclarationNode > m_Nodes[ 2 ];
			//};


			class __iberbarD3DApi__ CShaderState
				: public IShaderState
			{
			public:
				CShaderState(
					CDevice* pDevice,
					CVertexDeclaration* pVertexDeclaration,
					CVertexShader* pVertexShader,
					CPixelShader* pPixelShader );
				virtual ~CShaderState();

			public:
				virtual IShader* GetShader( EShaderType eShaderType ) override;

				CVertexDeclaration* GetVertexDeclaration() { return m_pVertexDeclaration; }
				CVertexShader* GetVertexShader() { return m_pVertexShader; }
				CPixelShader* GetPixelShader() { return m_pPixelShader; }

			protected:
				CDevice* m_pDevice;
				CVertexDeclaration* m_pVertexDeclaration;
				union
				{
					struct
					{
						CVertexShader* m_pVertexShader;
						CPixelShader* m_pPixelShader;
					};
					CShader* m_pShaders[ (int)EShaderType::__Count ];
				};
			};
		}
	}
}

//
//
//FORCEINLINE int iberbar::RHI::D3D9::CShaderVariableDeclaration::GetTotalByteSize() const
//{
//	return m_nTotalByteSize;
//}
//
//
//FORCEINLINE int iberbar::RHI::D3D9::CShaderVariableDeclaration::GetSamplerStageMax() const
//{
//	return m_nSamplerStageMax;
//}
//
//
//FORCEINLINE iberbar::RHI::D3D9::CVertexShader* iberbar::RHI::D3D9::CShaderState::GetVertexShader()
//{
//	return m_pVertexShader;
//}
//
//
//FORCEINLINE iberbar::RHI::D3D9::CPixelShader* iberbar::RHI::D3D9::CShaderState::GetPixelShader()
//{
//	return m_pPixelShader;
//}