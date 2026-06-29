// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : 2026-06-27 11:27:09
//  Last Modified : <260627.1949>
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
/// @file Bluetooth_UART.h
/// @author Robert Heller
/// @date 2026-06-27 11:27:09
/// 
///
//////////////////////////////////////////////////////////////////////////////

#ifndef __BLUETOOTH_UART_H
#define __BLUETOOTH_UART_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Stream.h>
#include <deque>

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"  // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"


class Bluetooth_UART : public Stream
{
public:
    Bluetooth_UART()
          : pServer(NULL), 
    deviceConnected(false), 
    oldDeviceConnected(false)
    {
    }
    bool begin(const char* name);
    bool DeviceConnected() const {return deviceConnected;}
    bool OldDeviceConnected() const {return oldDeviceConnected;}
    int available(void);
    int peek(void);
    int read(void);
    size_t readBytes(char *buffer, size_t length);  // read chars from stream into buffer
    size_t write(uint8_t c);
    size_t write(const uint8_t *buffer, size_t size);
    void flush();
private:
    constexpr static size_t BUFFER_SIZE = 2048;
    uint8_t buffer[BUFFER_SIZE];
    BLEServer *pServer;
    BLECharacteristic *pTxCharacteristic;
    BLECharacteristic *pRxCharacteristic;
    bool deviceConnected;
    bool oldDeviceConnected;
    template <class T> class FIFO : public std::deque<T>
    {
    public:
        void Send(T item)
        {
            this->push_back(item);
        }
        int Receive(T *item)
        {
            if (this->size() > 0)
            {
                *item = this->front(); this->pop_front();
                return 1;
            }
            else
            {
                return 0;
            }
        }
        int Peek(T *item) const
        {
            if (this->size() > 0)
            {
                *item = this->front();
                return 1;
            }
            else
            {
                return 0;
            }
        }
        size_t Available() const
        {
            return this->size();
        }
    };
    FIFO<uint8_t> rx_fifo;
    FIFO<uint8_t> tx_fifo;
    class UARTServerCallbacks : public BLEServerCallbacks {
    public:
        UARTServerCallbacks(Bluetooth_UART *parent)
                    : parent_(parent)
        {
        }
    private:
        Bluetooth_UART *parent_;
        void onConnect(BLEServer *pServer) {
            parent_->deviceConnected = true;
            //Serial.println("Device connected");
        };
        
        void onDisconnect(BLEServer *pServer) {
            parent_->deviceConnected = false;
            //Serial.println("Device disconnected");
        }
    };
    
    class UARTCallbacks : public BLECharacteristicCallbacks {
    public:
        UARTCallbacks(Bluetooth_UART *parent)
                    : parent_(parent)
        {
        }
    private:
        Bluetooth_UART *parent_;
        void onWrite(BLECharacteristic *pCharacteristic) {
            size_t rxLength = pCharacteristic->getLength();
            uint8_t *rxValue = pCharacteristic->getData();
            
            while (rxLength-- > 0)
            {
                parent_->rx_fifo.Send(*rxValue++);
            }
        }
    };
    
};

#endif // __BLUETOOTH_UART_H
