#include "main.h"

#include <lvgl.h>

#include <Arduino_GFX_Library.h>
#include <ChronosESP32.h>
// #include <CST816S.h>
#include <Timber.h>
#include <Preferences.h>
// #include "Button2.h"
#include "touch.h"
#include "kxtj3-1057.h"
#include "driver/rtc_io.h"
#include "ui/ui.h"

#include "faces/34_2_dial/34_2_dial.h"
#include "faces/75_2_dial/75_2_dial.h"
// #include "faces/79_2_dial/79_2_dial.h"
#include "faces/radar/radar.h"
// #include "faces/116_2_dial/116_2_dial.h"
#include "faces/756_2_dial/756_2_dial.h"
// #include "faces/tix_resized/tix_resized.h"
// #include "faces/pixel_resized/pixel_resized.h"
#include "faces/smart_resized/smart_resized.h"
#include "faces/kenya/kenya.h"
#include "faces/b_w_resized/b_w_resized.h"
// #include "faces/wfb_resized/wfb_resized.h"

Arduino_DataBus *bus = new Arduino_ESP32SPI(21 /* DC */, 22 /* CS */, 18 /* SCK */, 23 /* MOSI */, -1 /* MISO */, VSPI /* spi_num */);
Arduino_GC9A01 *gfx = new Arduino_GC9A01(bus, 19, 0 /* rotation */, true /* IPS */);

ChronosESP32 watch("DT78 ESP32");
// CST816S touch(2, 4, 15, 14); // sda, scl, rst, irq
KXTJ3 myIMU(0x0F); // Address can be 0x0E or 0x0F
Preferences prefs;

// Button2 button;
// Button2 button2;
// hw_timer_t *timer = __null;

/* Change to your screen resolution */
static uint32_t screenWidth;
static uint32_t screenHeight;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;
static lv_disp_drv_t disp_drv;

String days[7] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

const lv_img_dsc_t *notificationIcons[] = {
    &ui_img_sms_png,       // SMS
    &ui_img_mail_png,      // Mail
    &ui_img_penguin_png,   // Penguin
    &ui_img_skype_png,     // Skype
    &ui_img_whatsapp_png,  // WhatsApp
    &ui_img_mail_png,      // Mail2
    &ui_img_line_png,      // Line
    &ui_img_twitter_png,   // Twitter
    &ui_img_facebook_png,  // Facebook
    &ui_img_messenger_png, // Messenger
    &ui_img_instagram_png, // Instagram
    &ui_img_weibo_png,     // Weibo
    &ui_img_kakao_png,     // Kakao
    &ui_img_viber_png,     // Viber
    &ui_img_vkontakte_png, // Vkontakte
    &ui_img_telegram_png,  // Telegram
    &ui_img_chrns_png,     // Chronos
    &ui_img_wechat_png     // Wechat
};

const lv_img_dsc_t *weatherIcons[] = {
    &ui_img_602206286,
    &ui_img_602205261,
    &ui_img_602199888,
    &ui_img_602207311,
    &ui_img_dy4_png,
    &ui_img_602200913,
    &ui_img_602195540,
    &ui_img_602202963};

const lv_img_dsc_t *weatherNtIcons[] = {
    &ui_img_229834011, // assets\nt-0.png
    &ui_img_229835036, // assets\nt-1.png
    &ui_img_229827613, // assets\nt-2.png
    &ui_img_229828638, // assets\nt-3.png
    &ui_img_229838359, // assets\nt-4.png
    &ui_img_229839384, // assets\nt-5.png
    &ui_img_229831961, // assets\nt-6.png
    &ui_img_229832986, // assets\nt-7.png
};

struct Timer
{
  unsigned long time;
  long duration = 5000;
  bool active;
};

Timer screenTimer;
Timer alertTimer;
Timer searchTimer;

lv_event_t *click;
lv_obj_t *ui_cameraPanel;
lv_obj_t *ui_cameraTitle;
lv_obj_t *ui_cameraIcon;
lv_obj_t *ui_cameraLabel;
lv_obj_t *ui_cameraButton;
lv_obj_t *ui_cameraButtonLabel;

bool circular = false;

void showAlert();
bool isDay();
int getNotificationIconIndex(int id);
int getWeatherIconIndex(int id);
void cameraPanel();
void setTimeout(int i);

lv_obj_t *ui_faceSelect;
int wf = 0;

struct Face
{
  const char *name;
  const lv_img_dsc_t *preview;
  lv_obj_t **watchface;
};

Face face[] = {
    {.name = "Default", .preview = &digital_preview, .watchface = &ui_clockScreen},
    {.name = "Analog", .preview = &face_75_2_dial_dial_img_preview_0, .watchface = &face_75_2_dial},
    {.name = "Shadow", .preview = &face_34_2_dial_dial_img_preview_0, .watchface = &face_34_2_dial},
    // {.name = "Blue", .preview = &face_79_2_dial_dial_img_preview_0, .watchface = &face_79_2_dial},
    {.name = "Radar", .preview = &face_radar_dial_img_preview_0, .watchface = &face_radar},
    // {.name = "Outline", .preview = &face_116_2_dial_dial_img_preview_0, .watchface = &face_116_2_dial},
    {.name = "Red", .preview = &face_756_2_dial_dial_img_preview_0, .watchface = &face_756_2_dial},
    // {.name = "Tix", .preview = &face_tix_resized_dial_img_preview_0, .watchface = &face_tix_resized},
    // {.name = "Pixel", .preview = &face_pixel_resized_dial_img_preview_0, .watchface = &face_pixel_resized},
    {.name = "Smart", .preview = &face_smart_resized_dial_img_preview_0, .watchface = &face_smart_resized},
    {.name = "Kenya", .preview = &face_kenya_dial_img_preview_0, .watchface = &face_kenya},
    {.name = "B & W", .preview = &face_b_w_resized_dial_img_preview_0, .watchface = &face_b_w_resized},
    // {.name = "WFB", .preview = &face_wfb_resized_dial_img_preview_0, .watchface = &face_wfb_resized},
};

size_t faceSize = sizeof(face) / sizeof(face[0]);

void update_faces();

// void IRAM_ATTR onTimer()
// {
//   button.loop();
//   button2.loop();
// }

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
  lv_disp_flush_ready(disp);
}

