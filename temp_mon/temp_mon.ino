#include <Arduino.h>
#include <U8g2lib.h>
#include <DHT.h>
#include <DHT_U.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

int buttonState = 0;
int buttonSwitch = 1;

const int buttonPin = 1;
const int redLedPin = 25;
const int greenLedPin = 26;
const int blueLedPin = 27;

DHT dht(2, DHT22);
U8G2_SSD1306_128X64_ALT0_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); // SSD1306 and SSD1308Z are compatible
 
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);    //Low spped I2C
 
void setup(void) {
  WiFiDrv::pinMode(redLedPin, OUTPUT); // red
  WiFiDrv::pinMode(greenLedPin, OUTPUT); // green
  WiFiDrv::pinMode(blueLedPin, OUTPUT); // blue
  WiFiDrv::pinMode(buttonPin, OUTPUT); // button
  Serial.begin(9600);
  Serial.println(F("starting temperature and humidity monitor"));
  u8g2.begin();
  u8g2.clearBuffer();
  Wire.begin();
  dht.begin();
}
 
void loop(void) {
  float temp, hum;

  hum = dht.readHumidity();
  temp = dht.readTemperature();
  
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  
  if (isnan(hum) || isnan(temp)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    u8g2.drawStr(0,21,"no sensor!");
    u8g2.sendBuffer();

    WiFiDrv::analogWrite(25, 255);
    WiFiDrv::analogWrite(26, 0);
    WiFiDrv::analogWrite(27, 0);
    
    delay(1000);  
    return;
  }

  buttonState = digitalRead(buttonPin);
  
  Serial.println(buttonState);
  if (buttonState == 1){
    if (buttonSwitch == 1){
      buttonSwitch = 0;
    }else{
      buttonSwitch = 1;
    }
  }

  if (buttonSwitch == 0){
    if (temp >= 32 || hum >= 70 ) {
      WiFiDrv::analogWrite(25, 255);
      WiFiDrv::analogWrite(26, 0);
      WiFiDrv::analogWrite(27, 0);
    }
    else if(temp >= 30 || hum >= 60){
      WiFiDrv::analogWrite(25, 128);
      WiFiDrv::analogWrite(26, 128);
      WiFiDrv::analogWrite(27, 0);
    }
    else if(temp >= 25 || hum >= 55){
      WiFiDrv::analogWrite(25, 64);
      WiFiDrv::analogWrite(26, 128);
      WiFiDrv::analogWrite(27, 0);
    }
    else if(temp < 25 || hum < 55){
      WiFiDrv::analogWrite(25, 0);
      WiFiDrv::analogWrite(26, 128);
      WiFiDrv::analogWrite(27, 0);
    }
  }else{
      WiFiDrv::analogWrite(25, 0);
      WiFiDrv::analogWrite(26, 0);
      WiFiDrv::analogWrite(27, 0);
  }
  
  // print to monitor
  Serial.print(F("Humidity: "));
  Serial.print(hum);
  Serial.print(F("%  Temperature: "));
  Serial.print(temp);
  Serial.println(F("°C "));

  // print to oled
  u8g2.drawStr(0,10,"temp: ");
  u8g2.drawStr(35,10, String(temp).c_str()); 
  u8g2.drawStr(70,10,"C");
  u8g2.drawStr(0,21,"hum: ");
  u8g2.drawStr(35,21, String(hum).c_str()); 
  u8g2.drawStr(70,21,"%");

  u8g2.sendBuffer();
  delay(100);  
}
