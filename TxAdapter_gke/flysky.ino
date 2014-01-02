
/*
  Standalone Tx adapter version by Prof. G.K. Egan (gke) 2013.
 
  Almost all of the adapter code was originally derived from the work 
  of PhracturedBlue and others for the universal transmitter "Deviation". 
  
  There have been numerous modifications, and in some cases rewrites, 
  of the original code.
  
  https://bitbucket.org/PhracturedBlue/deviation

  The adapter requires an a7105 transceiver and an Arduino Pro Mini or similar 
  preferably at 3.3V.  It is possible to modify the original Turnigy 9X 
  removable Tx module using its a7105 transciever.

  Pinouts are defined in config.h and the defaults are those adopted by 
  Midelic whose source code is not open.
  
  http://www.rcgroups.com/forums/showthread.php?t=1954078&highlight=hubsan+adapt
 
  This project is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  Deviation is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License 
  if not, see <http://www.gnu.org/licenses/>.
 
*/

const uint8_t flyskyMap[] = {
  ROLL,PITCH,THROTTLE,YAW,AUX1,AUX2,AUX3,AUX4
};

static const char * const flysky_opts[] = {
  //zzz  "WLToys V9x9",  _tr_noop("Off"), _tr_noop("On"), NULL,
  NULL
};
#define WLTOYS_ON 1
#define WLTOYS_OFF 0
enum {
  PROTOOPTS_WLTOYS = 0,
};
static const uint8_t a7105Regs[] = {
  -1,  0x42, 0x00, 0x14, 0x00,  -1 ,  -1 , 0x00, 0x00, 0x00, 0x00, 0x01, 0x21, 0x05, 0x00, 0x50,
  0x9e, 0x4b, 0x00, 0x02, 0x16, 0x2b, 0x12, 0x00, 0x62, 0x80, 0x80, 0x00, 0x0a, 0x32, 0xc3, 0x0f,
  0x13, 0xc3, 0x00,  -1,  0x00, 0x00, 0x3b, 0x00, 0x17, 0x47, 0x80, 0x03, 0x01, 0x45, 0x18, 0x00,
  0x01, 0x0f,  -1,
};
static const uint8_t flyskyTxChannels[16][16] = {
  {
    0x0a, 0x5a, 0x14, 0x64, 0x1e, 0x6e, 0x28, 0x78, 0x32, 0x82, 0x3c, 0x8c, 0x46, 0x96, 0x50, 0xa0                }
  ,
  {
    0xa0, 0x50, 0x96, 0x46, 0x8c, 0x3c, 0x82, 0x32, 0x78, 0x28, 0x6e, 0x1e, 0x64, 0x14, 0x5a, 0x0a                }
  ,
  {
    0x0a, 0x5a, 0x50, 0xa0, 0x14, 0x64, 0x46, 0x96, 0x1e, 0x6e, 0x3c, 0x8c, 0x28, 0x78, 0x32, 0x82                }
  ,
  {
    0x82, 0x32, 0x78, 0x28, 0x8c, 0x3c, 0x6e, 0x1e, 0x96, 0x46, 0x64, 0x14, 0xa0, 0x50, 0x5a, 0x0a                }
  ,
  {
    0x28, 0x78, 0x0a, 0x5a, 0x50, 0xa0, 0x14, 0x64, 0x1e, 0x6e, 0x3c, 0x8c, 0x32, 0x82, 0x46, 0x96                }
  ,
  {
    0x96, 0x46, 0x82, 0x32, 0x8c, 0x3c, 0x6e, 0x1e, 0x64, 0x14, 0xa0, 0x50, 0x5a, 0x0a, 0x78, 0x28                }
  ,
  {
    0x50, 0xa0, 0x28, 0x78, 0x0a, 0x5a, 0x1e, 0x6e, 0x3c, 0x8c, 0x32, 0x82, 0x46, 0x96, 0x14, 0x64                }
  ,
  {
    0x64, 0x14, 0x96, 0x46, 0x82, 0x32, 0x8c, 0x3c, 0x6e, 0x1e, 0x5a, 0x0a, 0x78, 0x28, 0xa0, 0x50                }
  ,
  {
    0x50, 0xa0, 0x46, 0x96, 0x3c, 0x8c, 0x28, 0x78, 0x0a, 0x5a, 0x32, 0x82, 0x1e, 0x6e, 0x14, 0x64                }
  ,
  {
    0x64, 0x14, 0x6e, 0x1e, 0x82, 0x32, 0x5a, 0x0a, 0x78, 0x28, 0x8c, 0x3c, 0x96, 0x46, 0xa0, 0x50                }
  ,
  {
    0x46, 0x96, 0x3c, 0x8c, 0x50, 0xa0, 0x28, 0x78, 0x0a, 0x5a, 0x1e, 0x6e, 0x32, 0x82, 0x14, 0x64                }
  ,
  {
    0x64, 0x14, 0x82, 0x32, 0x6e, 0x1e, 0x5a, 0x0a, 0x78, 0x28, 0xa0, 0x50, 0x8c, 0x3c, 0x96, 0x46                }
  ,
  {
    0x46, 0x96, 0x0a, 0x5a, 0x3c, 0x8c, 0x14, 0x64, 0x50, 0xa0, 0x28, 0x78, 0x1e, 0x6e, 0x32, 0x82                }
  ,
  {
    0x82, 0x32, 0x6e, 0x1e, 0x78, 0x28, 0xa0, 0x50, 0x64, 0x14, 0x8c, 0x3c, 0x5a, 0x0a, 0x96, 0x46                }
  ,
  {
    0x46, 0x96, 0x0a, 0x5a, 0x50, 0xa0, 0x3c, 0x8c, 0x28, 0x78, 0x1e, 0x6e, 0x32, 0x82, 0x14, 0x64                }
  ,
  {
    0x64, 0x14, 0x82, 0x32, 0x6e, 0x1e, 0x78, 0x28, 0x8c, 0x3c, 0xa0, 0x50, 0x5a, 0x0a, 0x96, 0x46                }
  ,
};
const uint32_t flyskyID = 0x5475c52A;
//static const uint8_t flyskyID[] = { 0x02, 0x00, 0x00, 0x70 };
static uint8_t chanrow;
static uint8_t chancol;
static uint8_t chanoffset;
//static uint8_t packet[21];
static uint16_t counter = 255;

