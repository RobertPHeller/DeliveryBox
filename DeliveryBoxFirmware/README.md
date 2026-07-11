# This is the firmware

Target dev board: Adafruit Feather ESP32S3, No Psram version (8M flash).
 
This firmware manages the lock via a servo.  There is a numeric keypad and
an eight character 14-segment display.
 
The lock uses 8-digit codes to unlock the box.  There is one master code 
and any number of one-time codes.  The one time codes are given to delivery
drivers when a delivery is scheduled.  Once the driver uses the code to 
deliver the parcel, the code is removed and can't be used again.

The  firmware  can use  both  Blutooth  (BLE)  and  WiFi for  configuring  and
management.
