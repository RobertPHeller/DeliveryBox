// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : 2026-06-28 12:32:21
//  Last Modified : <260629.2046>
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
/// @file Bluetooth_ConfigServer.h
/// @author Robert Heller
/// @date 2026-06-28 12:32:21
/// 
///
//////////////////////////////////////////////////////////////////////////////

#ifndef __BLUETOOTH_CONFIGSERVER_H
#define __BLUETOOTH_CONFIGSERVER_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLESecurity.h>
#include "Singleton.h"
#include "LockProcess.h"
#include <SPIFFS.h>
#include <FS.h>
#include <stdio.h>
#include "Display.h"


#define SERVICE_UUID "3a6089ff-731f-4312-9761-6ecfa14b867e"
#define WIFI_CHARACTERISTIC_UUID "00e19a26-da6e-4ad2-896c-b5f0cbe04e43"
#define MASTERCODE_CHARACTERISTIC_UUID "fb90b3be-e21c-4aca-ae29-c05a0c3992e3"
#define ONETIMECODE_CHARACTERISTIC_UUID "3d32154a-50f1-4e7d-883e-2b95ad680d3f"
#define RESTART_CHARACTERISTIC_UUID "2f1c6d70-9d97-47d7-8c9b-cb0b96bddea6"

class Bluetooth_ConfigServer : public Singleton<Bluetooth_ConfigServer>
{
public:
    /** Constructor. */
    Bluetooth_ConfigServer() 
          : pServer_(NULL)
    , pSecurity_(NULL)
    , pService_(NULL)
    , pAdvertising_(NULL)
    , pWiFiCharacteristic_(NULL)
    , pMasterCodeCharacteristic_(NULL)
    , pOneTimeCodeCharacteristic_(NULL)
    , pRestartCharacteristic_(NULL)
    , deviceConnected(false)
    {
    }
    /** Start the server. */
    static void StartServer()
    {
        instance()->_startServer();
    }
    static bool IsConnected()
    {
        return instance()->deviceConnected;
    }
private:
    class SecurityCallbacks : public BLESecurityCallbacks {
#if defined(CONFIG_BLUEDROID_ENABLED)  
        void onAuthenticationComplete(esp_ble_auth_cmpl_t desc) override {
            LockProcess::LockProcess::DisplayCurrentLockState();
        }
#endif
#if defined(CONFIG_NIMBLE_ENABLED)
        void onAuthenticationComplete(ble_gap_conn_desc *desc) override {
            LockProcess::LockProcess::DisplayCurrentLockState();
        }
#endif
        void onPassKeyNotify(uint32_t pass_key)
        {
            char buffer[10];
            snprintf(buffer,sizeof(buffer),"%u",pass_key);
            Display::Display::ClearDisplay();
            for (char *p=buffer; *p != '\0'; p++)
            {
                Display::Display::WriteCharacter(*p);
            }
        }
    };
    class WiFiCharacteristicCallbacks : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic* pWiFiCharacteristic) {
            String value = pWiFiCharacteristic->getValue();
            String ssid, passPhrase;
            int nl = value.indexOf('\n');
            if (nl < 0)
            {
                ssid = value;
                passPhrase = "";
            }
            else
            {
                ssid = value.substring(0,nl-1);
                passPhrase = value.substring(nl+1);
            }
        }
    };
    class MasterCodeCharacteristicCallbacks : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic* pMasterCodeCharacteristic) {
            String value = pMasterCodeCharacteristic->getValue();
        }
    };
    class OneTimeCodeCharacteristicCallbacks : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic* pOneTimeCodeCharacteristic) {
            String value = pOneTimeCodeCharacteristic->getValue();
        }
    };
    class RestartCharacteristicCallbacks : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic* pRestartCharacteristic) {
            String value = pRestartCharacteristic->getValue();
            if (value == "REBOOT")
            {
                EspClass system;
                system.restart();
            }
        }
    };
    class ServerCallbacks: public BLEServerCallbacks {
    public:
        ServerCallbacks(Bluetooth_ConfigServer *parent) 
                    : parent_(parent)
        {
        }
    private:
        Bluetooth_ConfigServer *parent_;
        void onConnect(BLEServer* pServer) {
            parent_->deviceConnected = true;
        }
        void onDisconnect(BLEServer* pServer) {
            parent_->deviceConnected = false;
        }
    };
    void _startServer();
    BLEServer* pServer_;
    BLESecurity *pSecurity_;
    BLEService *pService_;
    BLEAdvertising *pAdvertising_;
    BLECharacteristic* pWiFiCharacteristic_;
    BLECharacteristic* pMasterCodeCharacteristic_;
    BLECharacteristic* pOneTimeCodeCharacteristic_;
    BLECharacteristic* pRestartCharacteristic_;
    bool deviceConnected;
};

#endif // __BLUETOOTH_CONFIGSERVER_H
