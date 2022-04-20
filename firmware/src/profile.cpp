#include <Arduino.h>
#include "profile.h"

int Profile::setDigits (const char* digits)
{
  for (size_t i = 0; digits[i] != '\0'; i++)
  {
    int digit = digits[i] - '0';
    if (digit > 9 || digit < 0) //Not a number
      return 0; 
    nixies[i].displayDigit = digit;
  }
  return 1;
}