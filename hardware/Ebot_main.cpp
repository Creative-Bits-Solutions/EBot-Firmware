#include "Servo.h"

#include "Ebot.h"
Servo myservo_0;


void setup()
{
	//Initialisations
	ebot_setup();
	ebotSingInit(0);


}

void loop()
{
	for(int i=0; i<22; i++){
		ebotSing(i);
	}
}
