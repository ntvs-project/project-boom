#pragma once

String version = "test";

void (*reset) (void) = 0;

unsigned int MINUTE = 5;
unsigned int SECOND = 0;

int OUT[] = {
  3, 4, 5, 6, 7, 8, 9, // 7seg
  10, 11, 12, 13, // scan
};

int IN[] = {
  14, // reset
  15, 16, 17, 18, 19, // test
};
