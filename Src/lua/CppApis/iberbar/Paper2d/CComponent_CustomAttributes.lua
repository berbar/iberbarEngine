

---@class iberbar.Paper2d.CComponent_CustomAttributes
local m = {}


--- 添加属性
---@param name string 属性名称
---@param value string|number|boolean 属性值
---@return void
function m:AddAttribute( name, value ) end


--- 获取属性值
---@param name string
---@return string|number|boolean
function m:GetAttribute( name ) end




--- export
if ( iberbar.Paper2d == nil ) then
    iberbar.Paper2d = {};
end
iberbar.Paper2d.CComponent_CustomAttributes = m;

