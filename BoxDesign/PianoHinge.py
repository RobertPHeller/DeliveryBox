#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Thu Jun 4 19:26:46 2020
#  Last Modified : <260620.1957>
#
#  Description	
#
#  Notes
#
#  History
#	
#*****************************************************************************
#
#    Copyright (C) 2020  Robert Heller D/B/A Deepwoods Software
#			51 Locke Hill Road
#			Wendell, MA 01379-9728
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# 
#
#*****************************************************************************


import Part
from FreeCAD import Base
import FreeCAD as App

from abc import ABCMeta, abstractmethod, abstractproperty

class PianoHinge_(object):
    __metaclass__ = ABCMeta
    _Length = 18*25.4
    @classmethod
    def Length(cls):
        return cls._Length
    _Thick  = (1/16)*25.4
    _FoldHeight = 1.14*25.4
    _FlangeWidth = 25.4
    @classmethod
    def FlangeWidth(cls):
        return cls._FlangeWidth
    _PinDia = .14*25.4
    @classmethod
    def PinDia(cls):
        return cls._PinDia
    _PinOff = (1.14*25.4)-(.14*25.4)-25.4
    _PinFlangeL = 12.7
    _HoleDia = 4.75
    _1stHoleOff = .2*25.4
    _Holespace  = 2.2*25.4
    _HoleCount = 9
    _HoleSideOff = .5*25.4
    _Color = (.9,.9,.9)
    @classmethod
    def Color(cls):
        return cls._Color
    def _flange(self,origin,startConn=True,connectAbove=True):
        YNorm = Base.Vector(0,1,0)
        Thick = Base.Vector(0,self._Thick,0)
        base = Part.makePlane(self._FlangeWidth,self._Length,
                              origin,YNorm).extrude(Thick)
        if connectAbove:
            zoff = self._FlangeWidth
        else:
            zoff = -(self._PinOff+(self._PinDia/2.0))
        if startConn:
            xoff = 0
        else:
            xoff = self._PinFlangeL
        while xoff+self._PinFlangeL <= self._Length:
            o = origin.add(Base.Vector(xoff,0,zoff))
            conn = Part.makePlane(self._PinOff+(self._PinDia/2.0),
                                  self._PinFlangeL,
                                  o,YNorm).extrude(Thick)
            base = base.fuse(conn)
            xoff += self._PinFlangeL*2
        holex = self._1stHoleOff
        holeRad = self._HoleDia/2.0
        holes = list()
        while holex < self._Length:
            ho = origin.add(Base.Vector(holex,0,self._HoleSideOff))
            holes.append(ho)
            hole = Part.Face(Part.Wire(Part.makeCircle(holeRad,ho,YNorm))
                            ).extrude(Thick)
            base = base.cut(hole)
            holex += self._Holespace
        return (base,holes)
            
class PianoHingeFlatOutsideBack(PianoHinge_):
    def __init__(self,name,origin):
        self.name = name
        if not isinstance(origin,Base.Vector):
            raise RuntimeError("origin is not a Vector!")
        self.origin = origin
        flange1,holes1 = self._flange(origin,startConn=False,connectAbove=True)
        pinorig = origin.add(Base.Vector(0,
                                         self._PinDia/2.0,
                                 self._FlangeWidth+self._PinOff+self._PinDia/2.0))
        XNorm = Base.Vector(1,0,0)
        pinL  = Base.Vector(self._Length,0,0)
        pin = Part.Face(Part.Wire(Part.makeCircle(self._PinDia/2.0,
                                                 pinorig,
                                                 XNorm))).extrude(pinL)
        flange2o = origin.add(Base.Vector(0,0,
                        self._FlangeWidth+(self._PinOff*2.0)+self._PinDia))
        flange2,holes2 = self._flange(flange2o,
                                      startConn=True,connectAbove=False)
        self.hinge = flange1.fuse(pin.fuse(flange2))
        self.holes = dict()
        hi = 1
        for h in holes1:
            self.holes[1,hi] = h
            hi += 1
        hi = 1
        for h in holes2:
            self.holes[2,hi] = h
            hi += 1
    def show(self,doc=None):
        if doc==None:
            doc = App.activeDocument()
        obj = doc.addObject("Part::Feature",self.name)
        obj.Shape = self.hinge
        obj.Label=self.name
        obj.ViewObject.ShapeColor=tuple([190/255.0,190/255.0,190/255.0])
    def MountingHole(self,f,h,baseY,height):
        mh = self.holes[f,h]
        mh = Base.Vector(mh.x,baseY,mh.z)
        return Part.Face(Part.Wire(Part.makeCircle(self._HoleDia/2.0,
                                   mh,Base.Vector(0,1,0)))
                        ).extrude(Base.Vector(0,height,0))


class PianoHingeFlatInsideClosedFront(PianoHinge_):
    def __init__(self,name,origin):
        self.name = name
        if not isinstance(origin,Base.Vector):
            raise RuntimeError("origin is not a Vector!")
        self.origin = origin
        flange1,holes1 = self._flange(origin,startConn=False,connectAbove=True)
        pinorig = origin.add(Base.Vector(0,
                                         self._PinDia/2.0,
                                 self._FlangeWidth+self._PinOff+self._PinDia/2.0))
        XNorm = Base.Vector(1,0,0)
        pinL  = Base.Vector(self._Length,0,0)
        pin = Part.Face(Part.Wire(Part.makeCircle(self._PinDia/2.0,
                                                 pinorig,
                                                 XNorm))).extrude(pinL)
        flange2o = origin.add(Base.Vector(0,self._PinDia-self._Thick,0))
        flange2,holes2 = self._flange(flange2o,
                                      startConn=True,connectAbove=True)
        self.hinge = flange1.fuse(pin.fuse(flange2))
        self.holes = dict()
        hi = 1
        for h in holes1:
            self.holes[1,hi] = h
            hi += 1
        hi = 1
        for h in holes2:
            self.holes[2,hi] = h
            hi += 1
    def show(self,doc=None):
        if doc==None:
            doc = App.activeDocument()
        obj = doc.addObject("Part::Feature",self.name)
        obj.Shape = self.hinge
        obj.Label=self.name
        obj.ViewObject.ShapeColor=tuple([190/255.0,190/255.0,190/255.0])
    def MountingHole(self,f,h,baseY,height):
        mh = self.holes[f,h]
        mh = Base.Vector(mh.x,baseY,mh.z)
        return Part.Face(Part.Wire(Part.makeCircle(self._HoleDia/2.0,
                                   mh,Base.Vector(0,1,0)))
                        ).extrude(Base.Vector(0,height,0))




if __name__ == '__main__':
    App.ActiveDocument=App.newDocument("testhinge")
    hinge = PianoHingeFlatOutsideBack_10inches("testhinge",Base.Vector(0,0,0))
    hinge.show()
