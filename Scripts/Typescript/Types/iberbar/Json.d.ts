

declare namespace iberbar.Json
{
    function Deserialize<T>( this: void, str: string ): T;
    function Serialize( this: void, obj: any ): string;
}