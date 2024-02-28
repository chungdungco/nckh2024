#include <SPI.h>
#include <SD.h>
#include <TinyGPS++.h>

File myFile;
const int CS = 5;
const unsigned long FILE_SAVE_INTERVAL = 30000; // Thời gian giữa các lần lưu tệp tin (30 giây)
unsigned long lastFileSaveTime = 0; // Thời điểm lưu tệp tin cuối cùng

long time1 = 0;
String command = "AT+CLBS=4,1";
int timeoffset = 7;
bool isRecording = false; // Trạng thái ghi/lưu

const int buttonPin = 2; // Chân của nút nhấn
int buttonState = 0; // Trạng thái của nút nhấn trước đó
int lastButtonState = LOW; // Trạng thái của nút nhấn trước đó

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

  // Thiết lập chân nút nhấn
  pinMode(buttonPin, INPUT);
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

  // Kiểm tra xem đã đến lúc lưu tệp tin mới chưa
  unsigned long currentTime = millis();
  if (currentTime - lastFileSaveTime >= FILE_SAVE_INTERVAL) {
    // Tạo tên tệp mới dựa trên thời gian hiện tại
    String filename = "/gps_data_" + String(currentTime) + ".txt";
    // Lưu dữ liệu vào tệp tin mới
    AppendFile(filename.c_str(), dataString.c_str());
    // Cập nhật thời gian lưu tệp tin cuối cùng
    lastFileSaveTime = currentTime;
  } else {
    // Nếu chưa đến thời điểm lưu tệp tin mới, chỉ nối dữ liệu vào tệp tin hiện tại
    AppendFile("/gps_data.txt", dataString.c_str());
  }
}

void loop() {
  // Đọc trạng thái của nút nhấn
  buttonState = digitalRead(buttonPin);

  // Nếu nút nhấn được nhấn (điều kiện chuyển đổi)
  if (buttonState != lastButtonState) {
    // Nếu trạng thái ghi/lưu là false (không ghi)
    if (!isRecording) {
      isRecording = true; // Bắt đầu ghi/lưu
      Serial.println("Recording started...");
    } else {
      isRecording = false; // Dừng ghi/lưu
      Serial.println("Recording stopped...");
    }
  }

  // Lưu trạng thái nút nhấn cho lần lặp tiếp theo
  lastButtonState = buttonState;

  // Nếu đang ghi/lưu
  if (isRecording) {
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
  }
}
