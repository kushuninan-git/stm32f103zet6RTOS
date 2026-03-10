/*
 * Copyright 2026 NXP
 * SPDX-License-Identifier: MIT
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"



void setup_scr_screen_wifi(lv_ui *ui){

	//Write codes screen_wifi
	ui->screen_wifi = lv_obj_create(NULL, NULL);

	//Write style LV_OBJ_PART_MAIN for screen_wifi
	static lv_style_t style_screen_wifi_main;
	lv_style_reset(&style_screen_wifi_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_wifi_main
	lv_style_set_bg_color(&style_screen_wifi_main, LV_STATE_DEFAULT, lv_color_make(0x3c, 0xdd, 0xc2));
	lv_style_set_bg_opa(&style_screen_wifi_main, LV_STATE_DEFAULT, 126);
	lv_obj_add_style(ui->screen_wifi, LV_OBJ_PART_MAIN, &style_screen_wifi_main);

	//Write codes screen_wifi_label_1
	ui->screen_wifi_label_1 = lv_label_create(ui->screen_wifi, NULL);
	lv_label_set_text(ui->screen_wifi_label_1, "1：打开手机连接热点");
	lv_label_set_long_mode(ui->screen_wifi_label_1, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_wifi_label_1, LV_LABEL_ALIGN_LEFT);

	//Write style LV_LABEL_PART_MAIN for screen_wifi_label_1
	static lv_style_t style_screen_wifi_label_1_main;
	lv_style_reset(&style_screen_wifi_label_1_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_wifi_label_1_main
	lv_style_set_radius(&style_screen_wifi_label_1_main, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_screen_wifi_label_1_main, LV_STATE_DEFAULT, lv_color_make(0x45, 0x7c, 0xe3));
	lv_style_set_bg_grad_color(&style_screen_wifi_label_1_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_wifi_label_1_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_wifi_label_1_main, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_wifi_label_1_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_wifi_label_1_main, LV_STATE_DEFAULT, &lv_font_SIMLI_16);
	lv_style_set_text_letter_space(&style_screen_wifi_label_1_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_wifi_label_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_wifi_label_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_wifi_label_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_wifi_label_1_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_wifi_label_1, LV_LABEL_PART_MAIN, &style_screen_wifi_label_1_main);
	lv_obj_set_pos(ui->screen_wifi_label_1, 20, 110);
	lv_obj_set_size(ui->screen_wifi_label_1, 180, 0);

	//Write codes screen_wifi_label_2
	ui->screen_wifi_label_2 = lv_label_create(ui->screen_wifi, NULL);
	lv_label_set_text(ui->screen_wifi_label_2, "2：打开设备app           ");
	lv_label_set_long_mode(ui->screen_wifi_label_2, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_wifi_label_2, LV_LABEL_ALIGN_LEFT);

	//Write style LV_LABEL_PART_MAIN for screen_wifi_label_2
	static lv_style_t style_screen_wifi_label_2_main;
	lv_style_reset(&style_screen_wifi_label_2_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_wifi_label_2_main
	lv_style_set_radius(&style_screen_wifi_label_2_main, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_screen_wifi_label_2_main, LV_STATE_DEFAULT, lv_color_make(0x45, 0x7c, 0xe3));
	lv_style_set_bg_grad_color(&style_screen_wifi_label_2_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_wifi_label_2_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_wifi_label_2_main, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_wifi_label_2_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_wifi_label_2_main, LV_STATE_DEFAULT, &lv_font_SIMLI_16);
	lv_style_set_text_letter_space(&style_screen_wifi_label_2_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_wifi_label_2_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_wifi_label_2_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_wifi_label_2_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_wifi_label_2_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_wifi_label_2, LV_LABEL_PART_MAIN, &style_screen_wifi_label_2_main);
	lv_obj_set_pos(ui->screen_wifi_label_2, 20, 150);
	lv_obj_set_size(ui->screen_wifi_label_2, 180, 0);

	//Write codes screen_wifi_label_3
	ui->screen_wifi_label_3 = lv_label_create(ui->screen_wifi, NULL);
	lv_label_set_text(ui->screen_wifi_label_3, "IP：192.168.4.1                    ");
	lv_label_set_long_mode(ui->screen_wifi_label_3, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_wifi_label_3, LV_LABEL_ALIGN_LEFT);

	//Write style LV_LABEL_PART_MAIN for screen_wifi_label_3
	static lv_style_t style_screen_wifi_label_3_main;
	lv_style_reset(&style_screen_wifi_label_3_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_wifi_label_3_main
	lv_style_set_radius(&style_screen_wifi_label_3_main, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_screen_wifi_label_3_main, LV_STATE_DEFAULT, lv_color_make(0x45, 0x7c, 0xe3));
	lv_style_set_bg_grad_color(&style_screen_wifi_label_3_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_wifi_label_3_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_wifi_label_3_main, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_wifi_label_3_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_wifi_label_3_main, LV_STATE_DEFAULT, &lv_font_SIMLI_16);
	lv_style_set_text_letter_space(&style_screen_wifi_label_3_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_wifi_label_3_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_wifi_label_3_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_wifi_label_3_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_wifi_label_3_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_wifi_label_3, LV_LABEL_PART_MAIN, &style_screen_wifi_label_3_main);
	lv_obj_set_pos(ui->screen_wifi_label_3, 20, 190);
	lv_obj_set_size(ui->screen_wifi_label_3, 180, 0);

	//Write codes screen_wifi_label_4
	ui->screen_wifi_label_4 = lv_label_create(ui->screen_wifi, NULL);
	lv_label_set_text(ui->screen_wifi_label_4, "端口号：333                    ");
	lv_label_set_long_mode(ui->screen_wifi_label_4, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_wifi_label_4, LV_LABEL_ALIGN_LEFT);

	//Write style LV_LABEL_PART_MAIN for screen_wifi_label_4
	static lv_style_t style_screen_wifi_label_4_main;
	lv_style_reset(&style_screen_wifi_label_4_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_wifi_label_4_main
	lv_style_set_radius(&style_screen_wifi_label_4_main, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_screen_wifi_label_4_main, LV_STATE_DEFAULT, lv_color_make(0x45, 0x7c, 0xe3));
	lv_style_set_bg_grad_color(&style_screen_wifi_label_4_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_wifi_label_4_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_wifi_label_4_main, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_wifi_label_4_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_wifi_label_4_main, LV_STATE_DEFAULT, &lv_font_SIMLI_16);
	lv_style_set_text_letter_space(&style_screen_wifi_label_4_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_wifi_label_4_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_wifi_label_4_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_wifi_label_4_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_wifi_label_4_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_wifi_label_4, LV_LABEL_PART_MAIN, &style_screen_wifi_label_4_main);
	lv_obj_set_pos(ui->screen_wifi_label_4, 20, 230);
	lv_obj_set_size(ui->screen_wifi_label_4, 180, 0);

	//Write codes screen_wifi_label_5
	ui->screen_wifi_label_5 = lv_label_create(ui->screen_wifi, NULL);
	lv_label_set_text(ui->screen_wifi_label_5, "3：填写您的wifi信息点击发送 ");
	lv_label_set_long_mode(ui->screen_wifi_label_5, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_wifi_label_5, LV_LABEL_ALIGN_LEFT);

	//Write style LV_LABEL_PART_MAIN for screen_wifi_label_5
	static lv_style_t style_screen_wifi_label_5_main;
	lv_style_reset(&style_screen_wifi_label_5_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_wifi_label_5_main
	lv_style_set_radius(&style_screen_wifi_label_5_main, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_screen_wifi_label_5_main, LV_STATE_DEFAULT, lv_color_make(0x45, 0x7c, 0xe3));
	lv_style_set_bg_grad_color(&style_screen_wifi_label_5_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_wifi_label_5_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_wifi_label_5_main, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_wifi_label_5_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_wifi_label_5_main, LV_STATE_DEFAULT, &lv_font_SIMLI_16);
	lv_style_set_text_letter_space(&style_screen_wifi_label_5_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_wifi_label_5_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_wifi_label_5_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_wifi_label_5_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_wifi_label_5_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_wifi_label_5, LV_LABEL_PART_MAIN, &style_screen_wifi_label_5_main);
	lv_obj_set_pos(ui->screen_wifi_label_5, 20, 270);
	lv_obj_set_size(ui->screen_wifi_label_5, 180, 0);

	//Write codes screen_wifi_label_name
	ui->screen_wifi_label_name = lv_label_create(ui->screen_wifi, NULL);
	lv_label_set_text(ui->screen_wifi_label_name, "name: XXXX");
	lv_label_set_long_mode(ui->screen_wifi_label_name, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_wifi_label_name, LV_LABEL_ALIGN_LEFT);

	//Write style LV_LABEL_PART_MAIN for screen_wifi_label_name
	static lv_style_t style_screen_wifi_label_name_main;
	lv_style_reset(&style_screen_wifi_label_name_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_wifi_label_name_main
	lv_style_set_radius(&style_screen_wifi_label_name_main, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_screen_wifi_label_name_main, LV_STATE_DEFAULT, lv_color_make(0x45, 0x7c, 0xe3));
	lv_style_set_bg_grad_color(&style_screen_wifi_label_name_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_wifi_label_name_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_wifi_label_name_main, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_wifi_label_name_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_wifi_label_name_main, LV_STATE_DEFAULT, &lv_font_SIMLI_16);
	lv_style_set_text_letter_space(&style_screen_wifi_label_name_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_wifi_label_name_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_wifi_label_name_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_wifi_label_name_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_wifi_label_name_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_wifi_label_name, LV_LABEL_PART_MAIN, &style_screen_wifi_label_name_main);
	lv_obj_set_pos(ui->screen_wifi_label_name, 20, 25);
	lv_obj_set_size(ui->screen_wifi_label_name, 180, 0);

	//Write codes screen_wifi_label_pass
	ui->screen_wifi_label_pass = lv_label_create(ui->screen_wifi, NULL);
	lv_label_set_text(ui->screen_wifi_label_pass, "pass: 1234567890");
	lv_label_set_long_mode(ui->screen_wifi_label_pass, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_wifi_label_pass, LV_LABEL_ALIGN_LEFT);

	//Write style LV_LABEL_PART_MAIN for screen_wifi_label_pass
	static lv_style_t style_screen_wifi_label_pass_main;
	lv_style_reset(&style_screen_wifi_label_pass_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_wifi_label_pass_main
	lv_style_set_radius(&style_screen_wifi_label_pass_main, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_screen_wifi_label_pass_main, LV_STATE_DEFAULT, lv_color_make(0x45, 0x7c, 0xe3));
	lv_style_set_bg_grad_color(&style_screen_wifi_label_pass_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_wifi_label_pass_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_wifi_label_pass_main, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_wifi_label_pass_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_wifi_label_pass_main, LV_STATE_DEFAULT, &lv_font_SIMLI_16);
	lv_style_set_text_letter_space(&style_screen_wifi_label_pass_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_wifi_label_pass_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_wifi_label_pass_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_wifi_label_pass_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_wifi_label_pass_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_wifi_label_pass, LV_LABEL_PART_MAIN, &style_screen_wifi_label_pass_main);
	lv_obj_set_pos(ui->screen_wifi_label_pass, 20, 55);
	lv_obj_set_size(ui->screen_wifi_label_pass, 180, 0);
}