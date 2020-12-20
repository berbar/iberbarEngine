

local _class={}


		-- Do destructor recursively.
        -- local ReleaseObj = function (Class, Object)
        --     print( "release" );
            local function Release(c, o)
                print( "release" );
                print( c.Destructor );
                if c.Destructor then
                    c.Destructor(o)
                end
 
                if c.super then
                    Release(c.super, o )
                end
            end
 
            --Release(Class)
        -- end

function class(super)
	local class_type={}
    class_type.ctor=false
    class_type.Destructor = false;
	class_type.super=super
	class_type.new=function(...)
		local obj={}
		do
			local create
			create = function(c,...)
				if c.super then
					create(c.super,...)
				end
				if c.ctor then
					c.ctor(obj,...)
				end
			end

			create(class_type,...)
		end

		setmetatable(obj,{ __index=_class[class_type], __gc=function (o)
            Release(class_type, o)
            print( "__gc" );
		end })
		return obj
	end
	local vtbl={}
	_class[class_type]=vtbl

	setmetatable(class_type,{__newindex=
		function(t,k,v)
			vtbl[k]=v
        end
	})

	if super then
		setmetatable(vtbl,{__index=
			function(t,k)
				local ret=_class[super][k]
				vtbl[k]=ret
				return ret
			end
		})
	end

	return class_type
end