static int flyskyInit(void) {
  uint8_t i;
  uint8_t if_calibration1;
  uint8_t vco_calibration0;
  uint8_t vco_calibration1;


  delay(10);//wait 10ms for A7105 wakeup
  a7105Reset;

  a7105WriteID(flyskyID);

  for (i = 0; i < 0x33; i++)
    if(a7105Regs[i] != 0xff)
      a7105WriteReg(i, a7105Regs[i]);

  a7105Strobe(A7105_STANDBY);
  a7105WriteReg(A7105_02_CALC,0x01);
  while(a7105ReadReg(A7105_02_CALC)){
    if_calibration1 = a7105ReadReg(0x22);
    if(if_calibration1 & 0x10){//do nothing
    }
  }

  a7105WriteReg(A7105_24_VCO_CURCAL,0x13);
  a7105WriteReg(A7105_26_VCO_SBCAL_II,0x3b);
  a7105WriteReg(0x0F,0x00);//channel 0
  a7105WriteReg(A7105_02_CALC,0x02);
  while(a7105ReadReg(A7105_02_CALC)){
    vco_calibration0 = a7105ReadReg(A7105_25_VCO_SBCAL_I);
    if(vco_calibration0&0x08){//do nothing
    }
  }

  a7105WriteReg(0x0F,0xA0);
  a7105WriteReg(A7105_02_CALC,0x02);
  while(a7105ReadReg(A7105_02_CALC)){
    vco_calibration1 = a7105ReadReg(0x25);
    if(vco_calibration1 & 0x08){//do nothing
    }
  }

  a7105WriteReg(A7105_25_VCO_SBCAL_I,0x08);
  a7105WriteReg(A7105_28_TX_TEST,0x1F);//set power to 1db maximum
  a7105Strobe(A7105_STANDBY);//stand-by strobe command
  
  flyskyBind();

  chanrow = flyskyID % 16;
  chanoffset = (flyskyID & 0xff) / 16;
  chancol=0;

} // flyskySetup

void flyskyBind(void) {
  uint16_t i; 

  flyskyBuildPacket(true);

  for (i = 0; i < 250; i++) {
    a7105WriteData(packet, 21, 1);
    delay(10);
    ledToggle(500);
  }
  LED_OFF();

} // flyskyBind

// simple direct version
void flyskyWritePacket2(uint8_t init){//except address(0x05)should sent to A7105 21 bytes totally)
  uint8_t i;

  CS_LO();
  a7105Write(A7105_05_FIFO_DATA);//TX/RX FIFO address
  a7105Write(init);//0xaa or 0x55(depend on bind packet or data packet)
  a7105Write((flyskyID >> 0) & 0xff);
  a7105Write((flyskyID >> 8) & 0xff);
  a7105Write((flyskyID >> 16) & 0xff);
  a7105Write((flyskyID >> 24) & 0xff);

  for(i = 0; i < 8; i++){
    packet[0+2*i] = lowByte(rcData[i]);//low byte of servo timing(1000-2000us)
    packet[1+2*i] = highByte(rcData[i]);//high byte of servo timing(1000-2000us)
    a7105Write(packet[0+2*i]);
    a7105Write(packet[1+2*i]);
  }
  CS_HI();
} // flyskyWritePacket2

static void flyskyBuildPacket(uint8_t binding) {
  uint8_t i;
  int16_t v;
  
//-100% =~ 0x03e8 1000us (min)
//+100% =~ 0x07ca 1994us (max)
//Center = 0x5d9 1497us (center)
//channel order AIL;ELE;THR;RUD;AUX1;AUX2;AUX3;AUX4

  packet[0] = binding ? 0xaa : 0x55;
  packet[1] = (flyskyID >> 0) & 0xff;
  packet[2] = (flyskyID >> 8) & 0xff;
  packet[3] = (flyskyID >> 16) & 0xff;
  packet[4] = (flyskyID >> 24) & 0xff;
  
  for (i = 0; i < RC_CHANS; i++) {
    v = rcData[flyskyMap[i]];
    packet[5 + i*2] = lowByte(v);
    packet[6 + i*2] = highByte(v);
  }
  if (false) { // zzzModel.proto_opts[PROTOOPTS_WLTOYS] == WLTOYS_ON) {
    if(rcData[AUX1] > 0) packet[12] |= 0x20;
    if(rcData[AUX2] > 0) packet[10] |= 0x40;
    if(rcData[AUX3] > 0) packet[10] |= 0x80;
    if(rcData[AUX4] > 0) packet[12] |= 0x10;
  }
} // flyskyBuildPacket


static uint16_t flyskyUpdate(void) {
  uint8_t chan;

  chan = flyskyTxChannels[chanrow][chancol] - chanoffset;
  flyskyBuildPacket(0);
  a7105WriteData(packet, 21, chan);
  chancol = (chancol + 1) % 16;
  if (!chancol)
    a7105SetPower(TXPOWER_150mW);

  return 1460;
} // flyskyUpdate

