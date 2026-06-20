// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : 2026-06-18 16:37:26
//  Last Modified : <260619.1342>
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
/// @file Keypad4x3.h
/// @author Robert Heller
/// @date 2026-06-18 16:37:26
/// 
///
//////////////////////////////////////////////////////////////////////////////

#ifndef __KEYPAD4X3_H
#define __KEYPAD4X3_H

#include <Arduino.h>
#include <Adafruit_Keypad.h>
#include "Singleton.h"

namespace Keypad4x3 {

/** Specialized 4x3 keypad.
 * See the documentation for the Adafruit_Keypad class for actual API.
 */
class Keypad4x3 : public Adafruit_Keypad, public Singleton<Keypad4x3>
{
public:
    /** Constructor. */
    Keypad4x3();
    /** Initializer */
    static void Initialize()
    {
        instance()->begin();
    }
private:
    
};

}


#endif // __KEYPAD4X3_H
