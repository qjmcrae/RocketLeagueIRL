// // ************************   DISP_LCD_INFO   ************************//

// General funcionality of LCD
void disp_lcd_info() {

  disp_time = disp_time + disp_delay;

  if (LCD_screen_old != LCD_screen)  // if screen changes, clear old screen
  {
    lcd.clear();
    LCD_screen_old = LCD_screen;
  }

  switch (LCD_screen) {
    case 0:  // Title screen
      Title_Screen();
      break;
    case 1:  // Main Screen
      Main_Screen();
      break;
    case 2:  // Position Information
      Collision_Screen();
      break;
    // case 4:  // Radio Information
    //   Radio_Screen();
    //   break;
    // case 6:  // Battery Information
    //   Battery_Screen();
    //   break;
    default:
      lcd.setCursor(0, 1);
      lcd.print(F("**Undefined Screen**"));
      lcd.setCursor(5, 2);
      lcd.print(LCD_screen);
      break;
  }
}



// ****************************************************//

// Specific Screen layouts

void Title_Screen() {  // Title screen
  //   01234567890123456789
  //0     
  //1     
  //2     
  //3     
  lcd.setCursor(0, 0);
  lcd.print(F("  RocketLeagueIRL   "));
  lcd.setCursor(0, 1);
  lcd.print("Team: ");
  lcd.print(team);
}

void Main_Screen() {  //
  // Main Screen
  //   01234567890123456789
  //0
  //1  
  //2  
  //3  
  lcd.setCursor(0, 0);
  lcd.print(F("Dist:         "));
  lcd.setCursor(5,0);
  lcd.print(dist_lidar_ft,1);
  lcd.setCursor(0,1);
  lcd.print(F("Throttle:           "));
  lcd.setCursor(9,1);
  lcd.print(esc_command);
  lcd.setCursor(0,2);
  lcd.print("speed:          ");
  lcd.setCursor(9,2);
  lcd.print(closing_speed);
  lcd.setCursor(0,3);
  lcd.print("alpha:    ");
  lcd.setCursor(7,3);
  lcd.print(alpha);
  lcd.setCursor(10, 1);

}

void Collision_Screen() {  // Radio Information
  //   01234567890123456789
  //0  
  //1  
  //2  
  //3  

  lcd.setCursor(0, 0);
  lcd.print(F("Collision/Reduction "));
  lcd.setCursor(0,1);
  lcd.print(F("Min:      Max:      "));
  lcd.setCursor(4,1);
  lcd.print(min_pulse_throttle);
  lcd.setCursor(14,1);
  lcd.print(max_pulse_throttle);
  lcd.setCursor(0,2);
  lcd.print(F("TTI:     Thresh:    "));
  lcd.setCursor(4,2);
  lcd.print(time_to_impact);
  lcd.setCursor(16,2);
  lcd.print(TTI_threshold);
}

// void Battery_Screen() {  // Battery Information
//   //   01234567890123456789
//   //0     Battery Status
//   //1  Cell 1: ##.## V
//   //2  Cell 2: ##.## V
//   //3  Total : ##.## V
//   calc_batt_voltage();  // measure battery voltage
//   lcd.setCursor(0, 0);
//   lcd.print(F("   Battery Status   "));
//   lcd.setCursor(0, 1);
//   lcd.print(F("Cell 1: "));
//   lcd.print(volts_cell_1);
//   lcd.print(F(" V"));
//   lcd.setCursor(0, 2);
//   lcd.print(F("Cell 2: "));
//   lcd.print(volts_cell_2);
//   lcd.print(F(" V"));
//   lcd.setCursor(0, 3);
//   lcd.print(F("Total : "));
//   lcd.print(volts_total);
//   lcd.print(F(" V"));
// }
