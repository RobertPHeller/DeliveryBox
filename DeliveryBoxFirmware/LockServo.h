// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : 2026-06-18 14:23:07
//  Last Modified : <260618.1505>
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
/// @file LockServo.h
/// @author Robert Heller
/// @date 2026-06-18 14:23:07
/// 
///
//////////////////////////////////////////////////////////////////////////////

#ifndef LOCKSERVO_H
#define LOCKSERVO_H

#include <Arduino.h>
#include <ESP32Servo.h> 
#include "GPIO_MAP.h" 
#include "Singleton.h"

namespace LockServo {

/** Class to manage the lock.
 */
class LockServo : public Servo, public Singleton<LockServo>
{
public:
    /** Constructor. */
    LockServo() 
    {
    }
    /** Initialize the lock servo. */
    static void Initialize()
    {
        instance()->_begin();
    }
    /** Close the lock. */
    static void lock()
    {
        instance()->_lock();
    }
    /** Open the lock. */
    static void unlock()
    {
        instance()->_unlock();
    }
    /** Is the lock closed? */
    static bool IsLocked()
    {
        return instance()->_locked;
    }
private:
    void _begin();
    void _lock();
    void _unlock();
    bool _locked;
    static constexpr int LockedPosition = 0;
    static constexpr int UnLockedPosition = 90;
};

}
#endif // LOCKSERVO_H
