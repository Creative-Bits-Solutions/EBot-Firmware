#include "arduino.h"

void myBlink(){
	pinMode(8,OUTPUT);
for(int i=0;i<100;i+=10)
{
	digitalWrite(8,HIGH);
	delay(i);
	digitalWrite(8,LOW);
	delay(100-i);
  }
}

