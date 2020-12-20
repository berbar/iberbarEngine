

declare namespace iberbar.Paper2d
{

    class CNode
    {
        protected constructor();
        public Init(): void;
        public SetZOrder( zorder: number ): void;
        public SetPosition( x: number, y: number ): void;
        public SetAnchor( x: number, y: number ): void
        public SetContentSize( w: number, h: number ): void;
        public InsertChild( node: CNode ): void;
        public AddComponent( component: CComponent ): void;
    }


    class CScene extends CNode
    {
        protected constructor();
        public static new(): CScene;
        public Init(): void;
        public GetCamera(): CCamera;
    }


    class CCamera
    {
        protected constructor();
        public SetPosition( x: number, y: number ): void;
        public SetOrthographicProjection( w: number, h: number ): void;
        public Build(): void;
    }


    class CComponent
    {
        protected constructor();
        public SetName( name: string ): void;
    }


    class CComponent_Updatable extends CComponent
    {
        public static new(): CComponent_Updatable;
        public BindCallback( callback: ( delta: number ) => void, ext?: any ): void;
    }


    type UComponent_HandleKeyboardInput_Event =
    {
        keycode: number;
    }

    class CComponent_HandleKeyboardInput extends CComponent
    {
        public static new(): CComponent_HandleKeyboardInput;
        public BindCallback( callback: ( event: UComponent_HandleKeyboardInput_Event ) => void, ext?: any ): void;
    }


    class CImage extends  CNode
    {
        public static new(): CImage;
        public SetTexture( texture: iberbar.Rhi.CTexture ): void;
        public SetColor( r: number, g: number, b: number, a?: number ): void;
        public SetUV( l: number, t: number, r: number, b: number ): void;
    }


    class CGridTerrain extends CNode
    {
        public static new(): CGridTerrain;
        public SetGridSize( w: number, h: number ): void;
    }



    function RunScene( this: void, scene: CScene ): void;
    function GetScene( this: void ): CScene;
}