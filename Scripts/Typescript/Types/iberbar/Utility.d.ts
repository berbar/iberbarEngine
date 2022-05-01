

declare namespace iberbar
{
    /**
     * 两个返回值，boolean和string
     * @param filepath 文件路径
     */
    function ReadFileAsText( this: void, filepath: string ): any;

    // function JsonEncode( this: void, obj: any ): string;
    // function JsonDecode<T>( this: void, str: string ): T;


    type URect = 
    {
        left: number;
        top: number;
        right: number;
        bottom: number;
    };


    enum UAlignHorizontal
    {
        Left = 0,
        Center = 1,
        Right = 2
    }

    enum UAlignVertical
    {
        Top = 0,
        Center = 1,
        Bottom = 2
    }
}