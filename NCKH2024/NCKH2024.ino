#include <SPI.h>
#include <SD.h>
#include <TinyGPS++.h>

File myFile;
const int CS = 5;

long time1 = 0;
String command = "AT+CLBS=4,1";
int timeoffset = 7;

void AppendFile(const char * path, const char * message){
  myFile = SD.open(path, FILE_APPEND); // Mở tệp để nối dữ liệu
  if (myFile) {
    myFile.println(message);
    myFile.close();
  } 
  else {
    Serial.println("error opening file ");
    Serial.println(path);
  }
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200); // Baudrate phù hợp với module SIM A7672S
  Serial.println("The device started, now you can pair it with bluetooth!");

  // Initializing SD card
  Serial.println("Initializing SD card...");
  if (!SD.begin(CS)) {
    Serial.println("Initialization failed!");
    return;
  }
  Serial.println("Initialization done.");

  // Open file for writing
  AppendFile("/gps_data.txt", "Latitude,Longitude,Time");
}
void parseCLBSData(String data) {
  int comma1 = data.indexOf(',');
  int comma2 = data.indexOf(',', comma1 + 1);
  int comma3 = data.indexOf(',', comma2 + 1);
  int comma4 = data.indexOf(',', comma3 + 1);
  int comma5 = data.indexOf(',', comma4 + 1);
  int comma6 = data.indexOf(',', comma5 + 1);

  String _latitude = data.substring(comma1 + 1, comma2);
  String _longitude = data.substring(comma2 + 1, comma3);
  String _datetime = data.substring(comma4 + 1, comma5);
  String _time = data.substring(comma5 + 1, comma6);

  int hour = _time.substring(0, 2).toInt();
  hour = (hour + timeoffset) % 24; // Adjusting timezone
  _time = String(hour) + _time.substring(2);

  String dataString = _latitude + "," + _longitude + "," + _time;
  AppendFile("/gps_data.txt", dataString.c_str()); // Gọi hàm AppendFile để nối dữ liệu
}

void loop() {
  // Reading data from SIM A7672S
  if (Serial2.available() > 0) {
    String receivedData = Serial2.readStringUntil('\n');
    receivedData.trim();
    if (receivedData.startsWith("+CLBS:")) {
      parseCLBSData(receivedData);
    }
  }

  // Sending command to SIM A7672S for obtaining location data
  if (millis() > time1 + 1000) {
    Serial2.println(command);
    time1 = millis();
  }
  //   // Sending command to SIM A7672S for obtaining location data
  // if (millis() > time1 + 1000) {
  //   Serial2.println(command);
  //   time1 = millis();
  // }
    //   // Sending command to SIM A7672S for obtaining location data
  // if (millis() > time1 + 1000) {
  //   Serial2.println(command);
  //   time1 = millis();
  // }
}