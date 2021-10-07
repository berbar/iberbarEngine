
iberbar.Gui = iberbar.Gui or {};

---@class iberbar.Gui.CButton : iberbar.Gui.CWidget
local CButton = {};


---@return iberbar.Gui.CButton
function CButton.new() end


--- 设置快捷键
---@param keycode number
function CButton:SetHotKey( keycode ) end

iberbar.Gui.CButton = CButton;