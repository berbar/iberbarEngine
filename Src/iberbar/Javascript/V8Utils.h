#pragma once

#include <iberbar/Javascript/Headers.h>
#include <iberbar/Utility/String.h>


namespace iberbar
{
	namespace iJavascript
	{

        FORCEINLINE static std::string ToStringA( v8::Isolate* Isolate, v8::Local<v8::Value> Value )
        {
            return std::string(*(v8::String::Utf8Value( Isolate, Value )));

        }

		FORCEINLINE v8::Local<v8::String> ToV8StringA( v8::Isolate* Isolate, const char* pText )
		{
			return v8::String::NewFromUtf8( Isolate, pText, v8::NewStringType::kNormal ).ToLocalChecked();
		}

		FORCEINLINE v8::Local<v8::String> ToV8StringW( v8::Isolate* Isolate, const wchar_t* pText )
		{
			std::string TextUtf8 = UnicodeToUtf8( pText );
			return v8::String::NewFromUtf8( Isolate, TextUtf8.c_str(), v8::NewStringType::kNormal ).ToLocalChecked();
		}

        FORCEINLINE std::string TryCatchToString( v8::Isolate* Isolate, v8::TryCatch* TryCatch )
        {
            v8::Isolate::Scope IsolateScope( Isolate );
            v8::HandleScope HandleScope( Isolate );
            v8::String::Utf8Value Exception( Isolate, TryCatch->Exception() );
            std::string ExceptionStr( *Exception );
            v8::Local<v8::Message> Message = TryCatch->Message();
            if ( Message.IsEmpty() )
            {
                // 如果没有提供更详细的信息，直接输出Exception
                return ExceptionStr;
            }
            else
            {
                v8::Local<v8::Context> Context( Isolate->GetCurrentContext() );

                // 输出 (filename):(line number): (message).
                v8::String::Utf8Value FileName( Isolate, Message->GetScriptResourceName() );
                int LineNum = Message->GetLineNumber( Context ).FromJust();
                std::string FileNameStr( *FileName );
                std::string LineNumStr = std::to_string( LineNum );
                std::string FileInfoStr;
                FileInfoStr.append( FileNameStr ).append( ":" ).append( LineNumStr ).append( ": " ).append( ExceptionStr );

                std::string FinalReport;
                FinalReport.append( FileInfoStr ).append( "\n" );

                // 输出调用栈信息
                v8::Local<v8::Value> StackTrace;
                if ( TryCatch->StackTrace( Context ).ToLocal( &StackTrace ) )
                {
                    v8::String::Utf8Value StackTraceVal( Isolate, StackTrace );
                    std::string StackTraceStr( *StackTraceVal );
                    FinalReport.append( "\n" ).append( StackTraceStr );
                }
                return FinalReport;
            }
        }

        FORCEINLINE CResult TryCatchToCResult( v8::Isolate* Isolate, v8::TryCatch* TryCatch )
        {
            CResult Result;
            Result.code = ResultCode::Bad;
            Result.data = TryCatchToString( Isolate, TryCatch );
            return Result;
        }
	}
}