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
//  Last Modified : <260619.0624>
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
#include <libb64/cdecode.h>
#include "Revision.h"
#include "LockServo.h"
#include "LockProcess.h"

static DeliveryBoxWebserver server;
DEFINE_SINGLETON_INSTANCE(DeliveryBoxWebserver);

void DeliveryBoxWebserver::_MainScreen()
{
    _processForm();
    send(200,"text/html", 
         header_("Delivery Box") +
         "<h1>Delivery Box</h1>" +
         _page() + 
         footer_());
}

String DeliveryBoxWebserver::_page()
{
    String result;
    if (LockServo::LockServo::IsLocked())
    {
        result += "<h3 style=\"color:green;\">Box is locked</h3>";
    }
    else
    {
        result += "<h3 style=\"color:red;\">Box is unlocked</h3>";
    }
    result += "<form method='post' action='/' >\n";
    result += "</form>\n";
    return result;
}

void DeliveryBoxWebserver::_processForm()
{
}

void DeliveryBoxWebserver::_notFound()
{
    send(404, "text/html", 
         header_("File Not Found") + 
         "<h1>File Not Found</h1>" +
         footer_());
}

void DeliveryBoxWebserver::_sendStyle()
{
    send(200, "text/css", R"stylesheet(
body { 
    background-color: #cccccc;
    font-family: Arial, Helvetica, Sans-Serif;
Color: #000088;
}
)stylesheet");
}

void DeliveryBoxWebserver::_sendJavaScript()
{
    send(200, "text/javascript" , R"javascript(
function dummy()
{
}
)javascript");
}

String DeliveryBoxWebserver::header_(String title)
{    return String(R"html(
<html>
<head>
<title>)html" + title + R"html(</title>
<meta http-equiv="content-language" content="en" />
<meta name="generator" content=")html" PROGRAM_NAME ":" REVISION_GIT_HASH R"html(" />
<link rel="stylesheet" href="/style.css" type="text/css" />
<script type="text/javascript" src="/javascript.js" />
</head>
<body>)html");
}

String DeliveryBoxWebserver::footer_()
{
    return String(R"html(
<footer id="colophon" class="site-footer" role="contentinfo">
<p><img src="/Robot1-110.png" width="63" height="110" alt="Country Robot" style="float:left;" />&copy; 2024 Robert Heller D/B/A Deepwoods Software (The Country Robot)</p>
</footer>
</body>
</html>)html");
}
          
#include "Robot1-110.h"
void DeliveryBoxWebserver::_sendRobot1_110()
{
    uint8_t buffer[base64_decode_expected_len(sizeof(ROBOT1_110))];
    int status = base64_decode_chars(ROBOT1_110,sizeof(ROBOT1_110),(char *)buffer);
    send(200,"image/png",String((const char *)buffer));
}

