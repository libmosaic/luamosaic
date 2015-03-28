local mosaic = require 'mosaic'

a = mosaic.New (0, 0)
print (getmetatable (a), type (a))
a:Resize (3, 3)

a:SetCh (0, 0, 'casd')
print (a:GetCh (0, 0))

a:SetAttr (0, 0, 3)
print (a:GetAttr (0, 0))

mosaic.color.Tcolor (mosaic.color.CN)
print 'oi, galera =]'
mosaic.color.Tcolor (mosaic.color.Normal)
