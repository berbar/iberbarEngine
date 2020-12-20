
#pragma once

#include <iberbar/Utility/Rect.h>
#include <iberbar/Utility/Stack.h>

namespace iberbar
{
	class __iberbarUtilityApi__ CViewportState
	{
	public:
		CViewportState() {}

	public:
		bool IsEmpty();
		void AddViewport( const CRect2i& rect );
		void PopViewport();
		const CRect2i* GetClipViewport();

	private:
		TListStack< CRect2i > m_ClipViewportStack;
	};
}

