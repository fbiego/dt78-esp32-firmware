#include "main.h"

#include <lvgl.h>

#include <Arduino_GFX_Library.h>
#include <ESP32Time.h>
// #include <CST816S.h>
#include "touch.h"
#include "bluetooth.h"
#include "kxtj3-1057.h"
#include "driver/rtc_io.h"

Arduino_DataBus *bus = new Arduino_ESP32SPI(21 /* DC */, 22 /* CS */, 18 /* SCK */, 23 /* MOSI */, -1 /* MISO */, VSPI /* spi_num */);
Arduino_GC9A01 *gfx = new Arduino_GC9A01(bus, 19, 0 /* rotation */, true /* IPS */);

ESP32Time rtc;
// CST816S touch(2, 4, 15, 14); // sda, scl, rst, irq
KXTJ3 myIMU(0x0F); // Address can be 0x0E or 0x0F

/* Change to your screen resolution */
static uint32_t screenWidth;
static uint32_t screenHeight;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;
static lv_disp_drv_t disp_drv;

lv_obj_t *ui_textScreen;
lv_obj_t *ui_textScroll;
lv_obj_t *ui_textIcon;
lv_obj_t *ui_textTitle;
lv_obj_t *ui_textMessage;
lv_obj_t *ui_textTime;

lv_obj_t *ui_watchScreen;
lv_obj_t *ui_backgroundImage;
lv_obj_t *ui_stepsProgress;
lv_obj_t *ui_weekdayText;
lv_obj_t *ui_dateText;
lv_obj_t *ui_hourIcon1;
lv_obj_t *ui_hourIcon2;
lv_obj_t *ui_minuteIcon1;
lv_obj_t *ui_minuteIcon2;
lv_obj_t *ui_timeSeparator;
lv_obj_t *ui_textAmPm;
lv_obj_t *ui_stepsIcon;
lv_obj_t *ui_stepsText;
lv_obj_t *ui_bluetoothIcon;
lv_obj_t *ui_chargingIcon;

lv_obj_t *ui_settingsScreen;
lv_obj_t *ui_scrollSettings;
lv_obj_t *ui_bluetoothText;
lv_obj_t *ui_bluetoothSwitch;
lv_obj_t *ui_brightnessText;
lv_obj_t *ui_autoScreenText;
lv_obj_t *ui_autoScreenSwitch;
lv_obj_t *ui_brightnessSlider;
lv_obj_t *ui_themeText;
lv_obj_t *ui_colorWheel;
lv_obj_t *ui_powerText;
lv_obj_t *ui_powerSlider;

lv_obj_t *ui_testScreen;
lv_obj_t *ui_buttonUp;
lv_obj_t *ui_buttonDown;
lv_obj_t *ui_scrollTest;
lv_obj_t *ui_vibrateText;
lv_obj_t *ui_buttonvibrate;
lv_obj_t *ui_accelerometerText;
lv_obj_t *ui_accXtext;
lv_obj_t *ui_accXBar;
lv_obj_t *ui_accYText;
lv_obj_t *ui_accYBar;
lv_obj_t *ui_accZText;
lv_obj_t *ui_accZBar;

lv_obj_t *ui_aboutScreen;
lv_obj_t *ui_aboutList;

lv_obj_t *ui_menuScreen;
lv_obj_t *ui_menuList;
lv_obj_t *ui_buttonMessages;
lv_obj_t *ui_buttonHealth;
lv_obj_t *ui_buttonSettings;
lv_obj_t *ui_buttonStopwatch;
lv_obj_t *ui_buttonTimer;
lv_obj_t *ui_buttonAbout;
lv_obj_t *ui_buttonFitness;
lv_obj_t *ui_buttonHlt;
lv_obj_t *ui_buttonWeather;
lv_obj_t *ui_buttonFind;
lv_obj_t *ui_buttonMusic;

lv_obj_t *ui_messageListScreen;
lv_obj_t *ui_messageList;

LV_IMG_DECLARE(ui_img_zero_64_png);  // assets\zero_64.png
LV_IMG_DECLARE(ui_img_one_64_png);   // assets\one_64.png
LV_IMG_DECLARE(ui_img_two_64_png);   // assets\two_64.png
LV_IMG_DECLARE(ui_img_three_64_png); // assets\three_64.png
LV_IMG_DECLARE(ui_img_four_64_png);  // assets\four_64.png
LV_IMG_DECLARE(ui_img_five_64_png);  // assets\five_64.png
LV_IMG_DECLARE(ui_img_six_64_png);   // assets\six_64.png
LV_IMG_DECLARE(ui_img_seven_64_png); // assets\seven_64.png
LV_IMG_DECLARE(ui_img_eight_64_png); // assets\eight_64.png
LV_IMG_DECLARE(ui_img_nine_64_png);  // assets\nine_64.png

lv_img_dsc_t digits[10] = {ui_img_zero_64_png, ui_img_one_64_png, ui_img_two_64_png, ui_img_three_64_png, ui_img_four_64_png,
                           ui_img_five_64_png, ui_img_six_64_png, ui_img_seven_64_png, ui_img_eight_64_png, ui_img_nine_64_png};

bool bgImg = true;
LV_IMG_DECLARE(ui_img_background1_png); // assets\background1.png
LV_IMG_DECLARE(ui_img_background2_png); // assets\background2png
LV_IMG_DECLARE(ui_img_steps_png);       // assets\steps.png
LV_IMG_DECLARE(ui_img_vibrate_png);     // assets\vibrate.png
LV_IMG_DECLARE(ui_img_comment_png);     // assets\comment.png

LV_IMG_DECLARE(ui_img_bluetooth_png); // assets\bluetooth.png
LV_IMG_DECLARE(ui_img_energy_png);    // assets\energy.png

LV_IMG_DECLARE(ui_img_steps_png);     // assets\steps.png
LV_IMG_DECLARE(ui_img_about_png);     // assets\about.png
LV_IMG_DECLARE(ui_img_fitness_png);   // assets\fitness.png
LV_IMG_DECLARE(ui_img_message_png);   // assets\message.png
LV_IMG_DECLARE(ui_img_heart_png);     // assets\heart.png
LV_IMG_DECLARE(ui_img_weather_png);   // assets\weather.png
LV_IMG_DECLARE(ui_img_music_png);     // assets\music.png
LV_IMG_DECLARE(ui_img_timer_png);     // assets\timer.png
LV_IMG_DECLARE(ui_img_stopwatch_png); // assets\stopwatch.png
LV_IMG_DECLARE(ui_img_setting_png);   // assets\setting.png
LV_IMG_DECLARE(ui_img_search_png);    // assets\search.png

LV_IMG_DECLARE(ui_img_touch_png);    // assets\touch.png
LV_IMG_DECLARE(ui_img_accl_png);    // assets\accl.png
LV_IMG_DECLARE(ui_img_watch_png);    // assets\watch.png

LV_IMG_DECLARE(ui_img_check_png); // assets\check.png

void ui_init(void);
void set_style(lv_obj_t *button);

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
  lv_disp_flush_ready(disp);
}

void set_time(uint8_t sc, uint8_t mn, uint8_t hr, uint8_t dy, uint8_t mt, int yr)
{
  rtc.setTime(sc, mn, hr, dy, mt, yr);
}

void checkButton1()
{
  button1State = digitalRead(BT1); // read the button state on the pin "BUTTON_PIN"

  lv_disp_t *display = lv_disp_get_default();
  lv_obj_t *actScr = lv_disp_get_scr_act(display);
  if (button1State)
  {
    startPress1 = millis();
    //    Serial.print("HIGH - ");
    // button was pressed, currently high
    if (actScr == ui_testScreen)
    {
      lv_arc_set_value(ui_buttonDown, 1);
    }
  }
  else
  {
    long dur = millis() - startPress1;

    Serial.print("BTN1: ");
    Serial.println(dur);
    if (actScr == ui_testScreen)
    {
      lv_arc_set_value(ui_buttonDown, 0);
    }

    if (dur > DEBOUNCE)
    {
      if (screenOn)
      {
        if (actScr == ui_testScreen)
        {
          screenOn = true;
          onTime = millis();
        }
        else

            if (actScr != ui_menuScreen)
        {
          load_screen(1);
        }
        else
        {
          load_screen(0);
        }
      }
      else
      {
        screenOn = true;
        onTime = millis();
      }
    }
  }
}

