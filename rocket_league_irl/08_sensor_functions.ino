// // ************************   SENSOR FUNCTIONS   ************************//

// // This file contains the various functions that the arduino uses to
// // monitor the state of the car and its surroundings using the various
// // sensors on the car, not including the gps and compass



// // ************************   CALC_BATT_VOLTAGE   ************************//

// // This function should calculate the total battery voltage using a voltage divider on pin A1,
// // and cell 1 voltage using a voltage divider on pin A0.
// // It assume the voltage dividers has values of R1, R2, R3, and R4
// void calc_batt_voltage() {
//   const byte R1 = 100;
//   const byte R2 = 39;
//   const byte R3 = 100;
//   const byte R4 = 100;
//   float tot_batt_scale = 1.0401;   // Emperically measured value
//   float cell_batt_scale = 1.0358;  // Emperically measured value
//   float tmp1 = analogRead(batt_volt_pin) * 3.3 / 1024.0;
//   volts_total = tmp1 * (R1 + R2) / R2 * tot_batt_scale;
//   float tmp2 = analogRead(batt_cell_1_pin) * 3.3 / 1024.0;
//   volts_cell_1 = tmp2 * (R3 + R4) / R4 * cell_batt_scale;
//   volts_cell_2 = volts_total - volts_cell_1;

// }  //End of calc_batt_voltage



// ************************   GET_LIDAR_DATA   ************************//

// Using front Lidar sensor, guestimate the distance to any object in front
float get_lidar_data() {
  int16_t dist_cm;  // Leave as int16_t
  float dist_lidar_ft;
  if (luna.getData(dist_cm, TFL_DEF_ADR))          // Gets distance data from lidar sensor in cm
    dist_lidar_ft = float(dist_cm) / 2.54 / 12.0;  // Returns the distance in feet

  calc_dist_time += calc_dist_delay;
  return dist_lidar_ft;
}  //End of get_lidar_data

// ************************      ************************//

float calc_speed(float dist_current) {
  static float velocity_prev;
  static float dist_prev;

  float vel_lim = 10.0;
  float dy = dist_current - dist_prev;
  // speed = delta_distance / delta_time
  // delta_y = new_dist - old_dist
  // delta_x = change in time - being called at x Hz, so divided by 1/x is multiplying by x, or by frequency (in Hz)
  float raw_velocity = constrain(dy * calc_dist_freq, -vel_lim, vel_lim);  // This should be in feet/sec


  alpha = 0.5;  // smoothing factor
  // alpha = 0.95;  // smoothing factor
  // if (alpha_case == 1) alpha = 0.05;
  // else if (alpha_case == 2) alpha = 0.5;

  // filter velocity with old velocity...
  float filtered_velocity = alpha * (velocity_prev) + (1 - alpha) * (raw_velocity);  // helps eliminate sensor noise or abrupt stops for passing objects i.e. vehicles

  velocity_prev = filtered_velocity;
  dist_prev = dist_current;

  Serial.print("Filtered:");
  Serial.print(filtered_velocity);
  Serial.print(", Raw:");
  Serial.print(raw_velocity);
  Serial.print(", max:");
  Serial.print(vel_lim);
  Serial.print(", min:");
  Serial.print(-vel_lim);

  Serial.println();

  return filtered_velocity;
}
