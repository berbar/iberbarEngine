
#pragma once

#include <iberbar/Gui/Headers.h>
#include <iberbar/Gui/BaseTypes.h>
#include <iberbar/Utility/Color.h>
#include <math.h>

namespace iberbar
{
	namespace Gui
	{
// 	const float BlendColorRate_Normal = 5.0f;
// 	const float BlendColorRate_Quick  = 6.0f;
		const float BlendColorRate_Normal = 0.7f;
		const float BlendColorRate_Quick = 0.8f;
		struct __iberbarGuiApi__ BlendColor
		{
			CColor4F currentColor;
			CColor4B stateColor[ uWidgetStateCount ];

			void SetColor( int state, CColor4B color )
			{
				if ( state == -1 )
				{
					for ( int lc_i = 0; lc_i < uWidgetStateCount; lc_i++ )
					{
						stateColor[ lc_i ] = color;
					}

				}
				else
				{
					if ( state < 0 && state >= uWidgetStateCount )
						return;

					stateColor[ state ] = color;
				}
			}

			void Blend( int state, float fElapsedTime, float fBlendStep = 0.7f )
			{
				CColor4F lc_color = stateColor[ state ];
				//currentColor = stateColor[ state ];
				Color4FLerp( &currentColor, &currentColor, &lc_color, 1.0f - powf( fBlendStep, 30 * fElapsedTime ) /*fBlendStep*fElapsedTime*/ );
			}

			void Refresh()
			{
				currentColor = CColor4F( stateColor[ (int)UWidgetState::Hidden ] );
			}
		};
	}
}