// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : 2026-06-18 16:47:42
//  Last Modified : <260618.1725>
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
/// @file Keypad4x3.cpp
/// @author Robert Heller
/// @date 2026-06-18 16:47:42
/// 
///
//////////////////////////////////////////////////////////////////////////////

static const char rcsid[] = "@(#) : $Id$";


#include <Arduino.h>
#include <Adafruit_Keypad.h>
#include "Singleton.h"
#include "Keypad4x3.h"
#include "GPIO_MAP.h"

namespace Keypad4x3 {

static constexpr byte ROWS = 4; // rows
static constexpr byte COLS =  3; // columns
//define the symbols on the buttons of the keypads
static char keys[ROWS][COLS] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};
static byte rowPins[ROWS] = KEYPAD_ROWS; //connect to the row pinouts of the keypad
static byte colPins[COLS] = KEYPAD_COLS; //connect to the column pinouts of the keypad


Keypad4x3::Keypad4x3()
          : Adafruit_Keypad (makeKeymap(keys), rowPins, colPins, ROWS, COLS)
    {
    }static Keypad4x3 keypad;
DEFINE_SINGLETON_INSTANCE(Keypad4x3);

}
