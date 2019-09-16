// LED definitions in the datasheet
// D13(PIN_LED_13): Blue
// TX (PIN_LED_TXL): Green
// RX (PIN_LED_RXL): Yellow

#define CPU_HZ 48000000

int blueLightState = 0;

void startTimer(int frequencyHz1);
void TC3_Handler();

void setup() {
  SerialUSB.begin(9600);
  //while(!SerialUSB);
  pinMode(PIN_LED_13, OUTPUT);
  pinMode(PIN_LED_RXL, OUTPUT);
  startTimer(1);
}

void loop() {}


void startTimer(int frequencyHz1){
  //16 bit top = 65526. 48Mhz clock. Dividing clock by 4 and 64. This equals 187500, or about 3 full cycles(1/3 of a second)

  //I switched to GCLK2. I'm not sure why, but I was having a ton of issues with GCLK0 not working correctly(wouldn't print to serial port, had no way of testing besides light)
  REG_GCLK_CLKCTRL =(uint16_t)(GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK2| GCLK_CLKCTRL_ID_TCC2_TC3);//set up TC3
  GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(2)  |
                      GCLK_GENCTRL_GENEN  |
                      GCLK_GENCTRL_DIVSEL |
                      GCLK_GENCTRL_SRC(7);  
  GCLK->GENDIV.reg = GCLK_GENDIV_ID(2) | GCLK_GENDIV_DIV(5);//2^(5+1) = 64
  
  while(GCLK->STATUS.bit.SYNCBUSY ==1);// wait for sync
  
  TcCount16* TC =(TcCount16*) TC3; //TC3
  
  TC->CTRLA.reg &= ~TC_CTRLA_ENABLE;
  TC->CTRLA.reg = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_WAVEGEN_NFRQ | TC_CTRLA_PRESCALER_DIV4; 
  while(TC->STATUS.bit.SYNCBUSY ==1);// wait for sync

  TC->INTENSET.reg =0; 
  TC->INTENSET.bit.MC0=1;

  NVIC_EnableIRQ(TC3_IRQn);

  TC->CTRLA.reg |= TC_CTRLA_ENABLE;
  while(TC->STATUS.bit.SYNCBUSY ==1);// wait for sync
}

void TC3_Handler(){
  blueLightState++;
  if(blueLightState == 3){
    blueLightState = 0;
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
  
  TcCount16* TC = (TcCount16*) TC3;
  //If this interrupt is due to the compare register matching the timer count we toggle the LED(now set to overflow)
  if(TC->INTFLAG.bit.MC0 == 1) {
    TC->INTFLAG.bit.MC0 = 1;
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
