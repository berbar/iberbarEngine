#pragma once

#include <iberbar/Utility/Unknown.h>
#include <iberbar/Poster/MathTypes.h>

namespace iberbar
{
	namespace Poster
	{
		struct __iberbarExports__ ULineRenderOptions
		{
		public:
			int width;
			int style;
		};

		class CTextureScale9;
		class CSurface;

		/*
			绘制九宫格图片
		*/
		void __iberbarExports__ DrawTextureScale9( CSurface* surface, const CRect2i& dest, CTextureScale9* texture );

		/*
			绘制图片到一个矩形区域中
		*/
		void __iberbarExports__ DrawTexture( CSurface* surface, const CRect2i& dest, CSurface* texture, __maybenull const CRect2i* src = nullptr );

		void __iberbarExports__ DrawTexture( CSurface* surface, const CRect2i& dest, CSurface* texture, const CColor4B& color, __maybenull const CRect2i* src = nullptr );

		/*
			绘制图片到一个圆形区域中
		*/
		void __iberbarExports__ DrawTextureInCircle( CSurface* surface, const CCircle& dest, CSurface* texture, __maybenull const CRect2i* src = nullptr );

		/*
			绘制图片到一个带有圆角的矩形区域中
		*/
		void __iberbarExports__ DrawTextureInCornersBox( CSurface* surface, const CRect2i& dest, int radius, CSurface* texture, __maybenull const CRect2i* src = nullptr );

		/*
			绘制线条
		*/
		void __iberbarExports__ DrawLine( CSurface* surface, const CPoint2i* points, int pointsCount, const CColor4B& color, const ULineRenderOptions* options = nullptr );

		void __iberbarExports__ DrawLine2( CSurface* surface, const CPoint2i& point1, const CPoint2i& point2, const CColor4B& color, const ULineRenderOptions* options = nullptr );

		/*
			绘制矩形
			@params thickness 线宽，-1时填充整个矩形区域
		*/
		void __iberbarExports__ DrawRectagle( CSurface* surface, const CRect2i& rect, const CColor4B& color, int thickness );



	}
}

