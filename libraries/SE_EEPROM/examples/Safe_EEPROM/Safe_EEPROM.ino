/*
The library is designed to work with EEPROM. All data is stored in 3 instances to enable automatic recovery in case of partial damage.
The library has functions for reading/writing single bytes and rows with a maximum length of 32 characters.
The size of data reserved for operation must not exceed the maximum EEPROM size of the controller divided by 3. The library functionality does not provide for size control of the protected unit.
All functions of the class use the standard functions EEPROM.read and EEPROM.write. They do not include the functions EEPROM.begin, EEPROM.commit EEPROM.end, which only need to be performed with some types of controllers (NodeMCU).

This library uses Arduino.h and EEPROM.h libraries.
*/

#include <se_eeprom.h>

void setup() 
{
  unsigned short i;
  SE_EEPROM my_eeprom;
  unsigned short eeprom_size=256;
  unsigned char ver=1;
  bool res_b; 
  Serial.begin(9600);
  //Задаем классу размер EEPROM
  my_eeprom.SetEEPROMSize(256);
  
  Serial.print("Setting the EEPROM memory size for the library: ");
  Serial.println(my_eeprom.GetEEPROMSize());
  
  Serial.println("We search for corrupted data and restore it");
  my_eeprom.EEPROMfix();

  Serial.println("We check the first launch on this controller");
  if (my_eeprom.ReadEEPROMByte(1)!=ver)
  {
    Serial.println("New controller. Clearing EEPROM...");
    res_b=my_eeprom.ClearEEPROMBlock(0,my_eeprom.GetEEPROMSize());
    if (res_b)
      Serial.println("Operation completed successfully.");
    else
      Serial.println("Operation failed");
      
    // Set old controller flag
    my_eeprom.WriteEEPROMByte(1, ver);
  }


  Serial.println("Sample of writting chars");
  for (i=0;i<eeprom_size;i++)
  {
    Serial.print("Try to set EEPROM[");
    Serial.print(i);
    Serial.print("] value ");
    Serial.print(i);
    Serial.print("...");
    
    if (my_eeprom.WriteEEPROMByte(i, i))
      Serial.println("OK");
    else
      Serial.println("ERR");
  }

  Serial.println("Sample of reading chars");
  for (i=0;i<eeprom_size;i++)
  {
    Serial.print("Current EEPROM[");
    Serial.print(i);
    Serial.print("] value ");
    Serial.println(my_eeprom.ReadEEPROMByte(i));
  }

  Serial.println("Sample of writing string");
  my_eeprom.WriteEEPROMStr32(64, "Hello World!!!");
  Serial.println("Sample of reading string");  
  Serial.println(my_eeprom.ReadEEPROMStr32(64));

};

void loop() {
  // put your main code here, to run repeatedly:

}
