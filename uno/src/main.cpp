#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0x00, 0xAA, 0xBB,0xCC,0xDE,0x02};
IPAddress ip(192,168,0,119);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

EthernetServer server(23);
boolean gotAMessage = false;

String message = "";

void setup() {
  Serial.begin(9600);
  while (!Serial){

  }

  Ethernet.begin(mac,ip,gateway,subnet);
  Serial.print("My IP adress: ");
  ip = Ethernet.localIP();
  for (byte thisByte = 0;thisByte < 4;thisByte++){
    Serial.print(ip[thisByte],DEC);
    Serial.print(".");
  }
  Serial.println();

  server.begin();
}

void loop() {
  EthernetClient client = server.available();

  if (client){
    if (!gotAMessage){
      Serial.println("->New Client");
      client.println("Hello");
      gotAMessage = true;
    }

    char thisChar = client.read();

    if (thisChar == '\r'){
      if(message.indexOf("disconnect") > -1){
        Serial.println("->Client closed connection");
        server.write(":( \n");
        client.stop();
        message = "";
      }else{
        server.write("received \n");
        Serial.println(message);
        message = "";
      }
    }else{
      message += thisChar;
    }

  }
}