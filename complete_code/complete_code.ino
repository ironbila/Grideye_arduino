#include <SparkFun_APDS9960.h>
#include <SPI.h>
#include <WiFi.h>
#include "Arduino.h"
#include <grideye.h>
#include <GE_SoftUart.h>
#include <GE_SoftUartParse.h>
#include <stdio.h>
#include <string.h>

#define MEASURE_NO 128
#define POSTS1 "{\"temperature\": %f,\"luminosity\": %d,\"seats\": "
#define POSTS2 "{\"1\": %s,\"2\": %s,\"3\": %s,\"4\": %s}}"
#define HUMAN_CAP 14

struct t_area_s {
  double t_min;
  double t_max;
  double av_t_min;
  double av_t_max;
  double curr_t;
  int human_detected;
  int human_ct;
  double av_array[MEASURE_NO];
} t_area_default = {100.0,0.1,100.0,0.1,0.0,0,0};

typedef struct t_area_s t_area;

// Global variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
uint16_t ambient_light = 0;

char posts1[128];
char posts2[128];

t_area t_area1=t_area_default, t_area2=t_area_default, t_area3=t_area_default, t_area4=t_area_default;

//grideye variables
static uint16_t Main_Delay = 110;
double thermistor=0;
grideye  GE_GridEyeSensor;
short    g_ashRawTemp[64];          /* temperature of 64 pixels      */
int      init_measure_bool=0;

// Local Network Settings
char ssid[] = "mineee";  // your network SSID (name)
char password[] = "warcraft28";    // your network password
int keyIndex = 0;             // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;

// Variable Setup
long lastConnectionTime = 0;
boolean lastConnected = false;
const int updateThingSpeakInterval = 20 * 1000; // 20 second interval at which to update ThingSpeak
/*
void init_area(t_area myarea) {
  myarea.t_min=100.1;
  myarea.t_max=0.1;
  myarea.av_t_min=100.1;
  myarea.av_t_max=0.1;
  myarea.curr_t=0.0;
  myarea.human_detected=0;
  myarea.human_ct=0;
}*/

WiFiClient client;

void print1d(double marray[], int count, int msize) {
  int ct2=0;
  for(int i=0; i<msize; i++) {
    Serial.print(marray[i]);
    Serial.print(" ");
    ct2++;
    if(ct2>=count) {
      ct2=0;
      Serial.println();
    }
  }
  Serial.println();
}

