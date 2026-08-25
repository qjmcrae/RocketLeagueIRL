
//=================  Pin definitions  =================//
// try to define each pins' use
//=====================================================//

// Digital pins, D-pins ...
// Pins D0, D1 used for hardware serial..
// TX = D0
// RX = D1
// Pins 9, 10 unused
byte Clk = 2;                  // Encoder Clk pin
byte Dt = 3;                   // Encoder DT pin
byte Sw = 4;                   // Encoder SW (button) pin
byte esc_out_pin = 5;          // Signal TO ESC
byte esc_in_pin = 6;           // Signal FROM ESC
byte steering_in_pin = 7;      // Signal to Steering Servo
byte steering_out_pin = 8;     // Signal to Steering Servo
byte team_pin = 11;            // Pin indicating which team
byte pixel_pin = 12;           // Neo-pixel LED strip
byte buzzer_pin = 13;          // Piezo electric buzzer pin

// Analog pins, A-pins...
// pins A4, A5 used for I2C protocol...
// SDA = A4;
// SCL = A5;
// Pins A3,A6,A7 unused
byte batt_cell_1_pin = A0;    // input pin for voltage divider of cell 1 of main battery
byte batt_volt_pin = A1;      // input pin for voltage divider of total voltage of main battery


// RC Input...
int throttle_command;   // -100 is full-speed backward, +100 is full-speed forward
int steering_command;   // +100 is full right turn, -100 is full left turn, 0 is straight

// The following are "reasonable" guesses - they are updated as you run...
int min_pulse_throttle     = 1100;
int max_pulse_throttle     = 1800;
int neutral_pulse_throttle = 1500;

// these min/max signal values can and will be adjusted to limit top speed.
int min_signal_throttle = 1300;
int max_signal_throttle = 1700;

// define ESC values for slowest and fastest we will limit the car
// note - the slowest is the slowest we will limit the top speed of the car, not the slowest we will allow the car to go!
// THESE NEED TO BE STUDIED!!!!
int esc_min_top_speed = 1600;
int esc_max_top_speed = max_signal_throttle;

// define distances (in feet) for car to slow down...
// THESE NEED TO BE STUDIED!!!!
int close_dist = 2;  // this would mean after something is this close or closer, it is as "slowed down" as we will go
int far_dist = 6;   // this is the distance where we start slowing down, any further away and we are at max allowable throttle

//=============== Initialize Variables ================//
// Setup program-level stuff, i.e. timers, etc.
//=====================================================//

// Battery Voltage
float volts_total;
float volts_cell_1;
float volts_cell_2;

bool beeped = 0;

volatile byte LCD_screen = 1;
byte LCD_screen_old = 0;
byte num_LCD_screens = 6;

int neo_delay = 100;
unsigned long neo_time = 0;

byte disp_freq = 2;  // in hz
int disp_delay = 1000 / disp_freq;
unsigned long disp_time = 0;

// Are these needed?
float calc_dist_freq = 0.5;  // in hz
long calc_dist_delay = 1000 / calc_dist_freq;
unsigned long calc_dist_time = 0;

// Servo variables ...
int servo_write_freq = 100;  // in hz
long servo_write_delay = 1000 / servo_write_freq;
unsigned long servo_write_time = 0;
byte servo_command = 90;
int esc_command = 1500;

// Values range between ~1000 and ~2000
int esc_default = 1500;        // 90;
int esc_stop = esc_default;

// disp variables
bool team;
float dist_lidar_ft;
int throttle_pulse;

//=============== Initialize Libraries ================//
// Include Libraries, Setup objects, modules, etc.
//=====================================================//

// Enables I2C Communication
#include <Wire.h>

// Setup Neo-pixel LED array
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel neo_pixel = Adafruit_NeoPixel(8, pixel_pin, NEO_GRB + NEO_KHZ800);

//Setup LCD Screen
#include <LiquidCrystal_I2C.h>
// next 2 lines for old LiquidCrystal_I2C library...
LiquidCrystal_I2C lcd_0x27(0x27, 20, 4);  // set the LCD address for a 20 chars and 4 line display
LiquidCrystal_I2C lcd_0x3F(0x3F, 20, 4);  // set the LCD address for a 20 chars and 4 line display
LiquidCrystal_I2C lcd = lcd_0x27;

// Initialize servo stuff ...
#include <Servo.h>
Servo esc_servo;  // create servo objects to control a servo

// Setup Dist sensor, Currently LiDAR
#include <TFLI2C.h>            // TFLuna-I2C Library v.0.1.1
TFLI2C luna;                   // create object for distance sensor, willing to rename
#define lidar_adr TFL_DEF_ADR  // set address for distance sensor
