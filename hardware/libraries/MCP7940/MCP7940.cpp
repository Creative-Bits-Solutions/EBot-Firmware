/*	library based on jeelabs RTClib [original library at https://github.com/jcw/rtclib ] altered to support Microchip MCP7940M RTC, used in Arduino
	based embedded environments. To use this library, add #include <MCP7940.h> to the top of your program.*/

#include <Wire.h>
#ifndef ENERGIA
#include <avr/pgmspace.h>
#else
#define pgm_read_word(data) *data
#define pgm_read_byte(data) *data
#define PROGMEM
#endif
#include "MCP7940.h"
#include <Arduino.h>

#define RTC_ADD           0x6F
#define RTC_CONREG        0x07
#define RTC_ALARM         0x0D
#define SECONDS_PER_DAY   86400L

/*	THIS SECTION COPIED DIRECTLY FROM RTClib [ https://github.com/jcw/rtclib ]
	USE PER http://opensource.org/licenses/mit-license.php */




DateTime::DateTime (uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec, uint8_t dw) {
  if (year >= 2000)
    year -= 2000;
  yOff = year;
  m = month;
  d = day;
  hh = hour;
  mm = min;
  ss = sec;
  dow=dw;
}

static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

//DateTime::DateTime (const char* date, const char* time) {
//  yOff = conv2d(date + 9);
//  switch(date[0]) {
//    case 'J': m = date[1] == 'a' ? 1 : m = date[2] == 'n' ? 6 : 7; break;
//    case 'F': m = 2; break;
//    case 'A': m = date[2] == 'r' ? 4 : 8; break;
//    case 'M': m = date[2] == 'r' ? 3 : 5; break;
//    case 'S': m = 9; break;
//    case 'O': m = 10; break;
//    case 'N': m = 11; break;
//    case 'D': m = 12; break;
//  }
//  d=conv2d(date+4);
//  hh=conv2d(time);
//  mm=conv2d(time+3);
//  ss=conv2d(time+6);
//}

//uint8_t DateTime::DayOfWeek() const{
//  uint16_t day = get()/SECONDS_PER_DAY;
//  return (day+6)%7;
//}


/*	END SECTION COPIED FROM RTClib */

/*	THIS SECTION BASED ON RTClib
	It has been adjusted to support the Microchip MCP7940M

	Version:	Initial
	Author:		C.Koiter
	Date:		August 2014
*/

void RTC_MCP7940::begin() {
  Wire.begin();
}




void RTC_MCP7940::adjust(const DateTime& dt) {	//change date and time registers based on user input
  Wire.beginTransmission(RTC_ADD);
  Wire.write((byte) 0);
  Wire.write(bin2bcd((byte) 10000000));
  Wire.write(bin2bcd(dt.minute()));
  Wire.write(bin2bcd(dt.hour()));
  Wire.write(bin2bcd(dt.DayOfWeek() | 0x08)); //enable RTC battery backup function and DOW
  Wire.write(bin2bcd(dt.day()));
  Wire.write(bin2bcd(dt.month()));
  Wire.write(bin2bcd(dt.year() - 2000));
  Wire.write((byte) 0);
  Wire.endTransmission();
}

bool RTC_MCP7940::isset() {		//check whether clock is set
  Wire.beginTransmission(RTC_ADD);
  Wire.write((byte) 0);
  Wire.endTransmission();
  Wire.requestFrom(RTC_ADD, 1);
  return ((Wire.read() & (1 << 7)) == (1 << 7));
}

DateTime RTC_MCP7940::now() {					//current date and time
  Wire.beginTransmission(RTC_ADD);
  Wire.write((byte) 0);
  Wire.endTransmission();
  Wire.requestFrom(RTC_ADD, 7);
  uint8_t ss = bcd2bin(Wire.read()) - 80;
  uint8_t mm = bcd2bin(Wire.read());
  uint8_t hh = bcd2bin(Wire.read());
  uint8_t dow = bcd2bin(Wire.read());
  uint8_t d = bcd2bin(Wire.read()& 0x3F);
  uint8_t m = bcd2bin(Wire.read() & 0x1F);
  uint16_t y = bcd2bin(Wire.read()) + 2000;

  return DateTime (y, m, d, hh, mm, ss, dow);
}

void RTC_MCP7940::configure(uint8_t value) {		//enable and disable alarms, external oscillator, sqw output
  /*
     bit 7    OUT : Logic Level for General Purpose Output bit
                      1 = MFP signal level is logic high
                      0 = MFP signal level is logic low
    bit 6 SQWEN   : Square Wave Output Enable bit
                      1 = Enable Square Wave Clock Output mode
                      0 = Disable Square Wave Clock Output mode
    bit 5 ALM1EN  : Alarm 1 Module Enable bit
                      1 = Alarm 1 enabled
                      0 = Alarm 1 disabled
    bit 4 ALM0EN  : Alarm 0 Module Enable bit
                      1 = Alarm 0 enabled
                      0 = Alarm 0 disabled
    bit 3 EXTOSC  : External Oscillator Input bit
                      1 = Enable X1 pin to be driven by external 32.768 kHz source
                      0 = Disable external 32.768 kHz input
    bit 2 CRSTRIM : Coarse Trim Mode Enable bit
                      Coarse Trim mode results in the MCP7940N applying digital trimming every 64 Hz clock cycle.
                      1 = Enable Coarse Trim mode. If SQWEN = 1, MFP will output trimmed 64 Hz (1) nominal clock signal.
                      0 = Disable Coarse Trim mode
                      See Section 5.6 “Digital Trimming” for details
    bit 1-0 SQWFS<1:0>: Square Wave Clock Output Frequency Select bits
                      If SQWEN = 1 and CSTRIM = 0:
                      Selects frequency of clock output on MFP
                      00 = 1 Hz (1)         01 = 4.096 kHz (1)         10 = 8.192 kHz (1)            11 = 32.768 kHz       If SQWEN = 0 or CRSTRIM = 1:

                      1 0 0 1 1 0 00 => 0x98
  */
  Wire.beginTransmission(RTC_ADD);
  Wire.write(RTC_CONREG);
  Wire.write(value);
  Wire.endTransmission();
}


