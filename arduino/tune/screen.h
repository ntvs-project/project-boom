
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

Adafruit_ST7789 tft = Adafruit_ST7789(PIN_CS, PIN_DC, PIN_RST);

bool dirtyF = true;
bool dirtyL = true;
bool dirtyC = true;
bool dirtyR = true;

void initScreen() {
  tft.init(240, 320);

  tft.setRotation(2);

  tft.fillScreen(0x0);

  tft.setTextColor(0xFFFF, 0x0);
  tft.setTextSize(3);
  tft.setTextWrap(false);

  tft.setCursor(20, 20);
  tft.print("YB:");
  tft.setCursor(75, 17);
  tft.print(Y);
  tft.setCursor(95, 17);
  tft.print(B);
}

void requestF() { dirtyF = true; }
void requestL() { dirtyL = true; }
void requestC() { dirtyC = true; }
void requestR() { dirtyR = true; }

void drawF();
void drawL();
void drawC();
void drawR();

void debugScreen() {
  if (dirtyF) drawF();
  if (dirtyL) drawL();
  if (dirtyC) drawC();
  if (dirtyR) drawR();

  dirtyF = false;
  dirtyL = false;
  dirtyC = false;
  dirtyR = false;
}

void drawF() {
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

void drawL() {
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

void drawC() {
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

void drawR() {
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
