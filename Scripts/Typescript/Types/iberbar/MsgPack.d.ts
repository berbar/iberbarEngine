

declare namespace iberbar.MsgPack
{
    function Decode<T=any>( this: void, bytes: string ): T;

    function Encode( this: void, data: any ): string;
}