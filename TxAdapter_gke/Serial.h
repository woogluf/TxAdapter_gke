#ifndef __Serial_h__
#define __Serial_h__

typedef struct { // not used by Adapter
  uint16_t thisVersion;
  uint8_t P8[PIDITEMS], I8[PIDITEMS], D8[PIDITEMS];
  uint8_t rcRate8;
  uint8_t rcExpo8;
  uint8_t rollPitchRate;
  uint8_t yawRate;
  uint8_t dynThrPID;
  uint8_t thrMid8;
  uint8_t thrExpo8;
  uint16_t activate[CHECKBOX_ITEMS];
  uint16_t cycletimeuS;
  int16_t minthrottleuS;
  uint8_t checksum; // must be last! 
} MwiiConf;

#endif
