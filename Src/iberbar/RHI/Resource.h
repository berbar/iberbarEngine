#pragma once

#include <iberbar/RHI/Headers.h>
#include <iberbar/RHI/Types.h>
#include <iberbar/Utility/Result.h>


namespace iberbar
{
	namespace RHI
	{
		enum class UResourceType
		{
			Unknown,
			Texture,
			VertexBuffer,
			IndexBuffer,
			ConstBuffer,
			Shader,
			Effect,
			VertexDeclaration,
			Font,
			StateBlock,
			BlendState,
			DepthStencilState,
			SamplerState,
		};

		class __iberbarRHIApi__ IResource
			: public CRef
		{
		public:
			IResource( UResourceType nResourceType );
			virtual ~IResource();

		public:
			inline UResourceType GetResourceType() const { return m_nResourceType; }

		public:
			virtual void OnLost() {}
			virtual CResult OnReset() { return CResult(); }
			bool IsManaged() const { return m_bIsManaged; }
			bool IsLost() const { return m_bIsLost; }

		protected:
			UResourceType m_nResourceType;
			bool m_bIsManaged;
			bool m_bIsLost;
		};
	}
}
