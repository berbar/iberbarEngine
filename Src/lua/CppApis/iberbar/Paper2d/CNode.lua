


---@class iberbar.Paper2d.CNode
local m = {};


---@return iberbar.Paper2d.CNode
function m.new() end


--- 初始化
---@return void
function m:Init() end


--- 设置Id，用于查找
---@param strId string
---@return void
function m:SetId( strId ) end


--- 获取Id
---@return string Id值
function m:SetId() end


--- 设置可见性，控制渲染
---@param bVisible boolean
---@return void
function m:SetVisible( bVisible ) end


--- 设置可用性，控制组件的更新、鼠标输入和键盘输入
------@param bEnable boolean
-----@return void
function m:SetEnable( bEnable ) end


--- 获取当前节点的可见性值，不等同于是否可见，判断是否可见，使用 IsVisible
---@return boolean
function m:GetVisible() end


--- 获取当前节点的可用性值，不等同于是否可用，判断是否可用，使用 IsEnable
---@return boolean
function m:GetEnable() end


---判断是否可见
---@return boolean
function m:IsVisible() end


--- 判断是否可用
---@return boolean
function m:IsEnable() end


--- 设置Z排序
---@param nZOrder number Z排序
---@return void
function m:SetZOrder( nZOrder ) end


--- 获取Z排序
---@return number
function m:GetZOrder() end


--- 设置xy位置，相对于父节点
---@param x number x坐标
---@param y number y坐标
---@return void
function m:SetPosition( x, y ) end


--- 获取xy位置，相对于父节点
---@return UVector2
function m:GetPosition() end


--- 设置节点内容尺寸
---@param w number 宽度，与x轴平行
---@param h number 高度，与y轴平行
---@return void
function m:SetContentSize( w, h ) end


--- 获取节点内容尺寸
---@return UVector2
function m:GetContentSize() end


--- 设置Anchor，0到1，子节点的位置基于Anchor
---@param x number
---@param y number
---@return void
function m:SetAnchor( x, y ) end


--- 获取Anchor，0到1，子节点的位置基于Anchor
---@return UVector2
function m:GetAnchor() end


--- 设置Pivot，自身的位置变换基于Pivot
---@param x number
---@param y number
---@return void
function m:SetPivot( x, y ) end


--- 获取Pivot，自身的位置变换基于Pivot
---@return UVector2
function m:GetPivot() end


--- 插入子节点
---@param node iberbar.Paper2d.CNode 子节点
---@return void
function m:InsertChild( node ) end


--- 移除子节点
---@param node iberbar.Paper2d.CNode 子节点
---@return void
function m:RemoveChild( node ) end


--- 移除所有节点
---@return void
function m:RemoveAllChildren() end


--- 查找子节点
---@param strId string 子节点Id
---@return iberbar.Paper2d.CNode
function m:FindChild( strId ) end


--- 获取所有子节点
---@return iberbar.Paper2d.CNode[]
function m:GetChildren() end


--- 添加组件
---@param component iberbar.Paper2d.CComponent 组件对象
---@return void
function m:AddComponent( component ) end


--- 获取单个组件
---@param strName string 组件对象名称
---@return iberbar.Paper2d.CComponent
function m:GetComponent( strName ) end


--- 获取默认名称的自定义属性组件
---@return iberbar.Paper2d.CComponent_CustomAttributes
function m:GetComponentDefaultCustomAttributes() end


if ( iberbar.Paper2d == nil ) then
    iberbar.Paper2d = {};
end
iberbar.Paper2d.CNode = m;
