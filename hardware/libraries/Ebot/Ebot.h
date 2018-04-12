#ifndef __EBOT_H__
#define __EBOT_H__



#include <Arduino.h>
//#include <SD.h>
#include <Motors.h>
//#include <EasyTransfer.h>
//#include <Wav.h>
#include "Adafruit_NeoPixel.h"

#include "EbotSound.h"

#ifdef EBOT_8
#include "MCP7940.h"
#include <IR.h>

#endif

#include <SoftwareSerial.h>

#define PRESSED 1
#define RELEASED 2

//in built pins on EBOT_4
#ifdef EBOT_4

#define RGB_LED 					11
#define BUZZER_BUILTIN 				10

//#define BLE_RST 		8
#define BLE_KEY 		15

#define	ebot4_led_pin		13
#define	ebot4_rgb_pin		11
#define ebot4_buzzer_pin	10


#define D_0				8
#define D_1				7
#define D_2				22
#define D_3				23

#define SW_1			16
#define SW_2			14

#endif


//MPU6050
#define quaternionX					0
#define quaternionY					1
#define quaternionZ					2
#define quaternionW					3
#define euler0							4
#define euler1							5
#define euler2							6
#define yaw									7
#define pitch								8
#define roll								9
#define realAccelerationX		10
#define realAccelerationY		11
#define realAccelerationZ		12
#define accelerationX				13
#define accelerationY				14
#define accelerationZ				15

#define angleX							16
#define angleY							17
#define angleZ							18

void mpu6050_init();
float getTempfromMPU();
float readFromMPU(int choice);



struct pinStatus
{
	unsigned char cu;
	unsigned char pre;
	boolean acted;
};


void ebot_setup();

#ifdef EBOT_8
void rtc_init();
void setTime(DateTime& dt);
DateTime getTime();
uint16_t getYear();
uint8_t getMonth();
uint8_t getDay() ;
uint8_t getHour();
uint8_t getMinute();
uint8_t getSecond();
uint8_t getDayOfWeek();
bool ir_rc_off();
bool ir_rc(IR *ir, unsigned char key);
#endif

//#ifdef EBOT_4
//bool ir_rc_off();
//bool ir_rc(IR *ir, unsigned char key);
//#endif

//#ifdef	EBOT_8
void ebotSing (int index);
void ebotSingInit (uint8_t index);
//#endif


unsigned char button(unsigned char PinNumber, unsigned char State);
unsigned char edge(unsigned char PinNumber, unsigned char State);

/////////* New functions for serial Check*/////////////////////
bool isSerCharEqls(HardwareSerial *s,char c);
bool isSerStrEqls(HardwareSerial *s, char *c);
bool isSerCharEqls(SoftwareSerial *s, char c);
bool isSerStrEqls(SoftwareSerial *s, char *c);

bool isSerCharEqls(char c);
bool isSerStrEqls(char *c);
/////////////////////////////////////////////////////////////


///////////////////* RGB LED strip */////////////////////////
uint32_t Wheel(Adafruit_NeoPixel *strip, byte WheelPos);
void colorWipe(Adafruit_NeoPixel *strip, uint8_t r,uint8_t g,uint8_t b, uint8_t wait);
void rainbow(Adafruit_NeoPixel *strip, uint8_t wait);
void colorGradient(Adafruit_NeoPixel *strip, uint8_t r1,uint8_t g1,uint8_t b1,uint8_t r2,uint8_t g2,uint8_t b2);
/////////////////////////////////////////////////////////////


void LMotor_1(int speed);
void RMotor_1(int speed);
#ifdef EBOT_X
void LMotor_2(int speed);
void RMotor_2(int speed);
#endif

#endif
