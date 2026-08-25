// ************************   ISR   ************************//

// This file contains the various ISR's that the arduino uses
// isr == Interrupt Service Routine



// ************************   ISR_ENCODER   ************************//

// isr of encoder to change the lcd screens
void isr_encoder() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if ((interruptTime - lastInterruptTime > 250) && (LCD_screen_old == LCD_screen)) {
    if (digitalRead(Dt) == LOW) {
      LCD_screen--;
    } else {
      LCD_screen++;
    }
    // wrap lcd screen around...
    // if (LCD_screen > num_LCD_screens) {
    //   LCD_screen = 0;
    // }
    // if (LCD_screen < 0) {
    //   LCD_screen = num_LCD_screens;
    // }
    LCD_screen = constrain(LCD_screen, 1, num_LCD_screens);  // Cap screens at limits
  }
  lastInterruptTime = interruptTime;
}




// ************************   ISR_ENCODER_NEW   ************************//

// different isr of encoder to change the lcd screens
void isr_encoder_new()  //
{
  if (LCD_screen_old == LCD_screen) {
    LCD_screen++;
    if (digitalRead(Dt) == LOW) {
      LCD_screen -= 2;
    }
  }
  // wrap lcd screen around...
  // if (LCD_screen > num_LCD_screens) {
  //   LCD_screen = 0;
  // }
  // if (LCD_screen < 0) {
  //   LCD_screen = num_LCD_screens;
  // }
  LCD_screen = constrain(LCD_screen, 0, num_LCD_screens);  // Cap screens at limits
}
