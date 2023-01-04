
#include <iberbar/Javascript/Device.h>
#include <iberbar/Javascript/JsModuleLoader.h>
#include <iberbar/Javascript/JsLogger.h>
#include <iberbar/Javascript/V8Utils.h>
#include <iberbar/Utility/FileHelper.h>


iberbar::iJavascript::CDevice* iberbar::iJavascript::CDevice::sm_pInstance = nullptr;


iberbar::iJavascript::CDevice::CDevice()
    : m_DefaultPlatform( nullptr )
    , m_Platform()
    , m_MainIsolate( nullptr )
    , m_DefaultContext()
    , m_GlobalObjs()
    , m_pLoggerDefault( new CDefaultLogger() )
    , m_pLogger( nullptr )
    , m_pModuleLoaderDefault( new CDefaultModuleLoader() )
    , m_pModuleLoader( nullptr )
{
    sm_pInstance = this;
    m_pLogger = m_pLoggerDefault;
    m_pModuleLoader = m_pModuleLoaderDefault;
}


iberbar::iJavascript::CDevice::~CDevice()
{
    SAFE_DELETE( m_pLoggerDefault );
    SAFE_DELETE( m_pModuleLoaderDefault );
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
    // 初始化Isolate和DefaultContext
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

    Global->Set( Context, ToV8StringA( m_MainIsolate, "__tgjsLog" ), v8::FunctionTemplate::New( m_MainIsolate, []( const v8::FunctionCallbackInfo<v8::Value>& Info )
    {
        auto Self = static_cast<CDevice*>((v8::Local<v8::External>::Cast( Info.Data() ))->Value());
        Self->Log( Info );
    }, This )->GetFunction( Context ).ToLocalChecked() ).Check();

    ExecuteModule( "iberbar/log.js" );
    ExecuteModule( "iberbar/modular.js" );

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


void iberbar::iJavascript::CDevice::FindModule( const v8::FunctionCallbackInfo<v8::Value>& Info )
{

}


void iberbar::iJavascript::CDevice::LoadModule( const v8::FunctionCallbackInfo<v8::Value>& Info )
{
    v8::Isolate* Isolate = Info.GetIsolate();
    v8::HandleScope HandleScope( Isolate );
    v8::Local<v8::Context> Context = Isolate->GetCurrentContext();
    v8::Context::Scope ContextScope( Context );

    if ( Info.Length() == 0 ) {
        Isolate->ThrowException(
            v8::String::NewFromUtf8( Isolate, "bad args", v8::NewStringType::kNormal ).ToLocalChecked() );
        return;
    }
    
    std::string strFilePath = ToStringA( Isolate, Info[0] );
    std::string strScriptText;
    CFileHelper FileHelper;
    if ( FileHelper.OpenFileA( strFilePath.c_str(), "r" ) )
    {
        strScriptText = FileHelper.ReadAsText();
        FileHelper.CloseFile();
    }



    v8::Local<v8::Value> Result = v8::Undefined( Isolate );

    v8::Local<v8::Object> global = Context->Global();
    global->Set( Context, v8::String::NewFromUtf8( Isolate, "exports" ).ToLocalChecked(), v8::Object::New( Isolate ) );
    v8::Local<v8::String> Source = v8::String::NewFromUtf8( Isolate, strScriptText.c_str(), v8::NewStringType::kNormal ).ToLocalChecked();
    v8::Local<v8::Script> CompiledScript = v8::Script::Compile( Context, Source ).ToLocalChecked();
    if ( CompiledScript.IsEmpty() ) {
        Result = Isolate->ThrowException( v8::String::NewFromUtf8( Isolate, "module import error: Compile" ).ToLocalChecked() );
    }
    else {

        CompiledScript->Run( Context );

        Info.GetReturnValue().Set( global->Get( Context, v8::String::NewFromUtf8( Isolate, "exports" ).ToLocalChecked() ).ToLocalChecked() );
    }
}


void iberbar::iJavascript::CDevice::Log( const v8::FunctionCallbackInfo<v8::Value>& Info )
{
    v8::Isolate* Isolate = Info.GetIsolate();
    v8::Isolate::Scope IsolateScope( Isolate );
    v8::HandleScope HandleScope( Isolate );
    v8::Local<v8::Context> Context = Isolate->GetCurrentContext();
    v8::Context::Scope ContextScope( Context );

    CHECK_V8_ARGS( EArgType::Int32, EArgType::String );

    auto Level = Info[ 0 ]->Int32Value( Context ).ToChecked();

    std::string Msg = ToStringA( Isolate, Info[ 1 ] );
    switch ( Level )
    {
    case 1:
        m_pLogger->Info( Msg );
        break;
    case 2:
        m_pLogger->Warn( Msg );
        break;
    case 3:
        m_pLogger->Error( Msg );
        break;
    default:
        m_pLogger->Log( Msg );
        break;
    }
}


void iberbar::iJavascript::CDevice::ExecuteModule( const std::string& ModuleName, std::function<std::string( const std::string&, const std::string& )> Preprocessor )
{
    std::string OutPath;
    std::string DebugPath;
    std::string Data;

    CResult Ret = LoadFile( "", ModuleName, OutPath, DebugPath, Data );
    if ( !Ret.IsOK() )
    {
        m_pLogger->Error( Ret.data.c_str() );
        return;
    }

    std::string Script = Data;

    if ( Preprocessor ) Script = Preprocessor( Script, OutPath );

    auto Isolate = m_MainIsolate;
    v8::Isolate::Scope IsolateScope( Isolate );
    v8::HandleScope HandleScope( Isolate );
    auto Context = v8::Local<v8::Context>::New( Isolate, m_DefaultContext );
    v8::Context::Scope ContextScope( Context );
    {
#if PLATFORM_MAC
        std::string FormattedScriptUrl = DebugPath;
#else
        // 修改URL分隔符格式，否则无法匹配Inspector协议在打断点时发送的正则表达式，导致断点失败
        std::string FormattedScriptUrl = DebugPath.replace( DebugPath.begin(), DebugPath.end(), '/', '\\' );
#endif
        v8::Local<v8::String> Name = ToV8StringA( Isolate, FormattedScriptUrl.c_str() );
        v8::ScriptOrigin Origin( Name );
        v8::Local<v8::String> Source = ToV8StringA( Isolate, Script.c_str() );
        v8::TryCatch TryCatch( Isolate );

        auto CompiledScript = v8::Script::Compile( Context, Source, &Origin );
        if ( CompiledScript.IsEmpty() )
        {
            m_pLogger->Error( TryCatchToString( Isolate, &TryCatch ) );
            return;
        }
        auto ReturnVal = CompiledScript.ToLocalChecked()->Run( Context );
        if ( TryCatch.HasCaught() )
        {
            m_pLogger->Error( TryCatchToString( Isolate, &TryCatch ) );
            return;
        }
    }
}


iberbar::CResult iberbar::iJavascript::CDevice::LoadFile(
    const std::string& RequiringDir, const std::string& ModuleName,
    std::string& OutPath, std::string& OutDebugPath,
    std::string& Data )
{
    if ( m_pModuleLoader->Search( RequiringDir, ModuleName, OutPath, OutDebugPath ) )
    {
        if ( !m_pModuleLoader->Load( OutPath, Data ) )
        {
            return MakeResult( ResultCode::Bad, "can not load [%s]", ModuleName.c_str() );
        }
    }
    else
    {
        return MakeResult( ResultCode::Bad, "can not find [%s]", ModuleName.c_str() );
    }
    return CResult();
}

