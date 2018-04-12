#include <Snowball.h>

int RECV_PIN = 6;
IR ir(RECV_PIN);
decode_results results;
int button;


void setup() {
  Serial.begin(9600);
  ir.enable(); // Start the receiver
}

void loop() {

   //Will only recive selected channel on DIP switches 
    button = ir.button();
    if(button > 0) {
      Serial.println(button, DEC);   
      ir.resume();
    }
  
}