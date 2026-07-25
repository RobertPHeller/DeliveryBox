package com.deepsoft.deliverybox

import android.os.Bundle
import android.os.Build
import android.Manifest
import android.widget.Button
import android.widget.EditText
import android.widget.LinearLayout
import android.widget.TextView
import android.widget.Toast
import androidx.activity.ComponentActivity
import androidx.activity.result.contract.ActivityResultContracts
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothManager
import android.content.Context
import androidx.core.content.ContextCompat
import android.content.pm.PackageManager
import androidx.appcompat.app.AlertDialog
import androidx.activity.result.ActivityResultLauncher

class MainActivity : ComponentActivity() {
    private lateinit var bluetoothManager: BluetoothConnectionManager
    
    private val bluetoothPermissionLauncher = registerForActivityResult(
                ActivityResultContracts.RequestMultiplePermissions()
    ) { permissions ->
        val allGranted = permissions.values.all { it }
        if (allGranted) {
            initializeBluetooth()
        } else {
            Toast.makeText(this, "Bluetooth permissions has not been granted!", Toast.LENGTH_LONG).show()
            finish()
        }
    }
    
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        
        bluetoothManager = BluetoothConnectionManager(this)
        
        if (!bluetoothManager.isBluetoothSupported()) {
            Toast.makeText(this, "Bluetooth not supported on this device", Toast.LENGTH_LONG).show()
            finish()
            return
        }
        
        checkAndRequestPermissions()
        
        // WiFiSubmit
        val WiFiSubmitButton: Button = findViewById(R.id.configureWiFi)
        WiFiSubmitButton.setOnClickListener {
            val ssid: EditText = findViewById(R.id.wifi_ssid)
            val thessid: String = ssid.text.toString()
            val wifi_password: EditText = findViewById(R.id.wifi_password)
            val thepassword: String = wifi_password.text.toString()
            bluetoothManager.writeWiFiCharacteristic(thessid,
                                                     thepassword)
        }
        // setMasterCode
        val setMasterCodeButton: Button = findViewById(R.id.setMasterCode)
        setMasterCodeButton.setOnClickListener {
            val masterCode: EditText = findViewById(R.id.masterCode)
            val theMasterCode: String = masterCode.text.toString()
            bluetoothManager.writeMasterCodeCharacteristic(theMasterCode)
        }
        // addOneTimeCode
        val addOneTimeCodeButton: Button = findViewById(R.id.addOneTimeCode)
        addOneTimeCodeButton.setOnClickListener {
            val oneTimeCode: EditText = findViewById(R.id.oneTimeCode)
            val theOneTimeCode: String = oneTimeCode.text.toString()
            bluetoothManager.writeOneTimeCodeCharacteristic(theOneTimeCode)
        }
        // reboot
        val rebootButton: Button = findViewById(R.id.reboot)
        rebootButton.setOnClickListener {
            bluetoothManager.writeRebootCharacteristic()
        }
    }
    
    private fun checkAndRequestPermissions() {
        val permissions = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            arrayOf(
                Manifest.permission.BLUETOOTH_SCAN,
                Manifest.permission.BLUETOOTH_CONNECT
            )
        } else {
            arrayOf(
                Manifest.permission.BLUETOOTH,
                Manifest.permission.BLUETOOTH_ADMIN,
                Manifest.permission.ACCESS_FINE_LOCATION
            )
        }

        val missingPermissions = permissions.filter {
            ContextCompat.checkSelfPermission(this, it) != PackageManager.PERMISSION_GRANTED
        }
        
        if (missingPermissions.isNotEmpty()) {
            if (shouldShowRequestPermissionRationale(missingPermissions.toString())) {
                showPermissionRationaleDialog()
            } else {
                bluetoothPermissionLauncher.launch(missingPermissions.toTypedArray())
            }
        } else {
            initializeBluetooth()
        }
    }
    private fun initializeBluetooth() {
        bluetoothManager.initialize()
    }
    private fun showPermissionRationaleDialog() {
        val permissions = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            arrayOf(
                Manifest.permission.BLUETOOTH_SCAN,
                Manifest.permission.BLUETOOTH_CONNECT
            )
        } else {
            arrayOf(
                Manifest.permission.BLUETOOTH,
                Manifest.permission.BLUETOOTH_ADMIN,
                Manifest.permission.ACCESS_FINE_LOCATION
            )
        }
        val missingPermissions = permissions.filter {
            ContextCompat.checkSelfPermission(this, it) != PackageManager.PERMISSION_GRANTED
        }
        
        AlertDialog.Builder(this)
          .setTitle("Bluetooth Permission Needed for Delivery Box App")
          .setMessage("This app requires access to Bluetooth to access your Delivery Box.")
          .setPositiveButton("OK") { _, _ ->
              bluetoothPermissionLauncher.launch(missingPermissions.toTypedArray())
          }
          .setNegativeButton("Cancel", null)
          .create()
          .show()
    }
}
