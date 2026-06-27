// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : 2026-06-17 16:08:41
//  Last Modified : <260627.1126>
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
/// @file Networking.cpp
/// @author Robert Heller
/// @date 2026-06-17 16:08:41
/// 
///
//////////////////////////////////////////////////////////////////////////////

static const char rcsid[] = "@(#) : $Id$";

#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>
#if defined(CONFIG_BT_ENABLED) && defined(CONFIG_BLUEDROID_ENABLED)
#include <BluetoothSerial.h>
#else
#include "Bluetooth_UART.h"
#endif
#include <ESPmDNS.h>
#include "Networking.h"
#include "DeliveryBoxWebserver.h"
#include <FS.h>
#include <SPIFFS.h>

#if defined(CONFIG_BT_ENABLED) && defined(CONFIG_BLUEDROID_ENABLED)
BluetoothSerial SerialBT;
#else
Bluetooth_UART SerialBT;
#endif

#define HOSTNAME "deliverybox"

namespace Networking {

static char ssid[64] = "";
static char passPhrase[128] = "";


void Initialize()
{

    // allow to address the device by the given name e.g. http://deliverybox
    WiFi.setHostname(HOSTNAME);
    File fp = SPIFFS.open("/secrets.txt");
    if (fp)
    {
        rgbLedWrite(RGB_BUILTIN,255,255,0);
        int l = fp.readBytesUntil('\r',ssid,sizeof(ssid));
        ssid[l] = '\0';
        if (fp.peek() == '\n') fp.read();
        l = fp.readBytesUntil('\r',passPhrase,sizeof(passPhrase));
        passPhrase[l] = '\0';
        fp.close();
    }
    else if (SerialBT.begin(HOSTNAME))
    {
        SerialBT.printf("SSID: ");SerialBT.flush();
        int l = SerialBT.readBytesUntil('\r',ssid,sizeof(ssid));
        ssid[l] = '\0';
        SerialBT.printf("Password: "); SerialBT.flush();
        l = SerialBT.readBytesUntil('\r',passPhrase,sizeof(passPhrase)); 
        passPhrase[l] = '\0';
        SerialBT.printf("\nOK\n");
        fp = SPIFFS.open("/secrets.txt", FILE_WRITE);
        fp.println(ssid);
        fp.println(passPhrase);
        fp.close();
        SerialBT.printf("/secrets.txt written.");
    }
    else
    {
        rgbLedWrite(RGB_BUILTIN,0,255,255);
        Serial.printf("/secrets.txt not found!, enter SSID and passPhrase\n");
        Serial.printf("SSID: ");Serial.flush(); 
        int l = Serial.readBytesUntil('\r',ssid,sizeof(ssid));
        ssid[l] = '\0';
        Serial.printf("\npassPhrase: ");Serial.flush();
        l = Serial.readBytesUntil('\r',passPhrase,sizeof(passPhrase));
        passPhrase[l] = '\0';
        Serial.printf("\nOK\n");
        fp = SPIFFS.open("/secrets.txt", FILE_WRITE);
        fp.println(ssid);
        fp.println(passPhrase);
        fp.close();
        Serial.printf("/secrets.txt written.");
    }
    // start WiFI
    WiFi.mode(WIFI_STA);
    if (strlen(ssid) == 0) {
        WiFi.begin();
    } else {
        WiFi.begin(ssid, passPhrase);
    }
    int count = 100;
    Serial.printf("Connect to WiFi...\n");
    while (WiFi.status() != WL_CONNECTED && count > 0) {
        delay(500);
        Serial.printf(".");
        count--;
    }
    if (count <= 0)
    {
        Serial.printf("not ");
    }
    Serial.printf("connected.\n");
    rgbLedWrite(RGB_BUILTIN,0,128,0);
    DeliveryBoxWebserver::StartServer();
    if (MDNS.begin(HOSTNAME))
    {
        MDNS.addService("http", "tcp", 80);
    }
}

}