void RTC_MCP7940::setAlarm(DateTime dt) {		//set Alarm 0 interval, interrupt flag, MFP state when triggered


  /*  bit 7 ALMPOL        : Alarm Interrupt Output Polarity bit
                              1 = Asserted output state of MFP is a logic high level
                              0 = Asserted output state of MFP is a logic low level
      bit 6-4 ALMxMSK<2:0>: Alarm Mask bits
                              000 = Seconds match
                              001 = Minutes match
                              010 = Hours match (logic takes into account 12-/24-hour operation)
                              011 = Day of week match
                              100 = Date match
                              101 = Reserved; do not use
                              110 = Reserved; do not use
                              111 = Seconds, Minutes, Hour, Day of Week, Date and Month
      bit 3 ALMxIF: Alarm Interrupt Flag bit (1,2)
                              1 = Alarm match occurred (must be cleared in software)
                              0 = Alarm match did not occur
      bit 2-0 WKDAY<2:0>: Binary-Coded Decimal Value of Day bits
                             Contains a value from 1 to 7. The representation is user-defined.
      Note  1:    If a match condition still exists when this bit is cleared, it will be set again automatically.
            2:    The ALMxIF bit cannot be written to a 1 in software. Writing to the ALMxWKDAY register will always clear
                  the ALMxIF bit.


                  0  111 0 111(make WKDAY 7 always) =>0111 0xxx => 0x7x
  */

  Wire.beginTransmission(RTC_ADD);
  Wire.write(0x0A);             //Alarm reg starts here
  Wire.write(bin2bcd(dt.second()));
  Wire.write(bin2bcd(dt.minute()));
  Wire.write(bin2bcd(dt.hour()));
  Wire.write(0x70 | (bin2bcd(dt.DayOfWeek())));
  Wire.write(bin2bcd(dt.day()));
  Wire.write(bin2bcd(dt.month()));
  Wire.write((byte) 0);
  Wire.endTransmission();
}

void RTC_MCP7940::clearAlarm() { 
  Wire.beginTransmission(RTC_ADD);
  Wire.write(RTC_CONREG);
  Wire.endTransmission();
  Wire.requestFrom(RTC_ADD, 7);
  uint8_t status = bcd2bin(Wire.read());

  Wire.beginTransmission(RTC_ADD);
  Wire.write(RTC_CONREG);             //Alarm reg starts here
  Wire.write(bin2bcd(status)& 0xEF);// Clear ALM0EN
  Wire.endTransmission();


  Wire.beginTransmission(RTC_ADD);
  Wire.write(RTC_ALARM);
  Wire.endTransmission();
  Wire.requestFrom(RTC_ADD, 7);
  status = bcd2bin(Wire.read());

  Wire.beginTransmission(RTC_ADD);
  Wire.write(RTC_CONREG);             //Alarm reg starts here
  Wire.write(bin2bcd(status)& 0xF7);// Clear ALM0EN
  Wire.endTransmission();
  


}
// uint8_t RTC_MCP7940::ordinalDate(uint8_t toDay, uint8_t toMonth){	//convert user date to ordinal (julian) date
// 	Wire.beginTransmission(RTC_ADD);
// 	Wire.write(0x05);
// 	Wire.endTransmission();
// 	Wire.requestFrom(RTC_ADD, 1);
// 	uint8_t leap = Wire.read();
// 	uint8_t ordinal = 0;
// 	if(leap>=32){
// 		switch(toMonth){
// 			case 1: ordinal = toDay; break;
// 			case 2: ordinal = toDay + 31; break;
// 			case 3: ordinal = toDay + 60; break;
// 			case 4: ordinal = toDay + 91; break;
// 			case 5: ordinal = toDay + 121; break;
// 			case 6: ordinal = toDay + 152; break;
// 			case 7: ordinal = toDay + 182; break;
// 			case 8: ordinal = toDay + 213; break;
// 			case 9: ordinal = toDay + 244; break;
// 			case 10: ordinal = toDay + 274; break;
// 			case 11: ordinal = toDay + 305; break;
// 			case 12: ordinal = toDay + 335; break;
// 		}
// 	}
// 	else if(leap<32){												//leap year support
// 		switch(toMonth){
// 			case 1: ordinal = toDay; break;
// 			case 2: ordinal = toDay + 31; break;
// 			case 3: ordinal = toDay + 59; break;
// 			case 4: ordinal = toDay + 90; break;
// 			case 5: ordinal = toDay + 120; break;
// 			case 6: ordinal = toDay + 151; break;
// 			case 7: ordinal = toDay + 181; break;
// 			case 8: ordinal = toDay + 212; break;
// 			case 9: ordinal = toDay + 243; break;
// 			case 10: ordinal = toDay + 273; break;
// 			case 11: ordinal = toDay + 304; break;
// 			case 12: ordinal = toDay + 334; break;
// 		}
// 	}
// 	return ordinal;
// }
