
--local socket = require("socket")
require( "Class" );

local m1 = class()
function m1:Destructor()
    print( "m1 destructor" );
end
function m1:Add()
    print( self.a + self.b );
end
function m1:Sub()
    print( self.a - self.b );
end
-- m1.__index = m1;
-- m1.__gc = function () print("gc m1 lalala") end

local m2 = class( m1 );
-- function m2:Destructor()
--     print( "m1 destructor" );
-- end
function m2:Mul()
    print( self.a * self.b );
end
function m2:Div()
    print( self.a / self.b );
end
-- m2.__index = m2
-- m2.__gc = function () print("gc m1 lalala") end

local s1 = m1.new();
s1.a = 4;
s1.b = 7;
s1:Add();
s1:Sub();

local s2 = m2.new();
s2.a = 4;
s2.b = 7;
s2:Add();
s2:Sub();
s2:Mul();
s2:Div();

s2 = nil;

collectgarbage( "collect" );

print( "yeah" );
