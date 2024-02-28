#include "se_eeprom.h"

SE_EEPROM::SE_EEPROM()
{
  eeprom_size=0;
};

/*
  unsigned short SetEEPROMSize(unsigned short byte_count)
  Описание:
    Устанавка количества байт с которыми в дальнейшем будут работать все функции класса.
  Параметры:
    byte_count долно быть числом кратным 32 и не первышать максимальный размер EEPROM разделенный на 3.
    Функция зарезервиек блок памяти в 3 раза превышающий byte_count.
  Возвращаемое значение:
    Количество байт для работы.
*/    
unsigned short SE_EEPROM::SetEEPROMSize(unsigned short byte_count)
{
  unsigned short tmp;
  tmp=byte_count/32;
  tmp=tmp*32;
  eeprom_size=tmp;
  Serial.println();
  return tmp;
};

/*
  unsigned short GetEEPROMSize()
  Описание:
    Получение количества байт установленных функцией SetEEPROMSize. 
  Возвращаемое значение:
    Количество байт для работы.
*/    
unsigned short SE_EEPROM::GetEEPROMSize()
{
  return eeprom_size; 
};

/*
  bool ClearEEPROMBlock(unsigned short start_index, unsigned short count)
  Описание:
    Заполняет значением 0 ячейки данных EEPROM. Используется для частичной или полной очистки EEPROM.
  Параметры:
    start_index - начальный байт для очистки. Значение должны варьироваться в диапазоне от 0 до значения возвращаемого GetEEPROMSize()-1.
    count - количество байт для очистки.
    Важно учитывать чтоб start_index+count были меньше GetEEPROMSize()-1
*/
bool SE_EEPROM::ClearEEPROMBlock(unsigned short start_index, unsigned short count)
{
  unsigned short i,j;
  if (start_index>=eeprom_size) return false; // Если запрашивают с номером болшье, чем есть
  j=count;
  if (start_index+count>eeprom_size)
    j=eeprom_size-start_index;

  i=start_index;
  while (i<j)
  {
    WriteEEPROMByte(i, 0);
    i++;
  }
  return true;
};  

/*
  void EEPROMfix()
  Описание:
    Проверяет данные EEPROM на проверждения. Проверяется совпадение всех 3 экземпляров байта. 
    Если один из них отличается от остальных 2, происходит восстановление поврежденных данных. 
    Если все 3 экземпляра данных отличаются, всем 3 экземплярам присваивается значение 0.
*/    
void SE_EEPROM::EEPROMfix()
{
  unsigned short i;
  unsigned char res_0, res_1, res_2, total_res;
 
  for (int i=0;i<eeprom_size;i++)
  {
    res_0=EEPROM.read(i*3);
    res_1=EEPROM.read(i*3+1);  
    res_2=EEPROM.read(i*3+2);
    if ((res_0==res_1) && (res_1==res_2)) continue;
    if (res_0==res_1) {EEPROM.write(i*3+2,res_0); continue;}
    if (res_0==res_2) {EEPROM.write(i*3+1,res_0);continue;}
    if (res_1==res_2) {EEPROM.write(i*3+2,res_1);continue;}
    // Если все 3 значения разные
    EEPROM.write(i*3,0);
    EEPROM.write(i*3+1,0);
    EEPROM.write(i*3+2,0);
  }
};

/*
  unsigned char ReadEEPROMByte(unsigned short index)
  Описание:
    Чтение ячейки EEPROM по индексу.
  Параметры:
    index - индекс байта. Значение должны варьироваться в диапазоне от 0 до значения возвращаемого GetEEPROMSize()-1.
  Возвращаемое значение:
    Возрващается значение ячейки. Если index выходит за допустимые пределы, возвращется значение 255.
*/    
unsigned char SE_EEPROM::ReadEEPROMByte(unsigned short index)
{
  unsigned char res_0, res_1, res_2, total_res;

  if (index>=eeprom_size) return 255; // Если запрашивают с номером болшье, чем есть

  res_0=EEPROM.read(index*3);
  res_1=EEPROM.read(index*3+1);  
  res_2=EEPROM.read(index*3+2);

  if (res_0==res_1) return res_0;
  if (res_0==res_2) return res_0;
  if (res_1==res_2) return res_1;
  return 255;
};

/*
  bool WriteEEPROMByte(unsigned short index, unsigned char value)
  Описание:
    Запись значения в 3 экземплярах в EEPROM.
  Параметры:
    index - индекс байта. Значение должны варьироваться в диапазоне от 0 до значения возвращаемого GetEEPROMSize()-1.
    value - байт для записи
  Возвращаемое значение:
    true в случае успешного выполнения операции, false - ошибка
*/
bool SE_EEPROM::WriteEEPROMByte(unsigned short index, unsigned char value)
{
  if (index>=eeprom_size) return false;
  if (ReadEEPROMByte(index)==value) return true;

  EEPROM.write(index*3,value);
  EEPROM.write(index*3+1,value);
  EEPROM.write(index*3+2,value);
  return true;
};

/*
  String ReadEEPROMStr32(unsigned short start_index)
  Описание:
    Чтение строки из EEPROM начиная с заданного индекса. Чтение строки продолжается до 32 байт или первого встреченного значения 0.
  Параметры:
    index - индекс байта с которого начнётся чтение сроки. Значение должны варьироваться в диапазоне от 0 до значения возвращаемого GetEEPROMSize()-1.
  Возвращаемое значение:
    Строка
*/
String SE_EEPROM::ReadEEPROMStr32(unsigned short start_index)
{
  unsigned short i;
  char ch;
  String res="";
  if (start_index>=eeprom_size) return res; // Если запрашивают с номером болшье, чем есть

	for (i=0;i<32;i++)
	{
		if (start_index+i>=eeprom_size) break;
    ch=ReadEEPROMByte(i);
    if (ch!=0)
      res=res+ch;
    else
      break;		
	}	
  return res; 
};

/*
  bool WriteEEPROMStr32(unsigned short start_index, String str)
  Описание:
    Запись строки в EEPROM в 3 экземплярах. 
  Параметры:
    start_index - индекс байта с которого начнётся запись строки. Значение должны варьироваться в диапазоне от 0 до значения возвращаемого GetEEPROMSize()-1.
    value - строка для записи
  Возвращаемое значение:
    true в случае успешного выполнения операции, false - ошибка

*/
bool SE_EEPROM::WriteEEPROMStr32(unsigned short start_index, String str)
{
  unsigned short i,len;
  if (start_index>=eeprom_size) return false; // Если запрашивают с номером болшье, чем есть
	len=str.length();
	for (i=0;i<32;i++)
	{
		if (start_index+i>=eeprom_size) break;
		if (i<len)
			WriteEEPROMByte(start_index+i,str[i]);
		else
		{
			WriteEEPROMByte(start_index+i,0);		
			break;
		}
	}
  return true;  
};
