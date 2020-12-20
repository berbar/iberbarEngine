
require( "../Utility/Class" );
require( "../Utility/Size" );
require( "../Utility/Rect" );
require( "../Utility/Circle" );


local rect = CRect.new(50,0,100,50);
print( rect.l );
print( rect:Width() );
print( type( rect ) );
local size = rect:Size();
print( size.w, size.h );
local circle = CCircle.new(0,0,30);
local circleBounding = circle:GetBounding();
print( circleBounding:Width() );
