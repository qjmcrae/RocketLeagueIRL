///////////////////////////////////////////////////////////////////////////
//
//                                void setup
//
///////////////////////////////////////////////////////////////////////////
void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Ice Hockey RC Car!");

  pinMode(Clk, INPUT);                // D2
  pinMode(Dt, INPUT);                 // D3
  pinMode(Sw, INPUT_PULLUP);          // D4
  pinMode(esc_out_pin, OUTPUT);       // D5
  pinMode(esc_in_pin, INPUT);         // D6
  pinMode(steering_in_pin, INPUT);    // D7
  pinMode(steering_out_pin, OUTPUT);  // D8
  pinMode(team_pin, INPUT);           // D11
  pinMode(pixel_pin, OUTPUT);         // D12
  pinMode(buzzer_pin, OUTPUT);        // D13
  // pinMode(batt_cell_1_pin, INPUT);  // A0
  // pinMode(batt_volt_pin, INPUT);    // A1

  attachInterrupt(digitalPinToInterrupt(Clk), isr_encoder, LOW);  //D2


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


  // //neo-pixel initialization
  // neo_pixel.begin();
  // neo_pixel.show();  // Initialize all pixels to 'off'

  // neo_design(1);  // (do some interesting stuff)
  // delay(1000);    // make sure to write above for at least some time ...

  // for (int j = 0; j < 64; j++) {
  //   neo_pixel.setPixelColor(j, neo_pixel.Color(neo_red, neo_green, neo_blue));
  // }
  // neo_pixel.show();

  delay(1500);

  disp_time = millis() + 1500;  // Don't change this display for extra 1.5 seconds
  LCD_screen = 2;               // go to the next LCD Screen

  // neo_design(0);  // turn neo_pixel off

  // Serial.println(F("end of setup"));
}  // end of setup
// ************************   END SETUP   ************************//


float speed(float distance_o, float distance_f, float last_velocity) {

  float dy = distance_f - distance_o;
  float raw_velocity = dy * 100;
  float alpha = .5;                                                                  // smoothing factor
  float filtered_velocity = last_velocity + alpha * (raw_velocity - last_velocity);  // helps eliminate sensor noise or abrupt stops for passing objects i.e. vehicles
  
  return filtered_velocity;
}

static float vel_last = 0;
static float si = 0;
static float sf = 0;
float velocity;

///////////////////////////////////////////////////////////////////////////
//
//                                void loop
//
///////////////////////////////////////////////////////////////////////////
// ************************   BEGIN LOOP   ************************//
void loop()  //
{
  team = digitalRead(team_pin);

  // get distance from LIDAR sensor
  dist_lidar_ft = get_lidar_data();
  Serial.print("dist = ");
  Serial.print(dist_lidar_ft);

  sf = dist_lidar_ft;
  velocity = speed(si, sf, vel_last);  // speed output
  Serial.print("Vel_last = ");
  Serial.print(vel_last);
  Serial.print("   Velocity = ");
  Serial.print(velocity);
  Serial.print("   si = ");
  Serial.print(si);
  Serial.print("    sf = ");
  Serial.print(sf);
  Serial.println();
  
  vel_last = velocity;

  si = sf;
  // set max throttle signal based on current lidar distance - idea is that is top speed, used below to proportionally pick speed
  static bool inertia = 0;                       // flag for abs-like system
  if (dist_lidar_ft <= 8 && velocity <= -7.33) {  // limit speed based on velocity and distance
    max_signal_throttle = constrain(map(dist_lidar_ft, close_dist, far_dist, esc_min_top_speed, esc_max_top_speed), esc_min_top_speed, esc_max_top_speed);
    inertia = 1;
  }     
  else  // overall top speed available
  {
    max_signal_throttle = esc_max_top_speed;
    inertia = 0;
  }

  //  update servo command every so often ...
  if (millis() > servo_write_time)  //
  {
    throttle_pulse = pulseIn(esc_in_pin, HIGH, 35000);
    if (throttle_pulse != 0)  // make sure to have a measurement
    {
      // this is NOT functional, but the idea is to slowly turn back min/max values in case there is anomolous reading
      // max_pulse_throttle -= 1;
      // min_pulse_throttle += 1;
      max_pulse_throttle = constrain(max(max_pulse_throttle, throttle_pulse), 1700, 2200);
      min_pulse_throttle = constrain(min(min_pulse_throttle, throttle_pulse), 900, 1200);
    }
    // adjust min/max if outside original range
    // if (throttle_pulse > max_pulse_throttle) max_pulse_throttle = throttle_pulse;
    // if (throttle_pulse < min_pulse_throttle && throttle_pulse > 300) min_pulse_throttle = throttle_pulse;

    Serial.print("    throttle_pulse = ");
    Serial.print(throttle_pulse);

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

    // Braking code
    esc_command = constrain(esc_command, min_signal_throttle, max_signal_throttle);
    static bool brake_disabled = 0;
    if (throttle_command < -10 && brake_disabled == 0) {
      esc_servo.write(min_signal_throttle);
      delay(50);
      esc_servo.write(neutral_pulse_throttle);
      delay(50);
      brake_disabled = 1;
    } else if (throttle_command > 0) {
      brake_disabled = 0;
    }

    if (inertia == 1) {  // abs-like functionality
      esc_servo.write(neutral_pulse_throttle);
      delay(50);
    }


    esc_servo.write(esc_command);
    servo_write_time = servo_write_time + servo_write_delay;

   // Serial.print(", ESC = ");
   // Serial.print(esc_command);

    // Serial.print(", max_throttle = ");
    // Serial.print(max_signal_throttle);

    // Serial.print(", throttle_command = ");
    // Serial.print(throttle_command);

    // Serial.print(", brake_disabled = ");
    // Serial.print(brake_disabled);

   // Serial.println();
  }

  unsigned long now = millis();
  //  if (now > neo_time) neo_design(1);
  // neo_design(2000);
  if (now > disp_time || LCD_screen_old != LCD_screen)
    disp_lcd_info();  // display info to LCD screen
  
}  // end of loop
// ************************   END LOOP   ************************/