void checkButton2()
{

  button2State = digitalRead(BT2); // read the button state on the pin "BUTTON_PIN"
  lv_disp_t *display = lv_disp_get_default();
  lv_obj_t *actScr = lv_disp_get_scr_act(display);
  if (button2State)
  {
    startPress2 = millis();
    //    Serial.print("HIGH - ");
    // button was pressed, currently high
    if (actScr == ui_testScreen)
    {
      lv_arc_set_value(ui_buttonUp, 1);
    }
  }
  else
  {
    long dur = millis() - startPress2;

    Serial.print("BTN2: ");
    Serial.println(dur);
    if (actScr == ui_testScreen)
    {
      lv_arc_set_value(ui_buttonUp, 0);
    }

    if (dur > DEBOUNCE)
    {
      if (screenOn)
      {

        if (actScr == ui_watchScreen)
        {
          screenOn = false;
        }
        else if (actScr == ui_testScreen)
        {
          screenOn = true;
          onTime = millis();
        }
        else
        {
          load_screen(0);
        }
      }
      else
      {
        screenOn = true;
        onTime = millis();
      }
    }
  }
}

void theme_change(lv_event_t *e)
{
  lv_obj_t *picker = lv_event_get_target(e);
  // val = lv_slider_get_value(picker); //
  lv_color_t c = lv_colorwheel_get_rgb(picker);
  lv_disp_t *display = lv_disp_get_default();
  lv_theme_t *theme = lv_theme_default_init(display, c, lv_palette_main(LV_PALETTE_RED),
                                            true, LV_FONT_DEFAULT);
  lv_disp_set_theme(display, theme);
  lv_obj_set_style_img_recolor(ui_hourIcon1, c, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_img_recolor(ui_hourIcon2, c, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_img_recolor(ui_minuteIcon1, c, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_img_recolor(ui_minuteIcon2, c, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(ui_timeSeparator, c, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(ui_dateText, c, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(ui_stepsProgress, c, LV_PART_INDICATOR | LV_STATE_DEFAULT);
}

static void event_handler(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);
  lv_obj_t *parent = lv_obj_get_parent(obj);

  lv_disp_t *display = lv_disp_get_default();
  lv_obj_t *actScr = lv_disp_get_scr_act(display);

  if (code == LV_EVENT_CLICKED)
  {
    if (parent == ui_messageList && actScr == ui_messageListScreen)
    {
      uint32_t i;
      for (i = 0; i < lv_obj_get_child_cnt(parent); i++)
      {
        lv_obj_t *child = lv_obj_get_child(parent, i);
        lv_state_t st = lv_obj_get_state(child);
        if (st == LV_STATE_FOCUSED)
        {
          sMsg = cMsg - i;
          break;
        }
      }

      load_screen(3);
    }
    else
    {
      if (actScr == ui_menuScreen)
      {

        if (obj == ui_buttonSettings)
        {
          load_screen(2);
        }
        if (obj == ui_buttonMessages)
        {
          load_screen(4);
        }
        if (obj == ui_buttonAbout)
        {
          load_screen(5);
        }
        if (obj == ui_buttonFind)
        {
          load_screen(6);
        }
      }
    }
  }
}

static void slider_event_cb(lv_event_t *e)
{
  lv_obj_t *slider = lv_event_get_target(e);
  if (slider == ui_brightnessSlider)
  {
    brightness = (int)lv_slider_get_value(slider);
  }
  if (slider == ui_powerSlider)
  {
    int v = (int)lv_slider_get_value(slider);
    lv_event_code_t code = lv_event_get_code(e);
    int opc = 255 - (v * 2.55);
    lv_obj_set_style_bg_opa(ui_powerSlider, opc, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_powerSlider, opc, LV_PART_MAIN | LV_STATE_DEFAULT);

    if (code == LV_EVENT_RELEASED)
    {
      if (v > 90)
      {
        deep_sleep();
      }
      else
      {
        lv_slider_set_value(ui_powerSlider, 0, LV_ANIM_ON);
      }
    }
  }
}

static void button_event_handler(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);

  if (code == LV_EVENT_PRESSED)
  {
    motor = true;
  }
  else if (code == LV_EVENT_RELEASED)
  {
    motor = false;
  }
}

void switch_event_cb(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);
  if (obj == ui_autoScreenSwitch)
  {
    autoScreen = lv_obj_has_state(ui_autoScreenSwitch, LV_STATE_CHECKED);
  }
  if (obj == ui_bluetoothSwitch)
  {
    bool state = lv_obj_has_state(ui_bluetoothSwitch, LV_STATE_CHECKED);
    if (state)
    {
      BLEDevice::startAdvertising();
    }
    else
    {
      BLEDevice::stopAdvertising();
    }
  }
  if (code == LV_EVENT_VALUE_CHANGED)
  {
  }
}

void my_gesture(lv_event_t *e)
{
  lv_obj_t *screen = lv_event_get_current_target(e);
  lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
  switch (dir)
  {
  case LV_DIR_LEFT:
    if (screen == ui_watchScreen)
    {
      load_screen(2);
    }
    if (screen == ui_textScreen)
    {
      load_screen(0);
    }

    break;
  case LV_DIR_RIGHT:
    if (screen == ui_watchScreen)
    {
      load_screen(3);
      sMsg = cMsg;
    }
    else if (screen == ui_textScreen)
    {
      load_screen(4);
    }
    else if (screen == ui_menuScreen)
    {
      load_screen(0);
    }
    else
    {
      load_screen(1);
    }

    break;
  case LV_DIR_TOP:
    if (screen == ui_watchScreen)
    {
      load_screen(1);
    }

    break;
  case LV_DIR_BOTTOM:
    bgImg = !bgImg;
    if (bgImg)
    {
      lv_img_set_src(ui_backgroundImage, &ui_img_background1_png);
    }
    else
    {
      lv_img_set_src(ui_backgroundImage, &ui_img_background2_png);
    }

    break;
  }
}

void my_input_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
  data->point.x = touch_data.xpos;
  data->point.y = touch_data.ypos;

  if (touch_data.touchpoints == 1 && touch_data.event == 2)
  {
    if (screenOn)
    {
      data->state = LV_INDEV_STATE_PR;
      // screenOn = true;
      onTime = millis();
      // Serial.println(touch_data.gesture);
    }
    else
    {
      data->state = LV_INDEV_STATE_REL;
    }
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
  // return false; /*No buffering now so no more data read*/
}

static void scroll_event_cb(lv_event_t *e)
{
  lv_obj_t *ui_menuList = lv_event_get_target(e);

  lv_area_t ui_menuList_a;
  lv_obj_get_coords(ui_menuList, &ui_menuList_a);
  lv_coord_t ui_menuList_y_center = ui_menuList_a.y1 + lv_area_get_height(&ui_menuList_a) / 2;

  lv_coord_t r = lv_obj_get_height(ui_menuList) * 7 / 10;
  uint32_t i;
  uint32_t child_cnt = lv_obj_get_child_cnt(ui_menuList);
  for (i = 0; i < child_cnt; i++)
  {
    lv_obj_t *child = lv_obj_get_child(ui_menuList, i);
    lv_area_t child_a;
    lv_obj_get_coords(child, &child_a);

    lv_coord_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2;

    lv_coord_t diff_y = child_y_center - ui_menuList_y_center;
    diff_y = LV_ABS(diff_y);

    /*Get the x of diff_y on a circle.*/
    lv_coord_t x;
    /*If diff_y is out of the circle use the last point of the circle (the radius)*/
    if (diff_y >= r)
    {
      x = r;
    }
    else
    {
      /*Use Pythagoras theorem to get x from radius and y*/
      uint32_t x_sqr = r * r - diff_y * diff_y;
      lv_sqrt_res_t res;
      lv_sqrt(x_sqr, &res, 0x8000); /*Use lvgl's built in sqrt root function*/
      x = r - res.i;
    }

    /*Translate the item by the calculated X coordinate*/
    lv_obj_set_style_translate_x(child, x, 0);

    /*Use some opacity with larger translations*/
    // lv_opa_t opa = lv_map(x, 0, r, LV_OPA_TRANSP, LV_OPA_COVER);
    // lv_obj_set_style_opa(child, LV_OPA_COVER - opa, 0);
  }
}

void IRAM_ATTR screenInt()
{
  screenOn = true;
  onTime = millis();
}

void wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    Serial.println("Wakeup caused by external signal using RTC_IO");
    break;
  case ESP_SLEEP_WAKEUP_EXT1:
    Serial.println("Wakeup caused by external signal using RTC_CNTL");
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    Serial.println("Wakeup caused by timer");
    break;
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
    Serial.println("Wakeup caused by touchpad");
    break;
  case ESP_SLEEP_WAKEUP_ULP:
    Serial.println("Wakeup caused by ULP program");
    break;
  default:
    Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
    if (rtc.getYear() < 2000)
    {
      rtc.setTime(1655998793); // set the time if not coming from deep sleep ie reset or power up
    }
    break;
  }
}

void deep_sleep()
{

  digitalWrite(MOTOR, HIGH);
  delay(100);
  digitalWrite(MOTOR, LOW);
  ledcWrite(ledChannel, 0);
  sleep_touch(true);
  myIMU.standby(true);
  rtc_gpio_pullup_en(GPIO_NUM_32);

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_32, 0); // 1 = High, 0 = Low

  // Go to sleep now
  Serial.println("Going to sleep now");
  esp_deep_sleep_start();
}

