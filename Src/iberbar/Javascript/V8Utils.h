#pragma once

#include <iberbar/Javascript/Headers.h>
#include <iberbar/Utility/String.h>


namespace iberbar
{
	namespace iJavascript
	{
        enum class EArgType
        {
            Int32,
            Number,
            String,
            External,
            Function,
            Object
        };

        FORCEINLINE static void ThrowException( v8::Isolate* Isolate, const std::string& Message )
        {
            ThrowException( Isolate, Message.c_str() );
        }

        FORCEINLINE static void ThrowException( v8::Isolate* Isolate, const char* Message )
        {
            auto ExceptionStr = v8::String::NewFromUtf8( Isolate, Message,
                v8::NewStringType::kNormal ).ToLocalChecked();
            Isolate->ThrowException( v8::Exception::Error( ExceptionStr ) );
        }

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

        FORCEINLINE static bool CheckArgument( const v8::FunctionCallbackInfo<v8::Value>& Info, const std::vector<EArgType>& TypesExpect )
        {
            if ( Info.Length() < TypesExpect.size() )
            {
                ThrowException( Info.GetIsolate(), StdFormat( "Bad parameters, the function expect %d, but  %d provided.", TypesExpect.size(), Info.Length() ) );
                return false;
            }

            for ( int i = 0; i < TypesExpect.size(); ++i )
            {
                switch ( TypesExpect[ i ] )
                {
                case EArgType::Int32:
                    if ( !Info[ i ]->IsInt32() )
                    {
                        ThrowException( Info.GetIsolate(), StdFormat( "Bad parameters #%d, expect a int32.", i ) );
                        return false;
                    }
                    else
                    {
                        break;
                    }
                case EArgType::Number:
                    if ( !Info[ i ]->IsNumber() )
                    {
                        ThrowException( Info.GetIsolate(), StdFormat( "Bad parameters #%d, expect a int32.", i ) );
                        return false;
                    }
                    else
                    {
                        break;
                    }
                case EArgType::String:
                    if ( !Info[ i ]->IsString() )
                    {
                        ThrowException( Info.GetIsolate(), StdFormat( "Bad parameters #%d, expect a string.", i ) );
                        return false;
                    }
                    else
                    {
                        break;
                    }
                case EArgType::External:
                    if ( !Info[ i ]->IsExternal() )
                    {
                        ThrowException( Info.GetIsolate(), StdFormat( "Bad parameters #%d, expect an external.", i ) );
                        return false;
                    }
                    else
                    {
                        break;
                    }
                case EArgType::Function:
                    if ( !Info[ i ]->IsFunction() )
                    {
                        ThrowException( Info.GetIsolate(), StdFormat( "Bad parameters #%d, expect a function.", i ) );
                        return false;
                    }
                    else
                    {
                        break;
                    }
                case EArgType::Object:
                    if ( !Info[ i ]->IsObject() )
                    {
                        ThrowException( Info.GetIsolate(), StdFormat( "Bad parameters #%d, expect a object.", i ) );
                        return false;
                    }
                    else
                    {
                        break;
                    }
                default:
                    break;
                }
            }

            return true;
        }
	}
}

#define CHECK_V8_ARGS(...) \
static std::vector<EArgType> ArgExpect = { __VA_ARGS__ }; \
if (!iberbar::iJavascript::CheckArgument(Info, ArgExpect)) \
{ \
    return; \
}