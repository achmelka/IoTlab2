// LED definitions in the datasheet
// D13(PIN_LED_13): Blue
// TX (PIN_LED_TXL): Green
// RX (PIN_LED_RXL): Yellow

#define CPU_HZ 48000000
#define TIMER_PRESCALER_DIV 1024

void startTimer3(int frequencyHz);
void startTimer4(int frequencyHz);
void setTimerFrequency3(int frequencyHz);
void setTimerFrequency4(int frequencyHz);
void TC3_Handler();
void TC4_Handler();


void setup() {
  SerialUSB.begin(9600);
  //while(!SerialUSB);
  pinMode(PIN_LED_13, OUTPUT);
  pinMode(PIN_LED_RXL, OUTPUT);
  startTimer3(1);
  startTimer4(3);

}
void loop() {}

void setTimerFrequency3(int frequencyHz){
  int compareValue = (CPU_HZ / (TIMER_PRESCALER_DIV * frequencyHz)) - 1;
  TcCount16* TC = (TcCount16*) TC3;
  //Make sure the count is in a proportional position to where it was
  //to prevent any jitter or disconnect when changing the compare value
  TC->COUNT.reg = map(TC->COUNT.reg, 0, TC->CC[0].reg, 0, compareValue);
  TC->CC[0].reg = compareValue;
  SerialUSB.println(TC->COUNT.reg);
  SerialUSB.println(TC->CC[0].reg);
  while(TC->STATUS.bit.SYNCBUSY == 1);
}

void startTimer3(int frequencyHz){
  
  REG_GCLK_CLKCTRL =(uint16_t)(GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0| GCLK_CLKCTRL_ID_TCC2_TC3);//set up TC3

  while(GCLK->STATUS.bit.SYNCBUSY ==1);// wait for sync
  
  TcCount16* TC =(TcCount16*) TC3; //TC3
  
  TC->CTRLA.reg &= ~TC_CTRLA_ENABLE;
  TC->CTRLA.reg = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_WAVEGEN_MFRQ | TC_CTRLA_PRESCALER_DIV1024; 
  while(TC->STATUS.bit.SYNCBUSY ==1);// wait for sync

  setTimerFrequency3(frequencyHz);

  //Enable the overflow interrupt 
  TC->INTENSET.reg =0; 
  TC->INTENSET.bit.OVF=1;

  NVIC_EnableIRQ(TC3_IRQn);

  TC->CTRLA.reg |= TC_CTRLA_ENABLE;
  while(TC->STATUS.bit.SYNCBUSY ==1);// wait for sync
}

void TC3_Handler(){
  TcCount16* TC = (TcCount16*) TC3;
  //If this interrupt is due to the compare register matching the timer count we toggle the LED(now set to overflow)
  if(TC->INTFLAG.bit.OVF == 1) {
    TC->INTFLAG.bit.OVF = 1;
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
}
/*-----------------------------------------------------------------------------------------*/
void setTimerFrequency4(int frequencyHz){
  int compareValue = (CPU_HZ / (TIMER_PRESCALER_DIV * frequencyHz)) - 1;
  TcCount16* TC = (TcCount16*) TC4;
  //Make sure the count is in a proportional position to where it was
  //to prevent any jitter or disconnect when changing the compare value
  TC->COUNT.reg = map(TC->COUNT.reg, 0, TC->CC[0].reg, 0, compareValue);
  TC->CC[0].reg = compareValue;
  SerialUSB.println(TC->COUNT.reg);
  SerialUSB.println(TC->CC[0].reg);
  while(TC->STATUS.bit.SYNCBUSY == 1);
}

void startTimer4(int frequencyHz){
  
  REG_GCLK_CLKCTRL =(uint16_t)(GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0| GCLK_CLKCTRL_ID_TC4_TC5);//set up TC4
  while(GCLK->STATUS.bit.SYNCBUSY ==1);// wait for sync
  
  TcCount16* TC =(TcCount16*) TC4; //TC4
  
  TC->CTRLA.reg &= ~TC_CTRLA_ENABLE;
  TC->CTRLA.reg = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_WAVEGEN_MFRQ | TC_CTRLA_PRESCALER_DIV1024;
  while(TC->STATUS.bit.SYNCBUSY ==1);// wait for sync

  setTimerFrequency4(frequencyHz);

  //Enable the overflow interrupt 
  TC->INTENSET.reg =0; 
  TC->INTENSET.bit.OVF=1;

  NVIC_EnableIRQ(TC4_IRQn);

  TC->CTRLA.reg |= TC_CTRLA_ENABLE;
  while(TC->STATUS.bit.SYNCBUSY ==1);// wait for sync
}

void TC4_Handler(){
  TcCount16* TC = (TcCount16*) TC4;
  //If this interrupt is due to the compare register matching the timer count we toggle the LED(now set to overflow)
  if(TC->INTFLAG.bit.OVF == 1) {
    TC->INTFLAG.bit.OVF = 1;
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
