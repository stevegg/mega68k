#include "defines.h"

unsigned long getAddress() {
  // For the 64 pin mc68000P10 there is no A0 pin so we need to do a final shift left
  // to get the proper value
  unsigned long xValue = ADDRESS_X_PIN;
  return ((xValue<<16) | (ADDRESS_HI_PIN<<8) | ADDRESS_LOW_PIN) << 1;
}

word getDataBus() {
  // Setup Data Bus DDR to INPUT
  DATA_LOW_DDR = B00000000;
  DATA_HI_DDR = B00000000;
  return ( DATA_HI_PIN << 8 ) | DATA_LOW_PIN;
}

void setDataBus(word data) {
  // Setup Data Bus DDR to output
  DATA_LOW_DDR = B11111111;
  DATA_HI_DDR = B11111111;
  DATA_LOW_PORT = data & 0xFF;
  DATA_HI_PORT = data >> 8;
}

word getROMorRAM(word address) {
  return 0x00;
}

void setRAM(word address, word data) {
  // Not setting ram

}