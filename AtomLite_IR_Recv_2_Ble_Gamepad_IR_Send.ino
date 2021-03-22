#include <M5Atom.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>
#include <BleGamepad.h> 

const uint16_t kIrLed = 26;
const uint16_t kRecvPin = 32;
const uint16_t kCaptureBufferSize = 1024;
const uint8_t kTimeout = 50;

const uint16_t rawPcOn[71] = {9144, 4436,  656, 506,  630, 508,  628, 508,  628, 508,  626, 484,  652, 508,  628, 508,  628, 508,  624, 1616,  628, 1618,  626, 1590,  652, 1616,  628, 1616,  626, 1616,  626, 1592,  650, 1616,  628, 510,  624, 512,  624, 510,  626, 510,  626, 510,  624, 512,  626, 1592,  652, 508,  626, 1616,  628, 1616,  626, 1616,  626, 1616,  624, 1618,  628, 1614,  624, 512,  622, 1618,  630, 39904,  9146, 2196,  624};  // NEC FF02FD
const uint16_t rawPcOff[71] = {9128, 4438,  654, 506,  624, 512,  622, 512,  598, 536,  602, 534,  624, 510,  600, 536,  620, 514,  622, 1618,  598, 1616,  650, 1618,  628, 1614,  602, 1640,  600, 1644,  624, 1618,  600, 1612,  630, 1640,  626, 510,  624, 506,  606, 1638,  622, 1618,  626, 508,  600, 534,  600, 536,  600, 534,  598, 1644,  598, 1618,  624, 536,  598, 538,  598, 1616,  624, 1644,  622, 1620,  622, 39900,  9138, 2174,  652};  // NEC FF9867
const uint16_t rawDenonChromecast[99] = {3290, 1728,  394, 442,  370, 466,  370, 1300,  368, 468,  396, 1274,  370, 466,  368, 1302,  398, 438,  394, 444,  394, 1276,  398, 438,  368, 468,  370, 1298,  370, 1302,  392, 442,  370, 468,  368, 466,  394, 442,  372, 464,  392, 444,  370, 466,  368, 466,  398, 1272,  370, 468,  396, 1274,  372, 462,  392, 444,  394, 442,  394, 1276,  396, 1274,  370, 466,  368, 468,  372, 1296,  370, 466,  370, 1300,  370, 1300,  396, 440,  368, 1302,  394, 442,  368, 468,  370, 466,  392, 442,  370, 1300,  370, 1300,  370, 1300,  370, 466,  394, 1276,  392, 444,  394};  // DENON 2A4C028CB43A
const uint16_t rawDenonPc[99] = {3284, 1732,  370, 466,  370, 466,  368, 1302,  368, 466,  368, 1300,  370, 468,  370, 1300,  370, 468,  368, 466,  394, 1276,  370, 466,  370, 466,  370, 1300,  396, 1274,  396, 440,  370, 466,  370, 468,  392, 442,  368, 466,  370, 466,  370, 466,  368, 466,  368, 1300,  370, 468,  368, 1302,  368, 468,  392, 442,  368, 466,  370, 466,  368, 1302,  368, 1302,  370, 468,  368, 1302,  368, 468,  394, 1276,  392, 1276,  394, 442,  370, 1302,  394, 442,  370, 466,  368, 466,  370, 466,  368, 1302,  368, 1302,  370, 466,  370, 466,  368, 466,  370, 466,  368};  // DENON 2A4C0286B430
const uint16_t rawDenonBluray[99] = {3290, 1726,  372, 464,  396, 440,  372, 1298,  372, 464,  396, 1274,  372, 464,  370, 1298,  372, 464,  370, 466,  370, 1298,  372, 464,  370, 466,  372, 1298,  372, 1298,  372, 464,  372, 464,  370, 466,  394, 442,  370, 466,  370, 466,  370, 466,  394, 440,  394, 1276,  396, 440,  396, 1274,  372, 466,  394, 442,  370, 466,  372, 1298,  372, 464,  370, 466,  370, 464,  370, 1298,  396, 440,  396, 1274,  372, 1298,  372, 466,  392, 1276,  372, 464,  372, 464,  372, 464,  370, 466,  370, 1298,  372, 1298,  372, 1298,  370, 1300,  370, 1300,  396, 440,  370};  // DENON 2A4C0288B43E

const CRGB CRGB_BLE_CONNECTED(0x00, 0x00, 0xf0);
const CRGB CRGB_BLE_DISCONNECTED(0x00, 0xf0, 0x00);

IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
IRsend irsend(kIrLed);

decode_results results;

BleGamepad bleGamepad;
bool isBleConnected = false;
uint32_t value;
  
void setup() {
  M5.begin(true, false, true);
  M5.dis.drawpix(0, CRGB_BLE_DISCONNECTED);
  irrecv.enableIRIn();
  irsend.begin();
  bleGamepad.begin();
}

void loop() {
  M5.update();
  if (irrecv.decode(&results)) {
    Serial.print(resultToHumanReadableBasic(&results));
    Serial.print(resultToSourceCode(&results));
    value = results.value;
    Serial.println(value);
    switch(value) {
      case 1086269655:
        value = 0;
        Serial.println("→");
        // Atom Lite 再起動
        esp_restart();
        break;
      case 42002538:
        value = 0;
        Serial.println("Denon+");
        delay(500);
        // TVの音量
        irsend.sendPanasonic(0x4004, 0x1000405);
        break;
      case 42526818:
        value = 0;
        Serial.println("Denon-");
        delay(500);
        // TVの音量
        irsend.sendPanasonic(0x4004, 0x1008485);
        break;
      case 1086296430:
        value = 0;
        Serial.println("DCR");
        delay(500);
        // TVの入力をHDMI1に切替
        irsend.sendPanasonic(0x4004, 0x100A0A1);
        delay(100);
        irsend.sendPanasonic(0x4004, 0x1908213);
        break;
      case 1086303570:
        value = 0;
        Serial.println("P.mode");
        delay(500);
        // PCの起動
        irsend.sendRaw(rawPcOn, 71, 38);
        delay(500);
        irsend.sendRaw(rawPcOff, 71, 38);
        // TVの入力をHDMI1に切替
        delay(100);
        irsend.sendPanasonic(0x4004, 0x100A0A1);
        delay(100);
        irsend.sendPanasonic(0x4004, 0x1908213);
        delay(100);
        // Denonの入力をPCに切替
        irsend.sendRaw(rawDenonPc, 99, 38);
        break;
      case 1086271695:
        value = 0;
        Serial.println("+");
        delay(500);
        // Denonの入力をPCに切替
        irsend.sendRaw(rawDenonPc, 99, 38);
        break;
      case 1080726861:
        value = 0;
        Serial.println("Blu-ray電源");
        delay(500);
        // Denonの入力をBlu-rayに切替
        irsend.sendRaw(rawDenonBluray, 99, 38);
        // TVの入力をHDMI1に切替
        delay(100);
        irsend.sendPanasonic(0x4004, 0x100A0A1);
        delay(100);
        irsend.sendPanasonic(0x4004, 0x1908213);
        break;
      case 1086288525:
        value = 0;
        Serial.println("-");
        delay(500);
        // Denonの入力をBlu-rayに切替
        irsend.sendRaw(rawDenonBluray, 99, 38);
        break;
      case 1086320655:
        value = 0;
        Serial.println("x");
        delay(500);
        // Denonの入力をChromecast with Google TVに切替
        irsend.sendRaw(rawDenonChromecast, 99, 38);
        break;
    }
  }
  if(bleGamepad.isConnected()) {
    if (!isBleConnected) {
      M5.dis.drawpix(0, CRGB_BLE_CONNECTED);
      isBleConnected = true;
      Serial.println("Connected");
    }
    // Check if the IR code has been received.
    if (value != 0) {
      switch(value){
        case 1086319125:
          Serial.println("電源");
          sendButton(BUTTON_64);
          break;
        case 1086304335:
        case 650079:
          Serial.println("上");
          sendLeftThumb(0, -32767);
          break;
        case 1086321165:
        case 387935:
          Serial.println("下");
          sendLeftThumb(0, 32767);
          break;
        case 1086310965:
        case 912223:
          Serial.println("左");
          sendLeftThumb(-32767, 0);
          break;
        case 1086261495:
        case 256863:
          Serial.println("右");
          sendLeftThumb(32767, 0);
          break;
        case 1086294135:
        case 854879:
          Serial.println("決定");
          sendButton(BUTTON_1);
          break;
        case 1086286485:
        case 461663:
        case 101328:
          Serial.println("戻る");
          sendButton(BUTTON_2);
          break;
        case 1086298215:
          Serial.println("ホーム");
          sendButton(BUTTON_3);
          break;
        default:
          Serial.println("他の値です");
      }
      value = 0;
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
