#include <SparkFun_APDS9960.h>
#include <SPI.h>
#include <WiFi.h>
#include <Wire.h>

//http://178.62.25.163:4000/api/v1/sensors/1

// Global variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
uint16_t ambient_light = 0;

// Local Network Settings
char ssid[] = "mineee";  // your network SSID (name)
char password[] = "warcraft28";    // your network password
int keyIndex = 0;             // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(4000);

// ThingSpeak Settings
char thingSpeakAddress[] = "178.62.25.163/api/v1/test";
String APIKey = "9P57ISVBC3S83NXJ";             // enter your channel's Write API Key
const int updateThingSpeakInterval = 20 * 1000; // 20 second interval at which to update ThingSpeak

// Variable Setup
long lastConnectionTime = 0;
boolean lastConnected = false;

// Initialize Arduino Ethernet Client
WiFiClient client;

void setup() {
  // Start Serial for debugging on the Serial Monitor
  Serial.begin(9600);

  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, password);
    // wait 10 seconds for connection:
    delay(10000);
  } 

  printWifiStatus();

  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }
  
  // Start running the APDS-9960 light sensor (no interrupts)
  if ( apds.enableLightSensor(false) ) {
    Serial.println(F("Light sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during light sensor init!"));
  }
  // Wait for initialization and calibration to finishM
  delay(500);
}

#define POSTS1 "{\"temperature\": %f,\"luminosity\": %d,\"seats\": "
#define POSTS2 "{\"1\": %s,\"2\": %s,\"3\": %s,\"4\": %s}}"

char posts1[128];
char posts2[128];

void loop() {
  
  if (  !apds.readAmbientLight(ambient_light) ) {
      Serial.println("Error reading light values");
    } 

  sprintf(posts1, POSTS1, 24.5, 1200);
  sprintf(posts2, POSTS2, "true","false","false","true");

  // Print Update Response to Serial Monitor
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  // Disconnect from ThingSpeak
  if (!client.connected() && lastConnected) {
    Serial.println("...disconnected");
    Serial.println();
    client.stop();
  }
  // Update ThingSpeak
  if (!client.connected() && (millis() - lastConnectionTime > updateThingSpeakInterval)) {
    postRequest(posts1,posts2);
    Serial.println("Posting to Thingspeak..."); 
  }
  lastConnected = client.connected();
  delay(5000);
}

void postRequest(String data1, String data2) {
  IPAddress my_server(178,62,25,163);
    
    if (client.connect(my_server, 4000)) {
      client.print("POST /api/v1/sensors/1 HTTP/1.1\r\n");
      client.print("Host: 178.62.25.163\r\n");
      //client.print("Urser-Agent: Aduino/1.0\r\n");
      client.print("Connection: close\r\n");
      client.print("Accept: application/json\r\n");
      client.print("Content-Type: application/json;charset=utf-8\r\n");
      client.print("Content-Length: ");
      client.print((data1.length()+data2.length()));
      client.print("\r\n");
      client.print("\r\n");
      client.print(data1);
      client.print(data2);
      client.print("\r\n");
      lastConnectionTime = millis();
      if(client.connected()) {
        Serial.println("connected to server and posting: ");
        Serial.println();
      }
    }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