void my_input_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
  get_read_touch();

  data->point.x = touch_data.xpos;
  data->point.y = touch_data.ypos;

  if (touch_data.touchpoints == 1 && touch_data.event == 2)
  {
    // if (screenOn)
    // {
    //   data->state = LV_INDEV_STATE_PR;
    //   // screenOn = true;
    //   onTime = millis();
    //   // Serial.println(touch_data.gesture);
    //   screenTimer.time = millis();
    //   screenTimer.active = true;
    // }
    // else
    // {
    //   data->state = LV_INDEV_STATE_REL;
    // }
    data->state = LV_INDEV_STATE_PR;
    screenTimer.time = millis();
    screenTimer.active = true;
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
  // return false; /*No buffering now so no more data read*/
}

void connectionCallback(bool state)
{
  Timber.d("Connection change");
  lv_obj_add_flag(ui_cameraPanel, LV_OBJ_FLAG_HIDDEN);
}

void notificationCallback(Notification notification)
{

  Timber.d("Notification Received from " + notification.app + " at " + notification.time);
  Timber.d(notification.message);
  onNotificationsOpen(click);
  showAlert();
  if (vibration)
  {
    digitalWrite(MOTOR, HIGH);
    delay(notifyTime);
    digitalWrite(MOTOR, LOW);
  }
}

