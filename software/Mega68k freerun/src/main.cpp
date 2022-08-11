#include <Arduino.h>
#include "defines.h"
#include "utils.h"

char buffer[128];

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

  // Set AS as INPUT
  DDRD &= ~(1<<ADDRESS_STROBE_PIN);

  resetCPU();
  Serial.println("Init complete.");
}

void loop() {

  // Check for Address Strobe
  if ( !(PIND & (1<<ADDRESS_STROBE_PIN)) ) {
    // Check R/W state
    boolean reading = (PINH & (1<<READ_WRITE_PIN));

    unsigned long address = getAddress();

    word data = getROMorRAM(address);
    setDataBus(data);

    sprintf(buffer, "%c 0x%06lX : 0x%04X", 
      (reading)?'R':'W',
      address, 
      data);
    Serial.println(buffer);

    // Assert DTACK
    PORTG &= ~(1 << DTACK_PORT);
    delayMicroseconds(2);
    // UnAssert DTACK
    PORTG |= (1 << DTACK_PORT);
  }

}