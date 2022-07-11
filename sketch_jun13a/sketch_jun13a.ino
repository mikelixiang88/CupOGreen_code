#include <SoftwareSerial.h>
#include <dht.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
SoftwareSerial mySerial(2, 3); // RX, TX
int ledPin = 11;
int motorpin = 9;
dht DHT;

#define DHT11_PIN 7
Adafruit_TCS34725 tcs = Adafruit_TCS34725();

void setup(void) {
  // put your setup code here, to run once:
  mySerial.begin(9600);
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(motorpin, OUTPUT);
  
  digitalWrite(motorpin,HIGH);
  if(tcs.begin()){
    Serial.println("Found sensor");
    sendCommand("AT");
    sendCommand("AT+ROLE=2");
    sendCommand("AT+NAME=CupOGreen");
  }
  else{
    Serial.println("Not Found");  
  }
}

void sendCommand(const char * command){
  Serial.print("Command send :");
  Serial.println(command);
  mySerial.println(command);
  //wait some time
  delay(100);
  
  char reply[100];
  int i = 0;
  while (mySerial.available()) {
    reply[i] = mySerial.read();
    i += 1;
  }
  //end the string
  reply[i] = '\0';
  Serial.print(reply);
  Serial.println("Reply end");
}
void readColor(){
  uint16_t r,g,b,c;
  tcs.getRawData(&r,&g,&b,&c);
  Serial.print("R: ");
  Serial.println(r, DEC);
  Serial.print("G: ");
  Serial.println(g, DEC);
  Serial.print("B: ");
  Serial.println(b, DEC);
  Serial.print("C: ");
  Serial.println(c, DEC);       
}
void light(char val){
  analogWrite(ledPin, val);
}

void readSerial(){
  char reply[50];
  int i = 0;
  while (mySerial.available()) {
    reply[i] = mySerial.read();
    i += 1;
  }
  //end the string
  reply[i] = '\0';
  if(strlen(reply) > 0){
    Serial.print(reply);
    Serial.println("We have just read some data");
    light(reply[0]);
    writeToBLE(reply[0]);
  }
}

void writeToBLE(char value) {
  Serial.print("Writing hex :");
  Serial.println(value, HEX);
  mySerial.write(value);
}

void send_data(){
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  delay(1000);
}


void loop() {
  
  readSerial();
  int brightness=analogRead(A0);
  Serial.print("Brightness =");
  //readColor();
  send_data();
}
