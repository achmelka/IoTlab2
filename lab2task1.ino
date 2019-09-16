// LED definitions in the datasheet
// D13(PIN_LED_13): Blue
// TX (PIN_LED_TXL): Green
// RX (PIN_LED_RXL): Yellow

// Variables will change:

long previousMillisBlue = 0; //will store last time the BLUE LED was updated
long previousMillisYellow = 0; //will store last time the YELLOW LED was updated
long interval1000 = 1000; //1 second "delay"
long inverval333 = 333; //1/3 second "delay"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!SerialUSB);
  SerialUSB.println("Start Task 1");
  
  pinMode(PIN_LED_13, OUTPUT);
  pinMode(PIN_LED_RXL, OUTPUT);
}

void loop() {
  //put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  //this will blink the blue light
  if(currentMillis - previousMillisBlue > interval1000){
    //update the currentMillis passed since the function began
    previousMillisBlue = currentMillis;
    //digitalRead is an arduino function that will return the state of a pin
    int blueLED = digitalRead(PIN_LED_13);
    if(blueLED == 1){
      SerialUSB.println("Blue LED is ON");
      digitalWrite(PIN_LED_13, LOW);
    }
    else{
      SerialUSB.println("Blue LED is OFF");
      digitalWrite(PIN_LED_13, HIGH);
    }
  }

  if(currentMillis - previousMillisYellow > inverval333){ 
    //update the currentMillis passed since the function began
    previousMillisYellow = currentMillis;
    int yellowLED = digitalRead(PIN_LED_RXL);
    if(yellowLED == 1){
      SerialUSB.println("Yellow LED is ON");
      digitalWrite(PIN_LED_RXL, LOW);
    }
    else{
      SerialUSB.println("Yellow LED is OFF");
      digitalWrite(PIN_LED_RXL, HIGH);
    }
  }
}
