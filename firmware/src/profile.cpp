#include <Arduino.h>
#include "profile.h"
#include <cstdio>

void Profile::setDigits (const int (&new_digits)[NIXIE_COUNT])
{
  for (size_t i = 0; i < NIXIE_COUNT; i++)
  {
    nixies[i].setDigit(new_digits[i]);
  }
}

int extractDigits (int number, int k, int* list)
{
    int i = k;
    int result = 0;
    while (i < k)
    { 
        result = number % 10;
        number = number / 10;
        list[i] = result;
        i--;
    }

    return result;
}

void Profile::displayNumber (int number)
{
  int digits [NIXIE_COUNT];
  int log = log10(number);
  extractDigits(number, log, digits);
  setDigits(digits);
}