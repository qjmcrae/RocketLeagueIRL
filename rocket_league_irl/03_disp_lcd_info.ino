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
    case 1:  // Title screen
      Title_Screen();
      break;
    case 2:  // Main Screen
      Main_Screen();
      break;
    // case 3:  // Position Information
    //   Position_Screen();
    //   break;
    // case 4:  // Radio Information
    //   Radio_Screen();
    //   break;
    // case 5:  // Environmental Information
    //   Environment_Screen();
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



// ************************   "******"_SCREEN   ************************//

// Specific Screen layouts

void Title_Screen() {  // Title screen
  //   01234567890123456789
  //0     GPS Guided Car
  //1      team_name[]
  //2       sub_name[]
  //3     course_name[]
  lcd.setCursor(0, 0);
  lcd.print(F("   GPS Guided Car "));
  lcd.setCursor(0, 1);
  lcd.print("Team: ");
  lcd.print(team);
  // lcd.setCursor(0, 2);
  // lcd.print(sub_name);
  // lcd.setCursor(0, 3);
  // lcd.print(course_name);
}

void Main_Screen() {  //
  // Main Screen
  //   01234567890123456789
  //0
  //1  LIDAR:####
  //2  Hdg ### GPS### Sat##
  //3  Dist ##m  ##:##:##ampm
  //   01234567890123456789
  //0  ###mph Alt ### D ###
  //1  LIDAR:####
  lcd.setCursor(0, 0);
  lcd.print(F("Dist:         "));
  lcd.setCursor(5,0);
  lcd.print(dist_lidar_ft,1);
  lcd.setCursor(0,1);
  lcd.print(F("Throttle:           "));
  lcd.setCursor(9,1);
  lcd.print(esc_command);
  lcd.setCursor(0,2);
  lcd.print("Velocity:            ");
  lcd.setCursor(9,2);
  lcd.print(velocity);
  // lcd.print();
  lcd.setCursor(10, 1);

}

// void Radio_Screen() {  // Radio Information
//   //   01234567890123456789
//   //0     Servo/ESC Info
//   //1  ESC Cmd: ###
//   //2  Steer Cmd: ###
//   //3  Heading Error:  ###
//   lcd.setCursor(0, 0);
//   lcd.print(F("   Servo/ESC Info   "));
//   lcd.setCursor(0, 1);
//   lcd.print(F("ESC Cmd:            "));
//   lcd.setCursor(10, 1);
//   lcd.print(esc_command);
//   lcd.setCursor(0, 2);
//   lcd.print(F("Steer Cmd:          "));
//   lcd.setCursor(10, 2);
//   lcd.print(servo_command);
//   lcd.setCursor(0, 3);
//   lcd.print(F("Heading Error:      "));
//   byte i = 14;
//   if (heading_error < 0) i = i - 1;
//   lcd.setCursor(i, 3);
//   lcd.print(heading_error);
// }

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
