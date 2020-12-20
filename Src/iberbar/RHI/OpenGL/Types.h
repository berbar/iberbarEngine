#pragma once


#include <iberbar/RHI/Types.h>
#include <iberbar/RHI/OpenGL/Headers.h>


namespace iberbar
{
	namespace RHI
	{
		namespace OpenGL
		{
            void ConvertVertexFormat( UVertexFormat nFormat, uint32* pnSize, GLenum* pnType );
            GLenum ConvertPrimitiveType( UPrimitiveType nPrimitiveType );
            GLenum ConvertIndexFormat( UIndexFormat nIndexFormat );
            void ConvertVertexAttrName( std::string& str, UVertexDeclareUsage nSemantic, uint32 nSemanticIndex );
            GLuint ConvertTextureFilterType( UTextureFilterType nFilterType );
            GLuint ConvertTextureAddress( UTextureAddress nAddress );
            GLsizei GetDataTypeSize( GLenum size );
		}
	}
}



inline void iberbar::RHI::OpenGL::ConvertVertexFormat( UVertexFormat nFormat, uint32* pnSize, GLenum* pnType )
{
    switch ( nFormat )
    {
        case UVertexFormat::FLOAT:
            *pnSize = 1;
            *pnType = GL_FLOAT;
            break;
        case UVertexFormat::FLOAT2:
            *pnSize = 2;
            *pnType = GL_FLOAT;
            break;
        case UVertexFormat::FLOAT3:
            *pnSize = 3;
            *pnType = GL_FLOAT;
            break;
        case UVertexFormat::FLOAT4:
            *pnSize = 4;
            *pnType = GL_FLOAT;
            break;
        case UVertexFormat::UBYTE4:
            *pnSize = 4;
            *pnType = GL_UNSIGNED_BYTE;
            break;
        case UVertexFormat::COLOR:
            *pnSize = 4;
            *pnType = GL_UNSIGNED_BYTE;
            break;
        default:
            break;
    }
}

inline GLenum iberbar::RHI::OpenGL::ConvertPrimitiveType( UPrimitiveType nPrimitiveType )
{
    switch ( nPrimitiveType )
    {
        case UPrimitiveType::Point:
            return GL_POINTS;
        case UPrimitiveType::Line:
            return GL_LINES;
        case UPrimitiveType::LineStrip:
            return GL_LINE_STRIP;
        case UPrimitiveType::Triangle:
            return GL_TRIANGLES;
        case UPrimitiveType::TriangleStrip:
            return GL_TRIANGLE_STRIP;
        default:
            break;
    }
    return 0;
}

inline GLenum iberbar::RHI::OpenGL::ConvertIndexFormat( UIndexFormat nIndexFormat )
{
    switch ( nIndexFormat )
    {
        case UIndexFormat::U_Int:
            return GL_UNSIGNED_INT;
        case UIndexFormat::U_Short:
            return GL_UNSIGNED_SHORT;
        default:
            break;
    }
    return GL_BYTE;
}

inline void iberbar::RHI::OpenGL::ConvertVertexAttrName( std::string& str, UVertexDeclareUsage nSemantic, uint32 nSemanticIndex )
{
    switch ( nSemantic )
    {
        case UVertexDeclareUsage::Position:
            str = "a_position";
            break;
        case UVertexDeclareUsage::Normal:
            str = "a_normal";
        case UVertexDeclareUsage::Color:
            str = "a_color";
            break;
        case UVertexDeclareUsage::TexCoord:
            str = "a_texcoord";
            break;
        case UVertexDeclareUsage::Sample:
            str = "a_sample";
            break;
        default:
            break;
    }

    if ( nSemanticIndex > 0 )
    {
        str += nSemanticIndex;
    }
}

inline GLuint iberbar::RHI::OpenGL::ConvertTextureFilterType( UTextureFilterType nFilterType )
{
    switch ( nFilterType )
    {
        case UTextureFilterType::Point:
            return GL_NEAREST;
        case UTextureFilterType::Linear:
            return GL_LINEAR;
        default:break;
    }
    return GL_LINEAR;
}

inline GLuint iberbar::RHI::OpenGL::ConvertTextureAddress( UTextureAddress nAddress )
{
    switch ( nAddress )
    {
        case UTextureAddress::Wrap:
            return GL_REPEAT;
        case UTextureAddress::Clamp:
#ifdef _WIN32
            return GL_CLAMP;
#endif
#ifdef __ANDROID__
            return GL_CLAMP_TO_EDGE;
#endif
        default:break;
    }
    return GL_REPEAT;
}



inline GLsizei iberbar::RHI::OpenGL::GetDataTypeSize( GLenum size )
{
    GLsizei ret = 0;
    switch ( size )
    {
        case GL_BOOL:
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
            ret = sizeof( GLbyte );
            break;
        case GL_BOOL_VEC2:
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
            ret = sizeof( GLshort );
            break;
        case GL_BOOL_VEC3:
            ret = sizeof( GLboolean );
            break;
        case GL_BOOL_VEC4:
        case GL_INT:
        case GL_UNSIGNED_INT:
        case GL_FLOAT:
            ret = sizeof( GLfloat );
            break;
        case GL_FLOAT_VEC2:
        case GL_INT_VEC2:
            ret = sizeof( GLfloat ) * 2;
            break;
        case GL_FLOAT_VEC3:
        case GL_INT_VEC3:
            ret = sizeof( GLfloat ) * 3;
            break;
        case GL_FLOAT_MAT2:
        case GL_FLOAT_VEC4:
        case GL_INT_VEC4:
            ret = sizeof( GLfloat ) * 4;
            break;
        case GL_FLOAT_MAT3:
            ret = sizeof( GLfloat ) * 9;
            break;
        case GL_FLOAT_MAT4:
            ret = sizeof( GLfloat ) * 16;
            break;
        default:
            break;
    }
    return ret;
}
