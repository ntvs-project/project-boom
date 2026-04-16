#pragma once

class Module {
  public:
    String name;

    Module(String n) {
      name = n;
    }

    virtual void init() {};
    virtual void loop() {};
    virtual void fini() {};
};

class Test : public Module {
  public:
    ui pin_idx;

    Test(String n, ui p) : Module(n) {
      pin_idx = p;
    }

    void init() override {
      // Serial.println(name + ".init ");
      for (int i=0; i<4; i++) digitalWrite(PIN_7SEG[i], bitRead(pin_idx + 1, i));
    }

    void loop() override {
      // Serial.print(name + ".loop ");

      for (int i=0; i<length(IDX_TEST); i++) {
        if (i != pin_idx && BTN.wasReleased(IDX_TEST[i]) == 1) {
          B.missed();
        }
      }
      if (BTN.wasReleased(IDX_TEST[pin_idx]) == 1) {
        fini();
      }
    }

    void fini() override {
      // Serial.println(name + ".fini ");
      for (int i=0; i<4; i++) digitalWrite(PIN_7SEG[i], bitRead(15, i));
      B.fini();
    }
};

Test test1("test1", 0);
Test test2("test2", 1);
Test test3("test3", 2);
Test test4("test4", 3);
Test test5("test5", 4);
