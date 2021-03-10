#include <M5Atom.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

const uint16_t kRecvPin = 32;
const uint16_t kCaptureBufferSize = 1024;
const uint8_t kTimeout = 50;

IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
decode_results results;  // Somewhere to store the results

void setup() {
  M5.begin(true, false, false);
  irrecv.enableIRIn();
}

void loop() {
  // Check if the IR code has been received.
  if (irrecv.decode(&results)) {
    uint32_t value = results.value;
    Serial.println(value);
  }
  delay(100);
}