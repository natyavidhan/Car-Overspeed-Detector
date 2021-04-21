#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>
#include "TinyGPS++.h"

//for alarm fuction
float lat[10]={28.6300, 28.6309, 28.7087, 28.6849, 28.6524, 28.6499, 28.6557, 28.6460, 28.5990, 28.6924};//latitude of all the police stations
float lon[10]={77.2142, 77.2255, 77.1181, 77.2026, 77.1929, 77.2175, 77.2190, 77.2081, 77.1241, 77.1738};//longitude of all the police station
const char * addresses [10] = {"192.168.1.102", "192.168.1.104", "192.168.1.102", "192.168.1.104", "192.168.1.102", "192.168.1.104", "192.168.1.102",  "192.168.1.104", "192.168.1.102", "192.168.1.104"};
const char *ssid = "ssid";  //ENTER YOUR WIFI SETTINGS
const char *password = "password";
const char* car = "DL5SAB9876";
const char* host = "overspeed-backend-server.studiousgamer.repl.co";
const int httpsPort = 443;
const char* fingerprint = "52 23 E7 B8 A3 34 71 A7 5E 9D 87 E8 D7 88 A6 34 38 E6 85 5A";
float mylat;
float mylon;
float D[10];


// speed detection
#define IRSENSOR_PIN 4
long newTime = millis();
long oldTime = millis();
bool lastRead = 0;
bool newRead = 0;
#define BLACK 0
#define WHITE 1
float Time;


//GPS
TinyGPSPlus gps;// GPS object to process the NMEA data
SoftwareSerial serial_connection(5,6);


void setup () {
  Serial.begin(115200);
  serial_connection.begin(9600);     //This opens up communications to the GPS
}


void formulas(){
  for(int i=0; i<10; i++)
  {float a=sqrt((sq(mylat-lat[i]))+(sq(mylon-lon[i])));
     D[i]={a};
    // Serial.println(D[i]);
    
  }
}
int smallest(float* array1, int length){
    int index = 0;
    float small = array1[index];
    for(int i = 0; i < length; i++){
      if(small > array1[i]){
        index = i;
        small= array1[i];
      }
    }
    return index;
  }


void coor() {
  while (serial_connection.available())
  {
    gps.encode(serial_connection.read());
  }
  if (gps.location.isUpdated()) {
    // Get the latest info from the gps object
    mylat = (gps.location.lat(), 6);
    mylon = (gps.location.lng(), 6);
  }
}


void msg()
{
//  coor();
  mylat = 28.6500;
  mylon = 77.2500;
  formulas();
  int ps = smallest(D, 10);
  const char* ip = addresses[ps];
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }

  String url = "/secret/?lat="; //the request URL is hidden so that now one enters random values in the database
  url += mylat;
  url += "&lon=";
  url += mylon;
  url += "&car_no=";
  url += car;
  url += "&station=";
  url += "1";
//  url += ps;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
  delay(100000);
}

void loop() {
      newRead = digitalRead(IRSENSOR_PIN);
  if (newRead == BLACK && newRead != lastRead) {
    //    Serial.print("Black ");
    lastRead = newRead;
  }
  if (newRead == WHITE && newRead != lastRead) {
    //    Serial.print("White ");
    newTime = millis();
    lastRead = newRead;
    Time = (newTime - oldTime);
    oldTime =  millis();
    Serial.println((28 / Time) * 36);
    if ((28 / Time) * 36 > 50) {
      Serial.println("Overspeeding");
      msg();
    }
    else {
      Serial.println("Normal Speed");
    }
  }

}
