#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

#include <SoftwareSerial.h>
SoftwareSerial wifi(12, 13);
#include <dht11.h>
#define dht_apin 2
int redPin = 11;
int greenPin = 10;
int bluePin = 9;
String comdata = "";
String cmdStr = "";
char LED = '0';
char AC = '0';
dht11 DHT;
char humidity = "";
char temperature = "";
String sendStatus = "";
int criticalTemp = 30;
unsigned int turnOn_AC[74] = {8850, 4450, 650, 550, 600, 1650, 600, 600, 600, 1650, 650, 1650, 650, 500, 650, 550, 650, 550,
                              600, 600, 600, 1650, 650, 1650, 600, 1650, 600, 600, 650, 550, 600, 550, 650, 550, 600, 600, 600, 600, 600, 550, 650, 550, 650, 550, 600, 1650,
                              650, 550, 600, 600, 600, 600, 600, 550, 650, 550, 650, 550, 600, 1650, 650, 550, 600, 1700, 600, 550, 650, 550, 600, 1700, 600, 550, 650, 23458
                             };//红外信号指令
IRsend irsend;                          // 定义 IRsend 物件来发射红外线讯号

void setColor(int red, int green, int blue)
{
  analogWrite(redPin, 255 - red);
  analogWrite(greenPin, 255 - green);
  analogWrite(bluePin, 255 - blue);
}

void getComdata() {
  if (comdata != "") {
    comdata = "";
  }
  while (wifi.available() > 0) {
    comdata += char(wifi.read());
    delay(4);
  }
}

void getConnect() {
  wifi.println("AT+CIPSTART=\"TCP\",\"i5nmvood.qcloud.la\",80");
  delay(2000);
  getComdata();
  if (comdata.length() > 0) {
    comdata.trim();//将字符串中的字符串去除，包括空格、回车、Tab等
    Serial.println(comdata);
  }
}

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Serial Connected!");
  wifi.begin(9600);
  wifi.println("wifi serial connected!");
  getComdata();
  getConnect();
  delay(500);
  getComdata();
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
  Serial.println("Set up done !");
}

void loop() {
  if (LED == '1') {                 //打开LED
    setColor(255, 255, 255);
  } else {
    setColor(0, 0, 0);
  }
  if (AC == '1') {
    irsend.sendRaw(turnOn_AC, 74, 32); //打开空调
  } else if (AC == '0' && int(DHT.temperature) > criticalTemp) {
    irsend.sendRaw(turnOn_AC, 74, 32); //如果温度高于30度，则自动打开空调
  } else {
    irsend.sendRaw(turnOn_AC, 74, 32); //关闭空调
  }
  wifi.println("GET http://i5nmvood.qcloud.la/weapp/getstatus");
  wifi.println("");
  delay(1000);
  getComdata();
  if (comdata.length() > 0) {
    Serial.println("");
    Serial.println(comdata);
    LED = comdata.charAt(29);
    AC = comdata.charAt(63);
    comdata = "";
  }
  delay(2000);
  humidity = char(DHT.humidity);
  temperature = char(DHT.temperature);
  sendStatus = humidity + "-" + temperature;
  cmdStr = "GET http://i5nmvood.qcloud.la/weapp/setLED?id=2&status=" + sendStatus;
  wifi.println(cmdStr);
  delay(500);
}
