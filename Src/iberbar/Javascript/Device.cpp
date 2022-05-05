
#include <iberbar/Javascript/Device.h>


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
    m_DefaultPlatform = m_Platform.get();
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

    v8::Local<v8::Context> Context = v8::Context::New( m_MainIsolate );
    v8::Local<v8::ObjectTemplate> Global = Context->Global();
}


void iberbar::iJavascript::CDevice::Shutdown()
{
    m_MainIsolate->Dispose();
    m_MainIsolate = nullptr;
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    //delete params.array_buffer_allocator;
    // Delete the platform explicitly here to write the tracing output to the
    // tracing file.
    m_Platform.reset();
}
