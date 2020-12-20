
-- CPoint = { x = 0, y = 0 };
-- CPoint.__index = CPoint;
-- function CPoint:new( x, y )
--     return setmetatable({ x = x, y = y }, { __index = CPoint } );
-- end

CPoint = class();

function CPoint:ctor( x, y )
    self.x = x or 0;
    self.y = y or 0;
end
