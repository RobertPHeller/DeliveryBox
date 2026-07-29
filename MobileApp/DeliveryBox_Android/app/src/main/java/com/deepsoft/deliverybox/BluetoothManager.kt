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
import android.annotation.SuppressLint
import android.app.Service
import java.util.concurrent.Executors
import android.bluetooth.*
//import android.bluetooth.BluetoothAdapter
//import android.bluetooth.BluetoothDevice
//import android.bluetooth.BluetoothManager
//import android.bluetooth.BluetoothGatt
//import android.bluetooth.BluetoothGattCallback
//import android.bluetooth.BluetoothProfile
//import android.bluetooth.BluetoothGattService
//import android.bluetooth.BluetoothGattCharacteristic
//import android.bluetooth.BluetoothGattCharacteristics
import android.content.Intent
import android.os.Binder
import android.os.IBinder
import android.util.Log
import androidx.annotation.RequiresPermission
import android.bluetooth.le.ScanFilter
import android.bluetooth.le.ScanSettings
import android.bluetooth.le.ScanCallback
import android.bluetooth.le.ScanResult
import android.bluetooth.BluetoothGattConnectionSettings
import android.content.Context
import android.content.IntentFilter
import android.os.Build
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import android.os.Handler
import android.os.ParcelUuid
import android.os.Looper


@SuppressLint("MissingPermission")
class BluetoothLeService(private val context: Context)  : Service() {
    private val ServiceUUID = UUID.fromString("3a6089ff-731f-4312-9761-6ecfa14b867e")
    private val WIFICharacteristicUUID = UUID.fromString("00e19a26-da6e-4ad2-896c-b5f0cbe04e43")
    private var WIFICharacteristic: BluetoothGattCharacteristic? = null
    private val MasterCodeCharacteristicUUID = UUID.fromString("fb90b3be-e21c-4aca-ae29-c05a0c3992e3")
    private var MasterCodeCharacteristic: BluetoothGattCharacteristic? = null
    private val OneTimeCodeCharacteristicUUID = UUID.fromString("3d32154a-50f1-4e7d-883e-2b95ad680d3f")
    private var OneTimeCodeCharacteristic: BluetoothGattCharacteristic? = null
    private val RestartCharacteristicUUID = UUID.fromString("2f1c6d70-9d97-47d7-8c9b-cb0b96bddea6")
    private var RestartCharacteristic: BluetoothGattCharacteristic? = null
    
    
    
    private val bluetoothManager = context.getSystemService(Context.BLUETOOTH_SERVICE) as BluetoothManager
    private val bluetoothAdapter: BluetoothAdapter? = bluetoothManager.adapter
    private var scanning = false
    private val bluetoothLeScanner = bluetoothAdapter?.getBluetoothLeScanner ()
    
    private var connectionState = STATE_DISCONNECTED
    
    
    companion object {
        const val ACTION_GATT_CONNECTED =
            "com.deepsoft.deliverybox.ACTION_GATT_CONNECTED"
        const val ACTION_GATT_DISCONNECTED =
            "com.deepsoft.deliverybox.ACTION_GATT_DISCONNECTED"

        private const val STATE_DISCONNECTED = 0
        private const val STATE_CONNECTED = 2
    }

