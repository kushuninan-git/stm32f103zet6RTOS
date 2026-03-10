/*
 * Copyright 2026 NXP
 * SPDX-License-Identifier: MIT
 */

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "guider_fonts.h"

typedef struct
{
	lv_obj_t *screen_open;
	lv_obj_t *screen_open_label_huanying;
	lv_obj_t *screen_open_label_ap;
	lv_obj_t *screen_open_img_zhutou;
	lv_obj_t *screen_data;
	lv_obj_t *screen_data_img_beijing;
	lv_obj_t *screen_data_label_time;
	lv_obj_t *screen_data_cont_tem;
	lv_obj_t *screen_data_label_tem;
	lv_obj_t *screen_data_img_1;
	lv_obj_t *screen_data_cont_hum;
	lv_obj_t *screen_data_label_hum;
	lv_obj_t *screen_data_img_2;
	lv_obj_t *screen_data_cont_kqm;
	lv_obj_t *screen_data_label_CO2;
	lv_obj_t *screen_data_label_VOC;
	lv_obj_t *screen_data_label_CH2O;
	lv_obj_t *screen_data_img_3;
	lv_obj_t *screen_data_img_4;
	lv_obj_t *screen_data_img_5;
	lv_obj_t *screen_data_cont_light;
	lv_obj_t *screen_data_img_6;
	lv_obj_t *screen_data_label_lx;
	lv_obj_t *screen_data_label_lm;
	lv_obj_t *screen_data_img_wifi;
	lv_obj_t *screen_data_img_nowifi;
	lv_obj_t *screen_data_cont_yanwu;
	lv_obj_t *screen_data_label_yanwu;
	lv_obj_t *screen_data_img_9;
	lv_obj_t *screen_wifi;
	lv_obj_t *screen_wifi_label_1;
	lv_obj_t *screen_wifi_label_2;
	lv_obj_t *screen_wifi_label_3;
	lv_obj_t *screen_wifi_label_4;
	lv_obj_t *screen_wifi_label_5;
	lv_obj_t *screen_wifi_label_name;
	lv_obj_t *screen_wifi_label_pass;
}lv_ui;

void setup_ui(lv_ui *ui);
extern lv_ui guider_ui;
void setup_scr_screen_open(lv_ui *ui);
void setup_scr_screen_data(lv_ui *ui);
void setup_scr_screen_wifi(lv_ui *ui);
LV_IMG_DECLARE(_voc_alpha_30x30);
LV_IMG_DECLARE(_wifi_alpha_20x20);
LV_IMG_DECLARE(_tem_alpha_30x30);
LV_IMG_DECLARE(_light_alpha_50x50);
LV_IMG_DECLARE(_yanwu_alpha_60x30);
LV_IMG_DECLARE(_ch2o_alpha_30x30);
LV_IMG_DECLARE(_co2_alpha_30x30);
LV_IMG_DECLARE(_hum_alpha_30x30);
LV_IMG_DECLARE(_beijing_alpha_240x320);
LV_IMG_DECLARE(_zhutou_alpha_100x100);
LV_IMG_DECLARE(_noWIFI_alpha_20x20);

#ifdef __cplusplus
}
#endif
#endif