void setup()
{
  // put your setup code here, to run once:

  Serial.begin(115200);

  wakeup_reason();

  gfx->begin();
  gfx->fillScreen(BLACK);

  init_i2c();
  init_touch();
  pinMode(TINT, INPUT);
  // attachInterrupt(TINT, screenInt, RISING);

  lv_init();
  init_BLE();

  if (myIMU.begin(sampleRate, accelRange) != 0)
  {
    Serial.print("Failed to initialize IMU.\n");
  }
  else
  {
    Serial.print("IMU initialized.\n");
  }

  myIMU.intConf(123, 1, 10, HIGH);

  // Get the IMU ID:
  myIMU.readRegister(&imuID, KXTJ3_WHO_AM_I);

  screenWidth = gfx->width();
  screenHeight = gfx->height();

  Serial.print("Width: ");
  Serial.print(screenWidth);
  Serial.print("\tHeight: ");
  Serial.println(screenHeight);
  disp_draw_buf = (lv_color_t *)malloc(sizeof(lv_color_t) * screenWidth * 10);
  if (!disp_draw_buf)
  {
    Serial.println("LVGL disp_draw_buf allocate failed!");
  }
  else
  {
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, screenWidth * 10);

    /* Initialize the display */
    lv_disp_drv_init(&disp_drv);
    /* Change the following line to your display resolution */
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /* Initialize the input device driver */
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_input_read;
    lv_indev_drv_register(&indev_drv);

    lv_disp_t *display = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(display, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                              true, LV_FONT_DEFAULT);
    lv_disp_set_theme(display, theme);
    ui_watchScreen_screen_init();
    ui_menuScreen_screen_init();
    ui_messageListScreen_screen_init();
    ui_settingsScreen_screen_init();
    ui_textScreen_screen_init();
    ui_aboutScreen_screen_init();
    ui_testScreen_screen_init();
    lv_disp_load_scr(ui_watchScreen);

    Serial.println("Setup done");
  }

  onTime = millis();
  // pinMode(BL, OUTPUT);
  // digitalWrite(BL, HIGH);
  pinMode(CHARGE, INPUT);
  pinMode(BATT, INPUT);
  pinMode(MOTOR, OUTPUT);
  digitalWrite(MOTOR, HIGH);
  delay(100);
  digitalWrite(MOTOR, LOW);
  pinMode(B0, INPUT_PULLUP);
  attachInterrupt(B0, screenInt, FALLING);

  pinMode(BT1, INPUT_PULLUP);
  attachInterrupt(BT1, checkButton1, CHANGE);
  pinMode(BT2, INPUT_PULLUP);
  attachInterrupt(BT2, checkButton2, CHANGE);

  ledcSetup(ledChannel, freq, resolution);
  // attach the channel to the GPIO to be ui_menuListrolled
  ledcAttachPin(ledPin, ledChannel);
  ledcWrite(ledChannel, 100);
}

