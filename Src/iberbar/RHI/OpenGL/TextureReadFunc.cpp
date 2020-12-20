
#include "TextureReadFunc.h"
#include <png.h>




bool iberbar::RHI::OpenGL::CreateTextureGLuintFromPngA( const char* filename, GLuint* pGLTex, CSize2i* pGLTexSize, uint32* format )
{
	FILE *fp = NULL;

#ifdef WIN32
	errno_t err = fopen_s( &fp, filename, "rb" );
#endif

#ifdef __ANDROID__
	fp = fopen( filename, "rb" );
#endif

	if ( fp == nullptr )
		return false;

	CreateTextureGLuintFromPngFile( fp, pGLTex, pGLTexSize, format );

	fclose( fp );

	return true;
}


#ifdef _WIN32
bool iberbar::RHI::OpenGL::CreateTextureGLuintFromPngW( const wchar_t* filename, GLuint* pGLTex, CSize2i* pGLTexSize, uint32* format )
{
	FILE* fp = NULL;

	errno_t err = _wfopen_s( &fp, filename, L"rb" );
	if ( fp == nullptr )
		return false;

	CreateTextureGLuintFromPngFile( fp, pGLTex, pGLTexSize, format );

	fclose( fp );

	return true;
}
#endif



bool iberbar::RHI::OpenGL::CreateTextureGLuintFromPngFile( FILE* pFile, GLuint* pGLTex, CSize2i* pGLTexSize, uint32* format )
{
	long file_size = 0, pos = 0;
	int color_type = 0, x = 0, y = 0, block_size = 0;

	png_infop info_ptr;
	png_structp png_ptr;
	png_bytep* row_point = NULL;

	png_uint_32 lc_nwidth;
	png_uint_32 lc_nheight;
	unsigned char* lc_cbData = NULL;

	if ( !pFile )
		return false;

	png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );
	info_ptr = png_create_info_struct( png_ptr );
	png_init_io( png_ptr, pFile );
	png_read_png( png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0 );

	lc_nwidth = png_get_image_width( png_ptr, info_ptr );
	lc_nheight = png_get_image_height( png_ptr, info_ptr );
	color_type = png_get_color_type( png_ptr, info_ptr );
	file_size = lc_nwidth * lc_nheight * 4;
	lc_cbData = new unsigned char[ file_size ];

	/* Get some usefull information from header */
	int bit_depth = png_get_bit_depth( png_ptr, info_ptr );
	color_type = png_get_color_type( png_ptr, info_ptr );
	/* Convert index color images to RGB images */
	if ( color_type == PNG_COLOR_TYPE_PALETTE )
		png_set_palette_to_rgb( png_ptr );
	/* Convert 1-2-4 bits grayscale images to 8 bits grayscale. */
	if ( color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8 )
		png_set_expand_gray_1_2_4_to_8( png_ptr );
	if ( png_get_valid( png_ptr, info_ptr, PNG_INFO_tRNS ) )
		png_set_tRNS_to_alpha( png_ptr );
	if ( bit_depth == 16 ) png_set_strip_16( png_ptr );
	else if ( bit_depth < 8 ) png_set_packing( png_ptr );
	/* Update info structure to apply transformations */
	//png_read_update_info( png_ptr, info_ptr );
	/* Retrieve updated information */
	png_get_IHDR( png_ptr, info_ptr, &lc_nwidth, &lc_nheight, &bit_depth, &color_type, NULL, NULL, NULL );

	//png_read_image( png_ptr, row_point );
	row_point = png_get_rows( png_ptr, info_ptr );

	block_size = (color_type == PNG_COLOR_TYPE_RGB_ALPHA) ? 4 : 3;

	if ( color_type == PNG_COLOR_TYPE_RGB_ALPHA )
	{
		for ( x = 0; x < lc_nheight; x++ )
			for ( y = 0; y < lc_nwidth * block_size; y += block_size )
			{
				lc_cbData[ pos++ ] = row_point[ x ][ y + 0 ];        //B
				lc_cbData[ pos++ ] = row_point[ x ][ y + 1 ];        //G
				lc_cbData[ pos++ ] = row_point[ x ][ y + 2 ];        //R
				lc_cbData[ pos++ ] = row_point[ x ][ y + 3 ];        //Alpha
			}
	}
	else
	{
		for ( x = 0; x < lc_nheight; x++ )
			for ( y = 0; y < lc_nwidth * block_size; y += block_size )
			{
				lc_cbData[ pos++ ] = row_point[ x ][ y + 0 ];        //B
				lc_cbData[ pos++ ] = row_point[ x ][ y + 1 ];        //G
				lc_cbData[ pos++ ] = row_point[ x ][ y + 2 ];        //R
				lc_cbData[ pos++ ] = 255;        //Alpha
			}
	}

	png_destroy_read_struct( &png_ptr, &info_ptr, 0 );

	GLuint lc_gltex;

	glEnable( GL_TEXTURE_2D );

	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
	glGenTextures( 1, &lc_gltex );
	glBindTexture( GL_TEXTURE_2D, lc_gltex );

	if ( color_type == PNG_COLOR_TYPE_RGB_ALPHA )
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, lc_nwidth, lc_nheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, lc_cbData );
	else
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, lc_nwidth, lc_nheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, lc_cbData );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	SAFE_DELETE_ARRAY( lc_cbData );

	*pGLTex = lc_gltex;
	*pGLTexSize = CSize2i( lc_nwidth, lc_nheight );
	*format = 0;

	return true;
}


