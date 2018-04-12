#include <Ebot.h>
//#include <SoftwareSerial.h>

#ifdef EBOT_8
	RTC_MCP7940 rtc; //rtc
#endif

//#ifdef	EBOT_8
EbotSound	eboSound;
//#endif

//#ifdef EBOT_4
/* =========== For MPU6050 ============================*/
#include "I2Cdev.h"
#include "libraries\MPU6050\MPU6050_6Axis_MotionApps20.h"
MPU6050 mpu;
// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };


void mpu6050_init() {
	// join I2C bus (I2Cdev library doesn't do this automatically)
	#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
	Wire.begin();
	TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
	#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
	Fastwire::setup(400, true);
	#endif

	mpu.initialize();
	if(mpu.testConnection()==0)
	Serial.println("MPU6050 connection failed");


	// load and configure the DMP
	devStatus = mpu.dmpInitialize();

	// supply your own gyro offsets here, scaled for min sensitivity
	mpu.setXGyroOffset(220);
	mpu.setYGyroOffset(76);
	mpu.setZGyroOffset(-85);
	mpu.setZAccelOffset(1688); // 1688 factory default for my test chip

	// make sure it worked (returns 0 if so)
	if (devStatus == 0) {
		// turn on the DMP, now that it's ready
		mpu.setDMPEnabled(true);

		// get expected DMP packet size for later comparison
		packetSize = mpu.dmpGetFIFOPacketSize();
	} else {
		// ERROR!
		// 1 = initial memory load failed
		// 2 = DMP configuration updates failed
		// (if it's going to break, usually the code will be 1)
		Serial.print(F("DMP Initialization failed (code "));
	}
}

float getTempfromMPU(){
	return mpu.getTemperatureDegreeC();
}



float readFromMPU(int choice){
	float MPUdata;
	boolean validData=0;
	while(validData==0){
		//Serial.print(".");
		mpuIntStatus = mpu.getIntStatus();
		// get current FIFO count
		fifoCount = mpu.getFIFOCount();
		// check for overflow (this should never happen unless our code is too inefficient)
		if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
			// reset so we can continue cleanly
			mpu.resetFIFO();
			//Serial.println(F("FIFO overflow!"));
			// otherwise, check for DMP data ready interrupt (this should happen frequently)
		} else if (mpuIntStatus & 0x02) {
			// wait for correct available data length, should be a VERY short wait
			while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
			// read a packet from FIFO
			mpu.getFIFOBytes(fifoBuffer, packetSize);

			validData=true;
			// track FIFO count here in case there is > 1 packet available
			// (this lets us immediately read more without waiting for an interrupt)
			mpu.dmpGetQuaternion(&q, fifoBuffer);
			mpu.dmpGetEuler(euler, &q);
			mpu.dmpGetGravity(&gravity, &q);
			mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
			mpu.dmpGetAccel(&aa, fifoBuffer);
			mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
			mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);

			switch(choice){
				/*case quaternionX:		MPUdata= q.x;			break;
				case quaternionY:		MPUdata= q.y;			break;
				case quaternionZ:		MPUdata= q.z;			break;
				case quaternionW:		MPUdata= q.w;			break;

				case euler0:			MPUdata= euler[0]* 180/M_PI;	break;
				case euler1:			MPUdata= euler[1]* 180/M_PI;	break;
				case euler2:			MPUdata= euler[2]* 180/M_PI;	break;

				case yaw:					MPUdata= ypr[0] * 180/M_PI;		break;
				case pitch:				MPUdata= ypr[1] * 180/M_PI;		break;
				case roll:				MPUdata= ypr[2] * 180/M_PI;		break;

				case realAccelerationX:			MPUdata= aaReal.x;		break;
				case realAccelerationY:			MPUdata= aaReal.y;		break;
				case realAccelerationZ:			MPUdata= aaReal.z;		break;

				case accelerationX:			MPUdata= aaWorld.x;		break;
				case accelerationY:			MPUdata= aaWorld.y;		break;
				case accelerationZ:			MPUdata= aaWorld.z;		break;
				*/
				case realAccelerationX:			MPUdata= aaReal.x;		break;
				case realAccelerationY:			MPUdata= aaReal.y;		break;
				case realAccelerationZ:			MPUdata= aaReal.z;		break;

				case accelerationX:			MPUdata= aaWorld.x;		break;
				case accelerationY:			MPUdata= aaWorld.y;		break;
				case accelerationZ:			MPUdata= aaWorld.z;		break;

				case angleZ:			MPUdata= ypr[0] * 180/M_PI;		break;//MPUdata= euler[0]* 180/M_PI;	break; //euler0
				case angleX:			MPUdata= ypr[2]  * 180/M_PI;	break; //euler1 data is not reliable
				case angleY:			MPUdata= ypr[1] * 180/M_PI;		break;//MPUdata= euler[2]* 180/M_PI;	break; //euler2

				default:						MPUdata= 0;		break;
			}
		}
	}
	return MPUdata;
}
//#endif
/*===========================End of MPU==========================*/


