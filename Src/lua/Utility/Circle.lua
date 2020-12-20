
CCircle = class();
---ctor
---@param x number
---@param y number
---@param r number
function CCircle:ctor( x, y, r )
    self.x = x;
    self.y = y;
    self.r = r;
end

function CCircle:Diameter()
    return self.r * 2;
end

function CCircle:GetBounding()
    return CRect.new( self.x - self.r, self.y - self.r, self.x + self.r, self.y + self.r );
end
