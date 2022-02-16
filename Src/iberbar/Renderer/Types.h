#pragma once


namespace iberbar
{
	namespace Renderer
	{
		enum class EVertexElementSlot
		{
			Position = 0,
			Color = 1,
			Normal = 2,
			Tex0 = 3,
			Tex1 = Tex0 + 1,
			Tex2 = Tex0 + 2,
			Tex3 = Tex0 + 3,
			Tex4 = Tex0 + 4,
			Tex5 = Tex0 + 5,
			Tex6 = Tex0 + 6,
			Tex7 = Tex0 + 7,
		};

		enum EVertexElementUsage
		{
			Position = 0x0001 << (int)EVertexElementSlot::Position,
			Color = 0x0001 << (int)EVertexElementSlot::Color,
			Normal = 0x0001 << (int)EVertexElementSlot::Normal,
			Tex0 = 0x0001 << (int)EVertexElementSlot::Tex0,
			Tex1 = 0x0001 << (int)EVertexElementSlot::Tex1,
			Tex2 = 0x0001 << (int)EVertexElementSlot::Tex2,
			Tex3 = 0x0001 << (int)EVertexElementSlot::Tex3,
			Tex4 = 0x0001 << (int)EVertexElementSlot::Tex4,
			Tex5 = 0x0001 << (int)EVertexElementSlot::Tex5,
			Tex6 = 0x0001 << (int)EVertexElementSlot::Tex6,
			Tex7 = 0x0001 << (int)EVertexElementSlot::Tex7,
		};
	}
}