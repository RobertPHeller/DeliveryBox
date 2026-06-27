// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : 2026-06-17 06:23:17
//  Last Modified : <260627.0904>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
/// @file DeliveryBoxFirmware.ino
/// @brief Main program.
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
///
/// @author Robert Heller
// 
//
//////////////////////////////////////////////////////////////////////////////

/** @mainpage Delivery Box Firmware 
 * A lockable box for delivery prople to deliver stuff to.
 * 
 * Target dev board: Adafruit Feather ESP32S3, No Psram version (8M flash).
 * 
 * This firmware manages the lock via a servo.  There is a numeric keypad and
 * an eight character 14-segment display.
 * 
 * The lock uses 8-digit codes to unlock the box.  There is one master code 
 * and any number of one-time codes.  The one time codes are given to delivery
 * drivers when a delivery is scheduled.  Once the driver uses the code to 
 * deliver the parcel, the code is removed and can't be used again.
 */

static const char rcsid[] = "@(#) : $Id$";

#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>
#include "BluetoothSerial.h"
#include <ESPmDNS.h>
#include <Adafruit_Keypad.h>
#include <Adafruit_LEDBackpack.h>
#include <ESP32Servo.h>
#include "GPIO_MAP.h"
#include "Networking.h"
#include "DeliveryBoxWebserver.h"
#include <FS.h>
#include <SPIFFS.h>
#include "LockServo.h"
#include "Display.h"
#include "Keypad4x3.h"
#include "LockProcess.h"

void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.path(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}


/** @brief Intialize everything.
 * - Start the serial port
 * - Mount the SPI File System
 * @returns nothing.
 */
void setup() {
    // put your setup code here, to run once:
    pinMode(BUILTIN_LED,OUTPUT);
    digitalWrite(BUILTIN_LED,HIGH);
    Serial.begin(115200);
    rgbLedWrite(RGB_BUILTIN,0,255,0);
    unsigned char led = 0;
    if(!SPIFFS.begin(true)) {
        Serial.printf("Failed to mount SPIFFS!\n");
        while (1) 
        {
            Serial.printf("!");
            rgbLedWrite(RGB_BUILTIN,led++,0,0);
            delay(100);
        }
    }
    rgbLedWrite(RGB_BUILTIN,0,0,255);
    listDir(SPIFFS,"/",0);
    rgbLedWrite(RGB_BUILTIN,0,0,0);
    Networking::Initialize();
    // Keypad init
    Keypad4x3::Keypad4x3::Initialize();
    // Display init
    Display::Display::InitDisplay();
    // Lock init
    LockServo::LockServo::Initialize();
    LockProcess::LockProcess::Initialize();
    
}

/** @brief Main loop.
 * @returns nothing.
 */
void loop() {
    // put your main code here, to run repeatedly:
    Keypad4x3::Keypad4x3::instance()->tick();
    while (Keypad4x3::Keypad4x3::instance()->available())
    {
        keypadEvent e = Keypad4x3::Keypad4x3::instance()->read();
        if(e.bit.EVENT == KEY_JUST_PRESSED)
        {
            LockProcess::LockProcess::ProcessKey(e.bit.KEY);
        }
    }
    BackgroundTask::RunTasks(10);
}    
