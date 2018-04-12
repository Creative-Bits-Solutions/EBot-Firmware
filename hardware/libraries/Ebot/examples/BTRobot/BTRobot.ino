#include <Snowball.h>

Motors motors;

void setup() {
        pinMode(LED,OUTPUT);
        Serial.begin(38400);  
        Serial1.begin(38400);
        
}

void loop() {
        int power = 0,turnspeed = 50,movespeed=100;   
        int input;
                
        while(1) {
        
        delay(10);          
        if (Serial1.available() > 0) {
                input = Serial1.read(); 
                Serial1.flush();                
                if(input > 200 ) continue; // garbage              
                switch(input) {
                  case 'U': 
                      if(power) {
                        motors.setMLSpeed(movespeed,movespeed);
                        motors.setMRSpeed(movespeed,movespeed);                          
                      }
                      break;
                  
                  case 'D':
                      if(power) {
                        motors.setMLSpeed(-1*movespeed ,-1*movespeed );
                        motors.setMRSpeed(-1*movespeed ,-1*movespeed );   
                      }
                      break;
                  
                  case 'L':
                      if(power) {
                        motors.setMLSpeed(turnspeed,turnspeed);
                        motors.setMRSpeed(-1*turnspeed,-1*turnspeed);                          
                      }
                      break;
                                    
                  case 'R':                  
                      if(power) {
                        motors.setMLSpeed(-1*turnspeed,-1*turnspeed);
                        motors.setMRSpeed(turnspeed,turnspeed); 
                      }
                      break;
                      
                  case 'S':                  
                      if(power) {
                        motors.setMLSpeed(0,0);
                        motors.setMRSpeed(0,0);        
                      }
                      break;                      
                      
                  case 'P':
                  
                      power = !power;
                      
                      if(power) {
                          digitalWrite(LED,HIGH);
                      } else {
                          digitalWrite(LED,LOW);
                        motors.setMLSpeed(0,0);
                        motors.setMRSpeed(0,0);                      
                      }             
                      break;    
                  
                }
        }
        }
}