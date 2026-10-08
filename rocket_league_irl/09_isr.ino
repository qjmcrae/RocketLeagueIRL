// ************************   ISR   ************************//
// 
// This file contains the ISR that the arduino uses
// isr == Interrupt Service Routine
// 

// ************************   ISR_ENCODER   ************************//

// isr of encoder to change the lcd screens
void isr_encoder() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  // if (interruptTime - lastInterruptTime > 100) {
  // // alpha_case += 1;
  // if (digitalRead(Dt) == LOW) {
  //   alpha_case += 1;
  // } else {
  //   alpha_case -= 1;
  // }
  // alpha_case = constrain(alpha_case, 1, 3);  // Cap screens at limits

  if (interruptTime - lastInterruptTime > 100) {
    if (digitalRead(Dt) == LOW) {
      LCD_screen += 1;
    } else {
      LCD_screen -= 1;
    }
    // wrap lcd screen around...
    if (LCD_screen > num_LCD_screens) {
      LCD_screen = 0;
    }
    if (LCD_screen < 0) {
      LCD_screen = num_LCD_screens;
    }
  }
  lastInterruptTime = interruptTime;
}
