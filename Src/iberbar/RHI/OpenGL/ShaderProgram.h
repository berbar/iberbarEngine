
#pragma once


#include <iberbar/RHI/OpenGL/Headers.h>
#include <iberbar/RHI/Shader.h>
#include <map>

namespace iberbar
{
	namespace RHI
	{
		namespace OpenGL
		{
			static const GLint GLShader_AttribLocation_Null = -1;
			static const GLint GLShader_UniformLocation_Null = -1;

			class CShaderProgram;
			class CShaderProgramManager;

			IBERBAR_UNKNOWN_PTR_DECLARE( CShaderProgram );
			IBERBAR_UNKNOWN_PTR_DECLARE( CShaderProgramManager );


			class __iberbarRHIOpenGLApi__ CShaderProgram
				: public IShader
			{
			public:
				CShaderProgram( void );
				virtual ~CShaderProgram() override;


			public:
				virtual CResult LoadFromSource( const char* pVertexSource, const char* pFragmentSource ) override;
				virtual CResult LoadFromFile( const char* pVertexFile, const char* pFragmentFile ) override;

			public:
				inline GLuint getVertextShader() { return m_VertexShader; }
				inline GLuint GetFragmentShader() { return m_FragmentShader; }
				inline GLuint GetProgramUint() { return m_Program; }

				GLint GetAttributeLocation( const char* strName );
				GLint GetUniformLocation( const char* strNam );

				void EnableVertexAttribArray( GLint index );
				void DisableVertexAttribArray( GLint index );

			private:
				GLuint m_VertexShader;
				GLuint m_FragmentShader;
				GLuint m_Program;
			};

			CResult __iberbarRHIOpenGLApi__ LoadShaderFromSource( GLenum shaderType, const char* pSource, GLuint* pShader );



			class __iberbarRHIOpenGLApi__ CShaderProgramManager
				: public CRef
			{
				typedef std::map< std::tstring, CShaderProgram* >  VGLShaderProgramList;

			public:
				CShaderProgramManager( void );
				~CShaderProgramManager();


			public:
				bool AddShaderProgram( const TCHAR* strKey, CShaderProgram* pShaderProgram );
				bool FindShaderProgram( const TCHAR* strKey, CShaderProgram** ppOutShaderProgram = nullptr );
				void UseShaderProgram( CShaderProgram* pUseShaderProgram );


			private:
				CShaderProgram* m_pUsingShaderProgram;
				VGLShaderProgramList m_vShaderPrograms;
			};
		}
	}
}


FORCEINLINE GLint iberbar::RHI::OpenGL::CShaderProgram::GetAttributeLocation( const char* strName )
{
	return glGetAttribLocation( m_Program, strName );
}

FORCEINLINE GLint iberbar::RHI::OpenGL::CShaderProgram::GetUniformLocation( const char* strName )
{
	return glGetUniformLocation( m_Program, strName );
}

FORCEINLINE void iberbar::RHI::OpenGL::CShaderProgram::EnableVertexAttribArray( GLint index )
{
	glEnableVertexAttribArray( index );
}

FORCEINLINE void iberbar::RHI::OpenGL::CShaderProgram::DisableVertexAttribArray( GLint index )
{
	glDisableVertexAttribArray( index );
}

