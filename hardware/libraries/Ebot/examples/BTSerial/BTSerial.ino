void setup() {
        Serial.begin(38400);  
        Serial1.begin(38400);
}

void loop() {

        if (Serial.available() > 0) {
                Serial1.write(Serial.read());
        }
        if (Serial1.available() > 0) {
                Serial.write(Serial1.read());
        }
}