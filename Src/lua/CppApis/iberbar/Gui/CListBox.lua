
--- @class iberbar.Gui.CListBox : iberbar.Gui.CWidget
local CListBox = {};


--- @return iberbar.Gui.CListBox
function CListBox.new() end


--- @param widget iberbar.Gui.CWidget
--- @return iberbar.Gui.CListBox
function CListBox.dynamic_cast( widget ) end


--- @param drag boolean
--- @return void
function CListBox:SetEnableDrag( drag ) end


--- 设置选择方式
--- @param type number 选择方式，1=单选，2=多选
--- @return void
function CListBox:SetSelectType( type ) end


--- @param w number
--- @param h number
--- @return void
function CListBox:SetItemSize( w, h ) end


--- @param margin number
--- @return void
function CListBox:SetItemMargin( margin ) end


--- @param direction number 排列方向，0=垂直，1=水平
--- @return void
function CListBox:SetItemDirection( direction ) end


--- @param func function
--- @param ext any
--- @return void
function CListBox:SetCallback_CreateItemElements( func, ext ) end


--- @param func function
--- @param ext any
--- @return void
function CListBox:SetCallback_UpdateItemElements( func, ext ) end


--- @param item table
--- @return void
function CListBox:AddItem( item ) end


--- 移除单个数据
--- @param index number 索引
--- @return number
function CListBox:RemoveItemOne( index ) end


--- 移除多个数据
--- @param indices number[] 索引数组
--- @return number
function CListBox:RemoveItems( indices ) end


--- 移除所有数据
--- @return void
function CListBox:RemoveItemsAll() end


--- @param index number
--- @return table
function CListBox:GetItem( index ) end


--- @param func function
--- @param ext any
--- @return number
function CListBox:FindItemOne( func, ext ) end


--- @param func function
--- @param ext any
--- @return number[]
function CListBox:FindItems( func, ext ) end


--- 判断是否被选中
--- @param index number
--- @return boolean
function CListBox:IsSelect( index ) end


--- 获取第一个选择项索引
--- @return number
function CListBox:GetSelectFirst() end


--- 获取所有选择项索引
--- @return number[]
function CListBox:GetSelectAll() end


iberbar.Gui.CListBox = CListBox;
