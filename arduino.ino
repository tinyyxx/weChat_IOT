#include <SoftwareSerial.h>


SoftwareSerial wifi(10, 11);
String comdata = "";
char LED = '0';

void setup() {
  Serial.begin(9600);
  Serial.println("Serial Connected!");
  wifi.begin(9600);
  wifi.println("wifi serial connected!");
  pinMode(13, OUTPUT);
}

void getComdata()
{
  while (wifi.available() > 0) {
    comdata += char(wifi.read());
    delay(2);
  }
}

void connect() {
  wifi.println("AT+CIPSTART=\"TCP\",\"wechatiot.applinzi.com\",80");
  getComdata();
  if (comdata.length() > 0) {
    comdata.trim();
    Serial.println(comdata);
  }

}

void loop() {
  connect();

  while (comdata.indexOf("CONNECT") == -1) {
    delay(3000);
    connect();
  }

  comdata = "";

  wifi.println("AT+CIPMODE=1");
  delay(200);
  getComdata();
  if (comdata.length() > 0) {
    Serial.println(comdata);
    comdata = "";
  }

  wifi.println("AT+CIPSEND");
  delay(200);
  getComdata();
  if (comdata.length() > 0) {
    Serial.println(comdata);
    comdata = "";
  }

  while (true) {
    if (LED == '0') {
      digitalWrite(13, LOW);
    }

    if (LED == '1') {
      digitalWrite(13, HIGH);
    }

    delay(100);

    wifi.println("GET http://wechatiot.applinzi.com/copydownup.php?token=leyizhou HTTP/1.0\r\n\r\n\r\n");
    wifi.println("");
    //delay(1000);
    getComdata();

    delay(100);
    if (comdata.length() > 0) {
      Serial.println("");
      Serial.println(comdata);
      int i = comdata.indexOf("{");
      if (i != -1)
        LED = comdata.charAt(i + 1);
      comdata = "";
    }
    delay(200);

  }



}
