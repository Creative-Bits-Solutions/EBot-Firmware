/*Library by TMRh20 2012-2013*/

#include <SD.h>
#include <Wav.h>

const int buffSize = 75; //note: there are 2 sound buffers. This will require (soundBuff*4) memory free
volatile unsigned int buffer[2][buffSize+1], buffCount = 0, resolution = 500;
volatile boolean buffEmpty[2] = {false,false}, whichBuff = false, loadCounter=0, playing = 0;
unsigned int tt=0;
int volMod=0;
boolean paused = 0, qual = 1, stopPlay = 0;

volatile byte *TIMSK[] = {&TIMSK1};	
volatile byte *TCCRnA[] = {&TCCR1A};
volatile byte *TCCRnB[] = {&TCCR1B};
volatile unsigned int *OCRnA[] = {&OCR1A};
volatile unsigned int *OCRnB[] = {&OCR1B};
volatile unsigned int *ICRn[]	= {&ICR1};

File sFile;


void Wav::setPin(){
	disable();
	pinMode(7,OUTPUT);
}

void Wav::quality(boolean q){
	if(!playing){qual = q;}
}

void Wav::play(char* filename){

  setPin(); 
  stop();

  if(!wavInfo(filename) ){ return; }//verify its a valid wav file
  sFile = SD.open(filename);

  if(sFile){
	playing = 1; paused = 0;
    sFile.seek(44); //skip the header info

	if(SAMPLE_RATE > 22050 ){ SAMPLE_RATE = 18000; Serial.print("SampleRate Too High");}


    if(qual){resolution = 8 * (1000000/SAMPLE_RATE);}
    else{ resolution = 16 * (1000000/SAMPLE_RATE);
	}

	unsigned int tmp = 0;
	if(volMod < 0 ){
		tmp = (sFile.read() >> volMod*-1);
	}else{
		tmp = (sFile.read() << volMod);
	}

	unsigned int mod = *OCRnA[tt];
	int mVal = 0;
	if(tmp > mod){ mVal = 3;
	}else{         mVal = -3;}

	for(int i=0; i<buffSize; i++){ mod = constrain(mod+mVal,mod,tmp); buffer[0][i] = mod; }
	for(int i=0; i<buffSize; i++){ mod = constrain(mod+mVal,mod,tmp); buffer[1][i] = mod; }
    whichBuff = 0; buffEmpty[0] = 0; buffEmpty[1] = 0; buffCount = 0;

    noInterrupts();
    *ICRn[tt] = resolution;        
    *TCCRnA[tt] = _BV(WGM11) | _BV(COM1A1) | _BV(COM1B0) | _BV(COM1B1); //WGM11,12,13 all set to 1 = fast PWM/w ICR TOP
    *TCCRnB[tt] = _BV(WGM13) | _BV(WGM12) | _BV(CS10);
    *TIMSK[tt] = ( _BV(ICIE1) | _BV(TOIE1) );
    
    TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00); 
    TCCR0B = (1 << CS00); // no prescale    
    TIMSK0 |= (1 << TOIE0);
    
    OCR0A = 0; 
                  
    interrupts();

  }else{Serial.println("Read fail"); }

}



void Wav::pause(){
	paused = !paused;
}

void Wav::volume(int upDown){

  if(upDown){
	  volMod++; volMod = min(volMod,3);
  }else{
	  volMod--; volMod = max(volMod, -4);
  }

}

void Wav::setVolume(int vol) {
    vol -= 4;
    min(vol,3);
    max(vol,-4);
    volMod = vol;
}

boolean Wav::wavInfo(char* filename){

  //check for the string WAVE starting at 8th bit in header of file
  File xFile = SD.open(filename);
  if(!xFile){return 0;}
  xFile.seek(8);
  char wavStr[] = {'W','A','V','E'};
  for (int i =0; i<4; i++){
	  if(xFile.read() != wavStr[i]){ Serial.println("WAV File Error"); break; }
  }

    xFile.seek(24);
    SAMPLE_RATE = xFile.read();
    SAMPLE_RATE = xFile.read() << 8 | SAMPLE_RATE;

    //verify that Bits Per Sample is 8 (0-255)
    xFile.seek(34); unsigned int dVar = xFile.read();
    dVar = xFile.read() << 8 | dVar;
    if(dVar != 8){Serial.print("Wrong BitRate"); xFile.close(); return 0;}
    xFile.close(); return 1;
}


ISR(TIMER1_CAPT_vect){
	  boolean a = !whichBuff;
	  if(buffEmpty[a]){
		*TIMSK[tt] &= ~(_BV(ICIE1));
		sei();
		unsigned int tmp;
		if(volMod < 0 ){ for(int i=0; i<buffSize; i++){ tmp = (sFile.read() >> volMod*-1); buffer[a][i] = min(tmp,resolution); 	} }
		else
		for(int i=0; i<buffSize; i++){ tmp = (sFile.read() << volMod); buffer[a][i] = min(tmp,resolution); 	}
		buffEmpty[a] = 0;
	  }

  if( playing && !paused){
		  *TIMSK[tt] |= ( _BV(ICIE1) | _BV(TOIE1) );
  }else
  if(paused && playing){*TIMSK[tt] &= ~( _BV(TOIE1) ); }

}


ISR(TIMER1_OVF_vect){

  loadCounter = !loadCounter;
  if(loadCounter == 0 && qual == 1){ return; }

  OCR0A =  *OCRnA[tt] = *OCRnB[tt] = buffer[whichBuff][buffCount];
    
  buffCount++;

  if(buffCount >= buffSize){
	  if(sFile.available() <= buffSize){
	  	playing = 0;
	  	*TIMSK[tt] &= ~( _BV(ICIE1) | _BV(TOIE1) );
	  	if(sFile){sFile.close();}
	  	OCR0A = 0;
	  }
      buffCount = 0;
      buffEmpty[whichBuff] = true;
      whichBuff = !whichBuff;
  }

}


void Wav::stop(){
	playing = 0;
	*TIMSK[tt] &= ~( _BV(ICIE1) | _BV(TOIE1) );
	TIMSK0 &= ~(1 << TOIE0);	
	if(sFile){sFile.close();}

}

void Wav::disable(){
  playing = 0;
  if(sFile){sFile.close();}
  *TIMSK[tt] &= ~( _BV(ICIE1) | _BV(TOIE1) );
  *TCCRnB[tt] &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12) );
  *TCCRnA[tt] = *TCCRnB[tt] = 0;
  TIMSK0 &= ~(1 << TOIE0);	  

}


boolean Wav::isPlaying(){
	return playing;
}

