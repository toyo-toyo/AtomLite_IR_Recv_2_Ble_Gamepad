#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <M5Atom.h>

const uint16_t kIrLed = 26; // AtomLite内蔵LED 12 IR UNIT 26
const uint32_t sendValue = 1234567890;    //送信する赤外線信号

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

void setup() {
  M5.begin(true, false, false);
  irsend.begin();
}

void loop() {
  M5.update();
  // Atom Lite Button
  if (M5.Btn.wasReleased()) {
    Serial.println("sendNEC()");
    irsend.sendNEC(sendValue);
  }
  delay(100);
}
