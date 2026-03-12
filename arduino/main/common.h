#pragma once

typedef unsigned long ul;
typedef unsigned int  ui;

ui OUT[] = {};
ui IN[]  = {};

ui TIME[] = { 0, 10 };
ui index;

#define length(arr) (sizeof(arr) / sizeof(arr[0]))

template <typename T>
void swap(T& a, T& b) {
  T tmp = a;
  a = b;
  b = tmp;
}

class Bomb {
  public:
    void (*reset) (void) = 0;
    void fini() {
      while (!Serial.available());
      Serial.read();
      reset();
    }

}; Bomb B;
