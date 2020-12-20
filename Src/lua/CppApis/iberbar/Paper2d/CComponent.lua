

---@class iberbar.Paper2d.CComponent
local m = {}


--- 获取该组件所附着的节点对象
---@return iberbar.Paper2d.CNode
function m:GetOwner() end


--- 设置名称，用于查找组件
---@param strName string
---@return void
function m:SetName( strName ) end


--- 获取组件名称
---@return string
function m:GetName() end


--- 允许或禁用该组件
---@param enable boolean true，允许；false，禁用
---@return void
function m:SetEnable( enable ) end


--- 获取当前组件的允许或禁用的设置值，不等同于判断是否处于允许或禁用状态，获取运行状态，使用IsEnable
---@return boolean
function m:GetEnable() end


--- 判断是否处于允许或禁用状态，获取运行状态，使用IsEnable
---@return boolean
function m:IsEnable() end



--- 导出
if ( iberbar.Paper2d == nil ) then
    iberbar.Paper2d = {};
end
iberbar.Paper2d.CComponent = m;
