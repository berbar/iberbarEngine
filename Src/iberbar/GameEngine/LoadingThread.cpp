
#include <iberbar/GameEngine/LoadingThread.h>
#include <iberbar/GameEngine/Application.h>
#include <iberbar/GameEngine/TextureManager.h>
#include <iberbar/GameEngine/BaseResourceManager.h>
#include <iberbar/Utility/FileHelper.h>
#include <iberbar/Utility/String.h>



std::pmr::synchronized_pool_resource g_MemoryPool;


iberbar::Game::CLoadingTaskQueue::CLoadingTaskQueue()
	: m_Mutex()
	, m_Steps()
{

}


iberbar::Game::CLoadingTaskQueue::~CLoadingTaskQueue()
{
	auto iter = m_Steps.begin();
	auto end = m_Steps.end();
	for ( ; iter != end; iter++ )
	{
		delete (*iter);
	}
	m_Steps.clear();
}





iberbar::Game::CLoadingThread::CLoadingThread()
	: m_Thread()
	, m_Mutex()
	, m_bRunThread( false )
	, m_bIsWaiting( true )
	, m_bStop( false )
#ifdef _WINDOWS
	//, m_hEventWhenYield( nullptr )
	, m_Handles()
#endif
	, m_TaskQueue()
{
	memset( m_Handles, 0, sizeof( m_Handles ) );
}


iberbar::Game::CLoadingThread::~CLoadingThread()
{
#ifdef _WINDOWS
	for ( int i = 0, s = (int)UEventIdForThread::__Count; i < s; i++ )
	{
		if ( m_Handles[ i ] )
		{
			::CloseHandle( m_Handles[ i ] );
			m_Handles[ i ] = nullptr;
		}
	}
#endif

	if ( m_StepResultQueue.empty() == false )
	{
		auto iter = m_StepResultQueue.begin();
		auto end = m_StepResultQueue.end();
		for ( ; iter != end; iter++ )
		{
			(*iter)->Execute();
			delete (*iter);
		}
		m_StepResultQueue.clear();
	}
}


void iberbar::Game::CLoadingThread::Initial()
{
#ifdef _WINDOWS
	for ( int i = 0, s = (int)UEventIdForThread::__Count; i < s; i++ )
	{
		m_Handles[ i ] = ::CreateEventW( NULL, FALSE, FALSE, NULL );
	}
#endif

	m_Thread = std::thread( &CLoadingThread::OnRunThread, this );
	m_bRunThread = true;
}


void iberbar::Game::CLoadingThread::Destroy()
{
	m_bStop = true;

	// 发送结束线程事件
	::SetEvent( m_Handles[ (int)UEventIdForThread::Shutdown ] );

	if ( m_Thread.joinable() == true )
	{
		m_Thread.join();
	}
}


void iberbar::Game::CLoadingThread::AddTask( CLoadingTask* pTask )
{
	assert( pTask );

	m_TaskQueue.Lock();
	m_TaskQueue.AddStep( pTask );
	pTask->AddRef();
	m_TaskQueue.Unlock();
}


void iberbar::Game::CLoadingThread::Wakeup()
{
	if ( m_bRunThread == true && m_bStop == false )
	{
		if ( m_bIsWaiting == true )
		{
#ifdef _WINDOWS
			::SetEvent( m_Handles[ (int)UEventIdForThread::Wakeup ] );
#endif
		}
	}
}


void iberbar::Game::CLoadingThread::AddStepResult( CLoadingStepResult* pStepResult )
{
	this->Lock();
	m_StepResultQueue.push_back( pStepResult );
	this->Unlock();
}


void iberbar::Game::CLoadingThread::LoopStepResults()
{
	assert( std::this_thread::get_id() == CApplication::sGetApp()->GetMainThreadId() );

	if ( m_StepResultQueue.empty() == true )
		return;

	if ( this->TryLock() == true )
	{
		auto iter = m_StepResultQueue.begin();
		auto end = m_StepResultQueue.end();
		for ( ; iter != end; iter ++ )
		{
			(*iter)->Execute();
			delete (*iter);
		}
		m_StepResultQueue.clear();
		this->Unlock();
	}
}


