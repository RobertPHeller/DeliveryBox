// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : 2026-06-18 14:25:55
//  Last Modified : <260618.1448>
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
/// @file LockServo.cpp
/// @author Robert Heller
/// @date 2026-06-18 14:25:55
/// 
///
//////////////////////////////////////////////////////////////////////////////

static const char rcsid[] = "@(#) : $Id$";


#include <Arduino.h>
#include <ESP32Servo.h> 
#include "GPIO_MAP.h" 
#include "LockServo.h"

namespace LockServo { 

static LockServo servo;
DEFINE_SINGLETON_INSTANCE(LockServo);

void LockServo::_begin()
{
    // Allow allocation of all timers
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    setPeriodHertz(50);    // standard 50 hz servo
    attach(LOCK_SERVO, 1000, 2000); // attaches the servo on pin 18 to the servo object
    // using default min/max of 1000us and 2000us
    // different servos may require different min/max settings
    // for an accurate 0 to 180 sweep
    _unlock();
}

void LockServo::_lock()
{
    write(LockedPosition);
    _locked = true;
}

void LockServo::_unlock()
{
    write(UnLockedPosition);
    _locked = false;
}


}
