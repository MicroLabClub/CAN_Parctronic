#include <Arduino.h>
#include <SoftwareSerial.h>
#include "lib_cond/lib_cond.h"
#include "dd_dypa05.h"
#include <stdint.h>

// Set up a new SoftwareSerial object
SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);

enum us_ID
{
  US_MOST_LEFT,
  US_LEFT,
  US_RIGHT,
  US_MOST_RIGHT,
  US_NR_OF
};

#define IN_BUFF_SIZE 10 // US_NR_OF*2 +2

uint8_t in_buff[IN_BUFF_SIZE];
int16_t us_distance[US_NR_OF];

uint16_t dd_dypa05_get_distance(size_t sensor_id){
  return us_distance[sensor_id];
}


void dd_dypa05_setup()
{
  // Set the baud rate for the SoftwareSerial object
  mySerial.begin(9600);
  // Serial.begin(9600);
}

void dd_dypa05_loop()
{
  if (mySerial.available() > 0)
  {
    uint8_t data = mySerial.read();

    if (data == 0xFF)
      Serial.println("=========================================================");

    // 1. FILTRU MEDIAN
    // 1.1. coletam fluxul de intrare in bufer FIFO
    fifo_push_uint8(data, in_buff, IN_BUFF_SIZE);
    // print_buff_uint8(in_buff, IN_BUFF_SIZE);

    int CS = buff_get_summ_uint8(in_buff + 1, IN_BUFF_SIZE - 1);
    CS &= 0xff;

    if (CS == in_buff[0])
    {
      Serial.println("Got a packet");
      print_buff_uint8(in_buff, IN_BUFF_SIZE);

      Serial.print(" CS: ");
      Serial.println(CS, HEX);

      for (size_t i = 0; i < US_NR_OF; i++)
      {
        us_distance[i] = in_buff[IN_BUFF_SIZE - 2 - i * 2 - 1];
        us_distance[i] += in_buff[IN_BUFF_SIZE - 2 - i * 2] << 8;

        // Serial.print(" | ");               /* code */
        // Serial.print(us_distance[i], HEX); /* code */
        // Serial.print(":");               /* code */
        // Serial.print(us_distance[i]); /* code */
      }
      //Serial.println();
    }
    // Serial.print(" | ");
    // Serial.print(data, HEX);
  }
}

void dd_dypa05_report(){
 for (size_t i = 0; i < US_NR_OF; i++)
  {
    us_distance[i] = in_buff[IN_BUFF_SIZE - 2 - i * 2 - 1];
    us_distance[i] += in_buff[IN_BUFF_SIZE - 2 - i * 2] << 8;

    Serial.print(" | ");
    Serial.print(us_distance[i], HEX);
    Serial.print(":");
    Serial.print(us_distance[i]);

    for (int sensor_id = US_MOST_LEFT; sensor_id <= US_MOST_RIGHT; ++sensor_id)
    {
      Serial.print(dd_dypa05_get_distance(sensor_id));
    }
  }
  Serial.println();
}