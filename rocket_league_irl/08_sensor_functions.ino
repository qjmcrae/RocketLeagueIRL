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
  return dist_lidar_ft;
}  //End of get_lidar_data