void loop()
{
  // put your main code here, to run repeatedly:

  if (screenOn)
  {
    get_read_touch();
  }

  lv_timer_handler(); /* let the GUI do its work */
  delay(5);

  String tm = rtc.getTime("%H:%M");
  int hr = rtc.getHour(hr24);
  int mn = rtc.getMinute();

  get_IMU();
  vector = sqrt((accX * accX) + (accY * accY) + (accZ * accZ));

  samples[x] = vector;
  x++;
  if (x >= SAMPLES)
  {
    x = 0;
    int st = calculate_steps();
    // lv_label_set_text(ui_Label9, String(st).c_str());
    curSteps += st;
  }
  lv_disp_t *display = lv_disp_get_default();
  lv_obj_t *actScr = lv_disp_get_scr_act(display);

  if (actScr == ui_watchScreen)
  {
    lv_label_set_text(ui_dateText, rtc.getTime("%d %B").c_str());
    lv_label_set_text(ui_weekdayText, rtc.getTime("%A").c_str());
    lv_label_set_text(ui_textAmPm, rtc.getAmPm(false).c_str());

    lv_img_set_src(ui_hourIcon1, &digits[hr / 10]);
    lv_img_set_src(ui_hourIcon2, &digits[hr % 10]);
    lv_img_set_src(ui_minuteIcon1, &digits[mn / 10]);
    lv_img_set_src(ui_minuteIcon2, &digits[mn % 10]);

    lv_label_set_text(ui_stepsText, String(curSteps).c_str());

    stepsProgress = (curSteps / 5000) * 100;

    lv_arc_set_value(ui_stepsProgress, curSteps);

    if (hr24)
    {
      lv_obj_add_flag(ui_textAmPm, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    }
    else
    {
      lv_obj_clear_flag(ui_textAmPm, LV_OBJ_FLAG_HIDDEN);
    }

    if (digitalRead(CHARGE))
    {
      lv_obj_add_flag(ui_chargingIcon, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    }
    else
    {
      lv_obj_clear_flag(ui_chargingIcon, LV_OBJ_FLAG_HIDDEN);
    }
    if (!deviceConnected)
    {
      lv_obj_add_flag(ui_bluetoothIcon, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    }
    else
    {
      lv_obj_clear_flag(ui_bluetoothIcon, LV_OBJ_FLAG_HIDDEN);
    }
  }
  else if (actScr == ui_testScreen)
  {
    digitalWrite(MOTOR, motor);

    lv_bar_set_value(ui_accXBar, map((accX * 100), -100, 100, 0, 100), LV_ANIM_ON);
    lv_bar_set_value(ui_accYBar, map((accY * 100), -100, 100, 0, 100), LV_ANIM_ON);
    lv_bar_set_value(ui_accZBar, map((accZ * 100), -100, 100, 0, 100), LV_ANIM_ON);
  }
  else if (actScr == ui_settingsScreen)
  {
    bool sw = lv_obj_has_state(ui_autoScreenSwitch, LV_STATE_CHECKED);
    if (sw != autoScreen)
    {
      if (autoScreen)
      {
        lv_obj_add_state(ui_autoScreenSwitch, LV_STATE_CHECKED);
      }
      else
      {
        lv_obj_clear_state(ui_autoScreenSwitch, LV_STATE_CHECKED);
      }
    }
  }

  if (isNotify)
  {
    isNotify = false;
    digitalWrite(MOTOR, HIGH);
    delay(notifyTime);
    digitalWrite(MOTOR, LOW);
  }

  if (accX > 0.3 && accX < 1.0 && accY > -0.3 && accY < 0.3 && accZ > -1.0 && accZ < -0.7)
  {
    if (autoScreen)
    {
      screenOn = true;
    }
  }

  if (screenOn)
  {
    // digitalWrite(BL, HIGH);
    ledcWrite(ledChannel, brightness + 1);
    String chrg = digitalRead(CHARGE) ? "" : "Charging";
    // String per = String(getBattery()) + "mV";
    if ((onTime + 10000) < millis())
    {
      screenOn = false;
    }
  }
  else
  {
    ledcWrite(ledChannel, 0);
    load_screen(0); // back to watchScreen
  }
}

void load_screen(int screen)
{
  switch (screen)
  {
  case 0:
    lv_disp_load_scr(ui_watchScreen);
    break;
  case 1:
    lv_disp_load_scr(ui_menuScreen);
    break;
  case 2:
    lv_disp_load_scr(ui_settingsScreen);
    break;
  case 3:
    lv_disp_load_scr(ui_textScreen);
    break;
  case 4:
    reload_message_list();
    lv_disp_load_scr(ui_messageListScreen);
    break;
  case 5:
    lv_disp_load_scr(ui_aboutScreen);
    break;
  case 6:
    lv_disp_load_scr(ui_testScreen);
    break;
  }
}

void reload_message_list()
{
  lv_obj_clean(ui_messageList);

  if (cMsg < 0)
  {
    lv_obj_t *msgSelect = lv_list_add_btn(ui_messageList, &ui_img_message_png, "No messages");
    set_style(msgSelect);
  }

  for (int i = cMsg; i >= 0 && i > (cMsg - 5); i--)
  {
    lv_obj_t *msgSelect = lv_list_add_btn(ui_messageList, &ui_img_comment_png, title[i % 5].c_str());
    lv_obj_add_event_cb(msgSelect, event_handler, LV_EVENT_CLICKED, NULL);
    set_style(msgSelect);
  }
}

void get_IMU()
{
  myIMU.standby(false);
  /*
    int16_t dataHighres = 0;

    if ( myIMU.readRegisterInt16( &dataHighres, KXTJ3_OUT_X_L ) != 0 ) {

      Serial.print("X: ");
      Serial.print(dataHighres);
    }

    if ( myIMU.readRegisterInt16( &dataHighres, KXTJ3_OUT_Y_L ) != 0 ) {

      Serial.print("\tY: ");
      Serial.print(dataHighres);
    }

    if ( myIMU.readRegisterInt16( &dataHighres, KXTJ3_OUT_Z_L ) != 0 ) {

      Serial.print("\tZ: ");
      Serial.println(dataHighres);
    }

  */

  accX = myIMU.axisAccel(X);
  accY = myIMU.axisAccel(Y);
  accZ = myIMU.axisAccel(Z);

  myIMU.standby(true);
}

int calculate_steps()
{
  float high = samples[0];
  float low = samples[0];
  for (int x = 0; x < SAMPLES; x++)
  {
    if (samples[x] < low)
    {
      low = samples[x];
    }
    if (samples[x] > high)
    {
      high = samples[x];
    }
  }
  float mid = (high + low) / 2;
  float thresh = (high - low) / 4;
  int h = 0, l = 0;
  int flag = 0;
  for (int x = 0; x < SAMPLES; x++)
  {
    if (samples[x] < (mid - thresh) && thresh > 0.1)
    {
      if (flag == 0 || flag == 1)
      {
        l++;
        flag = 2;
      }
    }
    else
    {
      flag = 0;
    }
    if (samples[x] > (mid + thresh) && thresh > 0.1)
    {
      if (flag == 0 || flag == 2)
      {
        h++;
        flag = 1;
      }
    }
    else
    {
      flag = 0;
    }
  }

  if (h < l)
  {
    return h;
  }
  else
  {
    return l;
  }
}

void set_style(lv_obj_t *button)
{

  lv_obj_set_style_pad_top(button, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(button, 10, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_color(button, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(button, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
}

///////////////////// SCREENS ////////////////////

void ui_textScreen_screen_init(void)
{

  // ui_textScreen

  ui_textScreen = lv_obj_create(NULL);

  lv_obj_clear_flag(ui_textScreen, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_set_style_bg_color(ui_textScreen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_textScreen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_event_cb(ui_textScreen, my_gesture, LV_EVENT_GESTURE, NULL);

  // ui_textScroll

  ui_textScroll = lv_obj_create(ui_textScreen);

  lv_obj_set_width(ui_textScroll, 240);
  lv_obj_set_height(ui_textScroll, 210);

  lv_obj_set_x(ui_textScroll, 0);
  lv_obj_set_y(ui_textScroll, -15);

  lv_obj_set_align(ui_textScroll, LV_ALIGN_CENTER);

  lv_obj_set_scrollbar_mode(ui_textScroll, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_scroll_dir(ui_textScroll, LV_DIR_VER);

  lv_obj_set_style_bg_color(ui_textScroll, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_textScroll, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(ui_textScroll, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_opa(ui_textScroll, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_textScroll, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_color(ui_textScroll, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_textScroll, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_width(ui_textScroll, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(ui_textScroll, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(ui_textScroll, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(ui_textScroll, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(ui_textScroll, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui_textScroll, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  // ui_textIcon

  ui_textIcon = lv_img_create(ui_textScroll);
  lv_img_set_src(ui_textIcon, &ui_img_comment_png);

  lv_obj_set_width(ui_textIcon, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_textIcon, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_textIcon, 0);
  lv_obj_set_y(ui_textIcon, -98);

  lv_obj_set_align(ui_textIcon, LV_ALIGN_CENTER);

  lv_obj_add_flag(ui_textIcon, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_clear_flag(ui_textIcon, LV_OBJ_FLAG_SCROLLABLE);

  // ui_textTitle

  ui_textTitle = lv_label_create(ui_textScroll);

  lv_obj_set_width(ui_textTitle, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_textTitle, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_textTitle, 0);
  lv_obj_set_y(ui_textTitle, -71);

  lv_obj_set_align(ui_textTitle, LV_ALIGN_CENTER);

  lv_label_set_long_mode(ui_textTitle, LV_LABEL_LONG_SCROLL_CIRCULAR);
  lv_label_set_text(ui_textTitle, "John Doe");

  lv_obj_set_style_text_font(ui_textTitle, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

  // ui_textMessage

  ui_textMessage = lv_label_create(ui_textScroll);

  lv_obj_set_width(ui_textMessage, 180);
  lv_obj_set_height(ui_textMessage, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_textMessage, 0);
  lv_obj_set_y(ui_textMessage, 40);

  lv_obj_set_align(ui_textMessage, LV_ALIGN_TOP_MID);

  lv_label_set_text(ui_textMessage,
                    "Lorem ipsum dolor sit amet\n, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");

  lv_obj_set_style_pad_left(ui_textMessage, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(ui_textMessage, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(ui_textMessage, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui_textMessage, 50, LV_PART_MAIN | LV_STATE_DEFAULT);

  // ui_textTime

  ui_textTime = lv_label_create(ui_textScreen);

  lv_obj_set_width(ui_textTime, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_textTime, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_textTime, -5);
  lv_obj_set_y(ui_textTime, 103);

  lv_obj_set_align(ui_textTime, LV_ALIGN_CENTER);

  lv_label_set_text(ui_textTime, "10:45");
}

void ui_watchScreen_screen_init(void)
{

  // ui_watchScreen

  ui_watchScreen = lv_obj_create(NULL);

  lv_obj_clear_flag(ui_watchScreen, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_set_style_bg_color(ui_watchScreen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_watchScreen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_event_cb(ui_watchScreen, my_gesture, LV_EVENT_GESTURE, NULL);

  // ui_backgroundImage

  ui_backgroundImage = lv_img_create(ui_watchScreen);
  lv_img_set_src(ui_backgroundImage, &ui_img_background1_png);

  lv_obj_set_width(ui_backgroundImage, 240);
  lv_obj_set_height(ui_backgroundImage, 240);

  lv_obj_set_x(ui_backgroundImage, 0);
  lv_obj_set_y(ui_backgroundImage, 0);

  lv_obj_set_align(ui_backgroundImage, LV_ALIGN_CENTER);

  lv_obj_add_flag(ui_backgroundImage, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_clear_flag(ui_backgroundImage, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_set_style_img_recolor(ui_backgroundImage, lv_color_hex(0x19174A), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_img_recolor_opa(ui_backgroundImage, 200, LV_PART_MAIN | LV_STATE_DEFAULT);

  // ui_stepsProgress

  ui_stepsProgress = lv_arc_create(ui_watchScreen);

  lv_obj_set_width(ui_stepsProgress, 240);
  lv_obj_set_height(ui_stepsProgress, 240);

  lv_obj_set_x(ui_stepsProgress, 0);
  lv_obj_set_y(ui_stepsProgress, 0);

  lv_obj_set_align(ui_stepsProgress, LV_ALIGN_CENTER);

  lv_obj_clear_flag(ui_stepsProgress, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

  lv_arc_set_range(ui_stepsProgress, 0, 100);
  lv_arc_set_value(ui_stepsProgress, 21);
  lv_arc_set_bg_angles(ui_stepsProgress, 120, 60);

  lv_obj_set_style_arc_color(ui_stepsProgress, lv_color_hex(0x4040FF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_opa(ui_stepsProgress, 50, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_color(ui_stepsProgress, lv_color_hex(0x39FA68), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_stepsProgress, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_color(ui_stepsProgress, lv_color_hex(0x39FA68), LV_PART_KNOB | LV_STATE_DEFAULT);

  // ui_weekdayText

  ui_weekdayText = lv_label_create(ui_watchScreen);

  lv_obj_set_width(ui_weekdayText, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_weekdayText, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_weekdayText, -1);
  lv_obj_set_y(ui_weekdayText, -50);

  lv_obj_set_align(ui_weekdayText, LV_ALIGN_CENTER);

  lv_label_set_text(ui_weekdayText, "Sunday");

  lv_obj_set_style_text_color(ui_weekdayText, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_opa(ui_weekdayText, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_weekdayText, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

  // ui_dateText

  ui_dateText = lv_label_create(ui_watchScreen);

  lv_obj_set_width(ui_dateText, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_dateText, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_dateText, 0);
  lv_obj_set_y(ui_dateText, 50);

  lv_obj_set_align(ui_dateText, LV_ALIGN_CENTER);

  lv_label_set_text(ui_dateText, "10 July");

  lv_obj_set_style_text_color(ui_dateText, lv_color_hex(0x2095F6), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_opa(ui_dateText, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_dateText, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

  // ui_hourIcon1

  ui_hourIcon1 = lv_img_create(ui_watchScreen);
  lv_img_set_src(ui_hourIcon1, &ui_img_zero_64_png);

  lv_obj_set_width(ui_hourIcon1, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_hourIcon1, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_hourIcon1, -70);
  lv_obj_set_y(ui_hourIcon1, 1);

  lv_obj_set_align(ui_hourIcon1, LV_ALIGN_CENTER);

  lv_obj_add_flag(ui_hourIcon1, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_clear_flag(ui_hourIcon1, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_set_style_img_recolor(ui_hourIcon1, lv_color_hex(0x2095F6), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_img_recolor_opa(ui_hourIcon1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  // ui_hourIcon2

  ui_hourIcon2 = lv_img_create(ui_watchScreen);
  lv_img_set_src(ui_hourIcon2, &ui_img_five_64_png);

  lv_obj_set_width(ui_hourIcon2, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_hourIcon2, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_hourIcon2, -30);
  lv_obj_set_y(ui_hourIcon2, 0);

  lv_obj_set_align(ui_hourIcon2, LV_ALIGN_CENTER);

  lv_obj_add_flag(ui_hourIcon2, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_clear_flag(ui_hourIcon2, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_set_style_img_recolor(ui_hourIcon2, lv_color_hex(0x2095F6), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_img_recolor_opa(ui_hourIcon2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  // ui_minuteIcon1

  ui_minuteIcon1 = lv_img_create(ui_watchScreen);
  lv_img_set_src(ui_minuteIcon1, &ui_img_one_64_png);

  lv_obj_set_width(ui_minuteIcon1, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_minuteIcon1, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_minuteIcon1, 30);
  lv_obj_set_y(ui_minuteIcon1, 0);

  lv_obj_set_align(ui_minuteIcon1, LV_ALIGN_CENTER);

  lv_obj_add_flag(ui_minuteIcon1, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_clear_flag(ui_minuteIcon1, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_set_style_img_recolor(ui_minuteIcon1, lv_color_hex(0x2095F6), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_img_recolor_opa(ui_minuteIcon1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  // ui_minuteIcon2

  ui_minuteIcon2 = lv_img_create(ui_watchScreen);
  lv_img_set_src(ui_minuteIcon2, &ui_img_nine_64_png);

  lv_obj_set_width(ui_minuteIcon2, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_minuteIcon2, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_minuteIcon2, 70);
  lv_obj_set_y(ui_minuteIcon2, 0);

  lv_obj_set_align(ui_minuteIcon2, LV_ALIGN_CENTER);

  lv_obj_add_flag(ui_minuteIcon2, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_clear_flag(ui_minuteIcon2, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_set_style_img_recolor(ui_minuteIcon2, lv_color_hex(0x2095F6), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_img_recolor_opa(ui_minuteIcon2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  // ui_timeSeparator

  ui_timeSeparator = lv_label_create(ui_watchScreen);

  lv_obj_set_width(ui_timeSeparator, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_timeSeparator, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_timeSeparator, 0);
  lv_obj_set_y(ui_timeSeparator, 0);

  lv_obj_set_align(ui_timeSeparator, LV_ALIGN_CENTER);

  lv_label_set_text(ui_timeSeparator, ":");

  lv_obj_set_style_text_color(ui_timeSeparator, lv_color_hex(0x2095F6), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_opa(ui_timeSeparator, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_timeSeparator, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);

  // ui_textAmPm

  ui_textAmPm = lv_label_create(ui_watchScreen);

  lv_obj_set_width(ui_textAmPm, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_textAmPm, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_textAmPm, 67);
  lv_obj_set_y(ui_textAmPm, 39);

  lv_obj_set_align(ui_textAmPm, LV_ALIGN_CENTER);

  lv_label_set_text(ui_textAmPm, "AM");

  // ui_stepsIcon

  ui_stepsIcon = lv_img_create(ui_watchScreen);
  lv_img_set_src(ui_stepsIcon, &ui_img_steps_png);

  lv_obj_set_width(ui_stepsIcon, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_stepsIcon, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_stepsIcon, -25);
  lv_obj_set_y(ui_stepsIcon, 100);

  lv_obj_set_align(ui_stepsIcon, LV_ALIGN_CENTER);

  lv_obj_add_flag(ui_stepsIcon, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_clear_flag(ui_stepsIcon, LV_OBJ_FLAG_SCROLLABLE);

  // ui_stepsText

  ui_stepsText = lv_label_create(ui_watchScreen);

  lv_obj_set_width(ui_stepsText, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_stepsText, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_stepsText, 110);
  lv_obj_set_y(ui_stepsText, 102);

  lv_obj_set_align(ui_stepsText, LV_ALIGN_LEFT_MID);

  lv_label_set_text(ui_stepsText, "12");

  lv_obj_set_style_text_align(ui_stepsText, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_stepsText, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

  // ui_bluetoothIcon

  ui_bluetoothIcon = lv_img_create(ui_watchScreen);
  lv_img_set_src(ui_bluetoothIcon, &ui_img_bluetooth_png);

  lv_obj_set_width(ui_bluetoothIcon, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_bluetoothIcon, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_bluetoothIcon, 50);
  lv_obj_set_y(ui_bluetoothIcon, 75);

  lv_obj_set_align(ui_bluetoothIcon, LV_ALIGN_CENTER);

  lv_obj_add_flag(ui_bluetoothIcon, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_clear_flag(ui_bluetoothIcon, LV_OBJ_FLAG_SCROLLABLE);

  // ui_chargingIcon

  ui_chargingIcon = lv_img_create(ui_watchScreen);
  lv_img_set_src(ui_chargingIcon, &ui_img_energy_png);

  lv_obj_set_width(ui_chargingIcon, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_chargingIcon, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_chargingIcon, -50);
  lv_obj_set_y(ui_chargingIcon, 75);

  lv_obj_set_align(ui_chargingIcon, LV_ALIGN_CENTER);

  lv_obj_add_flag(ui_chargingIcon, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_clear_flag(ui_chargingIcon, LV_OBJ_FLAG_SCROLLABLE);
}

void ui_settingsScreen_screen_init(void)
{

  // ui_settingsScreen

  ui_settingsScreen = lv_obj_create(NULL);

  lv_obj_clear_flag(ui_settingsScreen, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_set_style_bg_color(ui_settingsScreen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_settingsScreen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_event_cb(ui_settingsScreen, my_gesture, LV_EVENT_GESTURE, NULL);

  // ui_scrollSettings

  ui_scrollSettings = lv_obj_create(ui_settingsScreen);

  lv_obj_set_width(ui_scrollSettings, 240);
  lv_obj_set_height(ui_scrollSettings, 240);

  lv_obj_set_x(ui_scrollSettings, 0);
  lv_obj_set_y(ui_scrollSettings, 0);

  lv_obj_set_align(ui_scrollSettings, LV_ALIGN_CENTER);

  lv_obj_set_scrollbar_mode(ui_scrollSettings, LV_SCROLLBAR_MODE_OFF);

  lv_obj_set_style_bg_color(ui_scrollSettings, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_scrollSettings, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_scrollSettings, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(ui_scrollSettings, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(ui_scrollSettings, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(ui_scrollSettings, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui_scrollSettings, 70, LV_PART_MAIN | LV_STATE_DEFAULT);

  // ui_bluetoothText

  ui_bluetoothText = lv_label_create(ui_scrollSettings);

  lv_obj_set_width(ui_bluetoothText, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_bluetoothText, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_bluetoothText, -41);
  lv_obj_set_y(ui_bluetoothText, -75);

  lv_obj_set_align(ui_bluetoothText, LV_ALIGN_CENTER);

  lv_label_set_text(ui_bluetoothText, "Bluetooth");

  lv_obj_set_style_pad_left(ui_bluetoothText, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(ui_bluetoothText, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(ui_bluetoothText, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui_bluetoothText, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  // ui_bluetoothSwitch

  ui_bluetoothSwitch = lv_switch_create(ui_scrollSettings);

  lv_obj_set_width(ui_bluetoothSwitch, 50);
  lv_obj_set_height(ui_bluetoothSwitch, 25);

  lv_obj_set_x(ui_bluetoothSwitch, 43);
  lv_obj_set_y(ui_bluetoothSwitch, -75);

  lv_obj_set_align(ui_bluetoothSwitch, LV_ALIGN_CENTER);

  lv_obj_add_state(ui_bluetoothSwitch, LV_STATE_CHECKED);

  lv_obj_add_event_cb(ui_bluetoothSwitch, switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

  // ui_brightnessText

  ui_brightnessText = lv_label_create(ui_scrollSettings);

  lv_obj_set_width(ui_brightnessText, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_brightnessText, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_brightnessText, -38);
  lv_obj_set_y(ui_brightnessText, -50);

  lv_obj_set_align(ui_brightnessText, LV_ALIGN_CENTER);

  lv_label_set_text(ui_brightnessText, "Brightness");

  // ui_brightnessSlider

  ui_brightnessSlider = lv_slider_create(ui_scrollSettings);
  lv_slider_set_range(ui_brightnessSlider, 0, 255);
  lv_slider_set_value(ui_brightnessSlider, 100, LV_ANIM_OFF);
  if (lv_slider_get_mode(ui_brightnessSlider) == LV_SLIDER_MODE_RANGE)
    lv_slider_set_left_value(ui_brightnessSlider, 10,
                             LV_ANIM_OFF);

  lv_obj_set_width(ui_brightnessSlider, 150);
  lv_obj_set_height(ui_brightnessSlider, 10);

  lv_obj_set_x(ui_brightnessSlider, 0);
  lv_obj_set_y(ui_brightnessSlider, -30);

  lv_obj_set_align(ui_brightnessSlider, LV_ALIGN_CENTER);

  lv_obj_clear_flag(ui_brightnessSlider, LV_OBJ_FLAG_GESTURE_BUBBLE);

  lv_obj_add_event_cb(ui_brightnessSlider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

  // ui_autoScreenText

  ui_autoScreenText = lv_label_create(ui_scrollSettings);

  lv_obj_set_width(ui_autoScreenText, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_autoScreenText, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_autoScreenText, -45);
  lv_obj_set_y(ui_autoScreenText, 0);

  lv_obj_set_align(ui_autoScreenText, LV_ALIGN_CENTER);

  lv_label_set_text(ui_autoScreenText, "Raise to wake");

  // ui_autoScreenSwitch

  ui_autoScreenSwitch = lv_switch_create(ui_scrollSettings);

  lv_obj_set_width(ui_autoScreenSwitch, 50);
  lv_obj_set_height(ui_autoScreenSwitch, 25);

  lv_obj_set_x(ui_autoScreenSwitch, 45);
  lv_obj_set_y(ui_autoScreenSwitch, 0);

  lv_obj_set_align(ui_autoScreenSwitch, LV_ALIGN_CENTER);

  lv_obj_add_event_cb(ui_autoScreenSwitch, switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

  // ui_themeText

  ui_themeText = lv_label_create(ui_scrollSettings);

  lv_obj_set_width(ui_themeText, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_themeText, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_themeText, 0);
  lv_obj_set_y(ui_themeText, 30);

  lv_obj_set_align(ui_themeText, LV_ALIGN_CENTER);

  lv_label_set_text(ui_themeText, "ThemeColor");

  // ui_colorWheel

  ui_colorWheel = lv_colorwheel_create(ui_scrollSettings, true);

  lv_obj_set_width(ui_colorWheel, 100);
  lv_obj_set_height(ui_colorWheel, 100);

  lv_obj_set_x(ui_colorWheel, 0);
  lv_obj_set_y(ui_colorWheel, 80);

  lv_obj_set_align(ui_colorWheel, LV_ALIGN_TOP_MID);

  lv_obj_clear_flag(ui_colorWheel, LV_OBJ_FLAG_GESTURE_BUBBLE);

  lv_obj_add_event_cb(ui_colorWheel, theme_change, LV_EVENT_VALUE_CHANGED, NULL);

  // ui_powerText

  ui_powerText = lv_label_create(ui_scrollSettings);

  lv_obj_set_width(ui_powerText, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_powerText, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_powerText, 0);
  lv_obj_set_y(ui_powerText, 170);

  lv_obj_set_align(ui_powerText, LV_ALIGN_CENTER);

  lv_label_set_text(ui_powerText, "Slide to power off");

  // ui_powerSlider

  ui_powerSlider = lv_slider_create(ui_scrollSettings);
  lv_slider_set_range(ui_powerSlider, 0, 100);

  lv_obj_set_width(ui_powerSlider, 150);
  lv_obj_set_height(ui_powerSlider, 25);

  lv_obj_set_x(ui_powerSlider, 6);
  lv_obj_set_y(ui_powerSlider, 200);

  lv_obj_set_align(ui_powerSlider, LV_ALIGN_CENTER);
  lv_obj_add_flag(ui_powerSlider, LV_OBJ_FLAG_ADV_HITTEST);

  lv_obj_clear_flag(ui_powerSlider, LV_OBJ_FLAG_GESTURE_BUBBLE);

  lv_obj_add_event_cb(ui_powerSlider, slider_event_cb, LV_EVENT_ALL, NULL);
}

void ui_menuScreen_screen_init(void)
{

  // ui_menuScreen

  ui_menuScreen = lv_obj_create(NULL);

  lv_obj_set_style_bg_color(ui_menuScreen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_menuScreen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_event_cb(ui_menuScreen, my_gesture, LV_EVENT_GESTURE, NULL);

  ui_menuList = lv_obj_create(ui_menuScreen);
  lv_obj_set_size(ui_menuList, 240, 240);
  lv_obj_center(ui_menuList);
  lv_obj_set_flex_flow(ui_menuList, LV_FLEX_FLOW_COLUMN);
  lv_obj_add_event_cb(ui_menuList, scroll_event_cb, LV_EVENT_SCROLL, NULL);
  lv_obj_set_style_radius(ui_menuList, LV_RADIUS_CIRCLE, 0);
  lv_obj_set_style_clip_corner(ui_menuList, true, 0);
  lv_obj_set_scroll_dir(ui_menuList, LV_DIR_VER);
  // lv_obj_set_scroll_snap_y(ui_menuList, LV_SCROLL_SNAP_CENTER);
  lv_obj_set_scrollbar_mode(ui_menuList, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_style_bg_color(ui_menuList, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_menuList, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_pad_top(ui_menuList, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui_menuList, 50, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_buttonMessages = lv_list_add_btn(ui_menuList, &ui_img_message_png, "Messages");
  lv_obj_add_event_cb(ui_buttonMessages, event_handler, LV_EVENT_CLICKED, NULL);
  set_style(ui_buttonMessages);

  ui_buttonFitness = lv_list_add_btn(ui_menuList, &ui_img_fitness_png, "Fitness");
  lv_obj_add_event_cb(ui_buttonFitness, event_handler, LV_EVENT_CLICKED, NULL);
  set_style(ui_buttonFitness);

  ui_buttonHealth = lv_list_add_btn(ui_menuList, &ui_img_heart_png, "Health");
  lv_obj_add_event_cb(ui_buttonHealth, event_handler, LV_EVENT_CLICKED, NULL);
  set_style(ui_buttonHealth);

  ui_buttonWeather = lv_list_add_btn(ui_menuList, &ui_img_weather_png, "Weather");
  lv_obj_add_event_cb(ui_buttonWeather, event_handler, LV_EVENT_CLICKED, NULL);
  set_style(ui_buttonWeather);

  ui_buttonMusic = lv_list_add_btn(ui_menuList, &ui_img_music_png, "Music");
  lv_obj_add_event_cb(ui_buttonMusic, event_handler, LV_EVENT_CLICKED, NULL);
  set_style(ui_buttonMusic);

  ui_buttonStopwatch = lv_list_add_btn(ui_menuList, &ui_img_stopwatch_png, "Stopwatch");
  lv_obj_add_event_cb(ui_buttonStopwatch, event_handler, LV_EVENT_CLICKED, NULL);
  set_style(ui_buttonStopwatch);

  ui_buttonTimer = lv_list_add_btn(ui_menuList, &ui_img_timer_png, "Timer");
  lv_obj_add_event_cb(ui_buttonTimer, event_handler, LV_EVENT_CLICKED, NULL);
  set_style(ui_buttonTimer);

  ui_buttonFind = lv_list_add_btn(ui_menuList, &ui_img_search_png, "Tests");
  lv_obj_add_event_cb(ui_buttonFind, event_handler, LV_EVENT_CLICKED, NULL);
  set_style(ui_buttonFind);

  ui_buttonSettings = lv_list_add_btn(ui_menuList, &ui_img_setting_png, "Settings");
  lv_obj_add_event_cb(ui_buttonSettings, event_handler, LV_EVENT_CLICKED, NULL);
  set_style(ui_buttonSettings);

  ui_buttonAbout = lv_list_add_btn(ui_menuList, &ui_img_about_png, "About");
  lv_obj_add_event_cb(ui_buttonAbout, event_handler, LV_EVENT_CLICKED, NULL);
  set_style(ui_buttonAbout);

  /*Update the buttons position manually for first*/
  lv_event_send(ui_menuList, LV_EVENT_SCROLL, NULL);

  /*Be sure the fist button is in the middle*/
  lv_obj_scroll_to_view(lv_obj_get_child(ui_menuList, 0), LV_ANIM_ON);
}

void ui_messageListScreen_screen_init(void)
{

  // ui_messageListScreen

  ui_messageListScreen = lv_obj_create(NULL);

  lv_obj_set_style_bg_color(ui_messageListScreen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_messageListScreen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_event_cb(ui_messageListScreen, my_gesture, LV_EVENT_GESTURE, NULL);

  ui_messageList = lv_obj_create(ui_messageListScreen);
  lv_obj_set_size(ui_messageList, 240, 240);
  lv_obj_center(ui_messageList);
  lv_obj_set_flex_flow(ui_messageList, LV_FLEX_FLOW_COLUMN);
  lv_obj_add_event_cb(ui_messageList, scroll_event_cb, LV_EVENT_SCROLL, NULL);
  lv_obj_set_style_radius(ui_messageList, LV_RADIUS_CIRCLE, 0);
  lv_obj_set_style_clip_corner(ui_messageList, true, 0);
  lv_obj_set_scroll_dir(ui_messageList, LV_DIR_VER);
  lv_obj_set_scroll_snap_y(ui_messageList, LV_SCROLL_SNAP_CENTER);
  lv_obj_set_scrollbar_mode(ui_messageList, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_style_bg_color(ui_messageList, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_messageList, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_pad_top(ui_messageList, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui_messageList, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void ui_testScreen_screen_init(void)
{

  // ui_testScreen

  ui_testScreen = lv_obj_create(NULL);

  lv_obj_clear_flag(ui_testScreen, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_set_style_bg_color(ui_testScreen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_testScreen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_event_cb(ui_testScreen, my_gesture, LV_EVENT_GESTURE, NULL);

  // ui_buttonUp

  ui_buttonUp = lv_arc_create(ui_testScreen);

  lv_obj_set_width(ui_buttonUp, 240);
  lv_obj_set_height(ui_buttonUp, 240);

  lv_obj_set_x(ui_buttonUp, 0);
  lv_obj_set_y(ui_buttonUp, 0);

  lv_obj_set_align(ui_buttonUp, LV_ALIGN_CENTER);

  lv_arc_set_range(ui_buttonUp, 0, 1);
  lv_arc_set_value(ui_buttonUp, 0);
  lv_arc_set_bg_angles(ui_buttonUp, 310, 340);

  lv_obj_set_style_bg_color(ui_buttonUp, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_buttonUp, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

  // ui_buttonDown

  ui_buttonDown = lv_arc_create(ui_testScreen);

  lv_obj_set_width(ui_buttonDown, 240);
  lv_obj_set_height(ui_buttonDown, 240);

  lv_obj_set_x(ui_buttonDown, 0);
  lv_obj_set_y(ui_buttonDown, 0);

  lv_obj_set_align(ui_buttonDown, LV_ALIGN_CENTER);

  lv_arc_set_range(ui_buttonDown, 0, 1);
  lv_arc_set_value(ui_buttonDown, 0);
  lv_arc_set_bg_angles(ui_buttonDown, 380, 410);

  lv_obj_set_style_bg_color(ui_buttonDown, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_buttonDown, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

  // ui_scrollTest

  ui_scrollTest = lv_obj_create(ui_testScreen);

  lv_obj_set_width(ui_scrollTest, 240);
  lv_obj_set_height(ui_scrollTest, 240);

  lv_obj_set_x(ui_scrollTest, 0);
  lv_obj_set_y(ui_scrollTest, 0);

  lv_obj_set_align(ui_scrollTest, LV_ALIGN_CENTER);

  lv_obj_set_scrollbar_mode(ui_scrollTest, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_scroll_dir(ui_scrollTest, LV_DIR_VER);

  lv_obj_set_style_radius(ui_scrollTest, 120, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(ui_scrollTest, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_scrollTest, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_scrollTest, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_width(ui_scrollTest, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(ui_scrollTest, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(ui_scrollTest, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(ui_scrollTest, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(ui_scrollTest, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui_scrollTest, 70, LV_PART_MAIN | LV_STATE_DEFAULT);

  // ui_vibrateText

  ui_vibrateText = lv_label_create(ui_scrollTest);

  lv_obj_set_width(ui_vibrateText, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_vibrateText, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_vibrateText, 25);
  lv_obj_set_y(ui_vibrateText, -50);

  lv_obj_set_align(ui_vibrateText, LV_ALIGN_CENTER);

  lv_label_set_text(ui_vibrateText, "Vibrate Test");

  // ui_buttonvibrate

  ui_buttonvibrate = lv_imgbtn_create(ui_scrollTest);
  lv_imgbtn_set_src(ui_buttonvibrate, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_vibrate_png, NULL);
  lv_imgbtn_set_src(ui_buttonvibrate, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_vibrate_png, NULL);

  lv_obj_set_height(ui_buttonvibrate, 24);
  lv_obj_set_width(ui_buttonvibrate, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_buttonvibrate, -51);
  lv_obj_set_y(ui_buttonvibrate, -50);

  lv_obj_set_align(ui_buttonvibrate, LV_ALIGN_CENTER);

  lv_obj_set_style_outline_color(ui_buttonvibrate, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_buttonvibrate, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_width(ui_buttonvibrate, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(ui_buttonvibrate, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(ui_buttonvibrate, lv_color_hex(0x22540C), LV_PART_MAIN | LV_STATE_PRESSED);
  lv_obj_set_style_border_opa(ui_buttonvibrate, 255, LV_PART_MAIN | LV_STATE_PRESSED);
  lv_obj_set_style_border_width(ui_buttonvibrate, 2, LV_PART_MAIN | LV_STATE_PRESSED);
  lv_obj_set_style_img_recolor(ui_buttonvibrate, lv_color_hex(0x2B258D),
                               LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);
  lv_obj_set_style_img_recolor_opa(ui_buttonvibrate, 255, LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);

  lv_obj_add_event_cb(ui_buttonvibrate, button_event_handler, LV_EVENT_ALL, NULL);

  // ui_accelerometerText

  ui_accelerometerText = lv_label_create(ui_scrollTest);

  lv_obj_set_width(ui_accelerometerText, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_accelerometerText, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_accelerometerText, 0);
  lv_obj_set_y(ui_accelerometerText, 10);

  lv_obj_set_align(ui_accelerometerText, LV_ALIGN_CENTER);

  lv_label_set_text(ui_accelerometerText, "Accelerometer");

  // ui_accXtext

  ui_accXtext = lv_label_create(ui_scrollTest);

  lv_obj_set_width(ui_accXtext, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_accXtext, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_accXtext, -75);
  lv_obj_set_y(ui_accXtext, 30);

  lv_obj_set_align(ui_accXtext, LV_ALIGN_CENTER);

  lv_label_set_text(ui_accXtext, "X");

  // ui_accXBar

  ui_accXBar = lv_bar_create(ui_scrollTest);
  lv_bar_set_range(ui_accXBar, 0, 100);
  lv_bar_set_value(ui_accXBar, 25, LV_ANIM_OFF);

  lv_obj_set_width(ui_accXBar, 150);
  lv_obj_set_height(ui_accXBar, 10);

  lv_obj_set_x(ui_accXBar, 10);
  lv_obj_set_y(ui_accXBar, 30);

  lv_obj_set_align(ui_accXBar, LV_ALIGN_CENTER);

  // ui_accYText

  ui_accYText = lv_label_create(ui_scrollTest);

  lv_obj_set_width(ui_accYText, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_accYText, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_accYText, -75);
  lv_obj_set_y(ui_accYText, 50);

  lv_obj_set_align(ui_accYText, LV_ALIGN_CENTER);

  lv_label_set_text(ui_accYText, "Y");

  // ui_accYBar

  ui_accYBar = lv_bar_create(ui_scrollTest);
  lv_bar_set_range(ui_accYBar, 0, 100);
  lv_bar_set_value(ui_accYBar, 25, LV_ANIM_OFF);

  lv_obj_set_width(ui_accYBar, 150);
  lv_obj_set_height(ui_accYBar, 10);

  lv_obj_set_x(ui_accYBar, 10);
  lv_obj_set_y(ui_accYBar, 50);

  lv_obj_set_align(ui_accYBar, LV_ALIGN_CENTER);

  // ui_accZText

  ui_accZText = lv_label_create(ui_scrollTest);

  lv_obj_set_width(ui_accZText, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_accZText, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_accZText, -75);
  lv_obj_set_y(ui_accZText, 70);

  lv_obj_set_align(ui_accZText, LV_ALIGN_CENTER);

  lv_label_set_text(ui_accZText, "Z");

  // ui_accZBar

  ui_accZBar = lv_bar_create(ui_scrollTest);
  lv_bar_set_range(ui_accZBar, 0, 100);
  lv_bar_set_value(ui_accZBar, 25, LV_ANIM_OFF);

  lv_obj_set_width(ui_accZBar, 150);
  lv_obj_set_height(ui_accZBar, 10);

  lv_obj_set_x(ui_accZBar, 10);
  lv_obj_set_y(ui_accZBar, 70);

  lv_obj_set_align(ui_accZBar, LV_ALIGN_CENTER);
}

void ui_aboutScreen_screen_init(void)
{

  // ui_aboutScreen

  ui_aboutScreen = lv_obj_create(NULL);

  lv_obj_clear_flag(ui_aboutScreen, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_set_style_bg_color(ui_aboutScreen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_aboutScreen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_event_cb(ui_aboutScreen, my_gesture, LV_EVENT_GESTURE, NULL);

  ui_aboutList = lv_obj_create(ui_aboutScreen);
  lv_obj_set_size(ui_aboutList, 240, 240);
  lv_obj_center(ui_aboutList);
  lv_obj_set_flex_flow(ui_aboutList, LV_FLEX_FLOW_COLUMN);
  lv_obj_add_event_cb(ui_aboutList, scroll_event_cb, LV_EVENT_SCROLL, NULL);
  lv_obj_set_style_radius(ui_aboutList, LV_RADIUS_CIRCLE, 0);
  lv_obj_set_style_clip_corner(ui_aboutList, true, 0);
  lv_obj_set_scroll_dir(ui_aboutList, LV_DIR_VER);
  lv_obj_set_scroll_snap_y(ui_aboutList, LV_SCROLL_SNAP_CENTER);
  lv_obj_set_scrollbar_mode(ui_aboutList, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_style_bg_color(ui_aboutList, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_aboutList, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_pad_top(ui_aboutList, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui_aboutList, 50, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *info;
  //
  info = lv_list_add_btn(ui_aboutList, &ui_img_watch_png, "DT78 ESP32\nv1.0");
  set_style(info);
  String about = "Mac Address\n" + String(macAddr);
  info = lv_list_add_btn(ui_aboutList, &ui_img_bluetooth_png, about.c_str());
  set_style(info);
  about = "Touch Screen\nv" + String(touch_data.version15, HEX) + " (" + String(touch_data.versionInfo[0], HEX) + "-" + String(touch_data.versionInfo[0], HEX) + "-" + String(touch_data.versionInfo[0], HEX) + ")";
  info = lv_list_add_btn(ui_aboutList, &ui_img_touch_png, about.c_str());
  set_style(info);
  about = "Accelerometer\nv" + String(imuID, HEX);
  info = lv_list_add_btn(ui_aboutList, &ui_img_accl_png, about.c_str());

  set_style(info);
}