void configCallback(Config config, uint32_t a, uint32_t b)
{
  switch (config)
  {
  case CF_RTW:
    autoScreen = a == 1;
    if (autoScreen)
    {
      lv_obj_add_state(ui_rtwSwitch, LV_STATE_CHECKED);
    }
    else
    {
      lv_obj_clear_state(ui_rtwSwitch, LV_STATE_CHECKED);
    }
    break;
  case CF_WEATHER:

    if (a)
    {
      lv_label_set_text_fmt(ui_weatherTemp, "%d°C", watch.getWeatherAt(0).temp);
      // set icon ui_weatherIcon
      if (isDay())
      {
        lv_img_set_src(ui_weatherIcon, weatherIcons[getWeatherIconIndex(watch.getWeatherAt(0).icon)]);
      }
      else
      {
        lv_img_set_src(ui_weatherIcon, weatherNtIcons[getWeatherIconIndex(watch.getWeatherAt(0).icon)]);
      }
    }

    break;
  case CF_FONT:
    screenTimer.time = millis();
    screenTimer.active = true;
    if (((b >> 16) & 0xFFFF) == 0x01)
    { // Style 1
      if ((b & 0xFFFF) == 0x01)
      { // TOP
        lv_obj_set_style_text_color(ui_hourLabel, lv_color_hex(a), LV_PART_MAIN | LV_STATE_DEFAULT);
      }
      if ((b & 0xFFFF) == 0x02)
      { // CENTER
        lv_obj_set_style_text_color(ui_minuteLabel, lv_color_hex(a), LV_PART_MAIN | LV_STATE_DEFAULT);
      }
      if ((b & 0xFFFF) == 0x03)
      { // BOTTOM
        lv_obj_set_style_text_color(ui_dayLabel, lv_color_hex(a), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(ui_dateLabel, lv_color_hex(a), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(ui_weatherTemp, lv_color_hex(a), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(ui_amPmLabel, lv_color_hex(a), LV_PART_MAIN | LV_STATE_DEFAULT);
      }
    }

    break;
  case CF_CAMERA:
    if (b)
    {
      lv_disp_load_scr(ui_home);
      lv_obj_clear_flag(ui_cameraPanel, LV_OBJ_FLAG_HIDDEN);
      screenTimer.time = millis() + 50;
      screenTimer.active = true;
    }
    else
    {
      lv_obj_add_flag(ui_cameraPanel, LV_OBJ_FLAG_HIDDEN);
      screenTimer.active = true;
    }
    break;
  }
}

int getWeatherIconIndex(int id)
{
  switch (id)
  {
  case 0:
    return 0;
  case 1:
    return 1;
  case 2:
    return 2;
  case 3:
    return 3;
  case 4:
    return 4;
  case 5:
    return 5;
  case 6:
    return 6;
  case 7:
    return 7;
  default:
    return 0;
  }
}

int getNotificationIconIndex(int id)
{
  switch (id)
  {
  case 0x03:
    return 0;
  case 0x04:
    return 1;
  case 0x07:
    return 2;
  case 0x08:
    return 3;
  case 0x0A:
    return 4;
  case 0x0B:
    return 5;
  case 0x0E:
    return 6;
  case 0x0F:
    return 7;
  case 0x10:
    return 8;
  case 0x11:
    return 9;
  case 0x12:
    return 10;
  case 0x13:
    return 11;
  case 0x14:
    return 12;
  case 0x16:
    return 13;
  case 0x17:
    return 14;
  case 0x18:
    return 15;
  case 0xC0:
    return 16;
  case 0x09:
    return 17;
  default:
    return 0;
  }
}

static void onScroll(lv_event_t *e)
{
  lv_obj_t *list = lv_event_get_target(e);

  lv_area_t list_a;
  lv_obj_get_coords(list, &list_a);
  lv_coord_t list_y_center = list_a.y1 + lv_area_get_height(&list_a) / 2;

  lv_coord_t r = lv_obj_get_height(list) * 7 / 10;
  uint32_t i;
  uint32_t child_cnt = lv_obj_get_child_cnt(list);
  for (i = 0; i < child_cnt; i++)
  {
    lv_obj_t *child = lv_obj_get_child(list, i);
    lv_area_t child_a;
    lv_obj_get_coords(child, &child_a);

    lv_coord_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2;

    lv_coord_t diff_y = child_y_center - list_y_center;
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
    lv_obj_set_style_translate_x(child, circular ? x : 0, 0);

    /*Use some opacity with larger translations*/
    // lv_opa_t opa = lv_map(x, 0, r, LV_OPA_TRANSP, LV_OPA_COVER);
    // lv_obj_set_style_opa(child, LV_OPA_COVER - opa, 0);
  }
}

void onMessageClick(lv_event_t *e)
{
  // Your code here
  int index = (int)lv_event_get_user_data(e);

  index %= NOTIF_SIZE;
  Timber.i("Message clicked at index %d", index);

  lv_label_set_text(ui_messageTime, watch.getNotificationAt(index).time.c_str());
  lv_label_set_text(ui_messageContent, watch.getNotificationAt(index).message.c_str());
  lv_img_set_src(ui_messageIcon, notificationIcons[getNotificationIconIndex(watch.getNotificationAt(index).icon)]);

  lv_obj_scroll_to_y(ui_messagePanel, 0, LV_ANIM_ON);
  lv_obj_add_flag(ui_messageList, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(ui_messagePanel, LV_OBJ_FLAG_HIDDEN);
}

void addNotificationList(lv_obj_t *parent, Notification notif, int index)
{
  lv_obj_t *notificationItem = lv_obj_create(parent);
  lv_obj_set_width(notificationItem, 200);
  lv_obj_set_height(notificationItem, LV_SIZE_CONTENT); /// 50
  lv_obj_set_align(notificationItem, LV_ALIGN_CENTER);
  lv_obj_set_flex_flow(notificationItem, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(notificationItem, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
  lv_obj_clear_flag(notificationItem, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_radius(notificationItem, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(notificationItem, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(notificationItem, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(notificationItem, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_opa(notificationItem, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(notificationItem, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_side(notificationItem, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(notificationItem, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(notificationItem, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(notificationItem, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(notificationItem, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *notificationIcon = lv_img_create(notificationItem);
  lv_img_set_src(notificationIcon, notificationIcons[getNotificationIconIndex(notif.icon)]);
  lv_obj_set_width(notificationIcon, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(notificationIcon, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(notificationIcon, LV_ALIGN_CENTER);
  lv_obj_add_flag(notificationIcon, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
  lv_obj_clear_flag(notificationIcon, LV_OBJ_FLAG_SCROLLABLE); /// Flags

  lv_obj_t *notificationText = lv_label_create(notificationItem);
  lv_obj_set_width(notificationText, 140);
  lv_obj_set_height(notificationText, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(notificationText, LV_ALIGN_CENTER);
  lv_label_set_long_mode(notificationText, LV_LABEL_LONG_DOT);
  lv_label_set_text(notificationText, notif.message.c_str());

  lv_obj_add_event_cb(notificationItem, onMessageClick, LV_EVENT_CLICKED, (void *)index);
}

void addForecast(Weather weather)
{
  lv_obj_t *forecastItem = lv_obj_create(ui_forecastList);
  lv_obj_set_width(forecastItem, 200);
  lv_obj_set_height(forecastItem, 40);
  lv_obj_set_align(forecastItem, LV_ALIGN_CENTER);
  lv_obj_clear_flag(forecastItem, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_radius(forecastItem, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(forecastItem, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(forecastItem, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(forecastItem, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_opa(forecastItem, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(forecastItem, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_side(forecastItem, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(forecastItem, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(forecastItem, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(forecastItem, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(forecastItem, 10, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *forecastIcon = lv_img_create(forecastItem);
  lv_img_set_src(forecastIcon, weatherIcons[getWeatherIconIndex(weather.icon)]);
  lv_obj_set_width(forecastIcon, LV_SIZE_CONTENT);  /// 40
  lv_obj_set_height(forecastIcon, LV_SIZE_CONTENT); /// 32
  lv_obj_set_align(forecastIcon, LV_ALIGN_CENTER);
  lv_obj_add_flag(forecastIcon, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
  lv_obj_clear_flag(forecastIcon, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_img_set_zoom(forecastIcon, 150);

  lv_obj_t *forecastTemp = lv_label_create(forecastItem);
  lv_obj_set_width(forecastTemp, 58);
  lv_obj_set_height(forecastTemp, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(forecastTemp, 68);
  lv_obj_set_y(forecastTemp, 3);
  lv_obj_set_align(forecastTemp, LV_ALIGN_CENTER);
  lv_label_set_long_mode(forecastTemp, LV_LABEL_LONG_CLIP);
  lv_label_set_text_fmt(forecastTemp, "%d°C", weather.temp);
  lv_obj_set_style_text_font(forecastTemp, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *forecastDay = lv_label_create(forecastItem);
  lv_obj_set_width(forecastDay, 68);
  lv_obj_set_height(forecastDay, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(forecastDay, -56);
  lv_obj_set_y(forecastDay, 3);
  lv_obj_set_align(forecastDay, LV_ALIGN_CENTER);
  lv_label_set_long_mode(forecastDay, LV_LABEL_LONG_CLIP);
  lv_label_set_text(forecastDay, (days[weather.day]).c_str());
  lv_obj_set_style_text_font(forecastDay, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void watchfaceEvents(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);
  if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT)
  {
    lv_scr_load_anim(ui_notificationScreen, LV_SCR_LOAD_ANIM_OVER_RIGHT, 500, 0, false);
  }
  if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT)
  {
    lv_scr_load_anim(ui_settingsScreen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0, false);
  }
  if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_BOTTOM)
  {
    lv_scr_load_anim(ui_controlScreen, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 500, 0, false);
  }
  if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_TOP)
  {
    lv_scr_load_anim(ui_weatherScreen, LV_SCR_LOAD_ANIM_MOVE_TOP, 500, 0, false);
  }
  if (event_code == LV_EVENT_LONG_PRESSED)
  {
    // ui_home = ui_clockScreen;
    lv_scr_load_anim(ui_faceSelect, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, false);
    if ( wf >= lv_obj_get_child_cnt(ui_faceSelect)){
      wf = 0;
    }
    lv_obj_scroll_to_view(lv_obj_get_child(ui_faceSelect, wf), LV_ANIM_ON);
  }
}

void onWatchfaceChange(lv_event_t *e)
{
  watchfaceEvents(e);
}

void onFaceEvent(lv_event_t *e)
{
  watchfaceEvents(e);
}

void onFaceSelected(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);
  int index = (int)lv_event_get_user_data(e);
  
  if (event_code == LV_EVENT_CLICKED)
  {
    ui_home = *face[index].watchface;
    lv_scr_load_anim(ui_home, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, false);
    wf = index;
    prefs.putInt("watchface", index);
  }
}

void addWatchface(const char *name, const lv_img_dsc_t *src, int index)
{

  lv_obj_t *ui_faceItem = lv_obj_create(ui_faceSelect);
  lv_obj_set_width(ui_faceItem, 160);
  lv_obj_set_height(ui_faceItem, 180);
  lv_obj_set_align(ui_faceItem, LV_ALIGN_CENTER);
  lv_obj_clear_flag(ui_faceItem, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_radius(ui_faceItem, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(ui_faceItem, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_faceItem, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_faceItem, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_color(ui_faceItem, lv_color_hex(0x142ABC), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_faceItem, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_width(ui_faceItem, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(ui_faceItem, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(ui_faceItem, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(ui_faceItem, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(ui_faceItem, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui_faceItem, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *ui_facePreview = lv_img_create(ui_faceItem);
  lv_img_set_src(ui_facePreview, src);
  lv_obj_set_width(ui_facePreview, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_facePreview, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_facePreview, LV_ALIGN_TOP_MID);
  lv_obj_add_flag(ui_facePreview, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
  lv_obj_clear_flag(ui_facePreview, LV_OBJ_FLAG_SCROLLABLE); /// Flags

  lv_obj_t *ui_faceLabel = lv_label_create(ui_faceItem);
  lv_obj_set_width(ui_faceLabel, 160);
  lv_obj_set_height(ui_faceLabel, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_faceLabel, LV_ALIGN_BOTTOM_MID);
  lv_label_set_long_mode(ui_faceLabel, LV_LABEL_LONG_DOT);
  lv_label_set_text(ui_faceLabel, name);
  lv_obj_set_style_text_align(ui_faceLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_faceLabel, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_event_cb(ui_faceItem, onFaceSelected, LV_EVENT_ALL, (void *)index);
}

void init_face_select()
{
  ui_faceSelect = lv_obj_create(NULL);
  lv_obj_set_width(ui_faceSelect, 240);
  lv_obj_set_height(ui_faceSelect, 240);
  lv_obj_set_align(ui_faceSelect, LV_ALIGN_CENTER);
  lv_obj_set_flex_flow(ui_faceSelect, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(ui_faceSelect, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(ui_faceSelect, LV_OBJ_FLAG_SNAPPABLE); /// Flags
  lv_obj_set_scrollbar_mode(ui_faceSelect, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_style_radius(ui_faceSelect, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(ui_faceSelect, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_faceSelect, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_faceSelect, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(ui_faceSelect, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(ui_faceSelect, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(ui_faceSelect, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui_faceSelect, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_row(ui_faceSelect, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_column(ui_faceSelect, 15, LV_PART_MAIN | LV_STATE_DEFAULT);

  for (int x = 0; x < faceSize; x++)
  {
    addWatchface(face[x].name, face[x].preview, x);
  }
}

void onCaptureClick(lv_event_t *e)
{
  watch.capturePhoto();
}

void cameraPanel()
{
  ui_cameraPanel = lv_obj_create(ui_home);
  lv_obj_set_width(ui_cameraPanel, 240);
  lv_obj_set_height(ui_cameraPanel, 240);
  lv_obj_set_align(ui_cameraPanel, LV_ALIGN_CENTER);
  lv_obj_add_flag(ui_cameraPanel, LV_OBJ_FLAG_HIDDEN);                                    /// Flags
  lv_obj_clear_flag(ui_cameraPanel, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_bg_color(ui_cameraPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_cameraPanel, 235, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_cameraPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(ui_cameraPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(ui_cameraPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(ui_cameraPanel, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui_cameraPanel, 10, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_cameraTitle = lv_label_create(ui_cameraPanel);
  lv_obj_set_width(ui_cameraTitle, 150);
  lv_obj_set_height(ui_cameraTitle, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_cameraTitle, 0);
  lv_obj_set_y(ui_cameraTitle, 16);
  lv_obj_set_align(ui_cameraTitle, LV_ALIGN_TOP_MID);
  lv_label_set_text(ui_cameraTitle, "Camera");
  lv_obj_set_style_text_align(ui_cameraTitle, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_cameraTitle, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(ui_cameraTitle, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_opa(ui_cameraTitle, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_cameraTitle, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_side(ui_cameraTitle, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(ui_cameraTitle, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(ui_cameraTitle, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(ui_cameraTitle, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui_cameraTitle, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_cameraIcon = lv_img_create(ui_cameraPanel);
  lv_img_set_src(ui_cameraIcon, &ui_img_camera_png);
  lv_obj_set_width(ui_cameraIcon, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_cameraIcon, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_cameraIcon, -79);
  lv_obj_set_y(ui_cameraIcon, -5);
  lv_obj_set_align(ui_cameraIcon, LV_ALIGN_CENTER);
  lv_obj_add_flag(ui_cameraIcon, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
  lv_obj_clear_flag(ui_cameraIcon, LV_OBJ_FLAG_SCROLLABLE); /// Flags

  ui_cameraLabel = lv_label_create(ui_cameraPanel);
  lv_obj_set_width(ui_cameraLabel, 160);
  lv_obj_set_height(ui_cameraLabel, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_cameraLabel, 28);
  lv_obj_set_y(ui_cameraLabel, -6);
  lv_obj_set_align(ui_cameraLabel, LV_ALIGN_CENTER);
  lv_label_set_text(ui_cameraLabel, "To close the dialog, exit the camera from the app");
  lv_obj_set_style_text_font(ui_cameraLabel, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_cameraButton = lv_btn_create(ui_cameraPanel);
  lv_obj_set_width(ui_cameraButton, 108);
  lv_obj_set_height(ui_cameraButton, 40);
  lv_obj_set_x(ui_cameraButton, 0);
  lv_obj_set_y(ui_cameraButton, 62);
  lv_obj_set_align(ui_cameraButton, LV_ALIGN_CENTER);
  lv_obj_add_flag(ui_cameraButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
  lv_obj_clear_flag(ui_cameraButton, LV_OBJ_FLAG_SCROLLABLE);    /// Flags
  lv_obj_add_event_cb(ui_cameraButton, onCaptureClick, LV_EVENT_CLICKED, NULL);

  ui_cameraButtonLabel = lv_label_create(ui_cameraButton);
  lv_obj_set_width(ui_cameraButtonLabel, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_cameraButtonLabel, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_cameraButtonLabel, LV_ALIGN_CENTER);
  lv_label_set_text(ui_cameraButtonLabel, "Capture");
}

void onForecastOpen(lv_event_t *e)
{
  // lv_obj_scroll_to_y(ui_forecastPanel, 0, LV_ANIM_ON);
}

void onScrollMode(lv_event_t *e)
{
  lv_obj_t *obj = lv_event_get_target(e);
  circular = lv_obj_has_state(obj, LV_STATE_CHECKED);
  lv_obj_scroll_by(ui_settingsList, 0, circular ? 1 : -1, LV_ANIM_ON);

  prefs.putBool("circular", circular);
}

void onNotificationsOpen(lv_event_t *e)
{
  lv_obj_clean(ui_messageList);
  int c = watch.getNotificationCount();
  for (int i = 0; i < c; i++)
  {
    addNotificationList(ui_messageList, watch.getNotificationAt(i), i);
  }
  lv_obj_scroll_to_y(ui_messageList, 1, LV_ANIM_ON);
  lv_obj_clear_flag(ui_messageList, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(ui_messagePanel, LV_OBJ_FLAG_HIDDEN);
}

void onWeatherLoad(lv_event_t *e)
{
  if (isDay())
  {
    lv_obj_set_style_bg_img_src(ui_weatherScreen, &ui_img_857483832, LV_PART_MAIN | LV_STATE_DEFAULT);
  }
  else
  {
    lv_obj_set_style_bg_img_src(ui_weatherScreen, &ui_img_753022056, LV_PART_MAIN | LV_STATE_DEFAULT);
  }
  lv_obj_clear_flag(ui_weatherPanel, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(ui_forecastPanel, LV_OBJ_FLAG_HIDDEN);
  if (watch.getWeatherCount() > 0)
  {
    String updateTime = "Updated at\n" + watch.getWeatherTime();
    lv_label_set_text(ui_weatherCity, watch.getWeatherCity().c_str());
    lv_label_set_text(ui_weatherUpdateTime, updateTime.c_str());
    lv_label_set_text_fmt(ui_weatherCurrentTemp, "%d°C", watch.getWeatherAt(0).temp);
    if (isDay())
    {
      lv_img_set_src(ui_weatherCurrentIcon, weatherIcons[getWeatherIconIndex(watch.getWeatherAt(0).icon)]);
    }
    else
    {
      lv_img_set_src(ui_weatherCurrentIcon, weatherNtIcons[getWeatherIconIndex(watch.getWeatherAt(0).icon)]);
    }

    // lv_obj_clean(ui_forecastList);
    // int c = watch.getWeatherCount();
    // for (int i = 0; i < c; i++)
    // {
    //   addForecast(watch.getWeatherAt(i));
    // }
  }
}

void onLoadHome(lv_event_t *e)
{
  // if (isDay())
  // {
  //   lv_obj_set_style_bg_img_src( ui_clockScreen, &ui_img_857483832, LV_PART_MAIN | LV_STATE_DEFAULT);
  // }
  // else
  // {
  //   lv_obj_set_style_bg_img_src( ui_clockScreen, &ui_img_753022056, LV_PART_MAIN | LV_STATE_DEFAULT);
  // }
}

void onBrightnessChange(lv_event_t *e)
{
  // Your code here
  lv_obj_t *slider = lv_event_get_target(e);
  int v = lv_slider_get_value(slider);
  // tft.setBrightness(v);
  ledcWrite(ledChannel, v);

  prefs.putInt("brightness", v);
}

void onBatteryChange(lv_event_t *e)
{
  uint8_t lvl = lv_slider_get_value(ui_batterySlider);
  watch.setBattery(lvl);
}

void onStartSearch(lv_event_t *e)
{
  watch.findPhone(true);
}

void onEndSearch(lv_event_t *e)
{
  watch.findPhone(false);
}
void onVolumeUp(lv_event_t *e)
{
  watch.musicControl(VOLUME_UP);
}
void onVolumeDown(lv_event_t *e)
{
  watch.musicControl(VOLUME_DOWN);
}
void onSleepButton(lv_event_t *e)
{
  light_sleep();
}
void onPowerButton(lv_event_t *e)
{
  deep_sleep();
}
void onVibrationMode(lv_event_t *e)
{
  lv_obj_t *obj = lv_event_get_target(e);
  vibration = lv_obj_has_state(obj, LV_STATE_CHECKED);
  if (vibration)
  {
    digitalWrite(MOTOR, HIGH);
    delay(notifyTime);
    digitalWrite(MOTOR, LOW);
  }

  prefs.putBool("vibration", vibration);
}
void onRTWMode(lv_event_t *e)
{
  lv_obj_t *obj = lv_event_get_target(e);
  autoScreen = lv_obj_has_state(obj, LV_STATE_CHECKED);

  prefs.putBool("rtw", autoScreen);
}

void onClickAlert(lv_event_t *e)
{

  // cancel alert timer
  alertTimer.active = false;
  // change screen to notifications
  lv_disp_load_scr(ui_notificationScreen);

  // enable screen for timeout + 5 seconds
  screenTimer.time = millis() + 5000;
  screenTimer.active = true;

  // load the last received message
  lv_label_set_text(ui_messageTime, watch.getNotificationAt(0).time.c_str());
  lv_label_set_text(ui_messageContent, watch.getNotificationAt(0).message.c_str());
  lv_img_set_src(ui_messageIcon, notificationIcons[getNotificationIconIndex(watch.getNotificationAt(0).icon)]);

  lv_obj_scroll_to_y(ui_messagePanel, 0, LV_ANIM_ON);
  lv_obj_add_flag(ui_messageList, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(ui_messagePanel, LV_OBJ_FLAG_HIDDEN);
}

void onTimeoutChange(lv_event_t *e)
{
  lv_obj_t *obj = lv_event_get_target(e);
  uint16_t sel = lv_dropdown_get_selected(obj);
  Timber.i("Selected index: %d", sel);

  setTimeout(sel);
  prefs.putInt("timeout", sel);
}

void setTimeout(int i)
{
  if (i == 4)
  {
    screenTimer.duration = -1; // always on
  }
  else if (i == 0)
  {
    screenTimer.duration = 5000; // 5 seconds
    screenTimer.active = true;
  }
  else if (i < 4)
  {
    screenTimer.duration = 10000 * i; // 10, 20, 30 seconds
    screenTimer.active = true;
  }
}

void onMusicPlay(lv_event_t *e)
{
  watch.musicControl(MUSIC_TOGGLE);
}

void onMusicPrevious(lv_event_t *e)
{
  watch.musicControl(MUSIC_PREVIOUS);
}

void onMusicNext(lv_event_t *e)
{
  watch.musicControl(MUSIC_NEXT);
}

void showAlert()
{
  lv_disp_t *display = lv_disp_get_default();
  lv_obj_t *actScr = lv_disp_get_scr_act(display);
  if (actScr == ui_notificationScreen)
  {
    // at notifications screen, switch to message
    // enable screen for timeout + 5 seconds
    screenTimer.time = millis() + 5000;
    screenTimer.active = true;

    // load the last received message
    lv_label_set_text(ui_messageTime, watch.getNotificationAt(0).time.c_str());
    lv_label_set_text(ui_messageContent, watch.getNotificationAt(0).message.c_str());
    lv_img_set_src(ui_messageIcon, notificationIcons[getNotificationIconIndex(watch.getNotificationAt(0).icon)]);

    lv_obj_scroll_to_y(ui_messagePanel, 0, LV_ANIM_ON);
    lv_obj_add_flag(ui_messageList, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_messagePanel, LV_OBJ_FLAG_HIDDEN);
  }
  else
  {
    // attach the alert panel to current screen
    lv_obj_set_parent(ui_alertPanel, actScr);

    // load the last received message
    lv_label_set_text(ui_alertText, watch.getNotificationAt(0).message.c_str());
    lv_img_set_src(ui_alertIcon, notificationIcons[getNotificationIconIndex(watch.getNotificationAt(0).icon)]);

    // turn screen for timeout + 5 seconds
    screenTimer.time = millis() + 5000;
    screenTimer.active = true;

    alertTimer.time = millis();
    alertTimer.active = true;

    // show the alert
    lv_obj_clear_flag(ui_alertPanel, LV_OBJ_FLAG_HIDDEN);
  }
}

void logCallback(Level level, unsigned long time, String message)
{
  Serial.print(message);
}

void checkButton1()
{
  button1State = digitalRead(BT1); // read the button state on the pin "BUTTON_PIN"

  if (button1State)
  {
    startPress1 = millis();
    //    Serial.print("HIGH - ");
    // button was pressed, currently high
    // if (actScr == ui_testScreen)
    // {
    //   lv_arc_set_value(ui_buttonDown, 1);
    // }
  }
  else
  {
    long dur = millis() - startPress1;

    Serial.print("BTN1: ");
    Serial.println(dur);
    // if (actScr == ui_testScreen)
    // {
    //   lv_arc_set_value(ui_buttonDown, 0);
    // }

    if (dur > DEBOUNCE)
    {
      if (screenTimer.active)
      {
        lv_disp_t *display = lv_disp_get_default();
        lv_obj_t *actScr = lv_disp_get_scr_act(display);
        if (actScr == ui_home)
        {
          // turn off screen
          screenTimer.time = millis() - screenTimer.duration - 100;
        }
        // else if (actScr != ui_menuScreen)
        // {
        //   load_screen(1);
        // }
        else
        {
          lv_disp_load_scr(ui_home);
        }
      }
      else
      {
        screenTimer.time = millis();
        screenTimer.active = true;
      }
    }
  }
}

void checkButton2()
{

  button2State = digitalRead(BT2); // read the button state on the pin "BUTTON_PIN"

  if (button2State)
  {
    startPress2 = millis();
  }
  else
  {
    long dur = millis() - startPress2;

    Serial.print("BTN2: ");
    Serial.println(dur);

    if (dur > DEBOUNCE)
    {
      if (screenTimer.active)
      {
        lv_disp_t *display = lv_disp_get_default();
        lv_obj_t *actScr = lv_disp_get_scr_act(display);
        if (actScr == ui_home)
        {
          // screenOn = false;
          // turn off screen
          screenTimer.time = millis() - screenTimer.duration - 100;
        }
        else
        {
          lv_disp_load_scr(ui_home);
        }
      }
      else
      {
        screenTimer.time = millis();
        screenTimer.active = true;
      }
    }
  }
}

void IRAM_ATTR screenInt()
{
  screenTimer.time = millis();
  screenTimer.active = true;
}

void accInt()
{
  int v = digitalRead(TINT);
  // Serial.print("Touch interrupt with level: ");
  // Serial.println(v);
  screenTimer.time = millis();
  screenTimer.active = true;
}

void mpuInt()
{
  int v = digitalRead(MPU_INT);
  Serial.print("MPU interrupt with level: ");
  Serial.println(v);
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
    // rtc.setTime(1655998793); // set the time if not coming from deep sleep ie reset or power up
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
  // Detach LED channel from the pin
  ledcDetachPin(ledPin);
  // Set the pin to LOW
  digitalWrite(ledPin, LOW);
  // Enable deep sleep hold
  gpio_deep_sleep_hold_en();

  rtc_gpio_pullup_en(GPIO_NUM_32);

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_32, 0); // 1 = High, 0 = Low

  // Go to sleep now
  Serial.println("Going to deep sleep now");
  esp_deep_sleep_start();
}

void light_sleep()
{

  screenTimer.time = millis();
  screenTimer.active = true;
  sleep_touch(true);
  digitalWrite(MOTOR, LOW);
  ledcWrite(ledChannel, 0);
  myIMU.standby(true);
  rtc_gpio_pullup_en(GPIO_NUM_33);

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0); // 1 = High, 0 = Low

  // Go to sleep now
  Serial.println("Going to light sleep now");
  delay(100);
  esp_light_sleep_start();
}

// void handler(Button2 &btn)
// {
//   // Serial.println("click");
//   if (btn == button)
//   {
//     Serial.println("Top Button");
//   }

//   if (btn == button2)
//   {
//     Serial.println("Bottom Button");
//   }
//   switch (btn.getType())
//   {
//   case single_click:
//     Serial.print("single ");
//     break;
//   case double_click:
//     Serial.print("double ");
//     break;
//   case triple_click:
//     Serial.print("triple ");
//     break;
//   case long_click:
//     Serial.print("long");
//     break;
//   }

//   Serial.print(" (");
//   Serial.print(btn.getNumberOfClicks());
//   Serial.println(")");
// }

void power_save()
{
  digitalWrite(MOTOR, LOW);
  // ledcWrite(ledChannel, 0);
  // sleep_touch(true);
  myIMU.standby(true);
}

void setup()
{
  Serial.begin(115200);

  Timber.setLogCallback(logCallback);

  Timber.i("Starting up device");

  wakeup_reason();

  prefs.begin("my-app");

  gfx->begin();
  gfx->fillScreen(BLACK);

  init_i2c();
  init_touch();
  pinMode(TINT, INPUT);
  attachInterrupt(TINT, accInt, CHANGE);
  pinMode(MPU_INT, INPUT);
  // attachInterrupt(MPU_INT, mpuInt, CHANGE);

  lv_init();

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

    ui_init();

    init_face_34_2_dial();
    init_face_75_2_dial();
    // init_face_79_2_dial();
    init_face_radar();
    // init_face_116_2_dial();
    init_face_756_2_dial();
    init_face_smart_resized();
    // init_face_tix_resized();
    // init_face_pixel_resized();
    init_face_kenya();
    init_face_b_w_resized();
    // init_face_wfb_resized();

    init_face_select();

    wf = prefs.getInt("watchface", 0);
    if (wf >= faceSize)
    {
      wf = 0; // default
    }
    ui_home = *face[wf].watchface; // load saved watchface power on
    lv_disp_load_scr(ui_home);
    

    Serial.println("Setup done");
  }

  watch.setConnectionCallback(connectionCallback);
  watch.setNotificationCallback(notificationCallback);
  watch.setConfigurationCallback(configCallback);
  watch.begin();
  watch.set24Hour(true);
  watch.setBattery(85);

  // onTime = millis();
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
  // attachInterrupt(BT1, checkButton1, CHANGE);
  pinMode(BT2, INPUT_PULLUP);
  // attachInterrupt(BT2, checkButton2, CHANGE);

  // button.begin(BT1);
  // button.setClickHandler(handler);
  // button.setDoubleClickHandler(handler);
  // button.setTripleClickHandler(handler);

  // button2.begin(BT2);
  // button2.setClickHandler(handler);
  // button2.setDoubleClickHandler(handler);
  // button2.setTripleClickHandler(handler);

  // timer = timerBegin(0, 80, true);
  // timerAttachInterrupt(timer, &onTimer, true);
  // timerAlarmWrite(timer, 10000, true); // every 1 seconds
  // timerAlarmEnable(timer);

  ledcSetup(ledChannel, freq, resolution);
  // attach the channel
  ledcAttachPin(ledPin, ledChannel);
  ledcWrite(ledChannel, 100);

  String about = "v2.0 [fbiego]\nDT78 Watch\n" + watch.getAddress();
  lv_label_set_text(ui_aboutText, about.c_str());

  lv_obj_add_event_cb(ui_settingsList, onScroll, LV_EVENT_SCROLL, NULL);
  lv_obj_add_event_cb(ui_messageList, onScroll, LV_EVENT_SCROLL, NULL);
  lv_obj_scroll_to_y(ui_settingsList, 1, LV_ANIM_ON);

  bool intro = prefs.getBool("intro", true);

  if (intro)
  {
    showAlert();
    prefs.putBool("intro", false);
  }
  else
  {
    lv_obj_add_flag(ui_alertPanel, LV_OBJ_FLAG_HIDDEN);
  }

  cameraPanel();

  // load saved preferences
  int tm = prefs.getInt("timeout", 0);
  int br = prefs.getInt("brightness", 100);
  circular = prefs.getBool("circular", false);
  vibration = prefs.getBool("vibration", vibration);
  autoScreen = prefs.getBool("rtw", autoScreen);

  if (tm > 4)
  {
    tm = 4;
  }
  else if (tm < 0)
  {
    tm = 0;
  }

  ledcWrite(ledChannel, br);

  lv_dropdown_set_selected(ui_timeoutSelect, tm);
  lv_slider_set_value(ui_brightnessSlider, br, LV_ANIM_OFF);
  if (circular)
  {
    lv_obj_add_state(ui_Switch2, LV_STATE_CHECKED);
  }
  else
  {
    lv_obj_clear_state(ui_Switch2, LV_STATE_CHECKED);
  }
  if (autoScreen)
  {
    lv_obj_add_state(ui_rtwSwitch, LV_STATE_CHECKED);
  }
  else
  {
    lv_obj_clear_state(ui_rtwSwitch, LV_STATE_CHECKED);
  }
  if (vibration)
  {
    lv_obj_add_state(ui_vibrationSwitch, LV_STATE_CHECKED);
  }
  else
  {
    lv_obj_clear_state(ui_vibrationSwitch, LV_STATE_CHECKED);
  }

  screenTimer.active = true;
  screenTimer.time = millis();

  setTimeout(tm);

  Timber.i("Setup done");
  Timber.i(about);
}

void loop()
{

  lv_timer_handler(); /* let the GUI do its work */
  delay(5);

  if (screenTimer.active)
  {
    lv_disp_t *display = lv_disp_get_default();
    lv_obj_t *actScr = lv_disp_get_scr_act(display);
    if (actScr == ui_settingsScreen)
    {
      get_IMU();

      lv_bar_set_value(ui_accXBar, map((accX * 100), -100, 100, 0, 100), LV_ANIM_ON);
      lv_bar_set_value(ui_accYBar, map((accY * 100), -100, 100, 0, 100), LV_ANIM_ON);
      lv_bar_set_value(ui_accZBar, map((accZ * 100), -100, 100, 0, 100), LV_ANIM_ON);
    }
  }

  // vector = sqrt((accX * accX) + (accY * accY) + (accZ * accZ));

  // samples[x] = vector;
  // x++;
  // if (x >= SAMPLES)
  // {
  //   x = 0;
  //   int st = calculate_steps();
  //   // lv_label_set_text(ui_Label9, String(st).c_str());
  //   curSteps += st;
  // }
  watch.loop();
  // button.loop();
  // button2.loop();

  if (ui_home == ui_clockScreen)
  {
    if (!watch.isConnected())
    {
      lv_obj_add_state(ui_btStateButton, LV_STATE_CHECKED);
    }
    else
    {
      lv_obj_clear_state(ui_btStateButton, LV_STATE_CHECKED);
    }
    lv_label_set_text(ui_hourLabel, watch.getHourZ().c_str());
    lv_label_set_text(ui_dayLabel, watch.getTime("%A").c_str());
    lv_label_set_text(ui_minuteLabel, watch.getTime("%M").c_str());
    lv_label_set_text(ui_dateLabel, watch.getTime("%d\n%B").c_str());
    lv_label_set_text(ui_amPmLabel, watch.getAmPmC(false).c_str());
  }
  else
  {
    update_faces();
  }

  if (alertTimer.active)
  {
    if (alertTimer.time + alertTimer.duration < millis())
    {
      alertTimer.active = false;
      lv_obj_add_flag(ui_alertPanel, LV_OBJ_FLAG_HIDDEN);
    }
  }

  if (screenTimer.active)
  {
    uint8_t lvl = lv_slider_get_value(ui_brightnessSlider);
    ledcWrite(ledChannel, lvl);

    if (screenTimer.duration < 0)
    {
      // Timber.w("Always On active");
      screenTimer.active = false;
    }
    else if (watch.isCameraReady())
    {
      screenTimer.active = false;
    }
    else if (screenTimer.time + screenTimer.duration < millis())
    {
      Timber.w("Screen timeout");
      screenTimer.active = false;

      ledcWrite(ledChannel, 0);
      lv_disp_load_scr(ui_home);

      // light_sleep();
    }
  }
  else
  {
    // get_IMU();
  }

  if (autoScreen)
  {
    get_IMU();
    if (accX > 0.3 && accX < 1.0 && accY > -0.3 && accY < 0.3 && accZ > -1.0 && accZ < -0.7)
    {
      // screenOn = true;
      screenTimer.time = millis();
      screenTimer.active = true;
    }
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

bool isDay()
{
  return watch.getHour(true) > 7 && watch.getHour(true) < 21;
}

void update_faces()
{
  int sec = watch.getSecond();
  int min = watch.getMinute();
  int hr = watch.getHourC();
  bool md = watch.is24Hour();
  bool am = watch.getHour(true) < 12;
  int dy = watch.getDay();
  int mt = watch.getMonth() + 1;
  int yr = watch.getYear();
  int wk = watch.getDayofWeek();

  int temp = watch.getWeatherAt(0).temp;
  int ic = watch.getWeatherAt(0).icon;

  int bat = watch.getPhoneBattery();
  int con = watch.isConnected();

  if (ui_home == face_34_2_dial){
    update_time_34_2_dial(sec, min, hr, md, am, dy, mt, yr, wk);
    update_weather_34_2_dial(temp, ic);
    update_status_34_2_dial(bat, con);
    update_activity_34_2_dial(2735, 357, 345);
    update_health_34_2_dial(76, 97);
  }

  if (ui_home == face_75_2_dial)
  {
    update_time_75_2_dial(sec, min, hr, md, am, dy, mt, yr, wk);
    update_weather_75_2_dial(temp, ic);
    update_status_75_2_dial(bat, con);
    update_activity_75_2_dial(2735, 357, 345);
    update_health_75_2_dial(76, 97);
  }

  // if (ui_home == face_79_2_dial){
  //   update_time_79_2_dial(sec, min, hr, md, am, dy, mt, yr, wk);
  //   update_weather_79_2_dial(temp, ic);
  //   update_status_79_2_dial(bat, con);
  //   update_activity_79_2_dial(2735, 357, 345);
  //   update_health_79_2_dial(76, 97);
  // }

  if (ui_home == face_radar)
  {
    update_time_radar(sec, min, hr, md, am, dy, mt, yr, wk);
    update_weather_radar(temp, ic);
    update_status_radar(bat, con);
    update_activity_radar(2735, 357, 345);
    update_health_radar(76, 97);
  }

  // if (ui_home == face_116_2_dial){
  //   update_time_116_2_dial(sec, min, hr, md, am, dy, mt, yr, wk);
  //   update_weather_116_2_dial(temp, ic);
  //   update_status_116_2_dial(bat, con);
  //   update_activity_116_2_dial(2735, 357, 345);
  //   update_health_116_2_dial(76, 97);
  // }

  if (ui_home == face_756_2_dial)
  {
    update_time_756_2_dial(sec, min, hr, md, am, dy, mt, yr, wk);
    update_weather_756_2_dial(temp, ic);
    update_status_756_2_dial(bat, con);
    update_activity_756_2_dial(2735, 357, 345);
    update_health_756_2_dial(76, 97);
  }

  // if (ui_home == face_tix_resized){
  //   update_time_tix_resized(sec, min, hr, md, am, dy, mt, yr, wk);
  //   update_weather_tix_resized(temp, ic);
  //   update_status_tix_resized(bat, con);
  //   update_activity_tix_resized(2735, 357, 345);
  //   update_health_tix_resized(76, 97);
  // }

  // if (ui_home == face_pixel_resized){
  //   update_time_pixel_resized(sec, min, hr, md, am, dy, mt, yr, wk);
  //   update_weather_pixel_resized(temp, ic);
  //   update_status_pixel_resized(bat, con);
  //   update_activity_pixel_resized(2735, 357, 345);
  //   update_health_pixel_resized(76, 97);
  // }

  if (ui_home == face_smart_resized)
  {
    update_time_smart_resized(sec, min, hr, md, am, dy, mt, yr, wk);
    update_weather_smart_resized(temp, ic);
    update_status_smart_resized(bat, con);
    update_activity_smart_resized(2735, 357, 345);
    update_health_smart_resized(76, 97);
  }

  if (ui_home == face_kenya)
  {
    update_time_kenya(sec, min, hr, md, am, dy, mt, yr, wk);
    update_weather_kenya(temp, ic);
    update_status_kenya(bat, con);
    update_activity_kenya(2735, 357, 345);
    update_health_kenya(76, 97);
  }

  if (ui_home == face_b_w_resized)
  {
    update_time_b_w_resized(sec, min, hr, md, am, dy, mt, yr, wk);
    update_weather_b_w_resized(temp, ic);
    update_status_b_w_resized(bat, con);
    update_activity_b_w_resized(2735, 357, 345);
    update_health_b_w_resized(76, 97);
  }

  // if (ui_home == face_wfb_resized)
  // {
  //   update_time_wfb_resized(sec, min, hr, md, am, dy, mt, yr, wk);
  //   update_weather_wfb_resized(temp, ic);
  //   update_status_wfb_resized(bat, con);
  //   update_activity_wfb_resized(2735, 357, 345);
  //   update_health_wfb_resized(76, 97);
  // }
}
