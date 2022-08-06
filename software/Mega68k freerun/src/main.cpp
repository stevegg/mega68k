#include <Arduino.h>

// Low byte of address bus
#define ADDRESS_LOW_PIN     PINF
#define ADDRESS_LOW_PORT    PORTF
#define ADDRESS_LOW_DDR     DDRF

// Hi byte of address bus
#define ADDRESS_HI_PIN      PINK
#define ADDRESS_HI_PORT     PORTK
#define ADDRESS_HI_DDR      DDRK

// Low byte of data bus
#define DATA_LOW_PIN        PINA
#define DATA_LOW_PORT       PORTA
#define DATA_LOW_DDR        DDRA

// Hi byte of data bus
#define DATA_HI_PIN         PINC
#define DATA_HI_PORT        PORTC
#define DATA_HI_DDR         DDRC

#define ADDRESS_STROBE      20
#define READ_WRITE          7

#define DTACK_PORT          PORTG2
#define DTACK_PIN           PING2
#define RESET_PORT          PORTG1
#define RESET_PIN           PING1
#define HALT_PORT           PORTG0
#define HALT_PIN            PING0

#define RW_PORT             PORTH4

#define IS_READING          (PORTH & (1<<PORTH4)) == 1

char buffer[128];

word getAddress() {
  return ( ADDRESS_HI_PIN << 8 ) | ADDRESS_LOW_PIN;
}

word getDataBus() {
  return ( DATA_HI_PIN << 8 ) | DATA_LOW_PIN;
}

void setDataBus(word data) {
  DATA_LOW_PORT = data & 0xFF;
  DATA_HI_PORT = data >> 8;
}

word getROMorRAM(word address) {
  return 0x00;
}

void setRAM(word address, word data) {

}

/**
 * Called whenever the AddressStrobe pin goes low
 */
void addressStrobe() {
  Serial.print(". ");
  word address = getAddress();
  boolean reading = IS_READING;
  word data =getROMorRAM(address); 
  if ( reading ) {
    setDataBus(data);
  } else {
    setRAM(address, getDataBus());
  }
  sprintf(buffer, "%c %04X|(BINARY) : %04X %c", (reading?'R':'W'), address, data, (data & 0xFF));
  Serial.println(buffer);

  // Strobe DTACK
  //PORTG &= ~(1 << DTACK_PORT);
  //delayMicroseconds(1000);
  //PORTG |= (1<<DTACK_PORT);
}

// PORT MANIPULATION
// To set bits use PORT
// To read bits use PIN

void setup() {

  // Setup Serial port
  Serial.begin(9600);
  Serial.println("Initializing");

  // Setup reset and dtack pins as output
  DDRG = DDRG &
    (
      (1 << DDG0) | // pinMode(D41, OUTPUT)
      (1 << DDG1) | // pinMode(D40, OUTPUT)
      (1 << DDG2)   // pinMode(D39, OUTPUT)
    );

  // Set R/W pint to input
  DDRH = DDRH & ~( 1 << PORTH4);

  // Setup address as input
  ADDRESS_LOW_DDR = B00000000;
  ADDRESS_HI_DDR = B00000000;

  // Assert reset by pulling it low and set DTACK HIGH
  PORTG &= ~(1 << RESET_PORT);
  PORTG |= (1 << DTACK_PORT);

  // Attach interrupt to Address Strobe pin
  pinMode(ADDRESS_STROBE, INPUT);
  attachInterrupt(digitalPinToInterrupt(ADDRESS_STROBE), addressStrobe, FALLING);

  // Make sure we delay at least 200ms
  delay(400);

  // Now set reset to HIGH
  PORTG |= (1<<RESET_PORT);

  // Set DTACK LOW for free run
  PORTG &= ~(1 << DTACK_PORT);

  Serial.println("Init complete.");

}

void loop() {

}