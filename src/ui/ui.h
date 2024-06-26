// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.2.3
// LVGL version: 8.3.3
// Project name: C3_Watch

#ifndef _C3_WATCH_UI_H
#define _C3_WATCH_UI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"

#include "ui_events.h"
    void ui_event_clockScreen(lv_event_t *e);
    extern lv_obj_t *ui_clockScreen;
    extern lv_obj_t *ui_hourLabel;
    extern lv_obj_t *ui_minuteLabel;
    extern lv_obj_t *ui_dateLabel;
    extern lv_obj_t *ui_weatherIcon;
    extern lv_obj_t *ui_weatherTemp;
    extern lv_obj_t *ui_dayLabel;
    extern lv_obj_t *ui_amPmLabel;
    void ui_event_alertPanel(lv_event_t *e);
    extern lv_obj_t *ui_alertPanel;
    extern lv_obj_t *ui_alertIcon;
    extern lv_obj_t *ui_alertText;
    void ui_event_weatherScreen(lv_event_t *e);
    extern lv_obj_t *ui_weatherScreen;
    extern lv_obj_t *ui_weatherPanel;
    extern lv_obj_t *ui_weatherCity;
    extern lv_obj_t *ui_weatherCurrentIcon;
    extern lv_obj_t *ui_weatherCurrentTemp;
    extern lv_obj_t *ui_weatherUpdateTime;
    extern lv_obj_t *ui_forecastPanel;
    extern lv_obj_t *ui_forecastTitle;
    extern lv_obj_t *ui_forecastList;
    void ui_event_notificationScreen(lv_event_t *e);
    extern lv_obj_t *ui_notificationScreen;
    extern lv_obj_t *ui_messagePanel;
    extern lv_obj_t *ui_messageIcon;
    extern lv_obj_t *ui_messageTime;
    extern lv_obj_t *ui_messageContent;
    extern lv_obj_t *ui_messageList;
    void ui_event_settingsScreen(lv_event_t *e);
    extern lv_obj_t *ui_settingsScreen;
    extern lv_obj_t *ui_settingsList;
    extern lv_obj_t *ui_settingsTitle;
    extern lv_obj_t *ui_brightnessPanel;
    void ui_event_brightnessSlider(lv_event_t *e);
    extern lv_obj_t *ui_brightnessSlider;
    extern lv_obj_t *ui_brightnessIcon;
    extern lv_obj_t *ui_brightnessLabel;
    extern lv_obj_t *ui_scrollingPanel;
    extern lv_obj_t *ui_scrollIcon;
    void ui_event_Switch2(lv_event_t *e);
    extern lv_obj_t *ui_Switch2;
    extern lv_obj_t *ui_scrollLabel;
    extern lv_obj_t *ui_timeoutPanel;
    void ui_event_timeoutSelect(lv_event_t *e);
    extern lv_obj_t *ui_timeoutSelect;
    extern lv_obj_t *ui_timeoutIcon;
    extern lv_obj_t *ui_timeoutLabel;
    extern lv_obj_t *ui_batteryPanel;
    void ui_event_batterySlider(lv_event_t *e);
    extern lv_obj_t *ui_batterySlider;
    extern lv_obj_t *ui_batteryIcon;
    extern lv_obj_t *ui_batteryLabel;
    extern lv_obj_t *ui_aboutPanel;
    extern lv_obj_t *ui_aboutIcon;
    extern lv_obj_t *ui_aboutText;
    void ui_event_controlScreen(lv_event_t *e);
    extern lv_obj_t *ui_controlScreen;
    extern lv_obj_t *ui_controlTitle;
    void ui_event_musicPlayButton(lv_event_t *e);
    extern lv_obj_t *ui_musicPlayButton;
    void ui_event_musicPrevButton(lv_event_t *e);
    extern lv_obj_t *ui_musicPrevButton;
    void ui_event_musicNextButton(lv_event_t *e);
    extern lv_obj_t *ui_musicNextButton;
    extern lv_obj_t *ui_btStateButton;
    void ui_event_phoneSearchButton(lv_event_t *e);
    extern lv_obj_t *ui_phoneSearchButton;
    extern lv_obj_t *ui_powerPanel;
    extern lv_obj_t *ui_powerIcon;
    extern lv_obj_t *ui_powerLabel;
    void ui_event_sleepButton(lv_event_t *e);
    extern lv_obj_t *ui_sleepButton;
    void ui_event_powerButton(lv_event_t *e);
    extern lv_obj_t *ui_powerButton;
    extern lv_obj_t *ui_vibrationPanel;
    extern lv_obj_t *ui_vibrationIcon;
    void ui_event_vibrationSwitch(lv_event_t *e);
    extern lv_obj_t *ui_vibrationSwitch;
    extern lv_obj_t *ui_vibrationLabel;
    extern lv_obj_t *ui_accPanel;
    extern lv_obj_t *ui_accIcon;
    extern lv_obj_t *ui_accLabel;
    extern lv_obj_t *ui_accXBar;
    extern lv_obj_t *ui_accYBar;
    extern lv_obj_t *ui_accZBar;
    extern lv_obj_t *ui_rtwPanel;
    extern lv_obj_t *ui_rtwIcon;
    void ui_event_rtwSwitch(lv_event_t *e);
    extern lv_obj_t *ui_rtwSwitch;
    extern lv_obj_t *ui_rtwLabel;
    void ui_event_volumeUpButton(lv_event_t *e);
    extern lv_obj_t *ui_volumeUpButton;
    void ui_event_volumeDownButton(lv_event_t *e);
    extern lv_obj_t *ui_volumeDownButton;

    extern lv_obj_t *ui_home;

    extern lv_obj_t *ui____initial_actions0;

    LV_IMG_DECLARE(ui_img_753022056);      // assets\night-sky.png
    LV_IMG_DECLARE(ui_img_602195540);      // assets\dy-6.png
    LV_IMG_DECLARE(ui_img_wechat_png);     // assets\wechat.png
    LV_IMG_DECLARE(ui_img_857483832);      // assets\day-sky.png
    LV_IMG_DECLARE(ui_img_602206286);      // assets\dy-0.png
    LV_IMG_DECLARE(ui_img_chrns_png);      // assets\chrns.png
    LV_IMG_DECLARE(ui_img_brightness_png); // assets\brightness.png
    LV_IMG_DECLARE(ui_img_scrolling_png);  // assets\scrolling.png
    LV_IMG_DECLARE(ui_img_timeout_png);    // assets\timeout.png
    LV_IMG_DECLARE(ui_img_bat_png);        // assets\bat.png
    LV_IMG_DECLARE(ui_img_info_png);       // assets\info.png
    LV_IMG_DECLARE(ui_img_play_png);       // assets\play.png
    LV_IMG_DECLARE(ui_img_1592976543);     // assets\left-arrow.png
    LV_IMG_DECLARE(ui_img_2138854088);     // assets\right-arrow.png
    LV_IMG_DECLARE(ui_img_bt_png);         // assets\bt.png
    LV_IMG_DECLARE(ui_img_search_png);     // assets\search.png
    LV_IMG_DECLARE(ui_img_camera_png);     // assets\camera.png
    LV_IMG_DECLARE(ui_img_602205261);      // assets\dy-1.png
    LV_IMG_DECLARE(ui_img_602199888);      // assets\dy-2.png
    LV_IMG_DECLARE(ui_img_602207311);      // assets\dy-3.png
    LV_IMG_DECLARE(ui_img_602200913);      // assets\dy-5.png
    LV_IMG_DECLARE(ui_img_602202963);      // assets\dy-7.png
    LV_IMG_DECLARE(ui_img_dy4_png);        // assets\dy4.png
    LV_IMG_DECLARE(ui_img_facebook_png);   // assets\facebook.png
    // LV_IMG_DECLARE( ui_img_forest_png);   // assets\forest.png
    LV_IMG_DECLARE(ui_img_instagram_png); // assets\instagram.png
    LV_IMG_DECLARE(ui_img_kakao_png);     // assets\kakao.png
    // LV_IMG_DECLARE( ui_img_lake_png);   // assets\lake.png
    LV_IMG_DECLARE(ui_img_line_png);      // assets\line.png
    LV_IMG_DECLARE(ui_img_mail_png);      // assets\mail.png
    LV_IMG_DECLARE(ui_img_messenger_png); // assets\messenger.png
    // LV_IMG_DECLARE( ui_img_mountain_png);   // assets\mountain.png
    LV_IMG_DECLARE(ui_img_notify_png);    // assets\notify.png
    LV_IMG_DECLARE(ui_img_229834011);     // assets\nt-0.png
    LV_IMG_DECLARE(ui_img_229835036);     // assets\nt-1.png
    LV_IMG_DECLARE(ui_img_229827613);     // assets\nt-2.png
    LV_IMG_DECLARE(ui_img_229828638);     // assets\nt-3.png
    LV_IMG_DECLARE(ui_img_229838359);     // assets\nt-4.png
    LV_IMG_DECLARE(ui_img_229839384);     // assets\nt-5.png
    LV_IMG_DECLARE(ui_img_229831961);     // assets\nt-6.png
    LV_IMG_DECLARE(ui_img_229832986);     // assets\nt-7.png
    LV_IMG_DECLARE(ui_img_penguin_png);   // assets\penguin.png
    LV_IMG_DECLARE(ui_img_qr_png);        // assets\qr.png
    LV_IMG_DECLARE(ui_img_setting_png);   // assets\setting.png
    LV_IMG_DECLARE(ui_img_skype_png);     // assets\skype.png
    LV_IMG_DECLARE(ui_img_sms_png);       // assets\sms.png
    LV_IMG_DECLARE(ui_img_stars_png);     // assets\stars.png
    LV_IMG_DECLARE(ui_img_telegram_png);  // assets\telegram.png
    LV_IMG_DECLARE(ui_img_twitter_png);   // assets\twitter.png
    LV_IMG_DECLARE(ui_img_viber_png);     // assets\viber.png
    LV_IMG_DECLARE(ui_img_vkontakte_png); // assets\vkontakte.png
    LV_IMG_DECLARE(ui_img_weibo_png);     // assets\weibo.png
    LV_IMG_DECLARE(ui_img_whatsapp_png);  // assets\whatsapp.png

    LV_IMG_DECLARE( ui_img_power_save_png);   // assets\power_save.png
