
/// <reference path="./Utility.d.ts" />
/// <reference path="./Renderer.d.ts" />



declare namespace iberbar.Gui
{
    enum UWidgetState
    {
        Normal,
        MouseOver,
        Pressed,
        Focus,
        Disabled,
        Hidden,
    }

    type UFontDesc = {
        family: string;
        size: number;
        weight: number;
        italic: boolean;
    };
    type UXmlParser_CallbackGetTexture_1 = ( this: void, texName: string ) => iberbar.Rhi.CTexture | string | null;
    type UXmlParser_CallbackGetTexture_2<TExt> = ( this: TExt, texName: string ) => iberbar.Rhi.CTexture | string | null;
    type UXmlParser_CallbackGetFont_1 = ( this: void, fontDesc: UFontDesc ) => iberbar.Renderer.CFont | string | null;
    type UXmlParser_CallbackGetFont_2<TExt> = ( this: TExt, texName: UFontDesc ) => iberbar.Renderer.CFont | string | null;
    class CXmlParser
    {
        private constructor();
        public ReadFile( strFile: string, dialog: any ): string;
        public SetCallbackGetTexture( func: UXmlParser_CallbackGetTexture_1 ): void;
        public SetCallbackGetTexture<TExt>( func: UXmlParser_CallbackGetTexture_2<TExt>, ext: TExt ): void;
        public SetCallbackGetFont( func: UXmlParser_CallbackGetFont_1 ): void;
        public SetCallbackGetFont<TExt>( func: UXmlParser_CallbackGetFont_2<TExt>, ext: TExt ): void;
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
        ContextMenu,
        FocusIn,
        FocusOut,
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
        public SetPaddings( left: number, top: number, right: number, bottom: number ): void;
        public SetPaddings( paddings: number ): void;
        public GetPaddings(): URect;
        public SetVisible( visible: boolean ): void;
        public GetVisible(): boolean;
        public SetEnable( enable: boolean ): void
        public GetEnable(): boolean;
        public SetAlignHorizental( align: UAlignHorizontal ): void;
        public GetAlignHorizental(): UAlignHorizontal;
        public SetAlignVertical( align: UAlignVertical ): void;
        public GetAlignVertical(): UAlignVertical
        public SetPercentX( usePercent: boolean ): void;
        public SetPercentY( usePercent: boolean ): void;
        public SetPercentW( usePercent: boolean ): void;
        public SetPercentH( usePercent: boolean ): void;
        public Refresh(): void;
        public SetDebugId( debugId: number ): void;
        public GetBounding(): URect;
    }

    class CDialog extends CObject
    {
        protected constructor();
        public SetWidgetRoot( widget: CWidget ): void;
        public GetWidgetRoot(): CWidget;
        public RequestTop(): void;
        public SetModal( modal: boolean ): void;
        public IsModal(): boolean;
    }

    type UCallback_WidgetEvent = ( this: void, widget: CWidget, event: number, valueInt: number ) => void;
    type UCallback_WidgetEvent__<TExt> = ( this: TExt, widget: CWidget, event: number, valueInt: number ) => void;

    class CWidget extends CObject
    {
        protected constructor();
        public AddWidget( widget: CWidget ): void;
        public FindWidget( id: string ): CWidget;
        public AddEventCallback( eventId: number, callback: UCallback_WidgetEvent, defer?: boolean ): number;
        public AddEventCallback<TExt>( eventId: number, callback: UCallback_WidgetEvent__<TExt>, ext: TExt, defer?: boolean ): number;
        public RemoveEventCallback( handle: number ): void;
        public RemoveEventCallbacksAll(): void;
        public SetRenderElement( element: iberbar.Gui.CRenderElement ): void;
        public FindElement( elementId: string ): CRenderElement;
        public RequestFocus(): void;
    }

    type UCallback_ListBox_ItemElementAllocate_1 = ( this: void ) => CRenderElement;
    type UCallback_ListBox_ItemElementAllocate_2<TExt> = ( this: TExt ) => CRenderElement;
    type UCallback_ListBox_ItemElementRelease_1 = ( this: void, element: CRenderElement ) => void;
    type UCallback_ListBox_ItemElementRelease_2<TExt> = ( this: TExt, element: CRenderElement ) => void;
    type UCallback_ListBox_ItemElementCreate_1< TItem > = ( this: void, element: CRenderElement, item: TItem, index: number ) => void;
    type UCallback_ListBox_ItemElementCreate_2< TItem, TExt > = ( this: TExt, element: CRenderElement, item: TItem, index: number ) => void;
    type UCallback_ListBox_ItemElementUpdate_1< TItem > = ( this: void, element: CRenderElement, item: TItem, index: number, updateEvent: number ) => void;
    type UCallback_ListBox_ItemElementUpdate_2< TItem, TExt > = ( this: TExt, element: CRenderElement, item: TItem, index: number, updateEvent: number ) => void;
    type UCallback_ListBox_FindItem< TItem > = ( this: void, item: TItem ) => boolean;
    type UCallback_ListBox_FindItem__< TItem, TExt > = ( this: TExt, item: TItem ) => boolean;
    enum EListBoxDragStyle
    {
        None,
        DragView,
        DragItem
    }

