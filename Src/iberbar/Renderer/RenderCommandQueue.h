#pragma once

#include <iberbar/Renderer/RenderCommand.h>

namespace iberbar
{
	namespace Renderer
	{
		typedef std::vector<CRenderCommand*> URenderCommandList;

		class __iberbarRendererApi__ CRenderQueue
		{
		public:
			enum class UQueueGroup
			{
				Zindex_Negative = 0,
				Zindex_Zero = 1,
				Zindex_Positive = 2,
				Count
			};

		public:
			void PushBack( CRenderCommand* command );
			void Sort();
			void Clear();
			const URenderCommandList& GetQueueGroup( UQueueGroup group ) { return m_CommandLists[ (int)group ]; }

		private:
			URenderCommandList m_CommandLists[ (int)UQueueGroup::Count ];

			static inline bool CompareRenderCommand( iberbar::Renderer::CRenderCommand* a, iberbar::Renderer::CRenderCommand* b )
			{
				return a->GetZOrder() < b->GetZOrder();
			}
		};
	}
}





inline void iberbar::Renderer::CRenderQueue::PushBack( CRenderCommand* pCommand )
{
	int nZIndex = pCommand->GetZOrder();
	if ( nZIndex < 0 )
	{
		m_CommandLists[ (int)UQueueGroup::Zindex_Negative ].push_back( pCommand );
	}
	else if ( nZIndex > 0 )
	{
		m_CommandLists[ (int)UQueueGroup::Zindex_Positive ].push_back( pCommand );
	}
	else
	{
		m_CommandLists[ (int)UQueueGroup::Zindex_Zero ].push_back( pCommand );
	}
}

inline void iberbar::Renderer::CRenderQueue::Sort()
{
	std::stable_sort(
		std::begin( m_CommandLists[ (int)UQueueGroup::Zindex_Negative ] ),
		std::end( m_CommandLists[ (int)UQueueGroup::Zindex_Negative ] ),
		CompareRenderCommand );
	std::stable_sort(
		std::begin( m_CommandLists[ (int)UQueueGroup::Zindex_Positive ] ),
		std::end( m_CommandLists[ (int)UQueueGroup::Zindex_Positive ] ),
		CompareRenderCommand );
}

inline void iberbar::Renderer::CRenderQueue::Clear()
{
	for ( int i = 0; i < (int)UQueueGroup::Count; ++i )
	{
		m_CommandLists[ i ].clear();
	}
}