#pragma once

class Module {
  public:
    String name;
    virtual void init() {};
    virtual void loop() {};
    virtual void fini() {};
};

class Test1 : public Module {
  public:
    String name = "test1";
    void init() override {
      Serial.print(name + ".init ");
    }
    void loop() override {
      Serial.print(name + ".loop ");
      if (Serial.available()) {
        Serial.read();
        fini();
      }
    }
    void fini() override {
      Serial.print(name + ".fini ");
      index++;
    }
}; Test1 test1;

class Test2 : public Module {
  public:
    String name = "test2";
    void init() override {
      Serial.print(name + ".init ");
    }
    void loop() override {
      Serial.print(name + ".loop ");
      if (Serial.available()) {
        Serial.read();
        fini();
      }
    }
    void fini() override {
      Serial.print(name + ".fini ");
      index++;
    }
}; Test2 test2;

class Test3 : public Module {
  public:
    String name = "test3";
    void init() override {
      Serial.print(name + ".init ");
    }
    void loop() override {
      Serial.print(name + ".loop ");
      if (Serial.available()) {
        Serial.read();
        fini();
      }
    }
    void fini() override {
      Serial.print(name + ".fini ");
      index++;
    }
}; Test3 test3;

class Test4 : public Module {
  public:
    String name = "test4";
    void init() override {
      Serial.print(name + ".init ");
    }
    void loop() override {
      Serial.print(name + ".loop ");
      if (Serial.available()) {
        Serial.read();
        fini();
      }
    }
    void fini() override {
      Serial.print(name + ".fini ");
      index++;
    }
}; Test4 test4;

class Test5 : public Module {
  public:
    String name = "test5";
    void init() override {
      Serial.print(name + ".init ");
    }
    void loop() override {
      Serial.print(name + ".loop ");
      if (Serial.available()) {
        Serial.read();
        fini();
      }
    }
    void fini() override {
      Serial.print(name + ".fini ");
      index++;
    }
}; Test5 test5;

