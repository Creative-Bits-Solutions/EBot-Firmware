#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>


/*
   PCINT15-8: D7-0  : bit 1
   PCINT31-24: D15-8  : bit 3
   PCINT23-16: D23-16 : bit 2
   PCINT7-0: D31-24   : bit 0
   */

#define EROBOT_MINI                 1
#define NUM_DIGITAL_PINS            27
#define NUM_ANALOG_INPUTS           8
//#define analogInputToDigitalPin(p)  ((p < NUM_ANALOG_INPUTS) ? (p) + 12 : -1)
#define analogInputToDigitalPin(p)  ((p < NUM_ANALOG_INPUTS) ? (p) + 19 : -1)

//#define digitalPinHasPWM(p)         ((p) == 28 || (p) == 29 || (p) == 30 || (p) == 31 || (p) == 2 || (p) == 4 || (p) == 6 || (p) == 7)
#define digitalPinHasPWM(p)         ((p) == 2 || (p) == 4 || (p) == 6 || (p) == 7 )


static const uint8_t SCL = 0;
static const uint8_t SDA = 1;


static const uint8_t SS   = 2;
static const uint8_t MOSI = 3;
static const uint8_t MISO = 4;
static const uint8_t SCK  = 5;
//PD6 6
//PD7 7
static const uint8_t LED = 8;


static const uint8_t A0 = 19;
static const uint8_t A1 = 20;
static const uint8_t A2 = 21;
static const uint8_t A3 = 22;
static const uint8_t A4 = 23;
static const uint8_t A5 = 24;
static const uint8_t A6 = 25;
static const uint8_t A7 = 26;


//Pin chage interrupts are per port, and the pins in e-robot boad are not in sequence
//so soft serial and other libraries which use this need pin number from arduino to be able to use this
//#define digitalPinToPCICR(p)    (((p) >= 0 && (p) < NUM_DIGITAL_PINS) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICR(p)    (((p) >= 0 && (p) < NUM_DIGITAL_PINS) ? (&PCICR) : ((uint8_t *)0))
//#define digitalPinToPCICRbit(p) (((p) <= 7) ? 1 : (((p) <= 15) ? 3 : (((p) <= 23) ? 2 : 0)))
//#define digitalPinToPCMSK(p)    (((p) <= 7) ? (&PCMSK2) : (((p) <= 13) ? (&PCMSK0) : (((p) <= 21) ? (&PCMSK1) : ((uint8_t *)0))))
//
//#define digitalPinToPCICRbit(p) (((p) == 0)?2:((p)==1)?2:((p)==2)?1:((p)==3)?1:((p)==4)?1:((p)==5)?1:((p)==6)?3:((p)==7)?3:0)
#define digitalPinToPCICRbit(p) (((p) <= 1)?2:((p)<=5)?1:((p)<=7)?3:0)

//
//
//#define digitalPinToPCMSK(p) (((p)==0?(&PCMSK2):((p)==1)?(&PCMSK2):((p)==2)?(&PCMSK1):((p)==3)?(&PCMSK1):((p)==4)?(&PCMSK1):((p)==5)?(&PCMSK1):((p)==6)?(&PCMSK3):((p)==7)?(&PCMSK3):((uint8_t *)0)))
//#define digitalPinToPCMSK(p) (((p)==0?(&PCMSK2):((p)==1)?(&PCMSK2):((p)==2)?(&PCMSK1):((p)==3)?(&PCMSK1):((p)==4)?(&PCMSK1):((p)==5)?(&PCMSK1):((p)==6)?(&PCMSK3):((p)==7)?(&PCMSK3):(&PCMSK0)))
#define digitalPinToPCMSK(p) ((p)<=1?(&PCMSK2):((p <=5)?(&PCMSK1):((p)<=7)?(&PCMSK3):(&PCMSK0)))
//
//#define digitalPinToPCMSKbit(p) ((p) % 8)
#define digitalPinToPCMSKbit(p) (((p) <= 1)?(p) : ((p) <= 5)?(p)+2:((p)<=7)?(p):(p)-19)

#ifdef ARDUINO_MAIN


#define PA 1
#define PB 2
#define PC 3
#define PD 4

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] =
{
	NOT_A_PORT,
	(uint16_t) &DDRA,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] =
{
	NOT_A_PORT,
	(uint16_t) &PORTA,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] =
{
	NOT_A_PORT,
	(uint16_t) &PINA,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] =
{
  PC , // 0 | d0
  PC , // 1 | d1
  PB , // 4 | d2 SS
  PB , // 5 | d3 MOSI
  PB , // 6 | d4 MISO
  PB , // 7 | d5 SCK
  PD , // 6 | d6
  PD , // 7 | d7

  PC , // 2 | LED

  PB , // 0 | M1	9
  PB , // 1 | M2	10
  PB , // 2 | M3	11
  PC , // 7 | M4	12
  PD , // 4 | PWM1  13
  PD , // 5 | PWM2	14
  PC , // 3 | STDBY 15

  PC , // IR0
  PC , // IR1
  PC , // IR2

  PA , // 0 | ADC 0
  PA , // 1 | ADC 1
  PA , // 2 | ADC 2
  PA , // 3 | ADC 3
  PA , // 4 | ADC 4
  PA , // 5 | ADC 5
  PA , // 6 | ADC 6
  PA , // 7 | ADC 7

	PD , // 27 built in RGB
	PD , // 28 BT key
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] =
{
  _BV(0),
  _BV(1),
  _BV(4),
  _BV(5),
  _BV(6),
  _BV(7),
  _BV(6),
  _BV(7), // 7

  _BV(2),

  _BV(0),
  _BV(1),
  _BV(2),
  _BV(7),
  _BV(4),
  _BV(5),
  _BV(3),

  _BV(6),
  _BV(5),
  _BV(4),


  _BV(0),
  _BV(1),
  _BV(2),
  _BV(3),
  _BV(4),
  _BV(5),
  _BV(6),
  _BV(7),


	_BV(2),//27
  _BV(3), //28
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] =
{
	NOT_ON_TIMER, //0
	NOT_ON_TIMER,//1
	TIMER0B,//2
	NOT_ON_TIMER,//3
	TIMER3A,//4
	TIMER3B,//5
	TIMER2B,//6
	TIMER2A, //7

	NOT_ON_TIMER,

	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER1B, // Motor
	TIMER1A,
	NOT_ON_TIMER,

	NOT_ON_TIMER, // IR
	NOT_ON_TIMER,
	NOT_ON_TIMER,

	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,

	NOT_ON_TIMER,//27
	NOT_ON_TIMER,//28
};

#endif

#endif // Pins_Arduino_h
// vim:ai:cin:sts=2 sw=2 ft=cpp