#ifdef EBOT_4
#define NUMBER_OF_PINS	4
/*Adafruit_NeoPixel startup_strip = Adafruit_NeoPixel(4, 11, NEO_GRB + NEO_KHZ800);
uint8_t index = 0;
uint8_t rgb_startup_color[] =
{	0,	0,	255,		0,	255,255,		255,	0,	0,		255, 255, 255,
0,	0,	255,		0,	0,	255,		0,	0,	255,		255, 255, 255,
0,	255,255,		0,	255,255,		255,	0,	0,		255, 255, 255,
0,	0,	255,		0,	0,	255,		0,		0,	255,	0, 		0, 255,
0,	0,	0,			0,	0,	0,			0,		0,	0,		0, 		0, 0,
};

void startup_colorWipe(uint32_t c, uint8_t wait) {
for(uint16_t i=0; i<startup_strip.numPixels(); i++) {
startup_strip.setPixelColor(i, c);
startup_strip.show();
delay(wait);
}
}*/
#endif

#ifdef EBOT_8
#define NUMBER_OF_PINS	8
static unsigned char remote_cu_key = OFF;
//static decode_results res;
decode_results results ;
#endif

#ifdef EBOT_ARDUINO
#define NUMBER_OF_PINS	6
#endif

char serChar;
char serStr[31];

struct pinStatus edg[8], btn[8];

static Motors motors;

void ebot_setup()
{

	for(int i=0; i<NUMBER_OF_PINS; i++)
	{
		btn[i].cu =	btn[i].pre = HIGH ;
		btn[i].acted = false;
		edg[i].cu =	edg[i].pre = HIGH ;
		edg[i].acted = false;
	}
	#ifdef EBOT_4
	digitalWrite(17,LOW);//rx led
	pinMode(BLE_KEY,OUTPUT);
	digitalWrite(BLE_KEY,HIGH);
	#endif
	/*#ifdef EBOT_4
	edg[6].cu =	edg[6].pre = LOW ;
	edg[6].acted = false;
	edg[7].cu =	edg[7].pre = LOW ;
	edg[7].acted = false;

	#endif
	*/
	//#ifdef EBOT_4
	//	Serial.begin(9600);
	//	startup_strip.begin();
	//	startup_strip.show();
	//for (uint8_t ii = 0; ii < 5; ii++) {
	//startup_colorWipe(startup_strip.Color(rgb_startup_color[index++], rgb_startup_color[index++], rgb_startup_color[index++]),50); // Red
	//	delay(100);
	//	}
	//#endif
}

////////////////////////////////////////

#ifdef EBOT_8

void rtc_init(){
  rtc.begin();
  rtc.configure(0x98);
  //while (!rtc.isset());//wait till clock starts ticking

}

void setTime(DateTime& dt){
	rtc.adjust(dt);
}
DateTime getTime(){
	return rtc.now();
}

uint16_t getYear(){
	return rtc.now().year();
}
uint8_t getMonth()
{
	return rtc.now().month();
}
uint8_t getDay() {
	return rtc.now().day();
}
uint8_t getHour(){
	return rtc.now().hour();
}
uint8_t getMinute(){
	return rtc.now().minute();
}
uint8_t getSecond(){
	return rtc.now().second();
}
uint8_t getDayOfWeek(){
	return rtc.now().DayOfWeek();
}


bool ir_rc(IR *ir, unsigned char key)
{
	if(ir->decode(&results))
	{
		if(ir->channel() == results.channel)
		{
			remote_cu_key = results.value;
		}
		else
		remote_cu_key = KEY_IDLE;

		//remote_cu_key = results.value; // no channels for ebot 4
		ir->resume();
	}

	if(remote_cu_key == key)
	{
		remote_cu_key = KEY_IDLE;
		return true;
	}

	return false;
}
#endif


//#ifdef EBOT_4

//bool ir_rc(IR *ir, unsigned char key)
//{
// 	if(ir->decode(&results))
//	{
//		remote_cu_key = results.value; // no channels for ebot 4
//		ir->resume();
//	}
//	if(remote_cu_key == key)
//	{
//		remote_cu_key = KEY_IDLE;
//		return true;
//	}
//	return false;
//}
//#endif


