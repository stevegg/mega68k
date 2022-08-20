#ifndef __DEFINES_H
#define __DEFINES_H

#include <Arduino.h>

// #define DATA_BUS_WIDTH_16
#define DATA_BUS_WIDTH_8

// Low byte of address bus
#define ADDRESS_LOW_PIN     PINL
#define ADDRESS_LOW_PORT    PORTL
#define ADDRESS_LOW_DDR     DDRL

// Hi byte of address bus
#define ADDRESS_HI_PIN      PINC
#define ADDRESS_HI_PORT     PORTC
#define ADDRESS_HI_DDR      DDRC

// Extra byte of address bus
#define ADDRESS_X_PIN       PINF
#define ADDRESS_X_PORT      PORTF
#define ADDRESS_X_DDR       DDRF

// Low byte of data bus
#define DATA_LOW_PIN        PINA
#define DATA_LOW_PORT       PORTA
#define DATA_LOW_DDR        DDRA

// Hi byte of data bus
#ifdef DATA_BUS_WIDTH_16
#define DATA_HI_PIN         PINF
#define DATA_HI_PORT        PORTF
#define DATA_HI_DDR         DDRF
#endif

#define ADDRESS_STROBE_PIN  PORTH3
#define RW_PORT             PORTH6

#define DTACK_PORT          PORTG2
#define DTACK_PIN           PING2
#define RESET_PORT          PORTG1
#define RESET_PIN           PING1
#define HALT_PORT           PORTG0
#define HALT_PIN            PING0

#define RW_PIN              PORTH6
#define CLK_INPUT           PORTH5

#define VPA_PIN             PORTK4
#define BERR_PIN            PORTK1
#define BGACK_PIN           PORTK0
#define BR_PIN              PORTK3

#define IPL0_PIN            PORTE5
#define IPL1_PIN            PORTE4
#define IPL2_PIN            PORTE3

#define FC0_PIN             PORTK5
#define FC1_PIN             PORTK6
#define FC2_PIN             PORTK7

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

#define IsReading()         (PORTH & (1<<RW_PORT)) > 0

#endif