

declare namespace iberbar.Gui
{
    class CXmlParser
    {
        private constructor();
        public ReadFile( strFile: string, dialog: any ): void;
    }


    enum UDialogStyle
    {
        Overlapped = 0,
        Child = 1
    }


    enum UEvents
    {
        Clicked,
        ValueChanged,
    }

    class CObject
    {
        protected constructor();
        public SetId( id: string ): void;
        public GetId(): string;
        public SetPosition( x: number, y: number ): string;
        public GetPosition(): { x: number, y: number };
        public SetSize( w: number, h: number ): void;
        public GetSize(): { w: number, h: number };
        public SetVisible( visible: boolean ): void;
        public GetVisible(): boolean;
    }

    class CDialog extends CObject
    {
        protected constructor();
        public AddWidget( widget: CWidget ): void;
        public FindWidget( id: string ): CWidget;
        public RequestTop(): void;
    }

    type UCallback_WidgetEvent = ( this: void, widget: CWidget, event: number, valueInt: number ) => void;
    type UCallback_WidgetEvent__<TExt> = ( this: TExt, widget: CWidget, event: number, valueInt: number ) => void;

    class CWidget extends CObject
    {
        protected constructor();
        public AddEventCallback( eventId: number, callback: UCallback_WidgetEvent ): void;
        public AddEventCallback<TExt>( eventId: number, callback: UCallback_WidgetEvent__<TExt>, ext: TExt ): void;
    }

    type UCallback_ListBox_CreateItemElement< TItem > = ( this: void, element: any, item: TItem, index: number ) => void;
    type UCallback_ListBox_CreateItemElement__< TItem, TExt > = ( this: TExt, element: any, item: TItem, index: number ) => void;
    type UCallback_ListBox_FindItem< TItem > = ( this: void, item: TItem ) => boolean;
    type UCallback_ListBox_FindItem__< TItem, TExt > = ( this: TExt, item: TItem ) => boolean;

    class CListBox< TItem = any > extends CWidget
    {
        protected constructor();

        public static new< TItem = any >(): CListBox<TItem>;

        public static dynamic_cast< TItem = any >( widget: CWidget ): CListBox<TItem>;

        public SetEnableDrag( enable: boolean ): void;

        public SetSelectType( selectType: number ): void

        public SetItemSize( w: number, h: number ): void;
        
        public SetCallback_CreateItemElements( callback: UCallback_ListBox_CreateItemElement<TItem> ): void;

        public SetCallback_CreateItemElements< TExt >( callback: UCallback_ListBox_CreateItemElement__<TItem, TExt>, ext: TExt ): void;

        public AddItem( item: TItem ): void;

        /**
         * 查找单个数据项
         * @param func 查找方法
         */
        public FindItemOne( func: UCallback_ListBox_FindItem< TItem > ): number;

        /**
         * 查找单个数据项
         * @param func 查找方法
         * @param ext 上下文拓展参数
         */
        public FindItemOne< TExt >( func: UCallback_ListBox_FindItem__< TItem, TExt >, ext: TExt ): number;

        /**
         * 查找多个数据项
         * @param func 查找方法
         */
        public FindItems( func: UCallback_ListBox_FindItem< TItem > ): number[];

        /**
         * 查找多个数据项
         * @param func 查找方法
         * @param ext 上下文拓展参数
         */
        public FindItems< TExt >( func: UCallback_ListBox_FindItem__< TItem, TExt >, ext: TExt ): number[];

        public GetItem( index: number ): TItem;

        public IsSelect( index: number ): boolean;

        public GetSelectFirst(): number;

        public GetSelectAll(): number[];
    }

    function CreateDialog( this: void, style: number, dialogParent?: CDialog ): CDialog;
    function GetDialog( this: void, id: string ): CDialog;



    class CRenderElement extends CObject
    {
        protected constructor();
        public AddElement( element: CRenderElement ): void;
    }

    class CElementStateLabel extends CRenderElement
    {
        protected constructor();
        public static new(): CElementStateLabel;
        public SetFont( font: any ): void;
        public SetText( text: string ): void;
        public SetColor( state: number, color: number ): void;
    }
}