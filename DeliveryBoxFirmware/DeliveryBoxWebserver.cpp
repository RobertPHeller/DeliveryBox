// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : 2026-06-17 14:15:08
//  Last Modified : <260617.1425>
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
/// @file DeliveryBoxWebserver.cpp
/// @author Robert Heller
/// @date 2026-06-17 14:15:08
/// 
///
//////////////////////////////////////////////////////////////////////////////

static const char rcsid[] = "@(#) : $Id$";

#include <WebServer.h>
#include "Singleton.h"
#include "DeliveryBoxWebserver.h"

static DeliveryBoxWebserver server;
DEFINE_SINGLETON_INSTANCE(DeliveryBoxWebserver);

void DeliveryBoxWebserver::_MainScreen()
{
}

void DeliveryBoxWebserver::_notFound()
{
}

void DeliveryBoxWebserver::_sendStyle()
{
}

void DeliveryBoxWebserver::_sendJavaScript()
{
}

String DeliveryBoxWebserver::header_(String title)
{
    return String("<HEAD><TITLE>") + title + "</TITLE></HEAD>";
}

String DeliveryBoxWebserver::footer_()
{
    return "";
}
          
