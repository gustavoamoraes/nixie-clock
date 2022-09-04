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
    int i = 0;
    int result = 0;
    while (i < k)
    { 
        result = number % 10;
        number = number / 10;
        list[(k-1)-i] = result;
        i++;
    }

    return result;
}

void Profile::displayNumber (size_t number)
{
  int digits [NIXIE_COUNT] {0,0,0,0,0,0};

  if(number == 0)
    return setDigits(digits);

  extractDigits(number, NIXIE_COUNT, digits);
  setDigits(digits);
}