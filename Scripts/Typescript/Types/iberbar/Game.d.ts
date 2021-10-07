
declare namespace iberbar.Game
{
    function GetGuiXmlParser( this: void ): iberbar.Gui.CXmlParser;

    function GetOrCreateFont( this: void, family: string, size: number, weight: number, italic: boolean ): iberbar.Renderer.CFont | string;
    function GetFont( this: void ): iberbar.Renderer.CFont;
    function GetFont( this: void, family: string, size: number, weight: number, italic: boolean ): iberbar.Renderer.CFont;

    function GetOrCreateTexture( this: void, name: string ): iberbar.Rhi.CTexture | string;
    function GetTexture( this: void,  name: string ): iberbar.Rhi.CTexture;

    function Paper2dLoadGridTerrain( this: void, name: string, terrain: Paper2d.CGridTerrain ): void;

    function LogTrace( this: void,  tag: string, text: string ): void;
    function LogDebug( this: void,  tag: string, text: string ): void;
    function LogInfo( this: void,  tag: string, text: string ): void;
    function LogWarn( this: void,  tag: string, text: string ): void;
    function LogError( this: void,  tag: string, text: string ): void;
    function LogCritical( this: void,  tag: string, text: string ): void;
    function LogFatal( this: void,  tag: string, text: string ): void;


    class CLoadingTask
    {
        protected constructor();
        public SetTaskName( name: string ): void;
    }
    class CLoadingTask_KeyTask extends CLoadingTask
    {
        public static new(): CLoadingTask_KeyTask;
        public SetCallback( func: any ): void;
        public SetCallback( func: any, ext: any ): void;
    }
    type ULoadingTask_Textures_CallbackStep_1 = ( this: void, texName: string, texCountTotal: number, texIndex: number, ret: boolean, err: string ) => void;
    type ULoadingTask_Textures_CallbackStep_2<TExt> = ( this: TExt, texName: string, texCountTotal: number, texIndex: number, ret: boolean, err: string ) => void;
    type ULoadingTask_Textures_CallbackFinal_1 = ( this: void ) => void;
    type ULoadingTask_Textures_CallbackFinal_2<TExt> = ( this: TExt ) => void;
    class CLoadingTask_Textures extends CLoadingTask
    {
        public static new(): CLoadingTask_Textures;
        /**
         * 设置每加载一张纹理后线程休眠时间
         * @param milliseconds 休眠时间，毫秒
         */
        public SetSleepFor( milliseconds: number ): void;
        /**
         * 添加待加载的纹理
         * @param texName 纹理路径
         */
        public AddTexture( texName: string ): void;
        /**
         * 设置step回调
         * @param func 回调函数
         */
        public SetCallbackOnStep( func: ULoadingTask_Textures_CallbackStep_1 ): void;
        /**
         * 设置step回调
         * @param func 回调函数
         * @param ext 上下文拓展参数
         */
        public SetCallbackOnStep<TExt>( func: ULoadingTask_Textures_CallbackStep_2<TExt>, ext: TExt ): void;
        /**
         * 设置final回调
         * @param func 回调函数
         */
        public SetCallbackOnFinal( func: ULoadingTask_Textures_CallbackFinal_1 ): void;
        /**
         * 设置final回调
         * @param func 回调函数
         * @param ext 上下文拓展参数
         */
        public SetCallbackOnFinal<TExt>( func: ULoadingTask_Textures_CallbackFinal_2<TExt>, ext: TExt ): void;
    }
    /**
     * 唤醒loading线程，执行loading任务
     */
    function WakeupLoadingThread( this: void ): void;
    /**
     * 添加loading任务，需要唤醒线程才可以执行任务
     * @param task 任务
     */
    function AddLoadingTask( task: CLoadingTask ): void;
}