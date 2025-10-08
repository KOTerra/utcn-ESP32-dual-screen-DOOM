#include <Bluepad32.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ====== OLED SETUP ======
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_SDA 21   // <-- changed from 34 to 21
#define OLED_SCL 22   // <-- changed from 35 to 22

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Controller connected, index=%d\n", i);
      ControllerProperties properties = ctl->getProperties();
      Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n",
                    ctl->getModelName().c_str(),
                    properties.vendor_id, properties.product_id);
      myControllers[i] = ctl;
      foundEmptySlot = true;

      // Display controller info on OLED
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.printf("Controller #%d\n", i);
      display.printf("%s\n", ctl->getModelName().c_str());
      display.printf("VID:%04X PID:%04X\n", properties.vendor_id, properties.product_id);
      display.display();
      break;
    }
  }

  if (!foundEmptySlot) {
    Serial.println("CALLBACK: No empty slot for controller");
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Controller disconnected index=%d\n", i);
      myControllers[i] = nullptr;

      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.println("Controller disconnected");
      display.display();
      break;
    }
  }
}

void dumpGamepad(ControllerPtr ctl) {
  int idx = ctl->index();
  int lx = ctl->axisX();
  int ly = ctl->axisY();
  int rx = ctl->axisRX();
  int ry = ctl->axisRY();
  int buttons = ctl->buttons();

  Serial.printf("idx=%d, LX=%4d, LY=%4d, RX=%4d, RY=%4d, buttons=0x%04x\n",
                idx, lx, ly, rx, ry, buttons);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.printf("Gamepad #%d\n", idx);
  display.printf("LX:%4d  LY:%4d\n", lx, ly);
  display.printf("RX:%4d  RY:%4d\n", rx, ry);
  display.printf("BTN:0x%04X\n", buttons);
  display.display();
}

void processGamepad(ControllerPtr ctl) {
  dumpGamepad(ctl);
}

void processControllers() {
  for (auto myController : myControllers) {
    if (myController && myController->isConnected() && myController->hasData()) {
      if (myController->isGamepad()) {
        processGamepad(myController);
      } else {
        Serial.println("Unsupported controller");
      }
    }
  }
}

void setup() {
  Serial.begin(115200);

  // OLED init with correct pins
  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Bluepad32 + OLED");
  display.display();

  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %02X:%02X:%02X:%02X:%02X:%02X\n",
                addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();
  BP32.enableVirtualDevice(false);
}

void loop() {
  bool dataUpdated = BP32.update();
  if (dataUpdated)
    processControllers();

  delay(150);
}
