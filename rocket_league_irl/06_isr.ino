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
  if (interruptTime - lastInterruptTime > 100) {
    // alpha_case += 1;
    if (digitalRead(Dt) == LOW) {
      TTI_threshold += 0.1;
    } else {
      TTI_threshold -= 0.1;
    }
    TTI_threshold = constrain(TTI_threshold, 0.1, 3.0);  // Cap screens at limits
  }
  // if (interruptTime - lastInterruptTime > 100) {
  //   if (digitalRead(Dt) == LOW) {
  //     LCD_screen += 1;
  //   } else {
  //     LCD_screen -= 1;
  //   }
  //   // wrap lcd screen around...
  //   if (LCD_screen > num_LCD_screens) {
  //     LCD_screen = 1;
  //   }
  //   if (LCD_screen < 1) {
  //     LCD_screen = num_LCD_screens;
  //   }
  // }
  lastInterruptTime = interruptTime;
}
