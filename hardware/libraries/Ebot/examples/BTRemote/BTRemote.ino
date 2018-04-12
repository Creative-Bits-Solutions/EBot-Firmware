#include <Snowball.h>
#include <Servo.h>
EasyTransfer ETin,ETout;

CONTROLLER_STATE cState = {0}; 
CONTROLLER_SET sState = {0}; 

void setup()              
{
    Serial.begin(38400);
    Serial1.begin(38400);    
    
    ETin.begin(details(cState), &Serial1);  
    ETout.begin(details(sState), &Serial1);  
    
    pinMode(10,INPUT);
    pinMode(11,INPUT);
               
}

void loop()                
{
   // recive from serial1 and transmit to serial 
   int i;
   
   sState.speaker = digitalRead(10);
   sState.motor   = digitalRead(11);
   
   ETout.sendData();
   
  for(i=0; i<5; i++){
     if(ETin.receiveData()){    
           Serial.println("----------Starting---------");
           Serial.print("XAxis ");
           Serial.println(cState.xaxis,DEC);
           
           Serial.print("YAxis ");
           Serial.println(cState.yaxis,DEC);
           
           Serial.print("Sel ");
           Serial.println(cState.sel,DEC);
           
           Serial.print("S1 ");
           Serial.println(cState.s1,DEC);
           
           Serial.print("S2 ");
           Serial.println(cState.s2,DEC);
           
           Serial.print("S3 ");
           Serial.println(cState.s3,DEC);

           Serial.print("S4 ");
           Serial.println(cState.s4,DEC);

           Serial.print("LEFT ");
           Serial.println(cState.left,DEC);

           Serial.print("RIGHT ");
           Serial.println(cState.right,DEC);

           Serial.print("UP ");
           Serial.println(cState.up,DEC);

           Serial.print("DOWN ");
           Serial.println(cState.down,DEC);
           
           Serial.print("DL ");
           Serial.println(cState.dl,DEC);

           Serial.print("DR ");
           Serial.println(cState.dr,DEC);
                                 
     }
   }  

   delay(100);
}
