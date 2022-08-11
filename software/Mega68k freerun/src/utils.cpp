#include "defines.h"

unsigned long getAddress() {
  // We only use the first 7 bits of the X Address.  Last bit is E Signal from 68k
  unsigned long xAddress = ADDRESS_X_PIN & ~(1<<7);
  unsigned long hAddress = ADDRESS_HI_PIN;
  unsigned long lAddress = ADDRESS_LOW_PIN;

  // Combine X, H and L into an an address.  Since we don't have an A0 line
  // we'll shift the whole address left by 1 bit to account for this.
  return ((xAddress<<16) | (hAddress<<8) | lAddress) << 1;
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