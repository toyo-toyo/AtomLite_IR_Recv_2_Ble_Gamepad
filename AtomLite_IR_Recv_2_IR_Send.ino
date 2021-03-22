#include <M5Atom.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

const uint16_t kIrLed = 26;
const uint16_t kRecvPin = 32;
const uint16_t kCaptureBufferSize = 1024;
const uint8_t kTimeout = 50;

IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
IRsend irsend(kIrLed);

decode_results results;  // Somewhere to store the results

uint32_t value;
  
void setup() {
  M5.begin(true, false, true);
  irrecv.enableIRIn();
  irsend.begin();
}

void loop() {
  M5.update();
  if (irrecv.decode(&results)) {
    Serial.print(resultToHumanReadableBasic(&results));
    Serial.print(resultToSourceCode(&results));
    value = results.value;
    Serial.println(value);
    delay(500);
    switch(value) {
      case 1086296430:
        Serial.println("DCR");
        // HDMI1に切替
        irsend.sendPanasonic(0x4004, 0x100A0A1);
        delay(100);
        irsend.sendPanasonic(0x4004, 0x1908213);
        break;
      default:
        Serial.println("他の値です");
    }
  }
  delay(50);
}