void print2d_size4(double marray[][4]) {
  int i,j;
  for(i=0; i<4; i++) {
    for(j=0; j<4; j++) {
      Serial.print(marray[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println();
}

void print2d_size8(double marray[][8]) {
  int i,j;
  for(i=0; i<8; i++) {
    for(j=0; j<8; j++) {
      Serial.print(marray[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println();
}

void print_area_data(t_area myarea) {
  Serial.println(myarea.av_t_min);
  Serial.println(myarea.av_t_max);
  Serial.println(myarea.t_min);
  Serial.println(myarea.t_max);
  Serial.println(myarea.curr_t);
  Serial.println();
}

void m1dto2d_size8(double array1[],double array2[][8],int msize) {
  int i=0,j=0;
  int ct2=0;
  for(int ct=0; ct<msize*msize; ct++) {
    array2[i][j] = array1[ct];
    j++;
    ct2++;
    if(ct2>=msize) {
      i++;
      j=0;
      ct2=0;
    }
  }
}

void flip2d_size8(double marray[][8], int msize) {
  int i=0,j=0;
  double aux1,aux2;
  for(i=0; i<msize; i++) {
    for(j=i; j<msize; j++) {
      if(i!=j) {
        aux1=marray[i][j];
        aux2=marray[j][i];
        marray[i][j]=aux2;
        marray[j][i]=aux1;
      }
    }
  }
}

void m2dto1d_size8(double array1[],double array2[][8],int msize) {
  int i,j,ct=0;
  for(i=0; i<msize; i++) {
    for(j=0; j<msize; j++) {
      array1[ct]=array2[i][j];
      ct++;
    }
  }
}
void m1dto2d_size4(double array1[],double array2[][4],int msize) {
  int i=0,j=0;
  int ct2=0;
  for(int ct=0; ct<msize*msize; ct++) {
    array2[i][j] = array1[ct];
    j++;
    ct2++;
    if(ct2>=msize) {
      i++;
      j=0;
      ct2=0;
    }
  }
}

void flip2d_size4(double marray[][4], int msize) {
  int i=0,j=0;
  double aux1,aux2;
  for(i=0; i<msize; i++) {
    for(j=i; j<msize; j++) {
      if(i!=j) {
        aux1=marray[i][j];
        aux2=marray[j][i];
        marray[i][j]=aux2;
        marray[j][i]=aux1;
      }
    }
  }
}

void m2dto1d_size4(double array1[],double array2[][4],int msize) {
  int i,j,ct=0;
  for(i=0; i<msize; i++) {
    for(j=0; j<msize; j++) {
      array1[ct]=array2[i][j];
      ct++;
    }
  }
}

double return_av(double my_array[], int my_size) {
  float x=0.0;
  for(int i=0; i<my_size; i++) {
    x += my_array[i];
  }
  return x/my_size;
}

double return_max(double my_array[], int my_size) {
  float x=0.1;
  for(int i=0; i<my_size; i++) {
    if(my_array[i]>x)
      x = my_array[i];
  }
  return x;
}

double return_min(double my_array[], int my_size) {
  float x=100.1;
  for(int i=0; i<my_size; i++) {
    if(my_array[i]<x)
      x = my_array[i];
  }
  return x;
}

/******************************************************************************
  Function：GE_SentDatatoPC
  Description：Used to Sent data to PC
  Input ：None
  Output：None
  Others：None
******************************************************************************/
void GE_SourceDataInitialize( void )
{
  for ( int i = 0; i < 64; i++ )
  {
    g_ashRawTemp[i] = 0xAAAA;
  }
}
/******************************************************************************
  Function：GE_UpdateFerquency
  Description：Used to set the Grid-EYE update frequency
  Input：None
  Output：Grid-EYE frequency
  Return：None
  Others：None
******************************************************************************/
void GE_UpdateFerquency( uint8_t GE_SetFrequency )
{
  switch (GE_SetFrequency)
  {
    case 10:  /* set update frequency 10Hz */
      {
        Main_Delay = 85;
      }
      break;

    case 1:   /* set update frequency 1Hz */
      {

        Main_Delay = 985;
      }
      break;

    default:
      break;
  }
}

void setup() {
  // Start Serial for debugging on the Serial Monitor
  Serial.begin(9600);

  PIOA->PIO_MDER = 0x00000200;
  delay(1000);
  /*
  init_area(t_area1);
  init_area(t_area2);
  init_area(t_area3);
  init_area(t_area4);*/

  /* Initialize Grid-Eye data interface */
  GE_GridEyeSensor.init( 0 );

  /* Initialize variables  */
  GE_SourceDataInitialize( );

  /* Initialize software Software serial port UART1*/
  GE_SoftUartInit( );

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

double getGEdata(double final_values[64]) {
  uint8_t  aucThsBuf[2];              /* thermistor temperature        */
  short    thermistor_value;
  double   final_value;
  short    conv_temp[64];
  
  GE_GridEyeSensor.bAMG_PUB_I2C_Read(0x0E, 2, aucThsBuf );
  thermistor_value = shAMG_PUB_TMP_ConvThermistor(aucThsBuf);
  final_value = fAMG_PUB_CMN_ConvStoF(thermistor_value);

/* Get temperature register value. */
  for (int i = 0; i < 4; i++)
  {
    GE_GridEyeSensor.bAMG_PUB_I2C_Read(0x80 + 32 * i, 32, (uint8_t *)g_ashRawTemp + i * 32);
  }
  vAMG_PUB_TMP_ConvTemperature64((uint8_t *)g_ashRawTemp, conv_temp);

  for (int ct = 0; ct < 64; ct++) {
    double aux = fAMG_PUB_CMN_ConvStoF(conv_temp[ct]);
    final_values[ct] = aux;
  }

  return final_value;
}

void postRequest(String data1, String data2) {
  IPAddress my_server(178,62,25,163);
    
  if (client.connect(my_server, 4000)) {
    client.print("POST /api/v1/sensors/1 HTTP/1.1\r\n");
    client.print("Host: 178.62.25.163\r\n");
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
      Serial.print(data1);
      Serial.println(data2);
    }
  }
}

void distribute_data(double values[64],double area1[4][4],double area2[4][4],double area3[4][4],double area4[4][4]) {
  double final_temps[8][8];
  m1dto2d_size8(values, final_temps, 8);
  for(int i=0; i<8; i++) {
    for(int j=0; j<8; j++) {
      if(i<4 && j<4) {
        area1[i%4][j%4] = final_temps[i][j];
      }
      if(i<4 && j>=4) {
        area2[i%4][j%4] = final_temps[i][j];
      }
      if(i>=4 && j<4) {
        area3[i%4][j%4] = final_temps[i][j];
      }
      if(i>=4 && j>=4) {
        area4[i%4][j%4] = final_temps[i][j];
      }
    }
  }
}

void store_init_data(double marray[4][4], t_area* my_area, int cycle) {
  double temp[16];
  double curr_min, curr_max, curr_av;
  m2dto1d_size4(temp, marray, 4);
  curr_min = return_min(temp,16);
  curr_max = return_max(temp,16);
  curr_av = return_av(temp,16);
  my_area->av_array[cycle] = curr_av;
  if(curr_min<my_area->t_min)
    my_area->t_min = curr_min;
  if(curr_max>my_area->t_max)
    my_area->t_max = curr_max;
}

void compute_min_max_av(t_area* myarea) {
  double min_av = return_min(myarea->av_array, MEASURE_NO);
  double max_av = return_max(myarea->av_array, MEASURE_NO);
  myarea->av_t_min = min_av;
  myarea->av_t_max = max_av;
}

void init_measure() {
  double temps[64];
  double temps2[8][8];
  double area1[4][4],area2[4][4],area3[4][4],area4[4][4];
  for(int cycle=0; cycle<MEASURE_NO; cycle++) {
    getGEdata(temps);
    m1dto2d_size8(temps, temps2, 8);
    flip2d_size8(temps2, 8);
    m2dto1d_size8(temps, temps2, 8);
    distribute_data(temps,area1,area2,area3,area4);
    store_init_data(area1,&t_area1,cycle);
    store_init_data(area2,&t_area2,cycle);
    store_init_data(area3,&t_area3,cycle);
    store_init_data(area4,&t_area4,cycle);
    GE_UpdateFerquency(GE_UpdateFreGet());
    delay(Main_Delay);
  }
  compute_min_max_av(&t_area1);
  compute_min_max_av(&t_area2);
  compute_min_max_av(&t_area3);
  compute_min_max_av(&t_area4);
}

void set_curr_t(t_area* myarea, double area[4][4]) {
  double temp[16];
  m2dto1d_size4(temp, area, 4);
  myarea->curr_t = return_av(temp,16);
}

void compute_human_detection(t_area* myarea) {
  if(myarea->human_detected) {
    if(myarea->curr_t < myarea->av_t_max)
      myarea->human_ct--;
    else
      myarea->human_ct += 2;
    if(myarea->human_ct>HUMAN_CAP)
      myarea->human_ct=HUMAN_CAP;
    if(myarea->human_ct<0)
      myarea->human_detected=0;  
  }

  if(!myarea->human_detected) {
    if(myarea->curr_t > myarea->av_t_max)
      myarea->human_ct++;
    else
      myarea->human_ct -= 2;
    if(myarea->human_ct<0)
      myarea->human_ct=0;
    if(myarea->human_ct> HUMAN_CAP)
      myarea->human_detected=1;
  }
}

const char* booltostring(int x) {
  return x ? "true" : "false";
}

void loop() {
  
  if(!init_measure_bool) {
    Serial.println("init measure started");
    init_measure();
    init_measure_bool=1;
    Serial.println("init measure finished");
  }

  double temps[64];
  double temps2[8][8];
  double area1[4][4],area2[4][4],area3[4][4],area4[4][4];
  
  if (  !apds.readAmbientLight(ambient_light) ) {
    Serial.println("Error reading light values");
  } 

  thermistor=getGEdata(temps);
  m1dto2d_size8(temps, temps2, 8);
  flip2d_size8(temps2, 8);
  m2dto1d_size8(temps, temps2, 8);
  distribute_data(temps,area1,area2,area3,area4);
/*
  print2d_size4(area1);
  print2d_size4(area2);
  print2d_size4(area3);
  print2d_size4(area4);*/
  
  set_curr_t(&t_area1, area1);
  set_curr_t(&t_area2, area2);
  set_curr_t(&t_area3, area3);
  set_curr_t(&t_area4, area4);

  compute_human_detection(&t_area1);
  compute_human_detection(&t_area2);
  compute_human_detection(&t_area3);
  compute_human_detection(&t_area4);

  int det1 = t_area1.human_detected;
  int det2 = t_area2.human_detected;
  int det3 = t_area3.human_detected;
  int det4 = t_area4.human_detected;

  print2d_size8(temps2);
  /*
  print_area_data(t_area1);
  print_area_data(t_area2);
  print_area_data(t_area3);
  print_area_data(t_area4);
  */
  
  Serial.print(det1);
  Serial.print("  ");
  Serial.print(det2);
  Serial.println();
  Serial.print(det3);
  Serial.print("  ");
  Serial.print(det4);
  Serial.println();

  char *h1,*h2,*h3,*h4;

  strcpy(h1, booltostring(det1));
  strcpy(h2, booltostring(det2));
  strcpy(h3, booltostring(det3));
  strcpy(h4, booltostring(det4));

  sprintf(posts1, POSTS1, thermistor, ambient_light);
  sprintf(posts2, POSTS2, h2,h1,h4,h3);

  Serial.println(h1);
  Serial.println(h2);
  Serial.println(h3);
  Serial.println(h4);

  memset(h1, 0, sizeof(h1));
  memset(h2, 0, sizeof(h2));
  memset(h3, 0, sizeof(h3));
  memset(h4, 0, sizeof(h4));

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
    Serial.println("Posting to server..."); 
    postRequest(posts1,posts2);
  }
  
  lastConnected = client.connected();

  memset(&posts1[0], 0, sizeof(posts1));
  memset(&posts2[0], 0, sizeof(posts2));

  GE_UpdateFerquency(GE_UpdateFreGet());
  
  delay(Main_Delay);
}
