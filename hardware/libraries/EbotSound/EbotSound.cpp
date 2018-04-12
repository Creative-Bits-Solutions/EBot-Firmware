#include "Arduino.h"
#include "EbotSound.h"

EbotSound::EbotSound()
{
  //constructor
}

void EbotSound::init(uint8_t pinNumb)
{
  EbotSound::PinNumb= pinNumb;
  pinMode(PinNumb,OUTPUT);

 }

void EbotSound::_tone (float noteFrequency, long noteDuration, int silentDuration){

      if(silentDuration==0){silentDuration=1;}

      tone(EbotSound::PinNumb, noteFrequency, noteDuration);
      delay(noteDuration);       //milliseconds to microseconds
      //noTone(PIN_Buzzer);
      delay(silentDuration);
}


void EbotSound::bendTones (float initFrequency, float finalFrequency, float prop, long noteDuration, int silentDuration){

  //Examples:
  //  bendTones (880, 2093, 1.02, 18, 1);
  //  bendTones (note_A5, note_C7, 1.02, 18, 0);
  
  

  if(silentDuration==0){silentDuration=1;}

  if(initFrequency < finalFrequency)
  {
      for (int i=initFrequency; i<finalFrequency; i=i*prop) {
          _tone(i, noteDuration, silentDuration);
      }

  } else{

      for (int i=initFrequency; i>finalFrequency; i=i/prop) {
          _tone(i, noteDuration, silentDuration);
      }
  }
  
}