LV_IMG_DECLARE( ui_img_moon_png);   // assets\moon.png
LV_IMG_DECLARE( ui_img_switch_png);   // assets\switch.png
LV_IMG_DECLARE( ui_img_vibration_png);   // assets\vibration.png
LV_IMG_DECLARE( ui_img_gyroscope_png);   // assets\gyroscope.png
LV_IMG_DECLARE( ui_img_rtw_png);   // assets\rtw.png

LV_IMG_DECLARE( ui_img_play_pause_g_png);   // assets\play_pause_g.png
LV_IMG_DECLARE( ui_img_previous_g_png);   // assets\previous_g.png
LV_IMG_DECLARE( ui_img_next_g_png);   // assets\next_g.png
LV_IMG_DECLARE( ui_img_bluetooth_g_png);   // assets\bluetooth_g.png
LV_IMG_DECLARE( ui_img_search_g_png);   // assets\search_g.png
LV_IMG_DECLARE( ui_img_volupe_up_g_png);   // assets\volupe_up_g.png
LV_IMG_DECLARE( ui_img_volume_down_g_png);   // assets\volume_down_g.png

    LV_IMG_DECLARE(digital_preview);

    LV_FONT_DECLARE(ui_font_H1);
    LV_FONT_DECLARE(ui_font_Number_big);
    LV_FONT_DECLARE(ui_font_Number_extra);
    LV_FONT_DECLARE(ui_font_Subtitle);
    LV_FONT_DECLARE(ui_font_Title);

    void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