void iberbar::Game::CLoadingThread::OnRunThread()
{
#ifdef _WINDOWS
	while ( true )
	{
		DWORD nWaitRet = WaitForMultipleObjects( (int)UEventIdForThread::__Count, m_Handles, FALSE, INFINITE );
		DWORD nEvent = nWaitRet - WAIT_OBJECT_0;

		// Destroy命令
		if ( nEvent == (int)UEventIdForThread::Shutdown )
			break;

		m_bIsWaiting = false;

		for ( ;; )
		{
			if ( m_bStop == true )
				break;

			CLoadingTask* pTaskTemp = nullptr;

			if ( m_TaskQueue.TryLock() == true )
			{
				pTaskTemp = m_TaskQueue.PopStep();
				m_TaskQueue.Unlock();
			}
			else
			{
				OnYield();

				continue;
			}

			if ( pTaskTemp == nullptr )
				break;

			bool bFinishExecute = false;
			for ( ;; )
			{
				if ( m_bStop == true )
					break;

				if ( pTaskTemp->Execute( this ) <= 0 )
				{
					bFinishExecute = true;
				}

				OnYield();

				if ( bFinishExecute == true )
					break;
			}

			
			pTaskTemp->Release();

			OnYield();
		}

		m_bIsWaiting = true;
	}
#endif
}


void iberbar::Game::CLoadingThread::OnYield()
{
	//if ( m_hEventWhenYield )
	//	::SetEvent( m_hEventWhenYield );
	std::this_thread::yield();
}








iberbar::Game::CLoadingTask_KeyTask::CLoadingTask_KeyTask()
	: m_callback()
{
}


int iberbar::Game::CLoadingTask_KeyTask::Execute( CLoadingThread* pThread )
{
	pThread->AddStepResult( new _Result( m_callback ) );
	return 0;
}


void iberbar::Game::CLoadingTask_KeyTask::_Result::Execute()
{
	if ( m_callback )
	{
		m_callback();
	}
}


void* iberbar::Game::CLoadingTask_KeyTask::_Result::operator new(size_t n)
{
	assert( n > 0 );
	return LoadingThread_GetMemoryAllocator<CLoadingTask_KeyTask::_Result>().allocate( 1 );
}


void iberbar::Game::CLoadingTask_KeyTask::_Result::operator delete(void* p)
{
	if ( p )
	{
		LoadingThread_GetMemoryAllocator<CLoadingTask_KeyTask::_Result>().deallocate( (CLoadingTask_KeyTask::_Result*)p, 1 );
	}
}










iberbar::Game::CLoadingTask_Textures::CLoadingTask_Textures()
	: m_nMilliseconds( 5 )
	, m_TextureList()
	, m_nLoadedCount( 0 )
	, m_CallbackOnStep()
	, m_CallbackOnFinal()
{
}


