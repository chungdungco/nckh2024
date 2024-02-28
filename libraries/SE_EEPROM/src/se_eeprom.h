/*
	Библиотека предназначена для работы с EEPROM. Все данные сохраняются в 3 экземплярах для реализации возможности автоматического восстановления в случае частичного повреждения.
	Для работы с данными реализованы функции чтения/записи одиночных байтов и строк длиной не более 32 символов.
	Размер резервируемых для работы данных не должен превышать макимальный размер EEPROM контроллера деленный на 3. Функционал библиотеки не предусмативает контроль размера резервируемого блока. 
	Все функции класса используют в работе стандартные функции EEPROM.read и EEPROM.write. Они не включат в себя функции EEPROM.begin, EEPROM.commit EEPROM.end, которые нобходимо выполнять только с некоторыми типами контроллеров (NodeMCU).
	
	unsigned short SetEEPROMSize(unsigned short byte_count)
	Описание:
		Устанавка количества байт с которыми в дальнейшем будут работать все функции класса.
	Параметры:
		byte_count долно быть числом кратным 32 и не первышать максимальный размер EEPROM разделенный на 3.
		Функция зарезервиек блок памяти в 3 раза превышающий byte_count.
	Возвращаемое значение:
		Количество байт для работы.
	
	unsigned short GetEEPROMSize()
	Описание:
		Получение количества байт установленных функцией SetEEPROMSize. 
	Возвращаемое значение:
		Количество байт для работы.
		
  void ClearEEPROMBlock(unsigned short start_index, unsigned short count)
  Описание:
    Заполняет значением 0 ячейки данных EEPROM. Используется для частичной или полной очистки EEPROM.
  Параметры:
    start_index - начальный байт для очистки. Значение должны варьироваться в диапазоне от 0 до значения возвращаемого GetEEPROMSize()-1.
    count - количество байт для очистки.
    Важно учитывать чтоб start_byte+count были меньше GetEEPROMSize()-1
		
	void EEPROMfix()
	Описание:
		Проверяет данные EEPROM на проверждения. Проверяется совпадение всех 3 экземпляров байта. 
		Если один из них отличается от остальных 2, происходит восстановление поврежденных данных. 
		Если все 3 экземпляра данных отличаются, всем 3 экземплярам присваивается значение 0.

	unsigned char ReadEEPROMByte(unsigned short index)
	Описание:
		Чтение ячейки EEPROM по индексу.
	Параметры:
		index - индекс байта. Значение должны варьироваться в диапазоне от 0 до значения возвращаемого GetEEPROMSize()-1.
	Возвращаемое значение:
		Возрващается значение ячейки. Если index выходит за допустимые пределы или данные повреждены, возвращется значение 255.

	bool WriteEEPROMByte(unsigned short index, unsigned char value)
	Описание:
		Запись значения в 3 экземплярах в EEPROM.
	Параметры:
		index - индекс байта. Значение должны варьироваться в диапазоне от 0 до значения возвращаемого GetEEPROMSize()-1.
		value - байт для записи
	Возвращаемое значение:
		true в случае успешного выполнения операции, false - ошибка
		
	String ReadEEPROMStr32(unsigned short start_index)
	Описание:
		Чтение строки из EEPROM начиная с заданного индекса. Чтение строки продолжается до 32 байт или первого встреченного значения 0.
	Параметры:
		start_index - индекс байта с которого начнётся чтение сроки. Значение должны варьироваться в диапазоне от 0 до значения возвращаемого GetEEPROMSize()-1.
	Возвращаемое значение:
		Строка

	bool WriteEEPROMStr32(unsigned short start_index, String str)
	Описание:
		Запись строки в EEPROM в 3 экземплярах. 
	Параметры:
		start_index - индекс байта с которого начнётся запись строки. Значение должны варьироваться в диапазоне от 0 до значения возвращаемого GetEEPROMSize()-1.
		str - строка для записи
	Возвращаемое значение:
		true в случае успешного выполнения операции, false - ошибка

	Данная библиотека использует библиотеки	Arduino.h и EEPROM.h
	
  Created by Victor S. Bykov, March 31, 2021.
  Released into the public domain.
*/
#ifndef se_eeprom_h
#define se_eeprom_h
#include <Arduino.h>
#include <EEPROM.h>

class SE_EEPROM
{
  public:
		SE_EEPROM(); // Инициализация классов
		unsigned short SetEEPROMSize(unsigned short byte_count);
		unsigned short GetEEPROMSize();
		bool ClearEEPROMBlock(unsigned short start_byte, unsigned short count);
		void EEPROMfix();
		unsigned char ReadEEPROMByte(unsigned short index);
		bool WriteEEPROMByte(unsigned short index, unsigned char value);
		String ReadEEPROMStr32(unsigned short index);
		bool WriteEEPROMStr32(unsigned short index, String str);
  private:
		unsigned short eeprom_size;
};
#endif
