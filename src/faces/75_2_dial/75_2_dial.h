
// File generated by bin2lvgl
// developed by fbiego. 
// https://github.com/fbiego
// Watchface: 75_2_DIAL

#ifndef _FACE_75_2_DIAL_H
#define _FACE_75_2_DIAL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"

    extern lv_obj_t *face_75_2_dial;
	extern lv_obj_t *face_75_2_dial_0_1024;
	extern lv_obj_t *face_75_2_dial_2_216824;
	extern lv_obj_t *face_75_2_dial_3_59132;
	extern lv_obj_t *face_75_2_dial_19_89191;
	extern lv_obj_t *face_75_2_dial_35_138999;


	LV_IMG_DECLARE(face_75_2_dial_dial_img_0_1024_0);
	LV_IMG_DECLARE(face_75_2_dial_dial_img_1_213131_0);
	LV_IMG_DECLARE(face_75_2_dial_dial_img_1_213131_1);
	LV_IMG_DECLARE(face_75_2_dial_dial_img_1_213131_2);
	LV_IMG_DECLARE(face_75_2_dial_dial_img_1_213131_3);
	LV_IMG_DECLARE(face_75_2_dial_dial_img_1_213131_4);
	LV_IMG_DECLARE(face_75_2_dial_dial_img_1_213131_5);
	LV_IMG_DECLARE(face_75_2_dial_dial_img_1_213131_6);
	LV_IMG_DECLARE(face_75_2_dial_dial_img_2_216824_0);
	LV_IMG_DECLARE(face_75_2_dial_dial_img_2_216824_1);
	LV_IMG_DECLARE(face_75_2_dial_dial_img_2_216824_2);
	LV_IMG_DECLARE(face_75_2_dial_dial_img_2_216824_3);
	LV_IMG_DECLARE(face_75_2_dial_dial_img_2_216824_4);
	LV_IMG_DECLARE(face_75_2_dial_dial_img_2_216824_5);
	LV_IMG_DECLARE(face_75_2_dial_dial_img_2_216824_6);
	LV_IMG_DECLARE(face_75_2_dial_dial_img_3_59132_0);
	LV_IMG_DECLARE(face_75_2_dial_dial_img_19_89191_0);
	LV_IMG_DECLARE(face_75_2_dial_dial_img_35_138999_0);
	LV_IMG_DECLARE(face_75_2_dial_dial_img_preview_0);


    void onFaceEvent(lv_event_t * e);

    void init_face_75_2_dial(void);
    void update_time_75_2_dial(int second, int minute, int hour, bool mode, bool am, int day, int month, int year, int weekday);
    void update_weather_75_2_dial(int temp, int icon);
    void update_status_75_2_dial(int battery, bool connection);
    void update_activity_75_2_dial(int steps, int distance, int kcal);
    void update_health_75_2_dial(int bpm, int oxygen);


#ifdef __cplusplus
}
#endif

#endif