int iberbar::Game::CLoadingTask_Textures::Execute( CLoadingThread* pThread )
{
	uint32 nRest = (uint32)m_TextureList.size() - m_nLoadedCount;
	if ( nRest == 0 )
		return 0;

	int nIndex = m_nLoadedCount;
	std::string& strName = m_TextureList[ nIndex ];
	_StepResult* pStepResult = new _StepResult( strName, m_TextureList.size(), nIndex, m_CallbackOnStep );
	if ( CApplication::sGetApp()->GetTextureManager()->GetTextureA( strName.c_str() ) == false )
	{
		std::string strFilePath = CResourceFileSystem::GetResoucePath( strName );
		CFileHelper FileHelper;
		bool nRetOpen = FileHelper.OpenFileA( strFilePath.c_str(), "rb" );
		if ( nRetOpen == false )
		{
			pStepResult->SetFailure( StdFormat( "failed to open file'%s'", strFilePath.c_str() ) );
		}
		else
		{
			_ByteString Bytes( LoadingThread_GetMemoryAllocator<char>() );
			FileHelper.ReadAsBinary( Bytes );
			pStepResult->SetSuccess( Bytes );
		}
		FileHelper.CloseFile();
	}
	else
	{
		pStepResult->SetSuccess();
	}

	pThread->AddStepResult( pStepResult );

	std::this_thread::sleep_for( std::chrono::milliseconds( m_nMilliseconds ) );

	m_nLoadedCount++;
	nRest--;

	if ( nRest == 0 )
	{
		_FinalResult* pFinalResult = new _FinalResult( m_CallbackOnFinal );
		pThread->AddStepResult( pFinalResult );
	}

	return nRest;
}


void iberbar::Game::CLoadingTask_Textures::AddTexture( const char* strName )
{
	for ( const auto& strTextureTemp : m_TextureList )
	{
		if ( strcmp( strTextureTemp.c_str(), strName ) == 0 )
			return;
	}
	m_TextureList.push_back( strName );
}


iberbar::Game::CLoadingTask_Textures::_StepResult::_StepResult(
	const std::string& strName,
	int nTotal,
	int nIndex,
	std::function<_CallbackOnStep> Callback )
	: m_nProgressTotal( nTotal )
	, m_nProgressIndex( nIndex )
	, m_strName( strName )
	, m_nRet( false )
	, m_strError()
	, m_Bytes()
	, m_Callback( Callback )
{
}


void iberbar::Game::CLoadingTask_Textures::_StepResult::Execute()
{
	if ( m_Bytes.empty() == false )
	{
		std::printf( "loadtexture in memory, filename=%s", m_strName.c_str() );
		CResult ret = CApplication::sGetApp()->GetTextureManager()->AddTexture( m_strName.c_str(), m_Bytes.c_str(), m_Bytes.size() );
		if ( ret.IsOK() == false )
		{
			SetFailure( ret.data );
		}
	}

	if ( m_Callback )
	{
		m_Callback( m_strName, m_nProgressTotal, m_nProgressIndex, m_nRet, m_strError );
	}
}


void* iberbar::Game::CLoadingTask_Textures::_StepResult::operator new(size_t n)
{
	assert( n > 0 );
	return LoadingThread_GetMemoryAllocator<CLoadingTask_Textures::_StepResult>().allocate( 1 );
}


void iberbar::Game::CLoadingTask_Textures::_StepResult::operator delete(void* p)
{
	if ( p )
	{
		LoadingThread_GetMemoryAllocator<CLoadingTask_Textures::_StepResult>().deallocate( (CLoadingTask_Textures::_StepResult*)p, 1 );
	}
}


iberbar::Game::CLoadingTask_Textures::_FinalResult::_FinalResult( std::function<_CallbackOnFinal> Callback )
	: m_Callback( Callback )
{
}


void iberbar::Game::CLoadingTask_Textures::_FinalResult::Execute()
{
	if ( m_Callback )
	{
		m_Callback();
	}
}


void* iberbar::Game::CLoadingTask_Textures::_FinalResult::operator new(size_t n)
{
	assert( n > 0 );
	return LoadingThread_GetMemoryAllocator<CLoadingTask_Textures::_FinalResult>().allocate( 1 );
}


void iberbar::Game::CLoadingTask_Textures::_FinalResult::operator delete(void* p)
{
	if ( p )
	{
		LoadingThread_GetMemoryAllocator<CLoadingTask_Textures::_FinalResult>().deallocate( (CLoadingTask_Textures::_FinalResult*)p, 1 );
	}
}






std::pmr::memory_resource* iberbar::Game::LoadingThread_GetMemoryPool()
{
	return &g_MemoryPool;
}

