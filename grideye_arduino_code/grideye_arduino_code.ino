#include <Wire.h>
#include "Arduino.h"
#include <grideye.h>
#include <GE_SoftUart.h>
#include <GE_SoftUartParse.h>

#define MEASURE_NO 100

/*******************************************************************************
  variable value definition
*******************************************************************************/
static uint16_t Main_Delay = 110;
grideye  GE_GridEyeSensor;
uint8_t  aucThsBuf[2];              /* thermistor temperature        */
short    thermistor_value;
double   final_value;
short    conv_temp[64];
short    g_ashRawTemp[64];          /* temperature of 64 pixels      */
double   final_values[64], av_array[MEASURE_NO];
int      init_board = 0;
int      init_measure_bool=0;
double   min_av=100.1, max_av=0.1, min_t=100.1, max_t=0.1;
double   curr_av, curr_max, curr_min; 
int      human_detected=0, human_ct=0;

/******************************************************************************
  Function：GE_SentDatatoPC
  Description：Used to Sent data to PC
  Input：None
  Output：None
  Others：None
******************************************************************************/
void GE_SentDataToPC( void )
{
  GE_SoftUartPutChar( '*');
  GE_SoftUartPutChar( '*');
  GE_SoftUartPutChar( '*');
  GE_SoftUartPutChar( aucThsBuf[0] );
  GE_SoftUartPutChar( aucThsBuf[1] );
  for (int i = 0; i < 128; i++)
  {
    GE_SoftUartPutChar((char)(*((uint8_t *)(g_ashRawTemp) + i)));
  }
  GE_SoftUartPutChar( '\r');
  GE_SoftUartPutChar( '\n');
}

double return_av(double my_array[64]) {
  float x;
  for(int i=0; i<64; i++) {
    x += my_array[i];
  }
  return x/64;
}

double return_max(double my_array[64]) {
  float x=0.1;
  for(int i=0; i<64; i++) {
    if(my_array[i]>x)
      x = my_array[i];
  }
  return x;
}

double return_min(double my_array[64]) {
  float x=100.1;
  for(int i=0; i<64; i++) {
    if(my_array[i]<x)
      x = my_array[i];
  }
  return x;
}

void init_measure() {
  //GE_GridEyeSensor.bAMG_PUB_I2C_Read(0x0E, 2, aucThsBuf );
  //thermistor_value = shAMG_PUB_TMP_ConvThermistor(aucThsBuf);
  //final_value = fAMG_PUB_CMN_ConvStoF(thermistor_value);
  for(int cycle=0; cycle<MEASURE_NO; cycle++) {
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
    curr_min = return_min(final_values);
    curr_max = return_max(final_values);
    curr_av = return_av(final_values);
    av_array[cycle] = curr_av;
    if(curr_min<min_t)
      min_t = curr_min;
    if(curr_max>max_t)
      max_t = curr_max;
    delay(110);
  }
  min_av = return_min(av_array);
  max_av = return_max(av_array);
}

/******************************************************************************
  Function：GE_SentDatatoPC
  Description：Used to Sent data to PC
  Input：None
  Output：None
  Others：None
******************************************************************************/
void GE_SentDataToPhone( void )
{
  Serial.print("***");
  Serial.write(aucThsBuf[0]);
  Serial.write(aucThsBuf[1]);
  for ( int i = 0; i < 128; i++ )
  {
    Serial.write(*((uint8_t *)(g_ashRawTemp) + i));
  }
  Serial.print("\r\n");
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



void setup()
{
  /* Waiting for BLE Start to finish */
  PIOA->PIO_MDER = 0x00000200;
  delay(1000);

  /* start serial port at 57600 bps:*/
  Serial.begin(9600);

  /* Initialize Grid-Eye data interface */
  GE_GridEyeSensor.init( 0 );

  /* Initialize variables  */
  GE_SourceDataInitialize( );

  /* Initialize software Software serial port UART1*/
  GE_SoftUartInit( );
}

void loop()
{
  if(!init_board)
    delay(10000);
  init_board=1;

  if(!init_measure_bool)
    init_measure();
  init_measure_bool=1;
  
  /* Parse of the latest PC sent command */
  GE_CmdParse();

  /* Get thermistor register value. */
  GE_GridEyeSensor.bAMG_PUB_I2C_Read(0x0E, 2, aucThsBuf );

  thermistor_value = shAMG_PUB_TMP_ConvThermistor(aucThsBuf);
  final_value = fAMG_PUB_CMN_ConvStoF(thermistor_value);

  //Serial.println(final_value);

  /* Get temperature register value. */
  for (int i = 0; i < 4; i++)
  {
    GE_GridEyeSensor.bAMG_PUB_I2C_Read(0x80 + 32 * i, 32, (uint8_t *)g_ashRawTemp + i * 32);
  }

  vAMG_PUB_TMP_ConvTemperature64((uint8_t *)g_ashRawTemp, conv_temp);

  int ct2 = 0;

  for (int ct = 0; ct < 64; ct++) {
    double aux = fAMG_PUB_CMN_ConvStoF(conv_temp[ct]);
    final_values[ct] = aux;
    Serial.print(aux);
    Serial.print("  ");
    ct2++;
    if (ct2 >= 8) {
      ct2 = 0;
      Serial.println();
    }
  }
  Serial.println();
  Serial.println();

  curr_av = return_av(final_values);

  if(human_detected) {
    if(curr_av < max_av)
      human_ct--;
    else
      human_ct += 2;
    if(human_ct>11)
      human_ct=11;
    if(human_ct<1)
      human_detected=0;  
  }

  if(!human_detected) {
    if(curr_av > max_av)
      human_ct++;
    else
      human_ct -= 2;
    if(human_ct<0)
      human_ct=0;
    if(human_ct> 10)
      human_detected=1;
  }

  //Serial.println(human_detected);
  //Serial.println();

 /* Serial.println("max_av");
  Serial.println(max_av);
  Serial.println("min_av");
  Serial.println(min_av);
  Serial.println("max_t");
  Serial.println(max_t);
  Serial.println("min_t");
  Serial.println(min_t);*/

  /* Send Grid-Eye sensor data to PC */
  //GE_SentDataToPC( );

  /* Send Grid-Eye sensor data to phone */
  //GE_SentDataToPhone( );

  /* set update frequency */
  GE_UpdateFerquency(GE_UpdateFreGet());

  delay( 1000 );
}
