


#ifndef __iberbar__Gui__Element__FramemoveColor_h__
#define __iberbar__Gui__Element__FramemoveColor_h__

#include "iberbar\Base\Color.h"

namespace iberbar
{
	class IBERBAR_DLL FramemoveColor
	{
	public:
		FramemoveColor( void );


	public:
		void startBlend();    // 重新开始混合渐变
		void overBlend();     // 结束混合渐变
		void update( float nElapsedTime );


		void setBlendTimesMax( int nTimesMax ); // 混合次数，1次相当于有0引索渐变到1引索，2次等于先从0到1渐变再从1渐变到0，以次类推，当次数小于等于0时为无限循环
		void setBlendTimesMaxInfinite()                { setBlendTimesMax( 0 ); }
		void setBlendTimeNeed( float nSecondes ); // 一次混合渐变所需要的时间
		void setBaseColor( const Color4B& color0, const Color4B& color1 );
		
		Color4B getBaseColor0() const                  { return m_ColorBase[ 0 ]; }
		Color4B getBaseColor1() const                  { return m_ColorBase[ 1 ]; }
		Color4F getCurrentColor() const                { return m_ColorCurrent; }
		bool isOverBlend() const                       { return m_bBlendOver; }


	protected:
		int     m_iBlendTimesMax;  // 混合次数，1次相当于有0引索渐变到1引索，2次等于先从0到1渐变再从1渐变到0，以次类推，当次数为-1时为无限循环
		int     m_iBlendTimesAlready;
		bool    m_bBlendOver;
		float   m_fBlendTimeNeed; // 一次混合渐变所需要的时间
		float   m_fBlendTimeElapsed;
		Color4F m_ColorCurrent;
		Color4F m_ColorBase[ 2 ];
	};
}

#endif