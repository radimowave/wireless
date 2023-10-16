#include <BluetoothSerial.h>

BluetoothSerial SerialBT;
int SensorPin = 36; // กำหนดขาของ Pulse Heartbeat Sensor (D34) -VP pin

void setup() {
  Serial.begin(9600);
  SerialBT.begin("ESP32_MetaRadiator"); // ตั้งชื่อ Bluetooth Serial

  pinMode(SensorPin, INPUT);
}

void loop() {
  int Signal = analogRead(SensorPin); // อ่านค่าจาก Pulse Heartbeat Sensor

  // ส่งค่า Pulse Heartbeat ผ่าน Bluetooth
  //SerialBT.print("Pulse Heartbeat: ");
  Serial.println(Signal/20);   
  SerialBT.println(Signal/20);

  delay(1000); // รอ 1 วินาทีแล้วส่งข้อมูลอีกครั้ง
}
