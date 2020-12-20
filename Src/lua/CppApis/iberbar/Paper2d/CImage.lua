

---@class iberbar.Paper2d.CImage : iberbar.Paper2d.CNode
local m = {};


---@return iberbar.Paper2d.CImage
function m.new() end



---@param left number x坐标
---@param top number y坐标
---@param right number x坐标
---@param bottom number y坐标
---@return void
function m:SetUV( left, top, right, bottom ) end


---@param texture table
---@return void
function m:SetTexture( texture ) end



--- 导出CImage
if ( iberbar.Paper2d == nil ) then
    iberbar.Paper2d = {};
end
iberbar.Paper2d.CImage = m;