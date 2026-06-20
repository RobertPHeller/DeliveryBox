// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : 2026-06-18 15:30:41
//  Last Modified : <260619.1343>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
/// @copyright
///    Copyright (C) 2026  Robert Heller D/B/A Deepwoods Software
///			51 Locke Hill Road
///			Wendell, MA 01379-9728
///
///    This program is free software; you can redistribute it and/or modify
///    it under the terms of the GNU General Public License as published by
///    the Free Software Foundation; either version 2 of the License, or
///    (at your option) any later version.
///
///    This program is distributed in the hope that it will be useful,
///    but WITHOUT ANY WARRANTY; without even the implied warranty of
///    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
///    GNU General Public License for more details.
///
///    You should have received a copy of the GNU General Public License
///    along with this program; if not, write to the Free Software
///    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
/// @file Display.h
/// @author Robert Heller
/// @date 2026-06-18 15:30:41
/// 
///
//////////////////////////////////////////////////////////////////////////////

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_LEDBackpack.h> 
#include "GPIO_MAP.h"
#include "Singleton.h"

namespace Display {


/** Display digits and characters. 
 * Wrapped around the Adafruit_AlphaNum4 / Adafruit_LEDBackpack class.
 */
class Display : public Adafruit_AlphaNum4, public Singleton<Display>
{
public:
    /** Constructor. */
    Display() {}
    /** Initialize the display. */
    static void InitDisplay() {
        instance()->begin();
    }
    /** Clear the display. */
    static void ClearDisplay() {
        instance()->clear();
        instance()->writeDisplay();
        instance()->_pos = 0;
    }
    /** Write a character to the display.
     * @param ascii Character to write.
     * @param dot Light the decimal point if true.
     */
    static void WriteCharacter(uint8_t ascii, bool dot = false)
    {
        if (instance()->_pos >= 7) return;
        instance()->writeDigitAscii(instance()->_pos++, ascii, dot);
        instance()->writeDisplay();
    }
private:
    uint8_t _pos;
};

}
        

#endif // DISPLAY_H
