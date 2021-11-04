#pragma once


namespace iberbar
{
	namespace RHI
	{
		class IVertexBuffer;
		class IIndexBuffer;
	}

	namespace Renderer
	{
		class CMeshProcessor
		{
		public:
			RHI::IVertexBuffer* m_pVertexBuffer;
			RHI::IIndexBuffer* m_pIndexBuffer;
		};
	}
}