#pragma once

#include <iberbar/Paper2d/Node.h>
#include <iberbar/Utility/Result.h>
#include <functional>

namespace iberbar
{
	namespace RHI
	{
		class ITexture;
	}

	namespace Paper2d
	{
		class CAnimationController;

		struct UAnimationFrameNode
		{
			CColor4B Color;
			CRect2f rcTexCoord;
		};

		enum class UAnimationEvent
		{
			FrameFirst,
			FrameLast,
			Stop
		};
		typedef void UCallbackAnimationEvent( CAnimationController* pAnimation, UAnimationEvent nEvent );

		class __iberbarPaper2dApis__ CAnimationController
			: public CRef, public IClonable
		{
		public:
			CAnimationController();
			virtual ~CAnimationController();
		protected:
			CAnimationController( const CAnimationController& animation );

		public:
			virtual CAnimationController* Clone() const override;

		public:
			void SetTexture( RHI::ITexture* pTexture );
			void SetTimePerFrame( float nTime );
			void SetName( const char* strName );
			void AddFrameNode( const UAnimationFrameNode& FrameNode );
			void AddFrameList( const std::vector<UAnimationFrameNode>& FrameList );
			const UAnimationFrameNode* GetFrameNodeCurrent() const;
			const std::string& GetName() const;
			RHI::ITexture* GetTexture() const;

			void SetCallback( std::function<UCallbackAnimationEvent> Func );
			void Play( bool bLoop );
			void Stop();
			bool IsPlaying() const;
			void Update( float nElapsedTime );

		private:
			bool m_bStop;
			bool m_bLoop;
			float m_nTimeDelay;
			int m_nFrameCurrent;

		private:
			RHI::ITexture* m_pTexture;
			float m_nTimePerFrame;
			std::string m_strName;
			std::vector<UAnimationFrameNode> m_Frames;
			std::function<UCallbackAnimationEvent> m_Callback;
		};



		//class __iberbarPaper2dApis__ CAnimationControllerList
		//	: public CRef, public IClonable
		//{
		//public:
		//	void Add( CAnimationController* pAnimation );
		//	void Play( const char* strName );
		//	void Stop();
		//	bool IsPlaying() const;
		//	void Update( float nElapsedTime );

		//private:
		//	std::vector<CAnimationController*> m_AnimList;
		//};


		//CResult LoadAnimationControllerFromJson( CAnimationController* pAnimController, const char* strFile );
	}
}




inline void iberbar::Paper2d::CAnimationController::SetTimePerFrame( float nTime )
{
	m_nTimePerFrame = nTime;
}


inline void iberbar::Paper2d::CAnimationController::SetName( const char* strName )
{
	m_strName = strName;
}


inline void iberbar::Paper2d::CAnimationController::AddFrameNode( const UAnimationFrameNode& FrameNode )
{
	m_Frames.push_back( FrameNode );
}


inline const iberbar::Paper2d::UAnimationFrameNode* iberbar::Paper2d::CAnimationController::GetFrameNodeCurrent() const
{
	if ( m_nFrameCurrent < 0 || m_nFrameCurrent >= (int)m_Frames.size() )
		return nullptr;
	return &m_Frames[ m_nFrameCurrent ];
}


inline const std::string& iberbar::Paper2d::CAnimationController::GetName() const
{
	return m_strName;
}


inline iberbar::RHI::ITexture* iberbar::Paper2d::CAnimationController::GetTexture() const
{
	return m_pTexture;
}


inline void iberbar::Paper2d::CAnimationController::SetCallback( std::function<UCallbackAnimationEvent> func )
{
	m_Callback = func;
}


inline bool iberbar::Paper2d::CAnimationController::IsPlaying() const
{
	return m_bStop == false;
}

