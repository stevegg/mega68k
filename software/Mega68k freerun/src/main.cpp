#include <Arduino.h>
#include "defines.h"

char buffer[128];
volatile boolean initialized = false;

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

}

char binBuffer[33];

char *printBits(char *buffer, unsigned long value, int size) {
  memset(binBuffer, 0, size);
  for ( int i = 0; i < size; i ++ ) {
    buffer[i] = (value & (1<<(size-i))) ? '1' : '0';
  }
  return buffer;
}

/**
 * Called whenever the AddressStrobe pin goes low
 */
void addressStrobe() {

  // If initialization isn't complete then ignore the AS state
  if ( !initialized) {
    return;
  }

  // Check HALTED state
  boolean halted = !(PING & (1<<HALT_PIN));
  // Check R/W state
  boolean reading = (PORTH & (1<<READ_WRITE_PIN));

  // We only use the first 7 bits of the X Address.  Last bit is E Signal from 68k
  unsigned long xAddress = ADDRESS_X_PIN & ~(1<<7);
  unsigned long hAddress = ADDRESS_HI_PIN;
  unsigned long lAddress = ADDRESS_LOW_PIN;

  // Combine X, H and L into an an address.  Since we don't have an A0 line
  // we'll shift the whole address left by 1 bit to account for this.
  unsigned long address = ((xAddress<<16) | (hAddress<<8) | lAddress) << 1;

  word data = getROMorRAM(address);
  setDataBus(data);

  sprintf(buffer, "%c 0x%06lX : 0x%04X %s", 
    (reading)?'R':'W',
    address, 
    data, 
    (halted)?"(HALTED)": "");
  Serial.println(buffer);

  delay(200);
  // Assert DTACK
  PORTG &= ~(1 << DTACK_PORT);
  delayMicroseconds(10);
  // UnAssert DTACK
  PORTG |= (1 << DTACK_PORT);

}

void resetCPU() {
  Serial.println("Reset CPU");

  // Set HALT to an output
  DDRG |= (1<<HALT_PIN);

  // Bring both RESET and HALT pins low
  PORTG &= ~((1<<RESET_PIN) | (1<<HALT_PIN));
    // Make sure we delay at least 200ms
  delay(10);
  // Now set reset and HALT to HIGH
  PORTG |= ((1<<RESET_PIN) | (1<<HALT_PIN));

  // Set HALT to an input
  DDRG &= ~(1<<HALT_PIN);
}

// PORT MANIPULATION
// To set bits use PORT
// To read bits use PIN
// Set DDR Bits to 1 for OUTPUT, 0 for INPUT

void setup() {

  // Setup Serial port
  Serial.begin(9600);
  Serial.println("Initializing");

  // Set DTACK, RESET and HALT Pin as OUTPUT
  DDRG |= (
    (1<<DTACK_PIN) |
    (1<<RESET_PIN)
  );
  // Set HALT to INPUT
  DDRG &= ~(1<<HALT_PIN);

  // Set DTACK, RESET and HALT all hi
  PORTG |= ((1<<DTACK_PIN) | (1<<RESET_PIN) | (1<<HALT_PIN));

  // Set BERR, VPA and BGACK pins to output
  DDRK |= (
    (1<<VPA_PIN) |
    (1<<BERR_PIN) |
    (1<<BGACK_PIN) |
    (1<<BR_PIN)
  );

  // Set BERR, VPA and BGACK and BR hi
  PORTK |= ((1<<BERR_PIN) | (1<<VPA_PIN) | (1<<BGACK_PIN) | (1<<BR_PIN));

  // Set IPL0, IPL1 and IPL2 pins to output
  DDRE |= (
    (1<<IPL0_PIN) |
    (1<<IPL1_PIN) |
    (1<<IPL2_PIN)
  );
  // Set IPL0, IPL1 and IPL2 all hi
  PORTE |= ((1<<IPL0_PIN) | (1<<IPL1_PIN) | (1<<IPL2_PIN));

  // Set R/W, FC0, FC1 and FC2 pins to input
  DDRH = DDRH & ~((1<<RW_PIN) | (1<<FC0_PIN) | (1<<FC1_PIN) | (1<<FC2_PIN));

  // Setup address as input
  ADDRESS_LOW_DDR = B00000000;
  ADDRESS_HI_DDR = B00000000;
  ADDRESS_X_DDR = B00000000;

  // Data bus as output (may also be input)
  DATA_LOW_DDR = B11111111;
  DATA_HI_DDR = B11111111;

  // Set everything to zero
  DATA_LOW_PORT = 0x00;
  DATA_HI_PORT = 0x00;

  // Set CLK_INPUT pin to INPUT
  DDRG &= ~(1<<CLK_INPUT);

  // Attach interrupt to Address Strobe pin
  pinMode(ADDRESS_STROBE, INPUT);
  attachInterrupt(digitalPinToInterrupt(ADDRESS_STROBE), addressStrobe, FALLING);

  resetCPU();
  initialized = true;
  Serial.println("Init complete. Enter a command or ? for help");
}

void showHelp() {
  Serial.println("\nSteve's Mega68K Monitor");
  Serial.println("-----------------------\n");
  Serial.println("Available commands:\n");
  Serial.println("G - Go");
  Serial.println("N - Next");
  Serial.println("R - Reset CPU");
  Serial.println("S - STOP");

  Serial.println("\n");
}

void cpuReset() {
  resetCPU();
  delay(100);
}

void assertDTACK() {

}

char prompt[60];
boolean prompted = false;
void loop() {

  if ( !prompted ) {
    // Read address bus
    sprintf(prompt, "Ready 0x%08lX >", getAddress());
    Serial.print(prompt);
    prompted = true;
  }
  if ( Serial.available() ) {
    char c = Serial.read();
    Serial.println(c);
    if ( c == 'g' || c == 'G' ) {
      PORTG &= ~(1 << DTACK_PORT);
    } else if ( c == 'n' || c == 'N' ) {
      // Assert DTACK
      PORTG &= ~(1 << DTACK_PORT);
    } else if ( c == 'r' || c == 'R' ) {
      cpuReset();
    } else if ( c == 's' || c == 'S' ) {
      PORTG |= (1 << DTACK_PORT);
    } else if ( c == '?') {
      showHelp();
    } else {
      Serial.println("Syntax Error.");
    }
    prompted = false;
  }

 }