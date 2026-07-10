# This is the circuit design

The circuit design uses these parts: 

1. a main board containing an Adafruit ESP32-S3 Feather and connection headers
for a servo  and the  keypad  (there is a  STEMMA/QWIC  connector  for the I2C
connection to the Display Module on th Feather board).

![Main board fritzing module](MainControlModule_bb.png)

2.  a  display  module  with  a  HT16K33   breakout  and  4  dual   14-segment
alphanumberic display modules.

![Display board fritzing module](DisplayModule_bb.png)

3.  a 12-key keypad board.

![3x4 Keypad](Keypad.jpg)

4. A high torque metal gear servo will be used to operate a lock.

![High Torque Servo](Servo.jpg)
