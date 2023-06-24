#include <Arduino.h>
#include <U8g2lib.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

DHT dht(2, DHT22);
U8G2_SSD1306_128X64_ALT0_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); // SSD1306 and SSD1308Z are compatible
 
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);    //Low spped I2C
 
void setup(void) {
  WiFiDrv::pinMode(25, OUTPUT);
  WiFiDrv::pinMode(26, OUTPUT);
  WiFiDrv::pinMode(27, OUTPUT);
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
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
    delay(1000);  
    return;
    }
  if (temp >= 30) {
    WiFiDrv::analogWrite(25, 255);
  }
  else if(temp < 30){
    WiFiDrv::analogWrite(27, 255);
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
  delay(1000);  
}
