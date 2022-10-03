
#include <iberbar/Javascript/Device.h>
#include <iberbar/Javascript/V8Utils.h>


iberbar::iJavascript::CDevice* iberbar::iJavascript::CDevice::sm_pInstance = nullptr;


iberbar::iJavascript::CDevice::CDevice()
    : m_DefaultPlatform( nullptr )
    , m_Platform()
    , m_MainIsolate( nullptr )
    , m_DefaultContext()
{
    sm_pInstance = this;
}


iberbar::iJavascript::CDevice::~CDevice()
{
    sm_pInstance = nullptr;
}


iberbar::CResult iberbar::iJavascript::CDevice::Initial()
{
    m_Platform = v8::platform::NewDefaultPlatform();
    // init platform
    v8::V8::InitializePlatform( m_Platform.get() );
    v8::V8::Initialize();
    // set natives_blob / snapshot_blob file path.
    // you can use InitializeExternalStartupData(const char* natives_blob,  const char* snapshot_blob)
    //v8::V8::InitializeExternalStartupData(  "G:/Test/TestV8/TestV8_Console/Main.js" );
    std::unique_ptr<v8::StartupData> SnapshotBlob;
    if ( !SnapshotBlob )
    {
        SnapshotBlob = std::make_unique<v8::StartupData>();
        SnapshotBlob->data = (const char*)SnapshotBlobCode;
        SnapshotBlob->raw_size = sizeof( SnapshotBlobCode );
    }
    // ≥ı ºªØIsolate∫ÕDefaultContext
    v8::V8::SetSnapshotDataBlob( SnapshotBlob.get() );
    v8::Isolate::CreateParams params;
    //params.code_event_handler = OnJitEvent;
    params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    m_MainIsolate = v8::Isolate::New( params );

    v8::Isolate::Scope Isolatescope( m_MainIsolate );
    v8::HandleScope HandleScope( m_MainIsolate );

    v8::Local<v8::Context> Context = v8::Context::New( m_MainIsolate );
    v8::Local<v8::Object> Global = Context->Global();

    m_DefaultContext.Reset( m_MainIsolate, Context );

    auto This = v8::External::New( m_MainIsolate, this );

    Global->Set(Context, ToV8StringA( m_MainIsolate, "dumpStatisticsLog" ), v8::FunctionTemplate::New( m_MainIsolate, []( const v8::FunctionCallbackInfo<v8::Value>& Info )
    {
        auto Self = static_cast<CDevice*>((v8::Local<v8::External>::Cast( Info.Data() ))->Value());
        Self->DumpStatisticsLog( Info );
    }, This )->GetFunction( Context ).ToLocalChecked() ).Check();

    return CResult();
}


void iberbar::iJavascript::CDevice::Shutdown()
{
    m_DefaultContext.Reset();
    if ( m_MainIsolate )
    {
        m_MainIsolate->Dispose();
        m_MainIsolate = nullptr;
    }
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
}


void iberbar::iJavascript::CDevice::ExecuteFile( const char* pstrFilePath )
{

}


iberbar::CResult iberbar::iJavascript::CDevice::ExecuteScript( const char* pstrScript )
{
    auto Isolate = m_MainIsolate;
    v8::Isolate::Scope IsolateScope( Isolate );
    v8::HandleScope HandleScope( Isolate );
    auto Context = v8::Local<v8::Context>::New( Isolate, m_DefaultContext );

    v8::Context::Scope ContextScope( Context );
    {
        v8::Local<v8::String> source = ToV8StringA( Isolate, pstrScript );
        v8::TryCatch TryCatch( Isolate );
        v8::MaybeLocal<v8::Script> CompiledScript = v8::Script::Compile( Context, source );
        if ( CompiledScript.IsEmpty() )
        {
            return TryCatchToCResult( Isolate, &TryCatch );
        }

        CompiledScript.ToLocalChecked()->Run( Context );
        if ( TryCatch.HasCaught() )
        {
            return TryCatchToCResult( Isolate, &TryCatch );
        }
    }

    return CResult();
}


void iberbar::iJavascript::CDevice::DumpStatisticsLog( const v8::FunctionCallbackInfo<v8::Value>& Info )
{
#ifndef WITH_QUICKJS
    v8::HeapStatistics Statistics;

    v8::Isolate* Isolate = Info.GetIsolate();
    v8::Isolate::Scope IsolateScope( Isolate );
    v8::HandleScope HandleScope( Isolate );
    v8::Local<v8::Context> Context = Isolate->GetCurrentContext();
    v8::Context::Scope ContextScope( Context );

    Isolate->GetHeapStatistics( &Statistics );

    std::string StatisticsLog = StdFormat( (
        "------------------------\n"
        "Dump Statistics of V8:\n"
        "total_heap_size: %u\n"
        "total_heap_size_executable: %u\n"
        "total_physical_size: %u\n"
        "total_available_size: %u\n"
        "used_heap_size: %u\n"
        "heap_size_limit: %u\n"
        "malloced_memory: %u\n"
        "external_memory: %u\n"
        "peak_malloced_memory: %u\n"
        "number_of_native_contexts: %u\n"
        "number_of_detached_contexts: %u\n"
        "does_zap_garbage: %u\n"
        "------------------------\n" ),
        Statistics.total_heap_size(),
        Statistics.total_heap_size_executable(),
        Statistics.total_physical_size(),
        Statistics.total_available_size(),
        Statistics.used_heap_size(),
        Statistics.heap_size_limit(),
        Statistics.malloced_memory(),
        Statistics.external_memory(),
        Statistics.peak_malloced_memory(),
        Statistics.number_of_native_contexts(),
        Statistics.number_of_detached_contexts(),
        Statistics.does_zap_garbage()
    );

    printf_s( StatisticsLog.c_str() );
    //Logger->Info( StatisticsLog );
#endif // !WITH_QUICKJS
}
