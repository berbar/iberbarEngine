
-- CSize = { w = 0, y = 0 };
-- CSize.__index = CSize;
-- function CSize:new( w, h )
--     return setmetatable( { w = w, h = h }, { __index = CSize } );
-- end


CSize = class();
function CSize:ctor( w, h )
    self.w = w or 0;
    self.h = h or 0;
end