#pragma once


#include <iberbar/RHI/OpenGL/Headers.h>
#include <iberbar/RHI/VertexDeclaration.h>



namespace iberbar
{
	namespace RHI
	{
		namespace OpenGL
		{
			class CDevice;
			class CShaderProgram;

			struct UGLVertexAttribute
			{
				GLuint nLocation;
				GLuint nOffset;
				GLenum nType;
				GLuint nSize;
				GLboolean bNormallized;
			};

			class __iberbarRHIOpenGLApi__ CVertexDeclaration
				: public IVertexDeclaration
			{
			public:
				CVertexDeclaration( CDevice* pDevice, const UVertexElement* pVertexElements, uint32 nVertexElementsCount, uint32 nStride );
				virtual ~CVertexDeclaration();

			public:
				void Initial( CShaderProgram* pShader );
				inline const UGLVertexAttribute* GetGLVertexAttributes() const { return m_pGLVertexAttributes; }
				inline uint32 GetGLVertexAttributesCount() const { return m_nGLVertexAttributesCount; }

			protected:
				CDevice* m_pDevice;
				UGLVertexAttribute* m_pGLVertexAttributes;
				uint32 m_nGLVertexAttributesCount;
			};
		}
	}
}
