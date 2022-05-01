
declare function print( this: void, ...args: any[] ): void;

declare function require( this: void, str: string ): any;

declare function type( this: void, obj: any ): "function" | "table" | "number" | "nil" | "string";

declare function getmetatable( this: void, tbl: any ): any;

declare function collectgarbage( this: void, opt?: string, arg?: string ): any

declare function tostring( this: void, arg: any ): string;


declare namespace debug
{
    export function traceback( this: void ): any;
}


declare namespace string
{
    function format( this: void, fmt: string, ...args: any[] ): string;
    function match( this: void, s1: string, s2: string ): any;
}

declare interface String
{
    format( ...args: any[] ): string;
}

// declare var package: {
//     path: string;
// }

declare namespace package
{
    var path: string;
}


declare type ULuaCallable_1<TFunc extends (...args: any[]) => any> = ( this: void, ...args: Parameters<TFunc>) => ReturnType<TFunc>;
declare type ULuaCallable_2<TFunc extends (...args: any[]) => any, TExt=any> = ( this: void, context: TExt, ...args: Parameters<TFunc>) => ReturnType<TFunc>;