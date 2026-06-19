// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : 2026-06-18 21:08:59
//  Last Modified : <260618.2147>
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
/// @file LockProcess.cpp
/// @author Robert Heller
/// @date 2026-06-18 21:08:59
/// 
///
//////////////////////////////////////////////////////////////////////////////

static const char rcsid[] = "@(#) : $Id$";


#include <Arduino.h>
#include "Display.h"
#include "LockServo.h"
#include "Singleton.h"
#include <FS.h>
#include <SPIFFS.h>
#include <vector>
#include "LockProcess.h"
#include <string.h>

namespace LockProcess {

static LockProcess lockProcessor;
DEFINE_SINGLETON_INSTANCE(LockProcess);


static const char *LOCKED = "LOCKED";
static const char *UNLOCKED = "UNLOCKED";

LockProcess::LockProcess()
{
}

void LockProcess::_begin()
{
    File fp = SPIFFS.open("/master.txt");
    if (fp)
    {
        _MasterCode = fp.readStringUntil('\r');
        fp.close();
    }
    else
    {
        _MasterCode = "99999999"; // default master code.
    }
    fp = SPIFFS.open("/onetimecodes.txt");
    if (fp)
    {
        do
        {
            String code = fp.readStringUntil('\r');
            _OneTimeCodes.push_back(code);
            if (fp.peek() == '\n') fp.read();
        } while(fp);
        fp.close();
    }
    LockServo::LockServo::unlock();
    memset(_buffer,0,sizeof(_buffer));
    _bufferIndex = 0;
    Display::Display::ClearDisplay();
    _message(UNLOCKED);
}

void LockProcess::_message(const char *m)
{
    for (const char *p = m; *p; p++)
    {
        Display::Display::WriteCharacter((uint8_t) *p);
    }
}

void LockProcess::_processKey(uint8_t k)
{
    if (_bufferIndex == 0) Display::Display::ClearDisplay();
    if (_bufferIndex < CODELEN)
    {
        Display::Display::WriteCharacter(k);
        _buffer[_bufferIndex++] = (char) k;
    }
    if (LockServo::LockServo::IsLocked())
    {
        if (_MasterCode == _buffer)
        {
            LockServo::LockServo::unlock();
            memset(_buffer,0,sizeof(_buffer));
            _bufferIndex = 0;
            Display::Display::ClearDisplay(); 
            _message(UNLOCKED);
            return;
        }
        for (auto ic = _OneTimeCodes.begin(); ic != _OneTimeCodes.end(); ic++)
        {
            if (*ic == _buffer)
            {
                _OneTimeCodes.erase(ic);
                LockServo::LockServo::unlock();
                memset(_buffer,0,sizeof(_buffer));
                _bufferIndex = 0;
                Display::Display::ClearDisplay(); 
                _message(UNLOCKED);
                return;
            }
        }
    }
    else
    {
        if (String("***") == _buffer)
        {
            LockServo::LockServo::lock();
            memset(_buffer,0,sizeof(_buffer));
            _bufferIndex = 0;
            Display::Display::ClearDisplay();
            _message(LOCKED);
            return;
        }
    }
}


        
}
