local mosaic = require 'mosaic'
local color = mosaic.color

a = mosaic.New (0, 0)
print (getmetatable (a), type (a))
a:Resize (3, 3)

a:SetCh (1, 1, 'casd')
print (a:GetCh (1, 1))

a:SetAttr (1, 1, 3)
print (a:GetAttr (1, 1))

color.Tcolor (color.CN)
print 'oi, galera =]'
color.Tcolor (color.CN, true)
print 'temos cores, tem até negrito!'
color.Tcolor (color.Normal)

-- load!
print 'um moscat simplão'
mos = assert (mosaic.io.Load ("color.mosi"))

for i = 1, mos:GetHeight () do
	for j = 1, mos:GetWidth () do
		io.write (mos:GetCh (i, j))
	end
	io.write '\n'
end
