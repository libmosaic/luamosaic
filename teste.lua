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

print 'agora um SubMOSAIC'

sub = mos:Sub (5, 5, 3, 3)

sub:SetCh (3, 3, 'c')

for i = 1, sub:GetHeight () do
	for j = 1, sub:GetWidth () do
		color.Tcolor (sub:GetAttr (i, j))
		io.write (sub:GetCh (i, j))
	end
	color.Tcolor (color.Normal)
	print ()
end

---------------------------------------------------

for i = 1, mos:GetHeight () do
	for j = 1, mos:GetWidth () do
		color.Tcolor (mos:GetAttr (i, j))
		io.write (mos:GetCh (i, j))
	end
	color.Tcolor (color.Normal)
	print ()
end

