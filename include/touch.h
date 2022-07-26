
#include <Wire.h>

#define TSCL 4
#define TSDA 2
#define TINT 14
#define TRST 15

volatile bool i2cReading = false;

bool touch_enable = false;
bool was_touched = false;
bool touch_interrupt = false;
volatile bool change = false;

uint8_t touch_dev_addr = 0x15;

struct touch_data_struct
{
  byte gesture;
  byte touchpoints;
  byte event; // Event (0 = Down, 1 = Up, 2 = Contact)
  int xpos;
  int ypos;
  int last_xpos;
  int last_ypos;
  uint8_t version15;
  uint8_t versionInfo[3];
};

touch_data_struct touch_data;
bool state = false;

// int touchX = 120, touchY = 120;

void init_touch()
{
  if (!touch_enable)
  {
    touch_enable = true;
    pinMode(TRST, OUTPUT);
    pinMode(TINT, INPUT);

    digitalWrite(TRST, HIGH);
    delay(50);
    digitalWrite(TRST, LOW);
    delay(5);
    digitalWrite(TRST, HIGH);
    delay(50);

    user_i2c_read(touch_dev_addr, 0x15, &touch_data.version15, 1);
    delay(5);
    user_i2c_read(touch_dev_addr, 0xA7, touch_data.versionInfo, 3);
  }
}

void sleep_touch(bool state)
{
  digitalWrite(TRST, LOW);
  delay(5);
  digitalWrite(TRST, HIGH);
  delay(50);
  if (state)
  {
    touch_enable = false;
    byte standby_value = 0x03;
    user_i2c_write(touch_dev_addr, 0xA5, &standby_value, 1);
  }
}

void set_new_touch_interrupt()
{
  touch_interrupt = true;
}

bool get_new_touch_interrupt()
{
  bool temp = touch_interrupt;
  touch_interrupt = false;
  return temp;
}

bool get_was_touched()
{
  return was_touched;
}

void set_was_touched(bool state)
{
  was_touched = state;
}

void get_read_touch()
{
  if (!touch_enable)
    init_touch();

  byte data_raw[8];
  user_i2c_read(touch_dev_addr, 0x01, data_raw, 6);

  touch_data.gesture = data_raw[0];
  touch_data.touchpoints = data_raw[1];
  touch_data.event = data_raw[2] >> 6;
  touch_data.xpos = data_raw[3];
  touch_data.ypos = data_raw[5];
  if (touch_data.xpos == 255 && touch_data.ypos == 255)
  {
    touch_data.xpos = touch_data.last_xpos;
    touch_data.ypos = touch_data.last_ypos;
  }
  else
  {
    touch_data.last_xpos = touch_data.xpos;
    touch_data.last_ypos = touch_data.ypos;
  }
}

touch_data_struct get_touch()
{
  return touch_data;
}

void init_i2c()
{
  Wire.begin(TSDA, TSCL);
}

void set_i2cReading(bool state)
{
  i2cReading = state;
}

bool get_i2cReading()
{
  return i2cReading;
}

uint8_t user_i2c_read(uint16_t addr, uint8_t reg_addr, uint8_t *reg_data, uint32_t length)
{
  set_i2cReading(true);
  Wire.beginTransmission(addr);
  Wire.write(reg_addr);
  if (Wire.endTransmission(true))
    return -1;
  Wire.requestFrom(addr, length, true);
  for (int i = 0; i < length; i++)
  {
    *reg_data++ = Wire.read();
  }
  set_i2cReading(false);
  return 0;
}

uint8_t user_i2c_write(uint8_t addr, uint8_t reg_addr, const uint8_t *reg_data, uint32_t length)
{
  //byte error;
  set_i2cReading(true);
  Wire.beginTransmission(addr);
  Wire.write(reg_addr);
  for (int i = 0; i < length; i++)
  {
    Wire.write(*reg_data++);
  }
  if (Wire.endTransmission(true))
    return -1;
  set_i2cReading(false);
  return 0;
}
