
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <qrcode.h>

Adafruit_ST7789 tft = Adafruit_ST7789(PIN_CS, PIN_DC, PIN_RST);

bool dirtyF = true;
bool dirtyL = true;
bool dirtyC = true;
bool dirtyR = true;

bool debugDirtyF = true;
bool debugDirtyL = true;
bool debugDirtyC = true;
bool debugDirtyR = true;

void initScreen() {
  tft.init(240, 320);

  tft.setRotation(2);

  tft.fillScreen(0x0);

  tft.setTextColor(0xFFFF, 0x0);
  tft.setTextSize(3);
  tft.setTextWrap(false);
}

void initDebugScreen() {
  tft.setCursor(20, 20);
  tft.print("YB:");
  tft.setCursor(75, 17);
  tft.print(Y);
  tft.setCursor(95, 17);
  tft.print(B);
}

void requestF() { dirtyF = true; debugDirtyF = true; }
void requestL() { dirtyL = true; debugDirtyL = true; }
void requestC() { dirtyC = true; debugDirtyC = true; }
void requestR() { dirtyR = true; debugDirtyR = true; }

void drawF();
void drawL();
void drawC();
void drawR();

void debugDrawF();
void debugDrawL();
void debugDrawC();
void debugDrawR();

void initCircuitScreen() {
  tft.fillScreen(0x0);

  tft.drawLine(53, 159, 53, 69, 0xFFFF);
  tft.drawLine(53, 69, 108, 69, 0xFFFF);
  tft.drawLine(136, 69, 190, 69, 0xFFFF);
  tft.drawLine(190, 70, 190, 117, 0xFFFF);
  tft.drawLine(190, 124, 190, 182, 0xFFFF);
  tft.drawLine(189, 212, 189, 256, 0xFFFF);
  tft.drawLine(53, 256, 189, 256, 0xFFFF);
  tft.drawLine(53, 168, 53, 256, 0xFFFF);

  tft.drawRGBBitmap(30,  159, image_power_pixels,     48,  10);
  tft.drawRGBBitmap(187, 182, image_inductor_pixels,  8,   31);
  tft.drawRGBBitmap(182, 117, image_capacitor_pixels, 18,  8);
  tft.drawRGBBitmap(105, 65,  image_resistor_pixels,  36,  8);
}

void drawCircuitScreen() {
  tft.setTextColor(0xFFFF);
  tft.setTextWrap(false);
  tft.setTextSize(1);

  tft.setCursor(114, 80);
  tft.print("RRR");

  tft.setCursor(155, 118);
  tft.print("CCC");

  tft.setCursor(155, 194);
  tft.print("LLL");
}

void drawDebugScreen() {
  if (debugDirtyF) debugDrawF();
  if (debugDirtyL) debugDrawL();
  if (debugDirtyC) debugDrawC();
  if (debugDirtyR) debugDrawR();

  debugDirtyF = false;
  debugDirtyL = false;
  debugDirtyC = false;
  debugDirtyR = false;
}

void debugDrawF() {
  tft.fillRect(70, 60, 170, 21, 0x0);
  tft.setTextSize(3);

  tft.setCursor(20, 60);
  tft.print("F");
  tft.setCursor(70, 60);
  tft.print(getKnobValue(KNOB_F) / 10);
  tft.setTextSize(2);
  tft.setCursor(120, 67);
  tft.print(pgm_read_float(&value_F[getKnobValue(KNOB_F) / 10]));
  tft.setCursor(200, 67);
  tft.print(getUnit(KNOB_F, pgm_read_word(&power_F[getKnobValue(KNOB_F) / 10])));
}

void debugDrawL() {
  tft.fillRect(70, 100, 170, 21, 0x0);
  tft.setTextSize(3);

  tft.setCursor(20, 100);
  tft.print("L");
  tft.setCursor(70, 100);
  tft.print(getKnobValue(KNOB_L));
  tft.setTextSize(2);
  tft.setCursor(120, 107);
  tft.print(pgm_read_float(&value_L[getKnobValue(KNOB_L)]));
  tft.setCursor(200, 107);
  tft.print(getUnit(KNOB_L, pgm_read_word(&power_L[getKnobValue(KNOB_L)])));
}

void debugDrawC() {
  tft.fillRect(70, 140, 170, 21, 0x0);
  tft.setTextSize(3);

  tft.setCursor(20, 140);
  tft.print("C");
  tft.setCursor(70, 140);
  tft.print(getKnobValue(KNOB_C));
  tft.setTextSize(2);
  tft.setCursor(120, 147);
  tft.print(pgm_read_float(&value_C[getKnobValue(KNOB_C)]));
  tft.setCursor(200, 147);
  tft.print(getUnit(KNOB_C, pgm_read_word(&power_C[getKnobValue(KNOB_C)])));
}

void debugDrawR() {
  tft.fillRect(70, 180, 170, 21, 0x0);
  tft.setTextSize(3);

  tft.setCursor(20, 180);
  tft.print("R");
  tft.setCursor(70, 180);
  tft.print(getKnobValue(KNOB_R));
  tft.setTextSize(2);
  tft.setCursor(120, 187);
  tft.print(pgm_read_float(&value_R[getKnobValue(KNOB_R)]));
  tft.setCursor(200, 187);
  tft.print(getUnit(KNOB_R, pgm_read_word(&power_R[getKnobValue(KNOB_R)])));
}

void drawQRCode() {
  // Initialize TFT
  tft.init(240, 320);
  tft.setRotation(2);
  tft.fillScreen(ST77XX_WHITE);

  // Create QR code
  QRCode qrcode;

  // Version 4 = 33x33 modules
  uint8_t qrcodeData[qrcode_getBufferSize(4)];

  qrcode_initText(
    &qrcode,
    qrcodeData,
    4,
    ECC_LOW,
    "https://projectboom.rf.gd/analyze.php?code=t120m2"
  );

  // Size of each QR module in pixels
  int scale = 5;

  // Add 4-module quiet zone
  int border = 4 * scale;

  // Total QR size including border
  int qrPixelSize = qrcode.size * scale + border * 2;

  // Center on 240x320 screen
  int x = (240 - qrPixelSize) / 2;
  int y = (320 - qrPixelSize) / 2;

  // White background
  tft.fillScreen(ST77XX_WHITE);

  // debugDraw QR code
  for (uint8_t row = 0; row < qrcode.size; row++) {
    for (uint8_t col = 0; col < qrcode.size; col++) {
      if (qrcode_getModule(&qrcode, col, row)) {
        tft.fillRect(
          x + border + col * scale,
          y + border + row * scale,
          scale,
          scale,
          ST77XX_BLACK
        );
      }
    }
  }
}