    class CListBox< TItem = any > extends CWidget
    {
        protected constructor();

        public static new< TItem = any >(): CListBox<TItem>;

        public static dynamic_cast< TItem = any >( widget: CWidget ): CListBox<TItem>;

        public SetDragStyle( style: EListBoxDragStyle ): void;

        public SetSelectType( selectType: number ): void

        public SetItemSize( w: number, h: number ): void;
        
        public SetCallback_ItemElementAllocate( callback: UCallback_ListBox_ItemElementAllocate_1 ): void;
        public SetCallback_ItemElementAllocate< TExt >( callback: UCallback_ListBox_ItemElementAllocate_2<TExt>, ext: TExt ): void;
        public SetCallback_ItemElementCreate( callback: UCallback_ListBox_ItemElementCreate_1<TItem> ): void;
        public SetCallback_ItemElementCreate< TExt >( callback: UCallback_ListBox_ItemElementCreate_2<TItem, TExt>, ext: TExt ): void;
        public SetCallback_ItemElementUpdate( callback: UCallback_ListBox_ItemElementUpdate_1<TItem> ): void;
        public SetCallback_ItemElementUpdate< TExt >( callback: UCallback_ListBox_ItemElementUpdate_2<TItem, TExt>, ext: TExt ): void;

        public AddItem( item: TItem ): void;

        public UpdateItem( index: number, item: TItem ): void;
        /**
         * 更新item指定字段
         * @param index item索引
         * @param itemFields 需要变更的字段和值
         */
        public UpdateItemFields( index: number, itemFields: Partial<TItem>, updateEvent: number ): void;

        /**
         * 移除所有item
         */
        public RemoveItemsAll(): void;

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

        public GetItemContextMenu(): number;

        public GetItemContextMenuAtPoint(): { x: number, y: number };

        public GetItemBounding( index: number ): URect;
        public SetItemEnable( index: number, enable: boolean ): void;
        public IsItemEnable( index: number ): boolean;
    }


    class CEditBox extends CWidget
    {
        protected constructor();
        public static new(): CEditBox;
        public static dynamic_cast( widget: CWidget ): CEditBox;
        public GetValueText(): string;
        public SetValueText( text: string ): void;
    }


    class CProgressBar extends CWidget
    {
        protected constructor();
        public static new(): CProgressBar;
        public static dynamic_cast( widget: CWidget ): CProgressBar;
        public SetProgressValueMax( valueMax: number ): void;
        public SetProgressValue( value: number ): void;
        public GetProgressValueMax(): number;
        public GetProgressValue(): number;
    }


    /**
     * 
     * @param style 0=overlad, 1=child
     * @param dialogParent 
     */
    function CreateDialog( this: void, dialogParent?: CDialog ): CDialog;
    function GetDialog( this: void, id: string ): CDialog;



    class CRenderElement extends CObject
    {
        protected constructor();
        public AddElement( element: CRenderElement ): void;
        public Init(): void;
        public FindElement( id: string ): CRenderElement;
        public SetZOrder( zorder: number ): void;
    }

    class CElementColorRect extends CRenderElement
    {
        protected constructor();
        public static new(): CElementColorRect;
        public static dynamic_cast( element: CRenderElement ): CElementColorRect;
        public SetColor( state: number, color: number ): void;
        public SetColor( state: number, color: { r: number, g: number, b: number, a: number } ): void;
    }

    class CElementStateTexture extends CRenderElement
    {
        protected constructor();
        public static new(): CElementStateTexture;
        public static dynamic_cast( element: CRenderElement ): CElementStateTexture;
        public SetTexture( state: number, texture: Rhi.CTexture ): void;
        public SetColor( state: number, color: number ): void;
        public SetColor( state: number, color: { r: number, g: number, b: number, a: number } ): void;
        public SetUV( state: number, left: number, top: number, right: number, bottom: number ): void;
    }

    class CElementStateLabel extends CRenderElement
    {
        protected constructor();
        public static new(): CElementStateLabel;
        public static dynamic_cast( element: CRenderElement ): CElementStateLabel;
        public SetFont( font: iberbar.Renderer.CFont ): void;
        public SetText( text: string ): void;
        public SetColor( state: number, color: number ): void;
        public SetTextAlignHorizental( align: 0 | 1 | 2 ): void;
        public SetTextAlignVertical( align: 0 | 1 | 2 ): void;
    }
}