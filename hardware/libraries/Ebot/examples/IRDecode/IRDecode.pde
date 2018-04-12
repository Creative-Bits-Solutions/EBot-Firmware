#include <Snowball.h>

int RECV_PIN = 0;
IR ir(RECV_PIN);
decode_results results;


void setup() {
  Serial.begin(9600);
  ir.enable(); // Start the receiver
}

void loop() {
  
    if (ir.decode(&results)) {
    Serial.print(results.channel, DEC); 
    Serial.print(" "); 
    Serial.print(results.value, DEC);
    Serial.println();    
    ir.resume(); // Receive the next value
  }
  
}
