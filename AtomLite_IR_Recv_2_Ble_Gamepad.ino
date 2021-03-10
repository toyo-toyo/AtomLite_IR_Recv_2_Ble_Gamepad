#include <M5Atom.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>
#include <BleGamepad.h> 

const uint16_t kRecvPin = 32;
const uint16_t kCaptureBufferSize = 1024;
const uint8_t kTimeout = 50;
const CRGB CRGB_BLE_CONNECTED(0x00, 0x00, 0xf0);
const CRGB CRGB_BLE_DISCONNECTED(0x00, 0xf0, 0x00);

IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
decode_results results;  // Somewhere to store the results

BleGamepad bleGamepad;
bool isBleConnected = false;

void setup() {
  M5.begin(true, false, true);
  M5.dis.drawpix(0, CRGB_BLE_DISCONNECTED);
  irrecv.enableIRIn();
  bleGamepad.begin();
}

void loop() {
  M5.update();
  if(bleGamepad.isConnected()) {
    if (!isBleConnected) {
      M5.dis.drawpix(0, CRGB_BLE_CONNECTED);
      isBleConnected = true;
      Serial.println("Connected");
    }
    // Check if the IR code has been received.
    if (irrecv.decode(&results)) {
      uint32_t value = results.value;
      Serial.println(value);
      switch(value){
        case 1111111111:
          Serial.println("上");
          sendLeftThumb(0, -32767);
          break;
        case 2222222222:
          Serial.println("下");
          sendLeftThumb(0, 32767);
          break;
        case 3333333333:
          Serial.println("左");
          sendLeftThumb(-32767, 0);
          break;
        case 4444444444:
          Serial.println("右");
          sendLeftThumb(32767, 0);
          break;
        case 5555555555:
          Serial.println("決定");
          sendButton(BUTTON_1);
          break;
        case 6666666666:
          Serial.println("戻る");
          sendButton(BUTTON_2);
          break;
        default:
          Serial.println("他の値です");
      }
    }
  } else {
    if (isBleConnected) {
      M5.dis.drawpix(0, CRGB_BLE_DISCONNECTED);
      isBleConnected = false;
      Serial.println("Disconnected");
    }
  }
  delay(50);
}
void sendLeftThumb(int16_t x, int16_t y) {
  bleGamepad.setLeftThumb(x, y);
  delay(1);
  bleGamepad.setLeftThumb(0, 0);
}
void sendButton(uint8_t sendkey) {
  bleGamepad.press(sendkey);
  delay(1);
  bleGamepad.release(sendkey);
}