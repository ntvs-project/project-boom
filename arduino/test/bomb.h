#pragma once
#include "api.h"
#include "module.h"

Test1 test1;
Test2 test2;
Test3 test3;
Test4 test4;
Test5 test5;

Module* MODULES[] = {
  &test1,
  &test2,
  &test3,
  &test4,
  &test5,
};

class Bomb {
  public:
    Module* modules;

    void init();
    void loop();
    void fini();
    void test();
};

void Bomb::init() {
  modules = MODULES;
  for (Module* module : modules) {
    Serial.println(module -> name);
  }
}

void Bomb::loop() {
  // 
}

void Bomb::test() {
  static bool prev = false;
  bool current = digitalRead(14);

  if (prev && !current) reset();

  prev = current;
}
