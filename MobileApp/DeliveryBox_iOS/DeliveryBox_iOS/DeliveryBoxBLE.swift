//
// DeliveryBoxBLE.swift
//  DeliveryBox_iOS
//
//  Created by Robert Heller on 7/13/26.
//

import CoreBluetooth



class DeliveryBoxBLE: NSObject, CBCentralManagerDelegate {
    private var centralManager: CBCentralManager!
    private let ServiceUUID = CBUUID(string: "3a6089ff-731f-4312-9761-6ecfa14b867e")
    private let WIFICharacteristicUUID = CBUUID(string: "00e19a26-da6e-4ad2-896c-b5f0cbe04e43")
    private var WIFICharacteristic: CBCharacteristic!
    private let MasterCodeCharacteristicUUID = CBUUID(string: "fb90b3be-e21c-4aca-ae29-c05a0c3992e3")
    private var MasterCodeCharacteristic: CBCharacteristic!
    private let OneTimeCodeCharacteristicUUID = CBUUID(string: "3d32154a-50f1-4e7d-883e-2b95ad680d3f")
    private var OneTimeCodeCharacteristic: CBCharacteristic!
    private let RestartCharacteristicUUID = CBUUID(string: "2f1c6d70-9d97-47d7-8c9b-cb0b96bddea6")
    private var RestartCharacteristic: CBCharacteristic!
    private var peripheral_: CBPeripheral!
    private var connected = false
    
    override init() {
       super.init()
       centralManager = CBCentralManager(delegate: self, queue: nil)
    }
                        
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        if central.state == .poweredOn {
          // Start scanning for devices
          centralManager.scanForPeripherals(
            withServices: [ServiceUUID], 
            options: nil)
        }
    }
                                                
    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String : Any], rssi RSSI: NSNumber) {
        print("Discovered \(peripheral.name ?? "a device")")
        centralManager.stopScan()
        centralManager.connect(peripheral, options: nil)
    }
    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        print("Connected to \(peripheral.name ?? "a device")")
        //peripheral.delegate = self
        peripheral.discoverServices([ServiceUUID])
        connected = true
        peripheral_ = peripheral
    }
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        for service in peripheral.services! {
           peripheral.discoverCharacteristics(nil, for: service)
        }
    }
    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
        for characteristic in service.characteristics! {
           if characteristic.uuid == WIFICharacteristicUUID {
             WIFICharacteristic = characteristic
           }
           else if characteristic.uuid == MasterCodeCharacteristicUUID {
             MasterCodeCharacteristic = characteristic
           }
           else if characteristic.uuid == OneTimeCodeCharacteristicUUID {
             OneTimeCodeCharacteristic = characteristic
           }
           else if characteristic.uuid == RestartCharacteristicUUID {
             RestartCharacteristic = characteristic 
           } 
           print(characteristic)
        }
    }            
    func SendWiFiConfigure(ssid: String,password: String) {
        if !connected {return}
        print("Sending WiFi Config: '\(ssid)' '\(password)'")
        let data: Data = Data((ssid + "\n" + password).utf8)
        peripheral_.writeValue(data, for: WIFICharacteristic, 
                                type: .withoutResponse)
    }
    func SendMasterCode(masterCode: String) {
        if !connected {return}
        print("Sending Master Code: '\(masterCode)'")
        let data: Data = Data(masterCode.utf8)
        peripheral_.writeValue(data, for: MasterCodeCharacteristic,
                                type: .withoutResponse)        
    }
    func SendOneTimeCode(oneTimeCode: String) {
        if !connected {return}
        print("Sending One Time Code: '\(oneTimeCode)'")
        let data: Data = Data(oneTimeCode.utf8)
        peripheral_.writeValue(data, for: OneTimeCodeCharacteristic,
                                type: .withoutResponse)
    }
    func SendReboot() {
        if !connected {return}
        print("Sending Reboot")
        let data: Data = Data("REBOOT".utf8)
        peripheral_.writeValue(data, for: RestartCharacteristic,
                                type: .withoutResponse)
    }
}
