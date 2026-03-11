#pragma once

class Module {
  public:
    virtual void init() {}
    virtual bool loop() { return false; }
    virtual void fini() {}
};

class Test1 : public Module {
  public:
    String name = "test1";
    int pin = 15;

    void init() {}

    bool loop() {
      static bool prev = false;
      bool current = digitalRead(pin);

      if (prev && !current) return true;

      prev = current;
      return false;
    }

    void fini() {}
};

class Test2 : public Test1 {
  public:
    String name = "test2";
    int pin = 16;
};

class Test3 : public Test1 {
  public:
    String name = "test3";
    int pin = 17;
};

class Test4 : public Test1 {
  public:
    String name = "test4";
    int pin = 18;
};

class Test5 : public Test1 {
  public:
    String name = "test5";
    int pin = 19;
};
