#include <WiFi.h>                      // Thư viện kết nối WiFi.
#include <HTTPClient.h>                // Thư viện để tạo yêu cầu HTTP.
#include <WiFiClientSecure.h>          // Thư viện kết nối client bảo mật (SSL/TLS).
#include "freertos/FreeRTOS.h"         // Thư viện FreeRTOS cho đa nhiệm.
#include "freertos/task.h"             // Thư viện quản lý tác vụ của FreeRTOS.
#include <Arduino.h>                   // Thư viện Arduino cơ bản.
#include "FreeRTOSConfig.h"            // Tập tin cấu hình FreeRTOS.
#include <esp_task_wdt.h>              // Thư viện quản lý watchdog timer.
#define BLYNK_TEMPLATE_ID "TMPL6d3nOXz76"       // ID mẫu Blynk.
#define BLYNK_TEMPLATE_NAME "ESP32"             // Tên mẫu Blynk.
#define BLYNK_AUTH_TOKEN "-fUaRpB_YBcjXbGdDhmWzrGAR27sULlU" // Mã thông báo xác thực Blynk.
#define BLYNK_PRINT Serial                      // Cấu hình để in ra Serial monitor.
#include <BlynkSimpleEsp32.h>                   // Thư viện Blynk cho ESP32.
// Định nghĩa chân nút nhấn và led
#define bt0 15
#define led0 25
#define bt1 17
#define led1 26
#define bt2 16
#define led2 27
#define bt3 18
#define led3 14
#define bt4 19  
#define led4 12
//Định nghĩa chân ảo Blynk
#define VIRTUAL_PIN0 V0
#define VIRTUAL_PIN1 V1
#define VIRTUAL_PIN2 V2
#define VIRTUAL_PIN3 V3
#define VIRTUAL_PIN4 V4
//Biến trạng thái nút nhấn và đèn LED
int buttonState0 = HIGH;
int ledState0 = LOW;
int buttonState1 = HIGH;
int ledState1 = LOW;
int buttonState2 = HIGH;
int ledState2 = LOW;
int buttonState3 = HIGH;
int ledState3 = LOW;
int buttonState4 = HIGH;
int ledState4 = LOW;
//Biến cảm biến
float t = 0.0;
float h = 0.0;
int flameLevel = 0;
int gasLevel = 0;
//Thông tin kết nối WiFi và ThingSpeak
char auth[] = "-fUaRpB_YBcjXbGdDhmWzrGAR27sULlU";      // Mã thông báo xác thực Blynk.
char ssid[] = "NHOM THEM TRA SUA 5G";                 // SSID WiFi.
char pass[] = "0337027822";                           // Mật khẩu WiFi.
const int myChannelNumber = 2555593;                  // Số kênh ThingSpeak.
const char* writeAPIKey = "UUX4MOG2DG484L7S";         // Khóa API ThingSpeak.
const char* server = "api.thingspeak.com";            // Máy chủ ThingSpeak.
//Hàm điều khiển Blynk cho đèn LED
BLYNK_WRITE(V0) {
  int pinValue0 = param.asInt();    // Đọc giá trị từ chân ảo V0.
  ledState0 = pinValue0;            // Cập nhật trạng thái đèn LED 0.
  digitalWrite(led0, ledState0);    // Điều khiển đèn LED 0.
}
BLYNK_WRITE(V1) {
  int pinValue1 = param.asInt();    // Đọc giá trị từ chân ảo V1.
  ledState1 = pinValue1;            // Cập nhật trạng thái đèn LED 1.
  digitalWrite(led1, ledState1);    // Điều khiển đèn LED 1.
}
BLYNK_WRITE(V2) {
  int pinValue2 = param.asInt();    // Đọc giá trị từ chân ảo V2.
  ledState2 = pinValue2;            // Cập nhật trạng thái đèn LED 2.
  digitalWrite(led2, ledState2);    // Điều khiển đèn LED 2.
}
BLYNK_WRITE(V3) {
  int pinValue3 = param.asInt();    // Đọc giá trị từ chân ảo V3.
  ledState3 = pinValue3;            // Cập nhật trạng thái đèn LED 3.
  digitalWrite(led3, ledState3);    // Điều khiển đèn LED 3.
}
BLYNK_WRITE(V4) {
  int pinValue4 = param.asInt();    // Đọc giá trị từ chân ảo V4.
  ledState4 = pinValue4;            // Cập nhật trạng thái đèn LED 4.
  digitalWrite(led4, ledState4);    // Điều khiển đèn LED 4.
}
//Hàm kiểm tra nút nhấn và điều khiển đèn LED
void checkButton0() {
  if (digitalRead(bt0) == LOW) {            // Nếu nút nhấn bt0 được nhấn.
    if (buttonState0 == HIGH) {             // Nếu trạng thái nút trước đó là HIGH.
      ledState0 = !ledState0;               // Đổi trạng thái đèn LED 0.
      Blynk.virtualWrite(V0, ledState0);    // Cập nhật trạng thái đèn LED 0 trên Blynk.
      digitalWrite(led0, ledState0);        // Điều khiển đèn LED 0.
      delay(200);                           // Trì hoãn để tránh nhiễu.
    }
    buttonState0 = LOW;                     // Cập nhật trạng thái nút hiện tại là LOW.
  } else {
    buttonState0 = HIGH;                    // Nếu nút không được nhấn, trạng thái nút là HIGH.
  }
}
void checkButton1() {
  if (digitalRead(bt1) == LOW) {            // Nếu nút nhấn bt1 được nhấn.
    if (buttonState1 == HIGH) {             // Nếu trạng thái nút trước đó là HIGH.
      ledState1 = !ledState1;               // Đổi trạng thái đèn LED 1.
      Blynk.virtualWrite(V1, ledState1);    // Cập nhật trạng thái đèn LED 1 trên Blynk.
      digitalWrite(led1, ledState1);        // Điều khiển đèn LED 1.
      delay(200);                           // Trì hoãn để tránh nhiễu.
    }
    buttonState1 = LOW;                     // Cập nhật trạng thái nút hiện tại là LOW.
  } else {
    buttonState1 = HIGH;                    // Nếu nút không được nhấn, trạng thái nút là HIGH.
  }
}
void checkButton2() {
  if (digitalRead(bt2) == LOW) {
    if (buttonState2 == HIGH) {
      ledState2 = !ledState2;
      Blynk.virtualWrite(V2, ledState2);
      digitalWrite(led2, ledState2);
      delay(200);
    }
    buttonState2 = LOW;
  } else {
    buttonState2 = HIGH;
  }
}
void checkButton3() {
  if (digitalRead(bt3) == LOW) {
    if (buttonState3 == HIGH) {
      ledState3 = !ledState3;
      Blynk.virtualWrite(V3, ledState3);
      digitalWrite(led3, ledState3);
      delay(200);
    }
    buttonState3 = LOW;
  } else {
    buttonState3 = HIGH;
  }
}
void checkButton4() {
  if (digitalRead(bt4) == LOW) {
    if (buttonState4 == HIGH) {
      ledState4 = !ledState4;
      Blynk.virtualWrite(V4, ledState4);
      digitalWrite(led4, ledState4);
      delay(200);
    }
    buttonState4 = LOW;
  } else {
    buttonState4 = HIGH;
  }
}
void setup() {
  Serial.begin(9600);
  WiFi.disconnect();
  delay(2000);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  xTaskCreate( readData,"ReadData",4000,NULL,1,NULL);       
  xTaskCreate(sendData,"SendData",4000,NULL,1,NULL);      
  xTaskCreate(Task_Control,"Task_Control",8000,NULL,1,NULL);
  xTaskCreate(Task_Blynk_Loop,"loop1",6000,NULL,1,NULL);}
