##-*- makefile -*-############################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : 2026-06-16 16:07:14
#  Last Modified : <260616.1614>
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


set(deps
    driver
    heap
    nvs_flash
    vfs
    app_update
    spiffs
    json
    esp_timer
    Adafruit_Keypad
)

idf_component_register(SRCS DeliveryBox.cpp
		       REQUIRES "${deps}")

set_source_files_properties(DeliveryBox.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=maybe-uninitialized -Wno-implicit-fallthrough -Wno-ignored-qualifiers")

#add_custom_target(Documentation
#		  ALL
#		  COMMAND make PROJECTNAME="${project_name}" PROJECTNUMBER="${project_ver}" -C "${CMAKE_CURRENT_SOURCE_DIR}/../doc"
#		  VERBATIM
#		  )                    
