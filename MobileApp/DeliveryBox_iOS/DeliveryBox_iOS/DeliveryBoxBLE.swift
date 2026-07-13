//
// DeliveryBoxBLE.swift
//  DeliveryBox_iOS
//
//  Created by Robert Heller on 7/13/26.
//

import CoreBluetooth



class DeliveryBoxBLE: NSObject, CBCentralManagerDelegate {
    private var centralManager: CBCentralManager!
    private var ServiceUUID = CBUUID(string: "3a6089ff-731f-4312-9761-6ecfa14b867e")
    private var WIFICharacteristicUUID = CBUUID(string: "00e19a26-da6e-4ad2-896c-b5f0cbe04e43")
    private var MasterCodeCharacteristicUUID = CBUUID(string: "fb90b3be-e21c-4aca-ae29-c05a0c3992e3")
    private var OneTimeCodeCharacteristicUUID = CBUUID(string: "3d32154a-50f1-4e7d-883e-2b95ad680d3f")
    private var RestartCharacteristicUUID = CBUUID(string: "2f1c6d70-9d97-47d7-8c9b-cb0b96bddea6")
    
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
    func SendWiFiConfigure(ssid: String,password: String) {
        print("Sending WiFi Config: '\(ssid)' '\(password)'")
    }
    func SendMasterCode(masterCode: String) {
        print("Sending Master Code: '\(masterCode)'")
    }
    func SendOneTimeCode(oneTimeCode: String) {
        print("Sending One Time Code: '\(oneTimeCode)'")
    }
    func SendReboot() {
        print("Sending Reboot")
    }
}
