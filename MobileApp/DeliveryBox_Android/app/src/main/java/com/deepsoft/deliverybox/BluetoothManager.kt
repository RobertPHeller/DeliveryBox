/****************************************************************************
 *
 * Copyright (C) 2026 SEE FILE LICENSE.
 *
 * Synopsis:    Bluetooth Manager code
 * Authors:     Robert Heller
 *
 ****************************************************************************/

package com.deepsoft.deliverybox

import java.util.UUID
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothManager
import android.bluetooth.le.ScanFilter
import android.bluetooth.le.ScanSettings
import android.bluetooth.le.ScanCallback
import android.bluetooth.le.ScanResult

import android.content.Context
import android.content.IntentFilter
import android.os.Build
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import android.os.Handler
import android.os.ParcelUuid
import android.os.Looper



class BluetoothConnectionManager(private val context: Context) {
    private val ServiceUUID = UUID.fromString("3a6089ff-731f-4312-9761-6ecfa14b867e")
    private val WIFICharacteristicUUID = UUID.fromString("00e19a26-da6e-4ad2-896c-b5f0cbe04e43")
    private val MasterCodeCharacteristicUUID = UUID.fromString("fb90b3be-e21c-4aca-ae29-c05a0c3992e3")
    private val OneTimeCodeCharacteristicUUID = UUID.fromString("3d32154a-50f1-4e7d-883e-2b95ad680d3f")
    private val RestartCharacteristicUUID = UUID.fromString("2f1c6d70-9d97-47d7-8c9b-cb0b96bddea6")
    
    
    
    private val bluetoothManager = context.getSystemService(Context.BLUETOOTH_SERVICE) as BluetoothManager
    private val bluetoothAdapter: BluetoothAdapter? = bluetoothManager.adapter
    private var scanning = false
    private val bluetoothLeScanner = bluetoothAdapter?.getBluetoothLeScanner ()
    
    // Source - https://stackoverflow.com/a/79986182
    // Posted by DebuggingByCoffee
    // Retrieved 2026-07-27, License - CC BY-SA 4.0

    private val handler = Handler(Looper.getMainLooper())
    
    // Stops scanning after 10 seconds.
    private val SCAN_PERIOD: Long = 10000
    
    private fun scanLeDevice() {
        if (!isBluetoothSupported()) return
        if (bluetoothLeScanner == null) {
            return
        }
        try {
            if (!scanning) { // Stops scanning after a pre-defined scan period.
                handler.postDelayed({
                                    scanning = false
                                    bluetoothLeScanner.stopScan(leScanCallback)
                                }, SCAN_PERIOD)
                scanning = true
                bluetoothLeScanner.startScan(listOf(ScanFilter.Builder()
                                                    .setServiceData(ParcelUuid(ServiceUUID), 
                                                                    null, 
                                                                    null).build()),
                                                    ScanSettings.Builder()
                                                    .setNumOfMatches(ScanSettings.
                                                                     MATCH_NUM_ONE_ADVERTISEMENT)
                                                    .build(),
                                                    leScanCallback)
            } else {
                scanning = false
                bluetoothLeScanner.stopScan(leScanCallback)
            }
        }
        catch (e: SecurityException) {
            println("scanLeDevice: SecurityException caught: $e")
        }
    }
        
    private var foundDevice: BluetoothDevice?
    private var haveDevice: boolean = false
    // Device scan callback.
    private val leScanCallback: ScanCallback = object : ScanCallback() {
        override fun onScanResult(callbackType: Int, result: ScanResult) {
            super.onScanResult(callbackType, result)
            foundDevice = result.device
            haveDevice = true
        }
    }
    fun isBluetoothSupported(): Boolean = bluetoothAdapter != null
    
    fun isBluetoothEnabled(): Boolean = bluetoothAdapter?.isEnabled == true
    
    fun initialize() {
        if (isBluetoothSupported() && isBluetoothEnabled())
        {
            scanLeDevice()
        }
    }
    fun writeWiFiCharacteristic(ssid: String, password: String)
    {
    }
    fun writeMasterCodeCharacteristic(masterCode: String)
    {
    }
    fun writeOneTimeCodeCharacteristic(oneTimeCode: String)
    {
    }
    fun writeRebootCharacteristic()
    {
    }
}

sealed class ConnectionState {
    object Disconnected : ConnectionState()
    object Connecting : ConnectionState()
    data class Connected(val device: BluetoothDevice) : ConnectionState()
    data class Error(val message: String) : ConnectionState()
}