/////////////////////////////////sofserial//////////////////////////////////
bool isSerCharEqls(SoftwareSerial *s, char c){
	if(s->available()>0)
	{
		serChar=s->read();
	}

	if(serChar==c){
		c=' ';
		return true;
	}
	else
	return false;
}

bool isSerStrEqls(SoftwareSerial *s,char *c){
	if(s->available()>0)
	{
		s->setTimeout(200);
		byte size =s->readBytesUntil('\n',serStr, 30);
		serStr[size]=0;
		if(serStr[size-1]=='\r') //to discard carriage return
		serStr[size-1]=0;
	}

	if(!(strcmp(serStr,c))){
		strcpy(serStr,0);
		return true;
	}
	else
	return false;

}
/////////////////////////////Serial///////////////////////////////

bool isSerCharEqls(HardwareSerial *s, char c){

	if(s->available()>0)
	{
		serChar=s->read();
	}

	if(serChar==c){
		serChar=' ';
		return true;
	}
	else
	return false;
}

bool isSerStrEqls(HardwareSerial *s,char *c){
	if(s->available()>0)
	{
		s->setTimeout(200);
		byte size =s->readBytesUntil('\n',serStr, 30);
		serStr[size]=0;
		if(serStr[size-1]=='\r') //to discard carriage return
		serStr[size-1]=0;
	}

	if(!(strcmp(serStr,c))){
		strcpy(serStr,0);
		return true;
	}
	else
	return false;

}

//////////////////ebot4 specific Serial////////////////////////////////
bool isSerCharEqls(char c){

	if(Serial.available()>0)
	{
		serChar=Serial.read();
	}

	if(serChar==c){
		serChar=' ';
		return true;
	}
	else
	return false;
}

bool isSerStrEqls(char *c){
	if(Serial.available()>0)
	{
		Serial.setTimeout(200);
		byte size =Serial.readBytesUntil('\n',serStr, 30);
		serStr[size]=0;
		if(serStr[size-1]=='\r') //to discard carriage return
		serStr[size-1]=0;
	}

	if(!(strcmp(serStr,c))){
		strcpy(serStr,0);
		return true;
	}
	else
	return false;

}
////////////////////////////BUTTON////////////////////////////////
unsigned char button(unsigned char PinNumber, unsigned char State)
{
	//unsigned char pin;
	boolean digitialValue=digitalRead(PinNumber);
	#ifdef EBOT_4

	if(State == PRESSED)
	{
		return ((PinNumber==SW_1)||(PinNumber==SW_2)?digitialValue: !digitialValue );
	}
	else if(State == RELEASED)
	{
		return ((PinNumber==SW_1)||(PinNumber==SW_2)? !digitialValue: digitialValue );
	}

	#endif

	#ifdef EBOT_8
	if(State == PRESSED)
	{
		return (!digitialValue);
	}
	else if(State == RELEASED)
	{
		return (digitialValue );
	}

	#endif
/*	switch(PinNumber)
	{
		case A0:
		pin = 0;
		break;
		case A1:
		pin = 1;
		break;
		case A2:
		pin = 2;
		break;
		case A3:
		pin = 3;
		break;
		case A4:
		pin = 4;
		break;
		case A5:
		pin = 5;
		break;
		case A6:
		pin = 6;
		break;
		case A7:
		pin = 7;
		break;
		#ifdef EBOT_4
		//if((PinNumber==SW_1)||(PinNumber==SW_2)){
		//	return (digitalRead(PinNumber));
		//}
		case SW_1:
		pin = 6;
		break;
		case SW_2:
		pin = 7;
		break;
		#endif

		default:
		return false;
	}

	btn[pin].pre = btn[pin].cu;
	if(digitalRead(PinNumber))
	{
		//delay(50);
		if(digitalRead(PinNumber))
		{
			//btn[pin].cu = ((PinNumber==SW_1)||(PinNumber==SW_2))?LOW: HIGH; // because it's active high. //HIGH;
			btn[pin].cu = HIGH; // because it's active high. //HIGH;
		}
	}
	else
	//btn[pin].cu = ((PinNumber==SW_1)||(PinNumber==SW_2))?HIGH: LOW; //LOW;
  btn[pin].cu =LOW;

	if(State == PRESSED)
	{

		if((btn[pin].cu == HIGH) && (btn[pin].pre == LOW))
		{
			btn[pin].acted = true;
			return true;
		}
		return false;
	}
	if(State == RELEASED)
	{
		if((btn[pin].acted == true) && (btn[pin].cu == LOW))
		{
			btn[pin].acted = false;
			return true;
		}
		else if((btn[pin].cu == LOW) && (btn[pin].pre == HIGH))
		{
			return true;
		}

		return false;
	}
	return false;
*/
}
///////////////////////////////////////////////////////////////
unsigned char edge(unsigned char PinNumber, unsigned char State)
{
	digitalWrite(PinNumber, HIGH);

	unsigned char pin;
	switch(PinNumber)
	{
		case A0:
		pin = 0;
		break;
		case A1:
		pin = 1;
		break;
		case A2:
		pin = 2;
		break;
		case A3:
		pin = 3;
		break;
		case A4:
		pin = 4;
		break;
		case A5:
		pin = 5;
		break;
		case A6:
		pin = 6;
		break;
		case A7:
		pin = 7;
		break;
		default:
		return false;
	}

	edg[pin].pre = edg[pin].cu;
	if(!digitalRead(PinNumber))
	{
		edg[pin].cu = HIGH;
	}
	else
	edg[pin].cu = LOW;

	if(State == FALLING)
	{
		if((edg[pin].cu == HIGH) && (edg[pin].pre == LOW))
		{
			return true;
		}
		return false;
	}
	else if(State == RISING)
	{
		if((edg[pin].cu == LOW) && (edg[pin].pre == HIGH))
		{
			return true;
		}

		return false;
	}
	return false;
}
////////////////////////
void LMotor_1(int speed)
{
	#if defined(EBOT_8) || defined(EBOT_4)
	motors.setMLSpeed(speed*25.5);
	#endif
	#ifdef EBOT_X
	motors.setML1Speed(speed*25.5);
	#endif
}
//////////////////////
void RMotor_1(int speed)
{
	#if defined(EBOT_8) || defined(EBOT_4)
	motors.setMRSpeed(speed*25.5);
	//motors.setMRSpeed(255);
	#endif
	#ifdef EBOT_X
	motors.setMR1Speed(speed*25.5);
	#endif
}
///////////////////////
#ifdef EBOT_X
void LMotor_2(int speed)
{
	motors.setML2Speed(speed*25.5);
}
/////////
void RMotor_2(int speed)
{
	motors.setMR2Speed(speed*25.5);
}
#endif

