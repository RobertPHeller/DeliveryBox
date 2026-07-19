##-*- makefile -*-############################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : 2026-07-19 14:38:36
#  Last Modified : <260719.1450>
#
#  Description	
#
#  Notes
#
#  History
#	
#  $Log$
#
##############################################################################
#
#    Copyright (C) 2026  Robert Heller D/B/A Deepwoods Software
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
##############################################################################


INO = AutoSolarLights.ino

SOURCES = 

TARGET = Build/$(INO).merged.bin

BOARD = adafruit:avr:metro

PORT = /dev/ttyUSB0

$(TARGET): $(INO) $(SOURCES)
	-mkdir Build
	arduino --verbose --verify \
		--pref build.path=Build \
		--board $(BOARD) \
		$(INO)
		

upload: $(TARGET)
	arduino --verbose --upload \
		--board $(BOARD) --port $(PORT) \
		--pref build.path=Build \
		$(INO)
