#pragma once


#include <iberbar/Renderer/RendererBase.h>
#include <iberbar/Utility/Size.h>


namespace iberbar
{
	namespace Renderer
	{
		namespace OpenGL
		{
			class CDevice;

			class __iberbarExports__ CRenderer
				: public CRendererBase
			{
			public:
				CRenderer();
				virtual ~CRenderer();

			public:
				//virtual void Render() override;

				void Init();
				void SetDevice( CDevice* pDevice );
				void SetUISize( const CSize2i& size ) { m_UISize = size; }

			protected:
				virtual void DrawBatchTriangles() override;

			private:
				CDevice* m_pDevice;
				CSize2i m_UISize;
				void* m_pVertexBuffer;
				std::vector<Renderer::CTrianglesCommand*> m_TriangleCommandList;
				bool m_bIsRendering;
			};
		}
	}
}