void EbotSound::sing(int songName){

  switch(songName){

    case S_connection:
      _tone(note_E5,50,30);
      _tone(note_E6,55,25);
      _tone(note_A6,60,10);
    break;

    case S_disconnection:
      _tone(note_E5,50,30);
      _tone(note_A6,55,25);
      _tone(note_E6,50,10);
    break;

    case S_buttonPushed:
      bendTones (note_E6, note_G6, 1.03, 20, 2);
      delay(30);
      bendTones (note_E6, note_D7, 1.04, 10, 2);
    break;

    case S_mode1:
      bendTones (note_E6, note_A6, 1.02, 30, 10);  //1318.51 to 1760
    break;

    case S_mode2:
      bendTones (note_G6, note_D7, 1.03, 30, 10);  //1567.98 to 2349.32
    break;

    case S_mode3:
      _tone(note_E6,50,100); //D6
      _tone(note_G6,50,80);  //E6
      _tone(note_D7,300,0);  //G6
    break;

    case S_surprise:
      bendTones(800, 2150, 1.02, 10, 1);
      bendTones(2149, 800, 1.03, 7, 1);
    break;

    case S_OhOoh:
      bendTones(880, 2000, 1.04, 8, 3); //A5 = 880
      delay(200);

      for (int i=880; i<2000; i=i*1.04) {
           _tone(note_B5,5,10);
      }
    break;

    case S_OhOoh2:
      bendTones(1880, 3000, 1.03, 8, 3);
      delay(200);

      for (int i=1880; i<3000; i=i*1.03) {
          _tone(note_C6,10,10);
      }
    break;

    case S_cuddly:
      bendTones(700, 900, 1.03, 16, 4);
      bendTones(899, 650, 1.01, 18, 7);
    break;

    case S_sleeping:
      bendTones(100, 500, 1.04, 10, 10);
      delay(500);
      bendTones(400, 100, 1.04, 10, 1);
    break;

    case S_happy:
      bendTones(1500, 2500, 1.05, 20, 8);
      bendTones(2499, 1500, 1.05, 25, 8);
    break;

    case S_superHappy:
      bendTones(2000, 6000, 1.05, 8, 3);
      delay(50);
      bendTones(5999, 2000, 1.05, 13, 2);
    break;

    case S_happy_short:
      bendTones(1500, 2000, 1.05, 15, 8);
      delay(100);
      bendTones(1900, 2500, 1.05, 10, 8);
    break;

    case S_sad:
      bendTones(880, 669, 1.02, 20, 200);
    break;

    case S_confused:
      bendTones(1000, 1700, 1.03, 8, 2);
      bendTones(1699, 500, 1.04, 8, 3);
      bendTones(1000, 1700, 1.05, 9, 10);
    break;

    case S_fart1:
      bendTones(1600, 3000, 1.02, 2, 15);
    break;

    case S_fart2:
      bendTones(2000, 6000, 1.02, 2, 20);
    break;

    case S_fart3:
      bendTones(1600, 4000, 1.02, 2, 20);
      bendTones(4000, 3000, 1.02, 2, 20);
    break;
	case S_mario1:
       _tone(note_E7,12,1);
	   _tone(note_E7,12,12);
	   _tone(note_E7,12,12);
	   _tone(note_C7,12,1);
	   _tone(note_C7,12,12);
	   _tone(note_G7,12,30);
	   _tone(note_G6,12,30);
	   
	   _tone(note_C7,12,20);
	   _tone(note_G6,12,20);
   	   _tone(note_E6,12,20);
	   _tone(note_A6,12,20);
	   _tone(note_B6,12,12);
	   _tone(note_A6,12,1);
	   _tone(note_A6,12,12);
   
   	   _tone(note_G6,12,1);
	   _tone(note_E7,12,1);
   	   _tone(note_G7,12,1);
	   _tone(note_A7,12,12);
	   _tone(note_F7,12,1);
	   _tone(note_G7,12,12);
	   _tone(note_E7,12,12);
		_tone(note_C7,12,1);
		_tone(note_D7,12,1);
		 _tone(note_B6,12,20);
		 
		 _tone(note_C7,12,20);
	   _tone(note_G6,12,20);
   	   _tone(note_E6,12,20);
	   _tone(note_A6,12,20);
	   _tone(note_B6,12,12);
	   _tone(note_A6,12,1);
	   _tone(note_A6,12,12);
	   
	    _tone(note_G6,12,1);
	   _tone(note_E7,12,1);
   	   _tone(note_G7,12,1);
	   _tone(note_A7,12,12);
	   _tone(note_F7,12,1);
	   _tone(note_G7,12,12);
	   _tone(note_E7,12,12);
		_tone(note_C7,12,1);
		_tone(note_D7,12,1);
		 _tone(note_B6,12,20);
		 
	   
    break;
	
	case S_mario2:
       _tone(note_C4,12,1);
	   _tone(note_C5,12,1);
	   _tone(note_A3,12,1);
	   _tone(note_A4,12,1);
	   _tone(note_A3,12,1);
	   _tone(note_A4,12,9);
	   
       _tone(note_C4,12,1);
	   _tone(note_C5,12,1);
	   _tone(note_A3,12,1);
	   _tone(note_A4,12,1);
	   _tone(note_A3,12,1);
	   _tone(note_A4,12,9);
   
   	   _tone(note_F3,12,1);
	   _tone(note_F4,12,1);
   	   _tone(note_D3,12,1);
	   _tone(note_D4,12,1);
	   _tone(note_D3,12,1);
	   _tone(note_D4,12,9);
		 
  	   _tone(note_F3,12,1);
	   _tone(note_F4,12,1);
   	   _tone(note_D3,12,1);
	   _tone(note_D4,12,1);
	   
	   _tone(note_D3,12,1);
	   _tone(note_D4,12,12);
	   
	    _tone(note_D4,18,1);
	   _tone(note_C4,18,1);
   	   _tone(note_D4,18,1);
	   
	   
	   _tone(note_C4,6,1);
	   _tone(note_D4,6,1);
	   
	   _tone(note_D4,6,1);
	   _tone(note_G3,6,1);
	   
		_tone(note_G3,6,1);
		_tone(note_C4,6,1);
		
		 _tone(note_C4,18,1);
		 _tone(note_F4,18,1);
		 _tone(note_F4,18,1);
		 _tone(note_E3,18,1);
		 _tone(note_A4,18,1);
		 _tone(note_A4,18,1);
		 
		 _tone(note_G4,10,1);
		 _tone(note_D4,10,1);
		 _tone(note_B3,10,1);
		 
		 _tone(note_A3,10,1);
		 _tone(note_A3,10,1);
		 _tone(note_G3,10,9);
		 
	   
    break;
	

  }
}
