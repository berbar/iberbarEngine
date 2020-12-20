
#include <iberbar/Paper2d/Animation.h>
#include <iberbar/RHI/Texture.h>






iberbar::Paper2d::CAnimationController::CAnimationController()
	: m_bStop( true )
	, m_bLoop( false )
	, m_nTimeDelay( 0.0f )
	, m_nFrameCurrent( 0 )
	, m_pTexture( nullptr )
	, m_nTimePerFrame( 0.0f )
	, m_strName( "" )
	, m_Frames()
	, m_Callback()
{
}


iberbar::Paper2d::CAnimationController::CAnimationController( const CAnimationController& animation )
	: m_bStop( true )
	, m_bLoop( false )
	, m_nTimeDelay( 0.0f )
	, m_nFrameCurrent( 0 )
	, m_pTexture( animation.m_pTexture )
	, m_nTimePerFrame( animation.m_nTimePerFrame )
	, m_strName( "" )
	, m_Frames( animation.m_Frames )
	, m_Callback()
{
	UNKNOWN_SAFE_ADDREF( m_pTexture );
}


iberbar::Paper2d::CAnimationController::~CAnimationController()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pTexture );
}


iberbar::Paper2d::CAnimationController* iberbar::Paper2d::CAnimationController::Clone() const
{
	return new CAnimationController( *this );
}


void iberbar::Paper2d::CAnimationController::SetTexture( RHI::ITexture* pTexture )
{
	if ( m_pTexture != pTexture )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pTexture );
		m_pTexture = pTexture;
		UNKNOWN_SAFE_ADDREF( m_pTexture );
	}
}


void iberbar::Paper2d::CAnimationController::AddFrameList( const std::vector<UAnimationFrameNode>& FrameList )
{
	for ( size_t i = 0, s = FrameList.size(); i < s; i++ )
	{
		m_Frames.push_back( FrameList[ i ] );
	}
}


void iberbar::Paper2d::CAnimationController::Play( bool bLoop )
{
	if ( m_Frames.empty() == true )
	{
		m_bStop = true;
		return;
	}

	m_bStop = false;
	m_bLoop = bLoop;
	m_nFrameCurrent = 0;
	m_nTimeDelay = m_nTimePerFrame;
}


void iberbar::Paper2d::CAnimationController::Stop()
{
	m_bStop = true;
}


void iberbar::Paper2d::CAnimationController::Update( float nElapsedTime )
{
	if ( m_bStop == true || m_nTimePerFrame <= 0 )
		return;

	float nTimeDelay = m_nTimeDelay - nElapsedTime;
	if ( nTimeDelay > 0 )
	{
		m_nTimeDelay = nTimeDelay;
		return;
	}

	m_nFrameCurrent++;
	if ( m_nFrameCurrent >= (int)m_Frames.size() )
	{
		if ( m_Callback )
			m_Callback( this, UAnimationEvent::FrameLast );

		if ( m_bLoop == false )
		{
			m_bStop = true;

			if ( m_Callback )
				m_Callback( this, UAnimationEvent::Stop );

			return;
		}
		else
		{
			m_nFrameCurrent = 0;

			if ( m_Callback )
				m_Callback( this, UAnimationEvent::FrameFirst );
		}
	}
		
	
	m_nTimeDelay = nTimeDelay + m_nTimePerFrame;

	//const UAnimationInfo* pAnimationInfo = &m_AnimationList[ m_nAnimationPlaying ];
	//const UAnimationFrameNode* pFrameNode = &pAnimationInfo->m_Frames[ m_nFrameIndex ];
	//m_PlayingTexture.nTextureIndex = pAnimationInfo->nTextureIndex;
	//m_PlayingTexture.Color = pFrameNode->Color;
	//m_PlayingTexture.rcTexCoord = pFrameNode->rcTexCoord;
}



//
//
//namespace iberbar
//{
//	namespace Paper2d
//	{
//		void LoadAnimNodeFromJson( CAnimationController* pAnimController, const nlohmann::json& jsonAnimNode );
//		void LoadAnimFrameNodeFromJson( CAnimationController* pAnimController, const nlohmann::json& jsonFrameNode );
//	}
//}
//
//
//
//iberbar::CResult iberbar::Paper2d::LoadAnimationControllerFromJson( CAnimationController* pAnimController, const char* strFile )
//{
//	nlohmann::json jsonRoot = nlohmann::json::parse( "" );
//	if ( jsonRoot.contains( "AnimationList" ) == false )
//		return MakeResult( ResultCode::Bad, "" );
//
//	const nlohmann::json& jsonAnimList = jsonRoot.at( "AnimationList" );
//	if ( jsonAnimList.is_null() == true || jsonAnimList.is_array() == false )
//		return MakeResult( ResultCode::Bad, "" );
//
//	size_t nAnimCount = jsonAnimList.size();
//	for ( size_t i = 0; i < nAnimCount; i++ )
//	{
//		LoadAnimNodeFromJson( pAnimController, jsonAnimList.at( i ) );
//	}
//	
//	return CResult();
//}
//
//
//void iberbar::Paper2d::LoadAnimNodeFromJson( CAnimationController* pAnimController, const nlohmann::json& jsonAnimNode )
//{
//	std::string strName = JsonGetString( jsonAnimNode, "Name", "" );
//	if ( strName.empty() == true )
//		return;
//
//	float nTimePerFrame = JsonGetFloat( jsonAnimNode, "TimePerFrame", 0.0f );
//	if ( nTimePerFrame <= 0.0f )
//		return;
//
//
//}
//
//
//void iberbar::Paper2d::LoadAnimFrameNodeFromJson( CAnimationController* pAnimController, const nlohmann::json& jsonFrameNode )
//{
//
//}

