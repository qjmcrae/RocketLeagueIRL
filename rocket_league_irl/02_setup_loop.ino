///////////////////////////////////////////////////////////////////////////
//
//                                void setup
//
///////////////////////////////////////////////////////////////////////////
void setup() {
  Wire.begin();
  Serial.begin(115200);
  //Serial.println("Ice Hockey RC Car!");


  pinMode(Clk, INPUT);                // D2
  pinMode(Dt, INPUT);                 // D3
  pinMode(Sw, INPUT_PULLUP);          // D4
  pinMode(esc_out_pin, OUTPUT);       // D5
  pinMode(esc_in_pin, INPUT);         // D6
  pinMode(steering_in_pin, INPUT);    // D7
  pinMode(steering_out_pin, OUTPUT);  // D8
  pinMode(team_pin, INPUT_PULLUP);    // D11
  pinMode(pixel_pin, OUTPUT);         // D12
  pinMode(buzzer_pin, OUTPUT);        // D13
  // pinMode(batt_cell_1_pin, INPUT);  // A0
  // pinMode(batt_volt_pin, INPUT);    // A1

  attachInterrupt(digitalPinToInterrupt(Clk), isr_encoder, FALLING);  //D2


  // initialize servos
  esc_servo.attach(esc_out_pin);
  esc_servo.attach(esc_out_pin);
  // delay(250);

  // I2C addresses can be in decimal or hexadecimal
  Wire.begin();
  delay(250);

  // Test to see which LCD screen is attached - only ones I am aware of are 0x27 and 0x3F
  byte address = 63;  // This is 0x3F
  Wire.beginTransmission(address);
  byte error = Wire.endTransmission();
  if (error == 0) {  // this means there is a 0x3F - if it is there, assume it is the only one there
    lcd = lcd_0x3F;
  }

  // Initialize LCD
  lcd.init();
  lcd.begin(20, 4);
  lcd.setBacklight(HIGH);
  lcd.clear();
  //         01234567890123456789
  lcd.print("    Let's Play      ");
  lcd.print("   Some Hockey!     ");
  // disp_lcd_info();
  delay(100);


  //neo-pixel initialization
  neo_pixel.begin();
  neo_pixel.show();  // Initialize all pixels to 'off'

  // neo_design(1);  // (do some interesting stuff)
  // delay(1000);    // make sure to write above for at least some time ...

  team = digitalRead(team_pin);
  if (team) neo_red = 100;
  else neo_blue = 100;
  for (int j = 0; j < 64; j++) {
    neo_pixel.setPixelColor(j, neo_pixel.Color(neo_red, neo_green, neo_blue));
  }
  neo_pixel.show();

  delay(1500);

  disp_time = millis() + 1500;  // Don't change this display for extra 1.5 seconds
  LCD_screen = 2;               // go to the next LCD Screen

  // neo_design(0);  // turn neo_pixel off

  // Serial.println(F("end of setup"));
}  // end of setup
// ************************   END SETUP   ************************//






///////////////////////////////////////////////////////////////////////////
//
//                                void loop
//
///////////////////////////////////////////////////////////////////////////
// ************************   BEGIN LOOP   ************************//
void loop()  //
{

  if (millis() > calc_dist_time) {
    // get distance from LIDAR sensor, calculate velocity
    dist_lidar_ft = get_lidar_data();
    velocity = calc_speed(dist_lidar_ft);  // speed output
  }

  // set max throttle signal based on current lidar distance - idea is that is top speed, used below to proportionally pick speed
  static bool collision_reduction = 0;  // Flag for Collision Reduction                   // flag for abs-like system
  static bool braking_complete = 0;     // Flag indicating auto-braking is complete?

  float max_velocity_collision = -2.0;  // closing speed at which point we get nervous / respond

  if (dist_lidar_ft <= 10 && velocity <= max_velocity_collision)  // close and closing fast...
  {
    // map max speed based on lidar distance - basically slow down max speed if you are too close (and fast)
    max_signal_throttle = constrain(map(dist_lidar_ft, close_dist, far_dist, esc_min_top_speed, esc_max_top_speed), esc_min_top_speed, esc_max_top_speed);
    if (braking_complete == 0) collision_reduction = 1;
  }     //
  else  // we're either not close, or not closing fast, so allow top speed
  {
    max_signal_throttle = esc_max_top_speed;
    collision_reduction = 0;  // disable any collision reduction logic
  }

  //  update servo command every so often ...
  if (millis() > servo_write_time)  //
  {
    throttle_pulse = pulseIn(esc_in_pin, HIGH, 35000);
    if (throttle_pulse != 0)  // make sure to have a measurement
    {
      max_pulse_throttle = constrain(max(max_pulse_throttle, throttle_pulse), 1700, 2200);
      min_pulse_throttle = constrain(min(min_pulse_throttle, throttle_pulse), 900, 1200);
    }

    throttle_command = 0;
    if (throttle_pulse != 0)  // only do this if we have a value...
    {
      if (throttle_pulse > neutral_pulse_throttle)
        throttle_command = map(throttle_pulse, neutral_pulse_throttle, max_pulse_throttle, 0, 100);
      else
        throttle_command = map(throttle_pulse, min_pulse_throttle, neutral_pulse_throttle, -100, 0);
    }

    //  Map motor signals to ±100, based on min/max pwm signals (so you can adjust the max speed above)
    //  this also sets -100 to full-speed backwards, and +100 to full-speed forward
    if (throttle_command > 0)
      esc_command = map(throttle_command, 0, 100, neutral_pulse_throttle, max_signal_throttle);
    else
      esc_command = map(throttle_command, -100, 0, min_signal_throttle, neutral_pulse_throttle);

    //  The mapping above still allows the numbers to over-write the limits - this fixes the high
    //  and low signals to not go over the limits ...

    esc_command = constrain(esc_command, min_signal_throttle, max_signal_throttle);

    // Braking code - we're spoofing the "double-pump" braking system
    // if someone wants reverse, first send a quick "blip" of braking, then neutral, then back
    //   to braking so as to go in reverse
    static bool brake_disabled = 0;
    if (throttle_command < -10 && brake_disabled == 0) {
      esc_servo.write(min_signal_throttle);
      delay(75);
      esc_servo.write(neutral_pulse_throttle);
      delay(75);
      brake_disabled = 1;  // don't do this again until forward command is given
    } else if (throttle_command > 0) {
      brake_disabled = 0;
    }

    // close to wall case

    // Flag Definitions:
    // braking_complete - flag to determine if we should auto-brake as we get close to something...
    // collision_reduction - flag to enter collision reduction logic

    if (collision_reduction == 1 && velocity > max_velocity_collision) {
      braking_complete = 1;
    }  //
    // else if (collision_reduction == 1 && esc_command >= neutral_pulse_throttle + 25 && braking_complete == 0) {  // abs-like functionality
    else if (collision_reduction == 1 && throttle_command >= 0 && braking_complete == 0) {  // auto-braking functionality ??
                                                                                            //           collision flag on       have forward throttle     still need to brake
      esc_servo.write(min_pulse_throttle);                                                  // send a pulse of braking for 0.1 sec...
      delay(100);
    }                                                        //
    else if (dist_lidar_ft > 10 && braking_complete == 1) {  // you are far away, and no need to auto-brake
      collision_reduction = 0;
      braking_complete = 0;
    }

    esc_servo.write(esc_command);
    servo_write_time += servo_write_delay;
  }

  if (millis() > disp_time || LCD_screen_old != LCD_screen)
    disp_lcd_info();  // display info to LCD screen

}  // end of loop
// ************************   END LOOP   ************************/
