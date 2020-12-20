

--- Gui的xml解析器
--- @class iberbar.Gui.CXmlParser
local m = {};


--- 推入widget到栈顶
--- @param widget iberbar.Gui.CWidget
--- @return void
function m:PushWidget( widget ) end


--- 弹出栈顶的widget
--- @return void
function m:PopWidget() end


--- 读取xml文件
--- @param filepath string
--- @return void
function m:ReadFile( filepath ) end


iberbar.Gui.CXmlParser = m;
