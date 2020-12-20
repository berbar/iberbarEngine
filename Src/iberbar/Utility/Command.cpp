
#include <iberbar/Utility/Command.h>
#include <mutex>
#include <memory_resource>


namespace iberbar
{
	class CCommandQueueUnSync
		: public CCommandQueue
	{
	public:
		CCommandQueueUnSync( std::pmr::memory_resource* pMemoryRes );
		virtual ~CCommandQueueUnSync();

	public:
		virtual void AddCommand( CBaseCommand* pCommand ) override;
		virtual void Execute() override;

	private:
		//std::queue<CBaseCommand*> m_Commands;
		std::pmr::vector<CBaseCommand*> m_Commands;
	};
}






iberbar::CCommandQueue* iberbar::CreateCommandQueue( const UCommandQueueOptions& Options, std::pmr::memory_resource* pMemoryRes )
{
	if ( Options.bSync == true )
	{
		return nullptr;
	}
	else
	{
		return new CCommandQueueUnSync( pMemoryRes );
	}
}


void iberbar::DestroyCommandQueue( CCommandQueue* pQueue )
{
	delete pQueue;
}






iberbar::CCommandQueueUnSync::CCommandQueueUnSync( std::pmr::memory_resource* pMemoryRes )
	: m_Commands( pMemoryRes )
{

}


iberbar::CCommandQueueUnSync::~CCommandQueueUnSync()
{
	for ( auto pCommand : m_Commands )
	{
		delete pCommand;
	}
	m_Commands.clear();
}


void iberbar::CCommandQueueUnSync::AddCommand( CBaseCommand* pCommand )
{
	m_Commands.push_back( pCommand );
}


void iberbar::CCommandQueueUnSync::Execute()
{
	for ( auto pCommand : m_Commands )
	{
		pCommand->Execute();
		delete pCommand;
	}
	m_Commands.clear();
}







