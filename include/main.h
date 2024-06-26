#include <Arduino.h>

#define BL 5
#define CHARGE 34
#define BATT 27
#define MOTOR 25
#define B0 0
#define BT1 33
#define BT2 32
#define MPU_INT 13

String fw = "v1.0";
uint8_t imuID;
String macAddr = "AA:BB:CC:DD:EE:FF";


const unsigned long DEBOUNCE = 50ul; // debounce time

bool button1State, button2State;
long startPress1, startPress2;

float sampleRate = 6.25; // HZ - Samples per second - 0.781, 1.563, 3.125, 6.25, 12.5, 25, 50, 100, 200, 400, 800, 1600Hz
uint8_t accelRange = 4;  // Accelerometer range = 2, 4, 8, 16g

float accX, accY, accZ;

// bool screenOn = true;
// long onTime = 0;


// backlight pin
const int ledPin = BL; //

// setting PWM properties
const int freq = 1000;
const int ledChannel = 15;
const int resolution = 8;

int brightness = 100;
bool motor = false;
bool autoScreen = true;//raise to wake
bool vibration = true;
float vector;

bool powerSave = true;

bool isNotify = false;
long notifyTime = 100L;

#define SAMPLES 1000

float samples[SAMPLES];
int x = 0;
int curSteps = 0;
int stepsProgress = 0;

void init_touch();
void get_read_touch();
uint8_t user_i2c_write(uint8_t addr, uint8_t reg_addr, const uint8_t *reg_data, uint32_t length);
uint8_t user_i2c_read(uint16_t addr, uint8_t reg_addr, uint8_t *reg_data, uint32_t length);

void get_IMU();
void deep_sleep();
void light_sleep();
void power_save();
int calculate_steps();
