/*
 * Copyright 2026 NXP
 * SPDX-License-Identifier: MIT
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "key.h"

void setup_scr_screen_data(lv_ui *ui)
{

	// Write codes screen_data
	ui->screen_data = lv_obj_create(NULL, NULL);

	// Write style LV_OBJ_PART_MAIN for screen_data
	static lv_style_t style_screen_data_main;
	lv_style_reset(&style_screen_data_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_main
	lv_style_set_bg_color(&style_screen_data_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_opa(&style_screen_data_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_data, LV_OBJ_PART_MAIN, &style_screen_data_main);

	// Write codes screen_data_img_beijing
	ui->screen_data_img_beijing = lv_img_create(ui->screen_data, NULL);

	// Write style LV_IMG_PART_MAIN for screen_data_img_beijing
	static lv_style_t style_screen_data_img_beijing_main;
	lv_style_reset(&style_screen_data_img_beijing_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_img_beijing_main
	lv_style_set_image_recolor(&style_screen_data_img_beijing_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_screen_data_img_beijing_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_screen_data_img_beijing_main, LV_STATE_DEFAULT, 250);
	lv_obj_add_style(ui->screen_data_img_beijing, LV_IMG_PART_MAIN, &style_screen_data_img_beijing_main);
	lv_obj_set_pos(ui->screen_data_img_beijing, 0, 0);
	lv_obj_set_size(ui->screen_data_img_beijing, 240, 320);
	lv_obj_set_click(ui->screen_data_img_beijing, true);
	lv_img_set_src(ui->screen_data_img_beijing, &_beijing_alpha_240x320);
	lv_img_set_pivot(ui->screen_data_img_beijing, 0, 0);
	lv_img_set_angle(ui->screen_data_img_beijing, 0);

	// Write codes screen_data_label_time
	ui->screen_data_label_time = lv_label_create(ui->screen_data, NULL);
	lv_label_set_text(ui->screen_data_label_time, "2026-03-10 16:45");
	lv_label_set_long_mode(ui->screen_data_label_time, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_data_label_time, LV_LABEL_ALIGN_CENTER);

	// Write style LV_LABEL_PART_MAIN for screen_data_label_time
	static lv_style_t style_screen_data_label_time_main;
	lv_style_reset(&style_screen_data_label_time_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_label_time_main
	lv_style_set_radius(&style_screen_data_label_time_main, LV_STATE_DEFAULT, 15);
	lv_style_set_bg_color(&style_screen_data_label_time_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_data_label_time_main, LV_STATE_DEFAULT, lv_color_make(0xaa, 0x6e, 0xa8));
	lv_style_set_bg_grad_dir(&style_screen_data_label_time_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_data_label_time_main, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_data_label_time_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_data_label_time_main, LV_STATE_DEFAULT, &lv_font_ArchitectsDaughter_16);
	lv_style_set_text_letter_space(&style_screen_data_label_time_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_data_label_time_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_data_label_time_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_data_label_time_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_data_label_time_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_data_label_time, LV_LABEL_PART_MAIN, &style_screen_data_label_time_main);
	lv_obj_set_pos(ui->screen_data_label_time, 5, 5);
	lv_obj_set_size(ui->screen_data_label_time, 215, 0);

	// Write codes screen_data_cont_tem
	ui->screen_data_cont_tem = lv_cont_create(ui->screen_data, NULL);

	// Write style LV_CONT_PART_MAIN for screen_data_cont_tem
	static lv_style_t style_screen_data_cont_tem_main;
	lv_style_reset(&style_screen_data_cont_tem_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_cont_tem_main
	lv_style_set_radius(&style_screen_data_cont_tem_main, LV_STATE_DEFAULT, 20);
	lv_style_set_bg_color(&style_screen_data_cont_tem_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_data_cont_tem_main, LV_STATE_DEFAULT, lv_color_make(0xb6, 0x25, 0x25));
	lv_style_set_bg_grad_dir(&style_screen_data_cont_tem_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_data_cont_tem_main, LV_STATE_DEFAULT, 100);
	lv_style_set_border_color(&style_screen_data_cont_tem_main, LV_STATE_DEFAULT, lv_color_make(0xb6, 0x25, 0x25));
	lv_style_set_border_width(&style_screen_data_cont_tem_main, LV_STATE_DEFAULT, 1);
	lv_style_set_border_opa(&style_screen_data_cont_tem_main, LV_STATE_DEFAULT, 255);
	lv_style_set_pad_left(&style_screen_data_cont_tem_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_data_cont_tem_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_data_cont_tem_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_data_cont_tem_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_data_cont_tem, LV_CONT_PART_MAIN, &style_screen_data_cont_tem_main);
	lv_obj_set_pos(ui->screen_data_cont_tem, 0, 35);
	lv_obj_set_size(ui->screen_data_cont_tem, 100, 70);
	lv_obj_set_click(ui->screen_data_cont_tem, false);
	lv_cont_set_layout(ui->screen_data_cont_tem, LV_LAYOUT_OFF);
	lv_cont_set_fit(ui->screen_data_cont_tem, LV_FIT_NONE);

	// Write codes screen_data_label_tem
	ui->screen_data_label_tem = lv_label_create(ui->screen_data, NULL);
	lv_label_set_text(ui->screen_data_label_tem, "温度：30℃ ");
	lv_label_set_long_mode(ui->screen_data_label_tem, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_data_label_tem, LV_LABEL_ALIGN_CENTER);

	// Write style LV_LABEL_PART_MAIN for screen_data_label_tem
	static lv_style_t style_screen_data_label_tem_main;
	lv_style_reset(&style_screen_data_label_tem_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_label_tem_main
	lv_style_set_radius(&style_screen_data_label_tem_main, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_screen_data_label_tem_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_data_label_tem_main, LV_STATE_DEFAULT, lv_color_make(0xaa, 0x6e, 0xa8));
	lv_style_set_bg_grad_dir(&style_screen_data_label_tem_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_data_label_tem_main, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_data_label_tem_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_data_label_tem_main, LV_STATE_DEFAULT, &lv_font_SIMLI_16);
	lv_style_set_text_letter_space(&style_screen_data_label_tem_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_data_label_tem_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_data_label_tem_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_data_label_tem_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_data_label_tem_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_data_label_tem, LV_LABEL_PART_MAIN, &style_screen_data_label_tem_main);
	lv_obj_set_pos(ui->screen_data_label_tem, 15, 65);
	lv_obj_set_size(ui->screen_data_label_tem, 75, 0);

	// Write codes screen_data_img_1
	ui->screen_data_img_1 = lv_img_create(ui->screen_data, NULL);

	// Write style LV_IMG_PART_MAIN for screen_data_img_1
	static lv_style_t style_screen_data_img_1_main;
	lv_style_reset(&style_screen_data_img_1_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_img_1_main
	lv_style_set_image_recolor(&style_screen_data_img_1_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_screen_data_img_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_screen_data_img_1_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_data_img_1, LV_IMG_PART_MAIN, &style_screen_data_img_1_main);
	lv_obj_set_pos(ui->screen_data_img_1, 35, 35);
	lv_obj_set_size(ui->screen_data_img_1, 30, 30);
	lv_obj_set_click(ui->screen_data_img_1, true);
	lv_img_set_src(ui->screen_data_img_1, &_tem_alpha_30x30);
	lv_img_set_pivot(ui->screen_data_img_1, 0, 0);
	lv_img_set_angle(ui->screen_data_img_1, 0);

	// Write codes screen_data_cont_hum
	ui->screen_data_cont_hum = lv_cont_create(ui->screen_data, NULL);

	// Write style LV_CONT_PART_MAIN for screen_data_cont_hum
	static lv_style_t style_screen_data_cont_hum_main;
	lv_style_reset(&style_screen_data_cont_hum_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_cont_hum_main
	lv_style_set_radius(&style_screen_data_cont_hum_main, LV_STATE_DEFAULT, 20);
	lv_style_set_bg_color(&style_screen_data_cont_hum_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_data_cont_hum_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x04, 0xfa));
	lv_style_set_bg_grad_dir(&style_screen_data_cont_hum_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_data_cont_hum_main, LV_STATE_DEFAULT, 100);
	lv_style_set_border_color(&style_screen_data_cont_hum_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x04, 0xfa));
	lv_style_set_border_width(&style_screen_data_cont_hum_main, LV_STATE_DEFAULT, 1);
	lv_style_set_border_opa(&style_screen_data_cont_hum_main, LV_STATE_DEFAULT, 255);
	lv_style_set_pad_left(&style_screen_data_cont_hum_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_data_cont_hum_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_data_cont_hum_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_data_cont_hum_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_data_cont_hum, LV_CONT_PART_MAIN, &style_screen_data_cont_hum_main);
	lv_obj_set_pos(ui->screen_data_cont_hum, 140, 35);
	lv_obj_set_size(ui->screen_data_cont_hum, 100, 70);
	lv_obj_set_click(ui->screen_data_cont_hum, false);
	lv_cont_set_layout(ui->screen_data_cont_hum, LV_LAYOUT_OFF);
	lv_cont_set_fit(ui->screen_data_cont_hum, LV_FIT_NONE);

	// Write codes screen_data_label_hum
	ui->screen_data_label_hum = lv_label_create(ui->screen_data, NULL);
	lv_label_set_text(ui->screen_data_label_hum, "湿度：30%RH");
	lv_label_set_long_mode(ui->screen_data_label_hum, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_data_label_hum, LV_LABEL_ALIGN_CENTER);

	// Write style LV_LABEL_PART_MAIN for screen_data_label_hum
	static lv_style_t style_screen_data_label_hum_main;
	lv_style_reset(&style_screen_data_label_hum_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_label_hum_main
	lv_style_set_radius(&style_screen_data_label_hum_main, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_screen_data_label_hum_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_data_label_hum_main, LV_STATE_DEFAULT, lv_color_make(0xaa, 0x6e, 0xa8));
	lv_style_set_bg_grad_dir(&style_screen_data_label_hum_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_data_label_hum_main, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_data_label_hum_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_data_label_hum_main, LV_STATE_DEFAULT, &lv_font_SIMLI_16);
	lv_style_set_text_letter_space(&style_screen_data_label_hum_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_data_label_hum_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_data_label_hum_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_data_label_hum_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_data_label_hum_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_data_label_hum, LV_LABEL_PART_MAIN, &style_screen_data_label_hum_main);
	lv_obj_set_pos(ui->screen_data_label_hum, 150, 65);
	lv_obj_set_size(ui->screen_data_label_hum, 80, 0);

	// Write codes screen_data_img_2
	ui->screen_data_img_2 = lv_img_create(ui->screen_data, NULL);

	// Write style LV_IMG_PART_MAIN for screen_data_img_2
	static lv_style_t style_screen_data_img_2_main;
	lv_style_reset(&style_screen_data_img_2_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_img_2_main
	lv_style_set_image_recolor(&style_screen_data_img_2_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_screen_data_img_2_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_screen_data_img_2_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_data_img_2, LV_IMG_PART_MAIN, &style_screen_data_img_2_main);
	lv_obj_set_pos(ui->screen_data_img_2, 175, 35);
	lv_obj_set_size(ui->screen_data_img_2, 30, 30);
	lv_obj_set_click(ui->screen_data_img_2, true);
	lv_img_set_src(ui->screen_data_img_2, &_hum_alpha_30x30);
	lv_img_set_pivot(ui->screen_data_img_2, 0, 0);
	lv_img_set_angle(ui->screen_data_img_2, 0);

	// Write codes screen_data_cont_kqm
	ui->screen_data_cont_kqm = lv_cont_create(ui->screen_data, NULL);

	// Write style LV_CONT_PART_MAIN for screen_data_cont_kqm
	static lv_style_t style_screen_data_cont_kqm_main;
	lv_style_reset(&style_screen_data_cont_kqm_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_cont_kqm_main
	lv_style_set_radius(&style_screen_data_cont_kqm_main, LV_STATE_DEFAULT, 20);
	lv_style_set_bg_color(&style_screen_data_cont_kqm_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_data_cont_kqm_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0xff, 0x62));
	lv_style_set_bg_grad_dir(&style_screen_data_cont_kqm_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_data_cont_kqm_main, LV_STATE_DEFAULT, 100);
	lv_style_set_border_color(&style_screen_data_cont_kqm_main, LV_STATE_DEFAULT, lv_color_make(0x99, 0x99, 0x99));
	lv_style_set_border_width(&style_screen_data_cont_kqm_main, LV_STATE_DEFAULT, 1);
	lv_style_set_border_opa(&style_screen_data_cont_kqm_main, LV_STATE_DEFAULT, 255);
	lv_style_set_pad_left(&style_screen_data_cont_kqm_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_data_cont_kqm_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_data_cont_kqm_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_data_cont_kqm_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_data_cont_kqm, LV_CONT_PART_MAIN, &style_screen_data_cont_kqm_main);
	lv_obj_set_pos(ui->screen_data_cont_kqm, 0, 230);
	lv_obj_set_size(ui->screen_data_cont_kqm, 240, 90);
	lv_obj_set_click(ui->screen_data_cont_kqm, false);
	lv_cont_set_layout(ui->screen_data_cont_kqm, LV_LAYOUT_OFF);
	lv_cont_set_fit(ui->screen_data_cont_kqm, LV_FIT_NONE);

	// Write codes screen_data_label_CO2
	ui->screen_data_label_CO2 = lv_label_create(ui->screen_data, NULL);
	lv_label_set_text(ui->screen_data_label_CO2, "CO2  ：368.8   PPM");
	lv_label_set_long_mode(ui->screen_data_label_CO2, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_data_label_CO2, LV_LABEL_ALIGN_LEFT);

	// Write style LV_LABEL_PART_MAIN for screen_data_label_CO2
	static lv_style_t style_screen_data_label_CO2_main;
	lv_style_reset(&style_screen_data_label_CO2_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_label_CO2_main
	lv_style_set_radius(&style_screen_data_label_CO2_main, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_screen_data_label_CO2_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_data_label_CO2_main, LV_STATE_DEFAULT, lv_color_make(0x5c, 0x99, 0x73));
	lv_style_set_bg_grad_dir(&style_screen_data_label_CO2_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_data_label_CO2_main, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_data_label_CO2_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_data_label_CO2_main, LV_STATE_DEFAULT, &lv_font_SIMLI_16);
	lv_style_set_text_letter_space(&style_screen_data_label_CO2_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_data_label_CO2_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_data_label_CO2_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_data_label_CO2_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_data_label_CO2_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_data_label_CO2, LV_LABEL_PART_MAIN, &style_screen_data_label_CO2_main);
	lv_obj_set_pos(ui->screen_data_label_CO2, 38, 235);
	lv_obj_set_size(ui->screen_data_label_CO2, 180, 0);

	// Write codes screen_data_label_VOC
	ui->screen_data_label_VOC = lv_label_create(ui->screen_data, NULL);
	lv_label_set_text(ui->screen_data_label_VOC, "VOC  ：0.001   PPM");
	lv_label_set_long_mode(ui->screen_data_label_VOC, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_data_label_VOC, LV_LABEL_ALIGN_LEFT);

	// Write style LV_LABEL_PART_MAIN for screen_data_label_VOC
	static lv_style_t style_screen_data_label_VOC_main;
	lv_style_reset(&style_screen_data_label_VOC_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_label_VOC_main
	lv_style_set_radius(&style_screen_data_label_VOC_main, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_screen_data_label_VOC_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_data_label_VOC_main, LV_STATE_DEFAULT, lv_color_make(0x5c, 0x99, 0x73));
	lv_style_set_bg_grad_dir(&style_screen_data_label_VOC_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_data_label_VOC_main, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_data_label_VOC_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_data_label_VOC_main, LV_STATE_DEFAULT, &lv_font_SIMLI_16);
	lv_style_set_text_letter_space(&style_screen_data_label_VOC_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_data_label_VOC_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_data_label_VOC_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_data_label_VOC_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_data_label_VOC_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_data_label_VOC, LV_LABEL_PART_MAIN, &style_screen_data_label_VOC_main);
	lv_obj_set_pos(ui->screen_data_label_VOC, 38, 265);
	lv_obj_set_size(ui->screen_data_label_VOC, 180, 0);

	// Write codes screen_data_label_CH2O
	ui->screen_data_label_CH2O = lv_label_create(ui->screen_data, NULL);
	lv_label_set_text(ui->screen_data_label_CH2O, "CH2O：0.001  PPM");
	lv_label_set_long_mode(ui->screen_data_label_CH2O, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_data_label_CH2O, LV_LABEL_ALIGN_LEFT);

	// Write style LV_LABEL_PART_MAIN for screen_data_label_CH2O
	static lv_style_t style_screen_data_label_CH2O_main;
	lv_style_reset(&style_screen_data_label_CH2O_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_label_CH2O_main
	lv_style_set_radius(&style_screen_data_label_CH2O_main, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_screen_data_label_CH2O_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_data_label_CH2O_main, LV_STATE_DEFAULT, lv_color_make(0x5c, 0x99, 0x73));
	lv_style_set_bg_grad_dir(&style_screen_data_label_CH2O_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_data_label_CH2O_main, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_data_label_CH2O_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_data_label_CH2O_main, LV_STATE_DEFAULT, &lv_font_SIMLI_16);
	lv_style_set_text_letter_space(&style_screen_data_label_CH2O_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_data_label_CH2O_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_data_label_CH2O_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_data_label_CH2O_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_data_label_CH2O_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_data_label_CH2O, LV_LABEL_PART_MAIN, &style_screen_data_label_CH2O_main);
	lv_obj_set_pos(ui->screen_data_label_CH2O, 38, 295);
	lv_obj_set_size(ui->screen_data_label_CH2O, 180, 0);

	// Write codes screen_data_img_3
	ui->screen_data_img_3 = lv_img_create(ui->screen_data, NULL);

	// Write style LV_IMG_PART_MAIN for screen_data_img_3
	static lv_style_t style_screen_data_img_3_main;
	lv_style_reset(&style_screen_data_img_3_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_img_3_main
	lv_style_set_image_recolor(&style_screen_data_img_3_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_screen_data_img_3_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_screen_data_img_3_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_data_img_3, LV_IMG_PART_MAIN, &style_screen_data_img_3_main);
	lv_obj_set_pos(ui->screen_data_img_3, 0, 290);
	lv_obj_set_size(ui->screen_data_img_3, 30, 30);
	lv_obj_set_click(ui->screen_data_img_3, true);
	lv_img_set_src(ui->screen_data_img_3, &_ch2o_alpha_30x30);
	lv_img_set_pivot(ui->screen_data_img_3, 0, 0);
	lv_img_set_angle(ui->screen_data_img_3, 0);

	// Write codes screen_data_img_4
	ui->screen_data_img_4 = lv_img_create(ui->screen_data, NULL);

	// Write style LV_IMG_PART_MAIN for screen_data_img_4
	static lv_style_t style_screen_data_img_4_main;
	lv_style_reset(&style_screen_data_img_4_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_img_4_main
	lv_style_set_image_recolor(&style_screen_data_img_4_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_screen_data_img_4_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_screen_data_img_4_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_data_img_4, LV_IMG_PART_MAIN, &style_screen_data_img_4_main);
	lv_obj_set_pos(ui->screen_data_img_4, 0, 230);
	lv_obj_set_size(ui->screen_data_img_4, 30, 30);
	lv_obj_set_click(ui->screen_data_img_4, true);
	lv_img_set_src(ui->screen_data_img_4, &_co2_alpha_30x30);
	lv_img_set_pivot(ui->screen_data_img_4, 0, 0);
	lv_img_set_angle(ui->screen_data_img_4, 0);

	// Write codes screen_data_img_5
	ui->screen_data_img_5 = lv_img_create(ui->screen_data, NULL);

	// Write style LV_IMG_PART_MAIN for screen_data_img_5
	static lv_style_t style_screen_data_img_5_main;
	lv_style_reset(&style_screen_data_img_5_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_img_5_main
	lv_style_set_image_recolor(&style_screen_data_img_5_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_screen_data_img_5_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_screen_data_img_5_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_data_img_5, LV_IMG_PART_MAIN, &style_screen_data_img_5_main);
	lv_obj_set_pos(ui->screen_data_img_5, 0, 260);
	lv_obj_set_size(ui->screen_data_img_5, 30, 30);
	lv_obj_set_click(ui->screen_data_img_5, true);
	lv_img_set_src(ui->screen_data_img_5, &_voc_alpha_30x30);
	lv_img_set_pivot(ui->screen_data_img_5, 0, 0);
	lv_img_set_angle(ui->screen_data_img_5, 0);

	// Write codes screen_data_cont_light
	ui->screen_data_cont_light = lv_cont_create(ui->screen_data, NULL);

	// Write style LV_CONT_PART_MAIN for screen_data_cont_light
	static lv_style_t style_screen_data_cont_light_main;
	lv_style_reset(&style_screen_data_cont_light_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_cont_light_main
	lv_style_set_radius(&style_screen_data_cont_light_main, LV_STATE_DEFAULT, 20);
	lv_style_set_bg_color(&style_screen_data_cont_light_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_data_cont_light_main, LV_STATE_DEFAULT, lv_color_make(0xb6, 0x25, 0x25));
	lv_style_set_bg_grad_dir(&style_screen_data_cont_light_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_data_cont_light_main, LV_STATE_DEFAULT, 100);
	lv_style_set_border_color(&style_screen_data_cont_light_main, LV_STATE_DEFAULT, lv_color_make(0xb6, 0x25, 0x25));
	lv_style_set_border_width(&style_screen_data_cont_light_main, LV_STATE_DEFAULT, 1);
	lv_style_set_border_opa(&style_screen_data_cont_light_main, LV_STATE_DEFAULT, 255);
	lv_style_set_pad_left(&style_screen_data_cont_light_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_data_cont_light_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_data_cont_light_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_data_cont_light_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_data_cont_light, LV_CONT_PART_MAIN, &style_screen_data_cont_light_main);
	lv_obj_set_pos(ui->screen_data_cont_light, 90, 150);
	lv_obj_set_size(ui->screen_data_cont_light, 150, 70);
	lv_obj_set_click(ui->screen_data_cont_light, false);
	lv_cont_set_layout(ui->screen_data_cont_light, LV_LAYOUT_OFF);
	lv_cont_set_fit(ui->screen_data_cont_light, LV_FIT_NONE);

	// Write codes screen_data_img_6
	ui->screen_data_img_6 = lv_img_create(ui->screen_data, NULL);

	// Write style LV_IMG_PART_MAIN for screen_data_img_6
	static lv_style_t style_screen_data_img_6_main;
	lv_style_reset(&style_screen_data_img_6_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_img_6_main
	lv_style_set_image_recolor(&style_screen_data_img_6_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_screen_data_img_6_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_screen_data_img_6_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_data_img_6, LV_IMG_PART_MAIN, &style_screen_data_img_6_main);
	lv_obj_set_pos(ui->screen_data_img_6, 14, 161);
	lv_obj_set_size(ui->screen_data_img_6, 50, 50);
	lv_obj_set_click(ui->screen_data_img_6, true);
	lv_img_set_src(ui->screen_data_img_6, &_light_alpha_50x50);
	lv_img_set_pivot(ui->screen_data_img_6, 0, 0);
	lv_img_set_angle(ui->screen_data_img_6, 0);

	// Write codes screen_data_label_lx
	ui->screen_data_label_lx = lv_label_create(ui->screen_data, NULL);
	lv_label_set_text(ui->screen_data_label_lx, "light:0000lx");
	lv_label_set_long_mode(ui->screen_data_label_lx, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_data_label_lx, LV_LABEL_ALIGN_LEFT);

	// Write style LV_LABEL_PART_MAIN for screen_data_label_lx
	static lv_style_t style_screen_data_label_lx_main;
	lv_style_reset(&style_screen_data_label_lx_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_label_lx_main
	lv_style_set_radius(&style_screen_data_label_lx_main, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_screen_data_label_lx_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_data_label_lx_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_screen_data_label_lx_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_data_label_lx_main, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_data_label_lx_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_data_label_lx_main, LV_STATE_DEFAULT, &lv_font_SIMLI_16);
	lv_style_set_text_letter_space(&style_screen_data_label_lx_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_data_label_lx_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_data_label_lx_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_data_label_lx_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_data_label_lx_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_data_label_lx, LV_LABEL_PART_MAIN, &style_screen_data_label_lx_main);
	lv_obj_set_pos(ui->screen_data_label_lx, 97, 164);
	lv_obj_set_size(ui->screen_data_label_lx, 140, 0);

	// Write codes screen_data_label_lm
	ui->screen_data_label_lm = lv_label_create(ui->screen_data, NULL);
	lv_label_set_text(ui->screen_data_label_lm, "light:0000lm");
	lv_label_set_long_mode(ui->screen_data_label_lm, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_data_label_lm, LV_LABEL_ALIGN_LEFT);

	// Write style LV_LABEL_PART_MAIN for screen_data_label_lm
	static lv_style_t style_screen_data_label_lm_main;
	lv_style_reset(&style_screen_data_label_lm_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_label_lm_main
	lv_style_set_radius(&style_screen_data_label_lm_main, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_screen_data_label_lm_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_data_label_lm_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_screen_data_label_lm_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_data_label_lm_main, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_data_label_lm_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_data_label_lm_main, LV_STATE_DEFAULT, &lv_font_SIMLI_16);
	lv_style_set_text_letter_space(&style_screen_data_label_lm_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_data_label_lm_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_data_label_lm_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_data_label_lm_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_data_label_lm_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_data_label_lm, LV_LABEL_PART_MAIN, &style_screen_data_label_lm_main);
	lv_obj_set_pos(ui->screen_data_label_lm, 97, 191);
	lv_obj_set_size(ui->screen_data_label_lm, 140, 0);


		// Write codes screen_data_img_wifi
		ui->screen_data_img_wifi = lv_img_create(ui->screen_data, NULL);

		// Write style LV_IMG_PART_MAIN for screen_data_img_wifi
		static lv_style_t style_screen_data_img_wifi_main;
		lv_style_reset(&style_screen_data_img_wifi_main);

		// Write style state: LV_STATE_DEFAULT for style_screen_data_img_wifi_main
		lv_style_set_image_recolor(&style_screen_data_img_wifi_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
		lv_style_set_image_recolor_opa(&style_screen_data_img_wifi_main, LV_STATE_DEFAULT, 0);
		lv_style_set_image_opa(&style_screen_data_img_wifi_main, LV_STATE_DEFAULT, 255);
		lv_obj_add_style(ui->screen_data_img_wifi, LV_IMG_PART_MAIN, &style_screen_data_img_wifi_main);
		lv_obj_set_pos(ui->screen_data_img_wifi, 220, 8);
		lv_obj_set_size(ui->screen_data_img_wifi, 20, 20);
		lv_obj_set_click(ui->screen_data_img_wifi, true);
		lv_img_set_src(ui->screen_data_img_wifi, &_wifi_alpha_20x20);
		lv_img_set_pivot(ui->screen_data_img_wifi, 0, 0);
		lv_img_set_angle(ui->screen_data_img_wifi, 0);
	

	// //Write codes screen_data_img_nowifi
	// ui->screen_data_img_nowifi = lv_img_create(ui->screen_data, NULL);

	// //Write style LV_IMG_PART_MAIN for screen_data_img_nowifi
	// static lv_style_t style_screen_data_img_nowifi_main;
	// lv_style_reset(&style_screen_data_img_nowifi_main);

	// //Write style state: LV_STATE_DEFAULT for style_screen_data_img_nowifi_main
	// lv_style_set_image_recolor(&style_screen_data_img_nowifi_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	// lv_style_set_image_recolor_opa(&style_screen_data_img_nowifi_main, LV_STATE_DEFAULT, 0);
	// lv_style_set_image_opa(&style_screen_data_img_nowifi_main, LV_STATE_DEFAULT, 255);
	// lv_obj_add_style(ui->screen_data_img_nowifi, LV_IMG_PART_MAIN, &style_screen_data_img_nowifi_main);
	// lv_obj_set_pos(ui->screen_data_img_nowifi, 220, 8);
	// lv_obj_set_size(ui->screen_data_img_nowifi, 20, 20);
	// lv_obj_set_click(ui->screen_data_img_nowifi, true);
	// lv_img_set_src(ui->screen_data_img_nowifi,&_noWIFI_alpha_20x20);
	// lv_img_set_pivot(ui->screen_data_img_nowifi, 0,0);
	// lv_img_set_angle(ui->screen_data_img_nowifi, 0);

	// Write codes screen_data_cont_yanwu
	ui->screen_data_cont_yanwu = lv_cont_create(ui->screen_data, NULL);

	// Write style LV_CONT_PART_MAIN for screen_data_cont_yanwu
	static lv_style_t style_screen_data_cont_yanwu_main;
	lv_style_reset(&style_screen_data_cont_yanwu_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_cont_yanwu_main
	lv_style_set_radius(&style_screen_data_cont_yanwu_main, LV_STATE_DEFAULT, 20);
	lv_style_set_bg_color(&style_screen_data_cont_yanwu_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_data_cont_yanwu_main, LV_STATE_DEFAULT, lv_color_make(0xb6, 0x25, 0x25));
	lv_style_set_bg_grad_dir(&style_screen_data_cont_yanwu_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_data_cont_yanwu_main, LV_STATE_DEFAULT, 100);
	lv_style_set_border_color(&style_screen_data_cont_yanwu_main, LV_STATE_DEFAULT, lv_color_make(0xb6, 0x25, 0x25));
	lv_style_set_border_width(&style_screen_data_cont_yanwu_main, LV_STATE_DEFAULT, 1);
	lv_style_set_border_opa(&style_screen_data_cont_yanwu_main, LV_STATE_DEFAULT, 255);
	lv_style_set_pad_left(&style_screen_data_cont_yanwu_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_data_cont_yanwu_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_data_cont_yanwu_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_data_cont_yanwu_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_data_cont_yanwu, LV_CONT_PART_MAIN, &style_screen_data_cont_yanwu_main);
	lv_obj_set_pos(ui->screen_data_cont_yanwu, 0, 110);
	lv_obj_set_size(ui->screen_data_cont_yanwu, 150, 30);
	lv_obj_set_click(ui->screen_data_cont_yanwu, false);
	lv_cont_set_layout(ui->screen_data_cont_yanwu, LV_LAYOUT_OFF);
	lv_cont_set_fit(ui->screen_data_cont_yanwu, LV_FIT_NONE);

	// Write codes screen_data_label_yanwu
	ui->screen_data_label_yanwu = lv_label_create(ui->screen_data, NULL);
	lv_label_set_text(ui->screen_data_label_yanwu, "yanwu:0000lx");
	lv_label_set_long_mode(ui->screen_data_label_yanwu, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_data_label_yanwu, LV_LABEL_ALIGN_LEFT);

	// Write style LV_LABEL_PART_MAIN for screen_data_label_yanwu
	static lv_style_t style_screen_data_label_yanwu_main;
	lv_style_reset(&style_screen_data_label_yanwu_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_label_yanwu_main
	lv_style_set_radius(&style_screen_data_label_yanwu_main, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_screen_data_label_yanwu_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_data_label_yanwu_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_screen_data_label_yanwu_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_data_label_yanwu_main, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_data_label_yanwu_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_data_label_yanwu_main, LV_STATE_DEFAULT, &lv_font_SIMLI_16);
	lv_style_set_text_letter_space(&style_screen_data_label_yanwu_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_data_label_yanwu_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_data_label_yanwu_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_data_label_yanwu_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_data_label_yanwu_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_data_label_yanwu, LV_LABEL_PART_MAIN, &style_screen_data_label_yanwu_main);
	lv_obj_set_pos(ui->screen_data_label_yanwu, 5, 115);
	lv_obj_set_size(ui->screen_data_label_yanwu, 140, 0);

	// Write codes screen_data_img_9
	ui->screen_data_img_9 = lv_img_create(ui->screen_data, NULL);

	// Write style LV_IMG_PART_MAIN for screen_data_img_9
	static lv_style_t style_screen_data_img_9_main;
	lv_style_reset(&style_screen_data_img_9_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_data_img_9_main
	lv_style_set_image_recolor(&style_screen_data_img_9_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_screen_data_img_9_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_screen_data_img_9_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_data_img_9, LV_IMG_PART_MAIN, &style_screen_data_img_9_main);
	lv_obj_set_pos(ui->screen_data_img_9, 158, 111);
	lv_obj_set_size(ui->screen_data_img_9, 60, 30);
	lv_obj_set_click(ui->screen_data_img_9, true);
	lv_img_set_src(ui->screen_data_img_9, &_yanwu_alpha_60x30);
	lv_img_set_pivot(ui->screen_data_img_9, 0, 0);
	lv_img_set_angle(ui->screen_data_img_9, 0);
}