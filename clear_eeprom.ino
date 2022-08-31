#include <EEPROM.h>

void setup()
{
  for (int i = 0; i < 255; i++)
  {
    EEPROM.write(i, 0);
  }
  EEPROM.write(25,1);
}

void loop()
{
}