void loop() {
}
void readData(void* parameter) {
    while (1) {
      if (Serial.available()) {
        String data = Serial.readStringUntil('\n');
      if (data.startsWith("H:")) {
                int endIndex = data.indexOf(",");
                String humidityData = data.substring(2, endIndex);
                h = humidityData.toFloat();
                data = data.substring(endIndex + 1);
            }
      if (data.startsWith("T:")) {
                int endIndex = data.indexOf(",");
                String tempData = data.substring(2, endIndex);
                t = tempData.toFloat();
                data = data.substring(endIndex + 1);
            }
      if (data.startsWith("Gas:")) {
                int endIndex = data.indexOf(",");
                String gasData = data.substring(4, endIndex);
                gasLevel = gasData.toInt();
                data = data.substring(endIndex + 1);
            }
      if (data.startsWith("Flame:")) {
                String flameData = data.substring(6, data.length());
                flameLevel = flameData.toInt();
            }
  }
  delay(100);
}
}
void sendData(void* parameter) {
  while (1) {
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;
      String url = "http://" + String(server) + "/update?api_key=" + writeAPIKey;
      url += "&field1=" + String(t);
      url += "&field2=" + String(h);
      url += "&field3=" + String(flameLevel);
      url += "&field4=" + String(gasLevel);
      Serial.print("Temperature: ");
      Serial.println(t);
      Serial.print("Humidity: ");
      Serial.println(h);
      Serial.print("Flame level: ");
      Serial.println(flameLevel);
      Serial.print("Gas level: ");
      Serial.println(gasLevel);
      http.begin(client, url);
      int httpResponseCode = http.GET();
      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
    delay(1000);  // Gửi dữ liệu mỗi 10 giây
  }
}
void Task_Blynk_Loop(void *pvParameters) {
  (void)pvParameters;

  Blynk.begin(auth, ssid, pass);
  while (Blynk.connected() == false) {
  }
  Serial.println();
  Serial.println("Blynk Connected");
  Serial.println("Blynk Loop Task Started");
  while (1) {
    Blynk.run();
    delay(1);
  }
}
void Task_Control(void *pvParameters) {
  (void)pvParameters;
  pinMode(bt0, INPUT_PULLUP);
  pinMode(bt1, INPUT_PULLUP);
  pinMode(bt2, INPUT_PULLUP);
  pinMode(bt3, INPUT_PULLUP);
  pinMode(bt4, INPUT_PULLUP);
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  while (1) {
    checkButton0();
    checkButton1();
    checkButton2();
    checkButton3();
    checkButton4();
  }
}
