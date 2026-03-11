
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
