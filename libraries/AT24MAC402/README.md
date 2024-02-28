# AT24MAC402/602 Arduino library

Arduino library for Microchip (former Atmel) AT24MAC402/602 EEPROM with build in MAC address and serial number.<br>
The chip uses internally two I2C addresses for different memory areas.<br>
Only 1k bit can used for R/W operations by the user.

- 0x50 - 0x57 (7bit address) for R/W operations of the first 128 bytes of the EEPROM
- 0x58 - 0x5F (7bit address) for Read Only operations to get the MAC (EUI-48) address and the 128 bit serial number (UUID), also the EUI-64 only for the AT24MAC602 EEPROM type
- Depending of the package you can set 8 different I2C start addresses, beware that the address pins set I2C address of both memory areas

## Example
Here is a simple example which shows the capabilities of the library

```cpp
#include "AT24MAC402.h"

AT24MAC402 at24mac402;
uint8_t mac[6];
uint8_t uuid[16];

void setup() {
  Serial.begin(9600);
  delay(2000);
  at24mac402.begin();
  at24mac402.readMac(mac);
  Serial.print("MAC address ");
  Serial.println(MACtoString(mac));
  at24mac402.readUUID(uuid);
  Serial.print("UUID ");
  Serial.println(UUIDtoString(mac));
  for (uint8_t i = 0; i < 6; i++) {
    at24mac402.writebyte(i, i);
    }
  for (uint8_t i = 0; i < 6; i++) {
    Serial.print("Byte ");
    Serial.print(i);
    Serial.print(" Value ");
    Serial.println(at24mac402.readbyte(i));
    }
  }
  
void loop() {
  }
```

# Documentation

## Constructor
```cpp
AT24MAC402(uint8_t a0 = 0, uint8_t a1 = 0, uint8_t a2 = 0);
```
- **a0** set address pin A0 to HIGH or LOW
- **a1** set address pin A1 to HIGH or LOW
- **a2** set address pin A2 to HIGH or LOW

Create an EEPROM object with an optional settings of the address pins A0, A1 and A2.

**Example**

```cpp
AT24MAC402 at24mac402; // all address pins to GND using Wire
AT24MAC402 at24mac402(0, 0, 1); // address pin A2 high using Wire
```

## Methods

### **begin()**

```cpp
void begin(TwoWire &wire = Wire);
```
- **wire** set optional Wire class to use (Wire, Wire1, Wire2 ...)

Initialize the Wire (I2C) communication of the EEPROM.<br>
This must done in ```setup()```

**Example**

```cpp
setup() {
  at24mac402.begin();
  at24mac402.begin(Wire1); // for use with e.g. Wire1
  }
```

### **writeByte()**
```cpp
void writeByte(uint8_t address, uint8_t data);
```

- **address** set the EEPROM address
- **data** set the data

Write a single byte into the EEPROM.

**Example**

```cpp
at24mac402.writeByte(2, 23); // write the value 23 to EEPROM address 2
```

### **readByte()**

```cpp
uint8_t readByte(uint8_t address)
```

- **address** set the EEPROM address

Read a single byte from the EEPROM.<br>
Returns the stored value of the given address.

**Example**

```cpp
uint8_t data = at24mac402.readByte(2); // read the value from EEPROM address 2
```

### **readMac()**

```cpp
void readMac(uint8_t mac[]);
uint8_t* readMac();
```

- **mac** pointer to the mac address

Returns the MAC (EUI-48) address.

**Example**

```cpp
uint8_t mac[6];
at24mac402.readMac(mac);
```

### **readEUI64()**

```cpp
void readEUI64(uint8_t eui64[]);
uint8_t* readEUI64();
```

- **eui64address** pointer to the EUI64 address

Returns the EUI-64 address. Only for AT24MAC602!

**Example**

```cpp
uint8_t eui64[8];
at24mac402.readEUI64(eui64);
```

### **readUUID()**

```cpp
void readUUID(uint8_t uuid[]);
uint8_t* readUUID();
```

- **uuid** pointer to the UUID address

Returns the UUID (128bit serial number).

**Example**

```cpp
uint8_t uuid[16];
at24mac402.readUUID(uuid);
```

## Helper functions

### **MACtoString()**

```cpp
void MACtoString(uint8_t mac[], char macString[]);
char* MACtoString(uint8_t mac[]);
```

- **mac** pointer to the MAC address
- **macString** pointer to the formated string

Returns the MAC address as a formated string.

**Example**

```cpp
Serial.println(MACtoString(mac));
```

### **UUIDtoString()**

```cpp
void UUIDtoString(uint8_t uuid[], char uuidString[]);
char* UUIDtoString(uint8_t uuid[]);
```

- **uuid** pointer to the UUID
- **uuidString** pointer to the formated string

Returns the UUID as a formated string.

**Example**

```cpp
Serial.println(UUIDtoString(uuid));
```