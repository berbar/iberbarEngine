

--- @class iberbar.Gui.CRenderElement : iberbar.Gui.CObject
local m = {};


--- @return iberbar.Gui.CRenderElement
function m.new() end


--- @param zorder number
function m:SetZOrder( zorder ) end


--- @param id string
--- @return iberbar.Gui.CRenderElement
function m:FindElement( id ) end


--- @param element iberbar.Gui.CRenderElement
--- @return void
function m:AddChildElement( element ) end


--- @return iberbar.Gui.CRenderElement
function m:Clone() end


iberbar.Gui.CRenderElement = m;