    private val bluetoothGattCallback = object : BluetoothGattCallback() {
        override fun onConnectionStateChange(gatt: BluetoothGatt?, 
                                             status: Int, newState: Int) {
            if (newState == BluetoothProfile.STATE_CONNECTED) {
                // successfully connected to the GATT Server
                connectionState = STATE_CONNECTED
                broadcastUpdate(ACTION_GATT_CONNECTED)
            } else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
                // disconnected from the GATT Server
                connectionState = STATE_DISCONNECTED
                broadcastUpdate(ACTION_GATT_DISCONNECTED)
            }
        }
    }

    private fun broadcastUpdate(action: String) {
        val intent = Intent(action)
        sendBroadcast(intent)
    }
    
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
        
    private var foundDevice: BluetoothDevice? = null
    private var haveDevice: Boolean = false
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
    
    private val binder = LocalBinder()
    override fun onBind(intent: Intent): IBinder? {
        return binder
    }

    inner class LocalBinder : Binder() {
        fun getService(): BluetoothLeService {
            return this@BluetoothLeService
        }
    }
    var bluetoothGatt: BluetoothGatt? = null
    
    @Suppress("DEPRECATION")
    fun initialize() {
        if (isBluetoothSupported() && isBluetoothEnabled())
        {
            scanLeDevice()
        }
        if (haveDevice)
        {
            // connect then scan for characteristics for serviceUUID
            bluetoothGatt = foundDevice!!
                        .connectGatt(this,
                                     false,
                                     bluetoothGattCallback)
            if (connectionState == STATE_CONNECTED) {
                 bluetoothGatt!!.discoverServices()
                 val service: BluetoothGattService? = bluetoothGatt!!.getService(ServiceUUID)
                 WIFICharacteristic = service!!.getCharacteristic(WIFICharacteristicUUID)
                 MasterCodeCharacteristic = service!!.getCharacteristic(MasterCodeCharacteristicUUID)
                 OneTimeCodeCharacteristic = service!!.getCharacteristic(OneTimeCodeCharacteristicUUID)
                 RestartCharacteristic = service!!.getCharacteristic(RestartCharacteristicUUID)
            }
        }
    }
    @Suppress("DEPRECATION")
    private fun reconnectAfterDelay() {
        if (bluetoothGatt != null) {
            bluetoothGatt!!.close()
            bluetoothGatt = null
            WIFICharacteristic = null
            MasterCodeCharacteristic = null
            OneTimeCodeCharacteristic = null
            RestartCharacteristic = null
            connectionState = STATE_DISCONNECTED
            broadcastUpdate(ACTION_GATT_DISCONNECTED)
            foundDevice = null
            haveDevice = false
            Handler().postDelayed({initialize()}, 2000)
        }
    }
    @Suppress("DEPRECATION")
    fun writeWiFiCharacteristic(ssid: String, password: String)
    {
        val message: String = ssid + "\n" + password
        if (connectionState == STATE_CONNECTED && 
            bluetoothGatt != null) {
            val characteristic = WIFICharacteristic
            if (characteristic != null) {
                characteristic.setValue(message.toByteArray())
                characteristic.setWriteType(BluetoothGattCharacteristic.WRITE_TYPE_NO_RESPONSE)
                bluetoothGatt!!.writeCharacteristic(characteristic)
            }
        }
    }
    @Suppress("DEPRECATION")
    fun writeMasterCodeCharacteristic(masterCode: String)
    {
        if (connectionState == STATE_CONNECTED && 
            bluetoothGatt != null) {
            val characteristic = MasterCodeCharacteristic
            if (characteristic != null) {
                characteristic.setValue(masterCode.toByteArray())
                characteristic.setWriteType(BluetoothGattCharacteristic.WRITE_TYPE_NO_RESPONSE)
                bluetoothGatt!!.writeCharacteristic(characteristic)
            }
        }
    }
    @Suppress("DEPRECATION")
    fun writeOneTimeCodeCharacteristic(oneTimeCode: String)
    {
        if (connectionState == STATE_CONNECTED && 
            bluetoothGatt != null) {
            val characteristic = OneTimeCodeCharacteristic
            if (characteristic != null) {
                characteristic.setValue(oneTimeCode.toByteArray())
                characteristic.setWriteType(BluetoothGattCharacteristic.WRITE_TYPE_NO_RESPONSE)
                bluetoothGatt!!.writeCharacteristic(characteristic)
            }
        }
    }
    @Suppress("DEPRECATION")
    fun writeRebootCharacteristic()
    {
        if (connectionState == STATE_CONNECTED && 
            bluetoothGatt != null) {
            val characteristic = RestartCharacteristic
            if (characteristic != null) {
                characteristic.setValue("REBOOT".toByteArray())
                characteristic.setWriteType(BluetoothGattCharacteristic.WRITE_TYPE_NO_RESPONSE)
                bluetoothGatt!!.writeCharacteristic(characteristic)
                reconnectAfterDelay()
            }
        }
    }
}


