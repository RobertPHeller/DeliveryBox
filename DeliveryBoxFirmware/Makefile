##-*- makefile -*-############################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : 2026-06-17 06:24:54
#  Last Modified : <260619.0647>
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


INO = DeliveryBoxFirmware.ino

SOURCES = $(wildcard *.cpp) $(wildcard *.h)

TARGET = Build/$(INO).merged.bin

BOARD = adafruit_feather_esp32s3_nopsram
UPLOADMAX = 4194304
FLASHSIZE = 8MB
PORT  = /dev/ttyUSB0

all: $(TARGET) DOC

$(TARGET): $(INO) $(SOURCES) Revision.h partitions.csv
	-mkdir Build
	arduino --verbose --verify \
		--board esp32:esp32:$(BOARD) \
		--pref upload.maximum_size=$(UPLOADMAX)\
		--pref build.flash_size=$(FLASHSIZE) \
		--pref build.path=Build $(INO)

upload: $(TARGET)
	arduino --upload --board esp32:esp32:$(BOARD) \
		--pref build.path=Build --port $(PORT) \
		--pref upload.maximum_size=$(UPLOADMAX)\
		--pref build.flash_size=$(FLASHSIZE) \
		$(INO)
		
Revision.h: FORCE 
	python3 ./Revision.py -d -t -H -U

DOC: $(TARGET) Doxyfile
	-rm -rf latex html
	doxygen
	$(MAKE) -C latex
	mv latex/refman.pdf ./UserManual.pdf

FORCE:


