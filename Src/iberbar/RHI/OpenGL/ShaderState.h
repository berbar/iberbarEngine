#pragma once

#include <iberbar/RHI/Types.h>
#include <iberbar/RHI/ShaderState.h>
#include <iberbar/RHI/OpenGL/Headers.h>


namespace iberbar
{
	namespace RHI
	{
		namespace OpenGL
		{
			class CDevice;
			class CShaderProgram;
			class CVertexDeclaration;



			struct UShaderVariableDeclarationNode
			{
				UShaderVariableDeclarationNode()
					: nVarType( UShaderVariableType::VT_Unknown )
					, nOffset( 0 )
					, nElementSize( 0 )
					, nElementCount( 0 )
					, nTotalSize( 0 )
					, nLocation( -1 )
					, nTextureStage( 0 )
					, strName()
				{
				}
				UShaderVariableType nVarType;
				uint32 nOffset;
				uint32 nElementSize;
				uint32 nElementCount;
				uint32 nTotalSize;
				GLuint nLocation;
				GLuint nTextureStage;
				std::string strName;
			};

			class __iberbarRHIOpenGLApi__ CShaderVariableDeclaration
			{
			public:
				friend class CShaderState;

			public:
				CShaderVariableDeclaration();

				const std::vector< UShaderVariableDeclarationNode >* GetList() const { return &m_Nodes; }
				const UShaderVariableDeclarationNode* GetNode( const char* strName ) const;
				int GetTotalByteSize() const;
				int GetSamplerStateMax() const;

			private:
				void AddNode( const char* strName, GLuint nLocation, GLenum nVarType, GLint nElementCount );

			private:
				int m_nTotalByteSize;
				int m_nSamplerStateMax;
				std::vector< UShaderVariableDeclarationNode > m_Nodes;
			};



			class __iberbarRHIOpenGLApi__ CShaderState
				: public IShaderState
			{
			public:
				CShaderState( CDevice* pDevice, CShaderProgram* pProgram, CVertexDeclaration* pVertexDeclaration );
				virtual ~CShaderState();

				CShaderProgram* GetProgram();
				CVertexDeclaration* GetVertexDeclaration();
				const CVertexDeclaration* GetVertexDeclaration() const;
				const CShaderVariableDeclaration* GetShaderVariableDeclaration() const;
				CResult GenarateConstTable();

			private:
				CDevice* m_pDevice;
				CShaderProgram* m_pShaderProgram;
				CVertexDeclaration* m_pVertexDeclaration;
				CShaderVariableDeclaration* m_pShaderVariableDeclaration;
			};
		}
	}
}




inline int iberbar::RHI::OpenGL::CShaderVariableDeclaration::GetTotalByteSize() const
{
	return m_nTotalByteSize;
}


inline int iberbar::RHI::OpenGL::CShaderVariableDeclaration::GetSamplerStateMax() const
{
	return m_nSamplerStateMax;
}


inline const iberbar::RHI::OpenGL::CShaderVariableDeclaration* iberbar::RHI::OpenGL::CShaderState::GetShaderVariableDeclaration() const
{
	return m_pShaderVariableDeclaration;
}


inline iberbar::RHI::OpenGL::CShaderProgram* iberbar::RHI::OpenGL::CShaderState::GetProgram()
{
	return m_pShaderProgram;
}


inline iberbar::RHI::OpenGL::CVertexDeclaration* iberbar::RHI::OpenGL::CShaderState::GetVertexDeclaration()
{
	return m_pVertexDeclaration;
}


inline const iberbar::RHI::OpenGL::CVertexDeclaration* iberbar::RHI::OpenGL::CShaderState::GetVertexDeclaration() const
{
	return m_pVertexDeclaration;
}
