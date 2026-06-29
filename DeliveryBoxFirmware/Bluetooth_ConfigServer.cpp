// -!- C++ -!- //////////////////////////////////////////////////////////////
// 
//  System : Module : Object Name : $RCSfile$ Revision : $Revision$ Date : $Date$ Author : $Author$
//  Created By : Robert Heller Created : 2026-06-28 12:40:10 Last Modified : <260629.1418>
// 
//  Description
// 
//  Notes
// 
//  History
// 
///////////////////////////////////////////////////////////////////////////////////////////////////
// / @copyright
// / Copyright (C) 2026 Robert Heller D/B/A Deepwoods Software
// / 51 Locke Hill Road
// / Wendell, MA 01379-9728
// /
// / This program is free software; you can redistribute it and/or modify
// / it under the terms of the GNU General Public License as published by
// / the Free Software Foundation; either version 2 of the License, or
// / (at your option) any later version.
// /
// / This program is distributed in the hope that it will be useful,
// / but WITHOUT ANY WARRANTY; without even the implied warranty of
// / MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// / GNU General Public License for more details.
// /
// / You should have received a copy of the GNU General Public License
// / along with this program; if not, write to the Free Software
// / Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
// / @file Bluetooth_ConfigServer.cpp
// / @author Robert Heller
// / @date 2026-06-28 12:40:10
// /
// /
///////////////////////////////////////////////////////////////////////////////////////////////////

static const char rcsid[] = "@(#) : $Id$";


#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLESecurity.h>
#include "Singleton.h"
#include "BackgroundTask.h"
#include "Bluetooth_ConfigServer.h"
#include <SPIFFS.h>
#include <FS.h>
#include "GeneratedPassKey.h"

static Bluetooth_ConfigServer server;
DEFINE_SINGLETON_INSTANCE(Bluetooth_ConfigServer);

void Bluetooth_ConfigServer::_startServer()
{
    BLEDevice::init("Delivery Box");
    pSecurity_ = new BLESecurity();
    pSecurity_->setPassKey(true, PASSKEY);
    pSecurity_->setCapability(ESP_IO_CAP_OUT);
    pSecurity_->setAuthenticationMode(true, true, true);
    BLEDevice::setSecurityCallbacks(new SecurityCallbacks());
    pServer_ = BLEDevice::createServer();
    pServer_->advertiseOnDisconnect(true); 
    pService_ = pServer_->createService(SERVICE_UUID);
    uint32_t secure_properties = BLECharacteristic::PROPERTY_READ | 
          BLECharacteristic::PROPERTY_WRITE |
          BLECharacteristic::PROPERTY_READ_AUTHEN | 
          BLECharacteristic::PROPERTY_WRITE_AUTHEN;
    pWiFiCharacteristic_ = pService_->createCharacteristic(WIFI_CHARACTERISTIC_UUID,
                                                           secure_properties);
    pWiFiCharacteristic_->setCallbacks(new WiFiCharacteristicCallbacks());
#ifdef CONFIG_BLUEDROID_ENABLED
    pWiFiCharacteristic_->addDescriptor(new BLE2902());
#endif
    pWiFiCharacteristic_->setValue("Set WiFi SSID & passPhrase: ");

    pMasterCodeCharacteristic_ = pService_->createCharacteristic(MASTERCODE_CHARACTERISTIC_UUID,
                                                           secure_properties);
    pMasterCodeCharacteristic_->setCallbacks(new MasterCodeCharacteristicCallbacks());
#ifdef CONFIG_BLUEDROID_ENABLED
    pMasterCodeCharacteristic_->addDescriptor(new BLE2902());
#endif
    pMasterCodeCharacteristic_->setValue("Set Master Code: ");

    pOneTimeCodeCharacteristic_ = pService_->createCharacteristic(ONETIMECODE_CHARACTERISTIC_UUID,
                                                           secure_properties);
    pOneTimeCodeCharacteristic_->setCallbacks(new OneTimeCodeCharacteristicCallbacks());
#ifdef CONFIG_BLUEDROID_ENABLED
    pOneTimeCodeCharacteristic_->addDescriptor(new BLE2902());
#endif
    pOneTimeCodeCharacteristic_->setValue("Add a One Time Code: ");
    
    pRestartCharacteristic_ = pService_->createCharacteristic(RESTART_CHARACTERISTIC_UUID,
                                                           secure_properties);
    pRestartCharacteristic_->setCallbacks(new RestartCharacteristicCallbacks());
#ifdef CONFIG_BLUEDROID_ENABLED
    pRestartCharacteristic_->addDescriptor(new BLE2902());
#endif
    pRestartCharacteristic_->setValue("Reboot? ");
    
    pService_->start();
    pAdvertising_ = BLEDevice::getAdvertising();
    pAdvertising_->addServiceUUID(SERVICE_UUID);
    pAdvertising_->setScanResponse(true);
    pAdvertising_->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising_->setMaxPreferred(0x12);
    BLEDevice::startAdvertising();
}

