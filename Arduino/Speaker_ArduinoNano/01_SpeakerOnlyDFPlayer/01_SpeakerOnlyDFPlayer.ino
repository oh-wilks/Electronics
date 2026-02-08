#include <SoftwareSerial.h>

SoftwareSerial mp3(11, 10); // RX, TX

void sendCmd(byte cmd, byte p1, byte p2) {
  mp3.write((byte)0x7E);
  mp3.write((byte)0xFF);
  mp3.write((byte)0x06);
  mp3.write(cmd);
  mp3.write((byte)0x00);
  mp3.write(p1);
  mp3.write(p2);
  mp3.write((byte)0xEF);
}

void setup() {
  mp3.begin(9600);
  delay(1000);

  sendCmd(0x06, 0x00, 0x1E); // volume 30
  delay(500);
  sendCmd(0x03, 0x00, 0x01); // play 0001.mp3
}

void loop() {
}