//////////////////////////			RGB LED STRIP					////////////////////////
uint32_t Wheel(Adafruit_NeoPixel *strip, byte WheelPos) {
	WheelPos = 255 - WheelPos;
	if(WheelPos < 85) {
		return strip->Color((255 - WheelPos * 3)/2.5, 0, WheelPos * 3/2.5);
	}
	if(WheelPos < 170) {
		WheelPos -= 85;
		return strip->Color(0, WheelPos * 3/2.5, (255 - WheelPos * 3)/2.5);
	}
	WheelPos -= 170;
	return strip->Color(WheelPos * 3/2.5, (255 - WheelPos * 3)/2.5, 0);
}

void colorWipe(Adafruit_NeoPixel *strip, uint8_t r,uint8_t g,uint8_t b, uint8_t wait) {
	for(uint16_t i=0; i< strip->numPixels(); i++) {
		strip->setPixelColor(i, strip->Color(r/2.5,g/2.5,b/2.5)); // reduce brightness to avoid heating
		strip->show();
		delay(wait);
	}
}
void rainbow(Adafruit_NeoPixel *strip, uint8_t wait) {
	uint16_t i, j;
	for(j=0; j<256; j++) {
		for(i=0; i< strip->numPixels(); i++) {
			strip->setPixelColor(i, Wheel(strip,((i+j) & 255)));
		}
		strip->show();
		delay(wait);
	}
}

void colorGradient(Adafruit_NeoPixel *strip, uint8_t r1,uint8_t g1,uint8_t b1,uint8_t r2,uint8_t g2,uint8_t b2) {
	int rd,gd,bd;
	rd=(r2-r1)/ ((strip->numPixels())-1);
	gd=(g2-g1)/ ((strip->numPixels())-1);
	bd=(b2-b1)/ ((strip->numPixels())-1);
	for(uint8_t i=0;i<((strip->numPixels())-1) ;i++){
		strip->setPixelColor(i,strip->Color((r1+i*rd)/2.5,(g1+i*gd)/2.5,(b1+i*bd)/2.5));
	}
	strip->setPixelColor((strip->numPixels())-1,strip->Color(r2/2.5,g2/2.5,b2/2.5));
	strip->show();
	delay(20);
}
///////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////Sound//////////////////////////////////////////
//#ifdef	EBOT_8
void ebotSing (int index){
	eboSound.sing(index);
}
void ebotSingInit (uint8_t index){
	eboSound.init(index);
}
//#endif
///////////////////////////////////////////////////////
